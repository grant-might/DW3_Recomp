#!/usr/bin/env python3
"""Checks the game C with a modern compiler.

    lint.py [-v VERSION] [--update] [--strict] [--base REV] [FILE...]

GCC 2.8.1 builds a call to an undeclared function, a pointer of the wrong
type or a function that returns no value with at most a warning that the
build doesn't stop on. A modern GCC (the Makefile's LINT_CC,
mipsel-linux-gnu-gcc) compiles every C file the version builds, with the
build's own preprocessor flags, which `make lint` gives in LINT_CPPFLAGS
and the files in LINT_SRC. It writes nothing (-S -o /dev/null), so the
match never depends on it; it compiles rather than only parses because
some warnings (control reaching the end of a non-void function, a local
read before it is set) come from generating the code. __GNUC__ is the
lint compiler's own, not the 2 the build gives GCC 2.8.1: the C is checked
as a modern GCC sees it (FLEXIBLE in include/common.h).

Every warning it gives is reported, with the option that controls it, or
[warning] for one that no option controls ("excess elements in array
initializer"), and [error] for what it refuses outright. WARNINGS lists
the options the check turns on besides GCC's defaults, QUIET the defaults
it turns off and EXEMPT the ones it leaves unchecked in a part of the
source, each with what it finds or why it is off.

config/<version>/lint.txt lists what is known and left to fix, one line per
diagnostic: the file, the function and the message, without the line number,
which an edit elsewhere in the file would make stale. The script exits with
1 on anything it doesn't list, and warns about what it lists that is fixed
(fails, with --strict). --update rewrites it, with a `# checked:` line that
names the options it was written with, and keeps the comment above an
entry, which says why it stays. The list may only get shorter:
--base fails on an entry that the list of the git revision REV doesn't
have, as tools/shiftcheck.py does, unless REV didn't check that option
yet, so that turning one on brings its own entries. It compares the
entries without their file, so that a function moved to another file
brings no new entry.
"""
import argparse
import collections
import concurrent.futures
import os
import pathlib
import re
import shlex
import subprocess
import sys

# GCC 2.8.1's dialect, the game's signed char and -fno-builtin (its memcpy
# and the like are the SDK's, not the compiler's)
LINT_FLAGS = ["-S", "-o", os.devnull, "-std=gnu89", "-fsigned-char", "-fno-builtin",
              "-fdiagnostics-plain-output"]

# The warnings checked besides GCC's defaults, by what they find
WARNINGS = [
    # Plain mistakes, whatever the compiler: code that doesn't do what it
    # says, or whose behavior is undefined
    ("implicit-function-declaration", "a call to a function with no declaration"),
    ("implicit-int", "a declaration with no type, which is int"),
    ("incompatible-pointer-types", "a pointer of another type than the one declared"),
    ("int-conversion", "an integer used as a pointer or a pointer as an integer, without a cast"),
    ("return-type", "a non-void function that returns no value, or a void one that returns one"),
    ("sequence-point", "an object written twice, or read and written, between sequence points"),
    ("uninitialized", "a local read before anything is stored to it"),
    ("override-init", "a member given two values in one initializer"),
    ("sizeof-pointer-div", "sizeof a pointer divided by its element's, where an array's was meant"),
    ("sizeof-pointer-memaccess", "sizeof a pointer as the size of a copy or a fill through it"),
    # Declarations: every function that isn't static has its prototype in
    # the header of its module or overlay, which its callers include, and
    # each is declared once. GCC 12 has no such check for variables (GCC
    # 14's -Wmissing-variable-declarations).
    ("missing-prototypes", "a function that isn't static defined with no prototype before it"),
    ("missing-declarations", "the same, with no declaration at all"),
    ("redundant-decls", "a declaration made twice in one file"),
    ("nested-externs", "an extern declaration inside a function"),
    # From -Wall and -Wextra, those that suit the period's C. The first ones
    # found something, which is fixed or listed with its reason; the others
    # find nothing and keep it so.
    ("implicit-fallthrough", "a case that runs into the next with no /* fallthrough */ comment"),
    ("unused-variable", "a local that nothing uses"),
    ("unused-label", "a label that no goto jumps to"),
    ("parentheses", "an expression whose operators' precedence is easy to misread"),
    ("comment", "a /* inside a comment"),
    ("cast-function-type", "a function cast to a pointer of another function type"),
    ("unused-but-set-variable", "a local that is set and never read"),
    ("unused-value", "an expression whose value is computed and dropped"),
    ("unused-function", "a static function that nothing calls"),
    ("unused-local-typedefs", "a typedef inside a function that nothing uses"),
    ("empty-body", "an if, else or while whose body is an empty statement"),
    ("misleading-indentation", "a statement indented as if a condition guarded it"),
    ("dangling-else", "an else that goes with another if than its indentation says"),
    ("multistatement-macros", "a macro of several statements as the body of an if or a loop"),
    ("type-limits", "a comparison that its operand's type makes always true or false"),
    ("tautological-compare", "an expression compared with itself"),
    ("shift-negative-value", "a negative value shifted left"),
    ("bool-compare", "a truth value compared with an integer other than 0 or 1"),
    ("logical-not-parentheses", "!a == b, where !(a == b) was meant"),
    ("int-in-bool-context", "an integer expression used as a truth value where a comparison was meant"),
    ("enum-compare", "values of two enums compared"),
    ("enum-conversion", "a value of one enum stored in another"),
    ("address", "the address of an object or a function used as a truth value"),
    ("char-subscripts", "a char as an array index, which -fsigned-char makes negative past 0x7F"),
    ("switch", "a switch on an enum that leaves out one of its values with no default"),
    ("ignored-qualifiers", "a const or volatile on a function's return type"),
    ("old-style-declaration", "a storage class after the type, or other forms C89 calls obsolescent"),
    ("missing-parameter-type", "an old-style parameter with no declaration, which is int"),
    ("missing-braces", "an initializer of an inner aggregate without its braces"),
    ("memset-transposed-args", "a fill with its value and size swapped"),
    # Left off, as what they find is the period's C, not a mistake:
    # unused-parameter (a method or a callback takes its table's arguments,
    # whether it uses them or not), missing-field-initializers (a table
    # entry gives its first members, and the rest are 0), sign-compare (the
    # range checks compare with an unsigned constant, stat - 1 < 6U, as the
    # match's sltiu needs), pointer-sign (the text is Shift-JIS, which the
    # game reads as u8, and the C library and the SDK take char),
    # strict-prototypes and old-style-definition (the old-style definitions
    # and the declarations without a prototype that the match relies on).
    # maybe-uninitialized, clobbered, array-bounds and the like come from
    # the optimizer, which the lint doesn't run.
]

# GCC's default warnings that the check turns off, and why
QUIET = [
    # STAGSLCT_showBiosVersion's two-byte character constants: the match
    # depends on them (src/debug/stage_select/stage_select.c)
    "multichar",
]

# Warnings left unchecked in a part of the source, and why
EXEMPT = [
    ("src/field/stages/", ("missing-prototypes", "missing-declarations"),
     "a stage is a binary of one C file, and its functions keep splat's names, the same in"
     " many stages, so no header can declare them: a stage declares a function only where a"
     " use comes before its definition (tools/stage_externs.py)"),
]

# What a known list without a `# checked:` line was written with
LEGACY_CHECKED = {"implicit-function-declaration", "incompatible-pointer-types", "default"}

DIAGNOSTIC = re.compile(r"^(?P<file>[^:\s]+):\d+:\d+: (?P<kind>warning|error): (?P<msg>.*?)(?: \[-W(?P<flag>[\w=-]+)\])?$")
FLAG = re.compile(r": \[([\w=-]+)\] ")
IN_FUNCTION = re.compile(r"^(?P<file>[^:\s]+): In function '(?P<func>\w+)':$")
FILE_SCOPE = re.compile(r"^(?P<file>[^:\s]+): At top level:$")


def lint_file(cc, cppflags, path):
    """The diagnostics of one file, as `file: function: [flag] message`
    keys. A header's (or an included .inc.c's) are reported with its path."""
    # C: plain quotes, which keeps the list the same in every locale
    env = dict(os.environ, LC_ALL="C")
    warnings = ["-W" + w for w, _ in WARNINGS] + ["-Wno-" + w for w in QUIET]
    out = subprocess.run(cc + cppflags + LINT_FLAGS + warnings + [path], capture_output=True, text=True, env=env)
    found = []
    func = {}
    for line in out.stderr.splitlines():
        m = IN_FUNCTION.match(line)
        if m:
            func[m["file"]] = m["func"]
            continue
        m = FILE_SCOPE.match(line)
        if m:
            func[m["file"]] = None
            continue
        m = DIAGNOSTIC.match(line)
        if not m:
            continue
        flag = "error" if m["kind"] == "error" else m["flag"] or "warning"
        if any(m["file"].startswith(path) and flag in flags for path, flags, _ in EXEMPT):
            continue
        where = m["file"] + ": " + (func.get(m["file"]) or "(file scope)")
        found.append("%s: [%s] %s" % (where, flag, m["msg"]))
    if out.returncode and not any("[error]" in f for f in found):
        # a failure with no diagnostic of its own (the compiler is missing...)
        sys.exit("%s failed on %s:\n%s" % (cc[0], path, out.stderr.strip()))
    return path, found


def lint(cc, cppflags, files):
    """Every file's diagnostics, counted: a file's own add up, and those of a
    header or an .inc.c that several files include count once, as many times
    as the file that has the most of them."""
    total = collections.Counter()
    shared = collections.Counter()
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 1) as pool:
        for path, found in pool.map(lambda f: lint_file(cc, cppflags, f), files):
            mine = collections.Counter(found)
            for key, n in mine.items():
                if key.startswith(path + ":"):
                    total[key] += n
                else:
                    shared[key] = max(shared[key], n)
    return total + shared


def modern(cc, cppflags):
    """The build's preprocessor flags with the lint compiler's own __GNUC__
    and __GNUC_MINOR__ in place of GCC 2.8.1's."""
    version = subprocess.run(cc + ["-dumpfullversion"], capture_output=True, text=True)
    if version.returncode:
        sys.exit("%s failed: %s" % (cc[0], version.stderr.strip()))
    major, minor = version.stdout.strip().split(".")[:2]
    flags = [f for f in cppflags if not f.startswith(("-D__GNUC__=", "-D__GNUC_MINOR__="))]
    return flags + ["-D__GNUC__=" + major, "-D__GNUC_MINOR__=" + minor]


def category(key):
    """The option an entry's warning comes from, or "default" for GCC's own
    warnings and errors."""
    flag = FLAG.search(key)[1]
    return flag if flag in dict(WARNINGS) else "default"


def checked(text):
    """The options a known list was written with."""
    for line in text.splitlines():
        if line.startswith("# checked:"):
            return set(line.split(":", 1)[1].split())
    return LEGACY_CHECKED


def entries(text):
    return collections.Counter(l for l in text.splitlines() if l and not l.startswith("#"))


def reasons(text):
    """The comment above each entry of a known list (after its first three
    lines), which says why it stays: --update keeps it above the entry."""
    found = {}
    comment = []
    for line in text.splitlines()[3:]:
        if line.startswith("#"):
            comment.append(line)
        elif line:
            if comment and line not in found:
                found[line] = comment
            comment = []
    return found


def without_file(counter):
    """The entries as "function: [warning] message", whatever file has them."""
    return collections.Counter(k.split(": ", 1)[1] for k in counter.elements())


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("-v", "--version", default=os.environ.get("VERSION", "eu"))
    ap.add_argument("--update", action="store_true", help="write what is found as the known list")
    ap.add_argument("--strict", action="store_true", help="also fail on what the known list has that is fixed")
    ap.add_argument("--base", metavar="REV", help="also fail on what the known list has that REV's doesn't")
    ap.add_argument("files", nargs="*", help="the files to check (default: LINT_SRC)")
    args = ap.parse_args()
    cc = shlex.split(os.environ.get("LINT_CC", "mipsel-linux-gnu-gcc"))
    cppflags = shlex.split(os.environ.get("LINT_CPPFLAGS", ""))
    every = sorted(os.environ.get("LINT_SRC", "").split())
    files = args.files or every
    if not files or not cppflags:
        sys.exit("no files or flags: run it as make VERSION=%s lint" % args.version)
    known_path = pathlib.Path("config") / args.version / "lint.txt"

    found = lint(cc, modern(cc, cppflags), files)
    if args.update:
        lines = ["# What a modern GCC warns about in the C (tools/lint.py --update): the",
                 "# file, the function, the warning and its message, one per line",
                 "# checked: " + " ".join(sorted([w for w, _ in WARNINGS] + ["default"]))]
        why = reasons(known_path.read_text()) if known_path.exists() else {}
        # by warning, so that one comment can say why a group stays
        for entry in sorted(found.elements(), key=lambda k: (FLAG.search(k)[1], k)):
            lines += why.pop(entry, [])
            lines.append(entry)
        known_path.write_text("\n".join(lines) + "\n")
        print("wrote %s: %d" % (known_path, sum(found.values())))
        return

    known = entries(known_path.read_text()) if known_path.exists() else collections.Counter()
    added = collections.Counter()
    if args.base:
        base = subprocess.run(["git", "show", "%s:%s" % (args.base, known_path.as_posix())],
                              capture_output=True, text=True)
        # a revision from before the list has nothing to compare
        if base.returncode == 0:
            # an option REV didn't check brings its own entries
            before = checked(base.stdout)
            added = without_file(known) - without_file(entries(base.stdout))
            added = collections.Counter({k: n for k, n in added.items() if category(k) in before})
    # only a check of every file knows what is fixed
    new = found - known
    gone = known - found if not args.files else collections.Counter()
    per_flag = collections.Counter(FLAG.search(k)[1] for k in found.elements())
    print("%s: %s" % (args.version, ", ".join("%s %d" % kv for kv in sorted(per_flag.items())) or "nothing found"))
    for k in sorted(new.elements()):
        print("new: %s" % k)
    for k in sorted(gone.elements()):
        print("fixed: %s" % k)
    for k in sorted(added.elements()):
        print("added since %s: %s" % (args.base, k))
    if added:
        print("error: %d added to %s since %s, which may only get shorter"
              % (sum(added.values()), known_path, args.base))
    if gone:
        print("%s: %d fixed, which tools/lint.py -v %s --update drops from %s"
              % ("error" if args.strict else "warning", sum(gone.values()), args.version, known_path))
    sys.exit(1 if new or added or (gone and args.strict) else 0)


if __name__ == "__main__":
    main()
