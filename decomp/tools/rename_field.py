#!/usr/bin/env python3
"""Rename a struct field, in its definition and wherever the code uses it.

    tools/rename_field.py STRUCT OLD NEW [--via REGEX]... [--files GLOB]... [-n]
    tools/rename_field.py --spec FILE [-n]

A field's name is a word after `.` or `->`, so a rename is safe as a whole
word only when no other struct has a field of the same name. The tool reads
every struct defined under include/ and src/:

- In STRUCT's definition, the member OLD becomes NEW.
- When no other struct has a member OLD, every `.OLD` and `->OLD` under
  src/ and include/ becomes NEW.
- Otherwise each use needs a --via: a regular expression that has to match
  the text just before the `.` or `->` on the same line (`slot`,
  `slots\\[[^]]*\\]`, `pile`...). Uses that no --via matches are left alone
  and listed, so that a rerun with another --via can take them.
- `STRUCT.OLD` in a comment becomes `STRUCT.NEW` everywhere.

--files limits the uses (not the definition) to the files that match one of
the globs, relative to the repository (`src/cardgame/card_battle/*.c`).

A spec file holds one rename per line, `STRUCT OLD NEW [--via REGEX]...
[--files GLOB]...` (shell quoting), and comment lines that start with `#`;
the renames run in order. A rename
that has nothing left to do does nothing, so a spec can be run again after
a rebase: the tool is idempotent. A NEW that STRUCT already has is refused
unless OLD is gone (the rename was made).

Renaming a field never changes the code, so `make compare` checks it: a use
renamed in the wrong struct no longer compiles.
"""

import argparse
import fnmatch
import re
import shlex
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SUFFIXES = {".c", ".h"}
STRUCT_START = re.compile(r"\bstruct\s+([A-Za-z_]\w*)\s*\{")
COMMENT = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)


def source_files():
    out = []
    for top in ("include", "src"):
        out += sorted(p for p in (ROOT / top).rglob("*") if p.is_file() and p.suffix in SUFFIXES)
    return out


def body_end(text, start):
    """The index of the `}` that closes the `{` at start."""
    depth = 0
    i = start
    while i < len(text):
        c = text[i]
        if text.startswith("/*", i):
            i = text.index("*/", i) + 2
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    raise ValueError("unbalanced braces")


def member_names(body):
    """The names of the members a struct body declares (nested ones too)."""
    code = COMMENT.sub(" ", body)
    code = re.sub(r"^\s*#.*$", " ", code, flags=re.M)  # #if around members
    code = re.sub(r"[{}]", ";", code)
    names = set()
    for decl in code.split(";"):
        decl = decl.strip()
        if not decl:
            continue
        m = re.search(r"\(\s*\*\s*([A-Za-z_]\w*)\s*\)\s*\(", decl)
        if m:
            names.add(m.group(1))
            continue
        for part in decl.split(","):
            part = re.sub(r"\[[^\]]*\]", "", part)
            part = re.sub(r":\s*\w+\s*$", "", part).strip()
            m = re.search(r"([A-Za-z_]\w*)\s*$", part)
            if m:
                names.add(m.group(1))
    return names


def structs(texts):
    """{(path, name): (body start, body end)} and {name: members}."""
    spans = {}
    members = {}
    for path, text in texts.items():
        for m in STRUCT_START.finditer(text):
            start = m.end() - 1
            try:
                end = body_end(text, start)
            except ValueError:
                continue
            name = m.group(1)
            spans[(path, name)] = (start, end)
            members.setdefault(name, set()).update(member_names(text[start + 1:end]))
    return spans, members


def word(name):
    return r"(?<![A-Za-z0-9_])" + re.escape(name) + r"(?![A-Za-z0-9_])"


def rename_in_body(body, old, new):
    """OLD as a member's name: in the declaration, not in its comment."""
    out = []
    for line in body.split("\n"):
        if "*/" in line and "/*" not in line:
            out.append(line)  # the end of a comment
            continue
        code, sep, comment = line.partition("/*")
        if code.strip().startswith("/") or not code.strip():
            # an offset comment first: `/* 0x0 */ s16 old; /* ... */`
            m = re.match(r"(\s*/\*.*?\*/)(.*)$", line)
            if m:
                head, rest = m.groups()
                code, sep, comment = rest.partition("/*")
                out.append(head + re.sub(word(old), new, code) + sep + comment)
                continue
            out.append(line)
            continue
        out.append(re.sub(word(old), new, code) + sep + comment)
    return "\n".join(out)


def rename(texts, struct, old, new, vias, globs, dry):
    spans, members = structs(texts)
    defs = [(p, s) for (p, n), s in spans.items() if n == struct]
    if not defs:
        sys.exit(f"struct {struct} isn't defined")
    have = members[struct]
    # renamed before (in a branch rebased onto this one, say): the
    # definition is done, but uses written since may still say OLD
    done = old not in have
    if done and new not in have:
        sys.exit(f"{struct} has no field {old}")
    if not done and new in have:
        sys.exit(f"{struct} already has a field {new}")
    shared = sorted(n for n, m in members.items() if n != struct and old in m)
    if shared and not vias:
        sys.exit(f"{old} is a field of {', '.join(shared)} too: give --via")
    changed = 0

    for path, (start, end) in [] if done else defs:
        text = texts[path]
        body = text[start:end]
        new_body = rename_in_body(body, old, new)
        if new_body != body:
            texts[path] = text[:start] + new_body + text[end:]
            changed += 1

    use = re.compile(r"(\.|->)(\s*)" + word(old))
    via_res = [re.compile("(?:" + v + r")\s*$") for v in vias]
    comment_ref = re.compile(re.escape(struct) + r"\." + word(old))
    left = []
    for path in list(texts):
        rel = str(path.relative_to(ROOT))
        text = comment_ref.sub(f"{struct}.{new}", texts[path])
        if globs and not any(fnmatch.fnmatch(rel, g) for g in globs):
            if text != texts[path]:
                texts[path] = text
            continue
        out = []
        pos = 0
        for m in use.finditer(text):
            line_start = text.rfind("\n", 0, m.start()) + 1
            before = text[line_start:m.start()]
            if shared and not any(v.search(before) for v in via_res):
                line = text.count("\n", 0, m.start()) + 1
                left.append(f"{rel}:{line}: {text[line_start:text.find(chr(10), m.end())].strip()}")
                continue
            out.append(text[pos:m.start()] + m.group(1) + m.group(2) + new)
            pos = m.end()
        out.append(text[pos:])
        text = "".join(out)
        if text != texts[path]:
            texts[path] = text
            changed += 1
    for line in left:
        print(f"left: {line}", file=sys.stderr)
    return changed


def parse(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("struct", nargs="?")
    ap.add_argument("old", nargs="?")
    ap.add_argument("new", nargs="?")
    ap.add_argument("--via", action="append", default=[])
    ap.add_argument("--files", action="append", default=[])
    ap.add_argument("--spec")
    ap.add_argument("-n", "--dry-run", action="store_true")
    return ap.parse_args(argv)


def main():
    args = parse(sys.argv[1:])
    jobs = []
    if args.spec:
        for line in Path(args.spec).read_text().splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            job = parse(shlex.split(line))
            jobs.append(job)
    else:
        if not (args.struct and args.old and args.new):
            sys.exit("give STRUCT OLD NEW or --spec")
        jobs.append(args)
    paths = source_files()
    original = {p: p.read_text(errors="surrogateescape") for p in paths}
    texts = dict(original)
    for job in jobs:
        rename(texts, job.struct, job.old, job.new, job.via, job.files, args.dry_run)
    for path in paths:
        if texts[path] != original[path]:
            print(path.relative_to(ROOT))
            if not args.dry_run:
                path.write_text(texts[path], errors="surrogateescape")


if __name__ == "__main__":
    main()
