"""Read-only view of the matching decompilation tree.

Nothing here writes: it lists, counts, parses and reads. The launcher shows the 100% complete tree
and nothing else - no root picker, because there is only one decompilation worth opening.

Progress is NOT recomputed from the sources. A function counter over the C files is a guess
(measured: 2,866 functions in `src/`, ~20% short of the published 3,606, because stage overlays and
oddly formatted signatures defeat the pattern), and publishing a 20%-short figure as "matches" would
be a lie with a decimal point. The authoritative numbers are the ones the project and decomp.dev
publish, and they live in the tree: `README.md`'s progress table. That is what `published()`
parses. The per-file facts shown next to it - lines, size, and stub count - are counted exactly.
"""

from __future__ import annotations

import dataclasses
import pathlib
import re

# The real 100% tree, the only decompilation this launcher opens.
ROOT = pathlib.Path(
    r"C:\Users\space\Downloads\Videogame decomp\complete\Digimon World 3 [DECOMP]\dw3_decomp-main"
)

# Vendored third-party tooling (decomp-permuter, m2c, maspsx, psyq_headers): not the decompilation,
# so it stays out of the tree and out of the totals. The page says so.
SKIP_DIRS = {"external"}

TEXT_SUFFIXES = {".c", ".h", ".inc", ".s", ".py", ".txt", ".md",
                 ".yaml", ".yml", ".mk", ".sh", ".toml", ".json", ".ini", ".cfg"}
CODE_SUFFIXES = {".c", ".h", ".inc", ".s"}
C_SUFFIXES = {".c", ".h", ".inc"}

LINE_CAP = 4000          # a viewer shows this many lines of one file and says how many it skipped
SEARCH_HITS = 300        # results per search
SEARCH_FILE_CAP = 2000   # files visited per search
MAX_READ = 4_000_000     # bytes; bigger files are shown truncated

STUB_RE = re.compile(r"\b(?:INCLUDE_ASM|GLOBAL_ASM)\s*\(")
# A definition at brace depth 0 - `void STGDGLAB_openMenu(LabMenu *menu) {`, and the two-line form
# whose brace sits on its own line. Deliberately conservative: it undercounts rather than inventing.
DEF_RE = re.compile(r"^\s*(?:static\s+|inline\s+)*[A-Za-z_][A-Za-z0-9_]*(?:\s+|\s*\*\s*)+"
                    r"[A-Za-z_][A-Za-z0-9_]*\s*\([^;{}]*\)\s*$")
NAME_RE = re.compile(r"([A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}]*\)\s*$")
NOT_A_NAME = {"if", "for", "while", "switch", "return", "else", "do", "sizeof", "defined", "case"}

# "| Executable, game code | Europe | 346 / 346 | 100.00 % | 100.00 % |"
ROW_RE = re.compile(r"^\|\s*(?P<label>[^|]+?)\s*\|\s*(?P<region>Europe|USA|Japan)?\s*\|\s*"
                    r"(?P<matched>[\d,]+)\s*/\s*(?P<total>[\d,]+)\s*\|\s*(?P<code>[\d.]+)\s*%\s*\|\s*"
                    r"(?P<data>[\d.]+)\s*%\s*\|")
BADGE_RE = re.compile(r"https://img\.shields\.io/badge/(?P<what>[^)\"]+?)-(?P<value>[^)\"]+?)-(?P<colour>\w+)\)")


def available() -> bool:
    return ROOT.is_dir()


def _unpct(text: str) -> str:
    return text.replace("%20", " ").replace("%7C", "|").replace("%7c", "|").strip()


@dataclasses.dataclass
class FileInfo:
    rel: str
    size: int
    lines: int
    funcs: int
    stubs: int

    @property
    def suffix(self) -> str:
        return pathlib.PurePosixPath(self.rel).suffix.lower()


@dataclasses.dataclass
class Node:
    """One entry of the project tree, with its subtree rolled up."""
    name: str
    rel: str
    is_dir: bool
    size: int = 0
    lines: int = 0
    funcs: int = 0
    stubs: int = 0
    files: int = 0
    children: list["Node"] = dataclasses.field(default_factory=list)


@dataclasses.dataclass
class Inventory:
    root: pathlib.Path
    tree: Node
    files: list[FileInfo]

    @property
    def totals(self) -> tuple[int, int, int, int]:
        """(files, lines, funcs, stubs) over the visible tree."""
        return (len(self.files), sum(f.lines for f in self.files),
                sum(f.funcs for f in self.files), sum(f.stubs for f in self.files))

    @property
    def c_files(self) -> int:
        return sum(1 for f in self.files if f.suffix == ".c")


def _read(path: pathlib.Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def _strip_noise(line: str, in_block: bool) -> tuple[str, bool]:
    """Remove comments and string bodies from one line.

    Braces and quotes inside comments or literals must not reach the depth counter: a single `{` in a
    comment used to leave the counter permanently one level deep, so every function after it was
    invisible to the count. Returns the cleaned line and the carried block-comment state.
    """
    out: list[str] = []
    i, n = 0, len(line)
    while i < n:
        if in_block:
            end = line.find("*/", i)
            if end == -1:
                return "".join(out), True
            i, in_block = end + 2, False
            continue
        char = line[i]
        if char == "/" and i + 1 < n and line[i + 1] == "*":
            i, in_block = i + 2, True
            continue
        if char == "/" and i + 1 < n and line[i + 1] == "/":
            break
        if char in "\"'":
            quote = char
            i += 1
            while i < n:
                if line[i] == "\\":
                    i += 2
                    continue
                if line[i] == quote:
                    i += 1
                    break
                i += 1
            out.append('""' if quote == '"' else "''")
            continue
        out.append(char)
        i += 1
    return "".join(out), in_block


def count_code(text: str) -> tuple[int, int]:
    """(function definitions, stub macros) in one C-ish file.

    Conservative by design: it counts a definition only when a signature that looks like one is
    followed by a brace, so it undercounts rather than inventing. Both the one-line form
    (`void foo(int a) {`) and the two-line form (`void foo(int a)\\n{`) are counted - the bare brace
    has to be tested BEFORE `endswith("{")`, or the same check swallows it.
    """
    depth = 0
    funcs = 0
    stubs = 0
    in_block = False
    pending: str | None = None          # a signature whose brace has not arrived yet
    for raw in text.splitlines():
        line, in_block = _strip_noise(raw, in_block)
        line = line.strip()
        stubs += len(STUB_RE.findall(line))
        if depth == 0:
            # Three shapes: a signature with its brace on the same line, a bare brace continuing the
            # previous line's signature, and a signature alone with the brace on the next line.
            if "{" in line:
                signature = line[:line.index("{")].strip() or pending
            else:
                signature = None
            if signature and DEF_RE.match(signature):
                name = NAME_RE.search(signature)
                if name and name.group(1) not in NOT_A_NAME:
                    funcs += 1
            pending = line if "{" not in line and DEF_RE.match(line) else None
        depth += line.count("{") - line.count("}")
        if depth < 0:
            depth = 0
    return funcs, stubs


def scan(root: pathlib.Path | None = None) -> Inventory:
    """Walk the tree once: exact sizes and line counts, functions counted, stubs counted exactly."""
    root = root or ROOT
    files: list[FileInfo] = []

    def walk(directory: pathlib.Path) -> Node:
        node = Node(directory.name, directory.relative_to(root).as_posix(), True)
        try:
            entries = sorted(directory.iterdir(), key=lambda p: (p.is_file(), p.name.lower()))
        except OSError:
            # The tree moved or a directory is unreadable: report what could be read rather than
            # raising out of a tab's first paint.
            return node
        for entry in entries:
            if entry.is_dir():
                if entry.name in SKIP_DIRS or entry.name.startswith("."):
                    continue
                child = walk(entry)
            else:
                try:
                    size = entry.stat().st_size
                except OSError:
                    continue
                rel = entry.relative_to(root).as_posix()
                suffix = entry.suffix.lower()
                if suffix in CODE_SUFFIXES and size <= MAX_READ:
                    try:
                        text = _read(entry)
                    except OSError:
                        text = ""
                    # Stubs are counted in `.c` files only: a header can contain the INCLUDE_ASM
                    # macro's own definition (3 such lines live in include/), which is not an
                    # unimplemented function and must not show up as one.
                    funcs, stubs = (count_code(text) if suffix in C_SUFFIXES else (0, 0))
                    if suffix != ".c":
                        stubs = 0
                    info = FileInfo(rel, size, len(text.splitlines()), funcs, stubs)
                else:
                    info = FileInfo(rel, size, 0, 0, 0)
                files.append(info)
                child = Node(entry.name, rel, False, info.size, info.lines, info.funcs, info.stubs, 1)
            node.size += child.size
            node.lines += child.lines
            node.funcs += child.funcs
            node.stubs += child.stubs
            node.files += child.files
            node.children.append(child)
        return node

    return Inventory(root, walk(root), files)


def published() -> dict:
    """The project's own published progress, parsed from its README.

    decomp.dev's badges and the README table are generated from the same `make report` data, so this
    is the number the project stands behind. Returns {} when the README is missing or has been
    reshaped, and the page then shows the inventory alone instead of a guessed percentage.
    """
    readme = ROOT / "README.md"
    if not readme.is_file():
        return {}
    rows: list[dict] = []
    badges: dict[str, str] = {}
    for line in _read(readme).splitlines():
        row = ROW_RE.match(line)
        if row:
            rows.append({
                "label": row.group("label").replace("*", "").strip(),
                "region": row.group("region") or "All",
                "matched": int(row.group("matched").replace(",", "")),
                "total": int(row.group("total").replace(",", "")),
                "code": float(row.group("code")),
                "data": float(row.group("data")),
            })
            continue
        badge = BADGE_RE.search(line)
        if badge:
            badges[_unpct(badge.group("what"))] = _unpct(badge.group("value"))
    if not rows:
        return {}
    # The README pairs each category with its region by leaving the second row's label cell empty;
    # carry the label down so the USA rows read as something.
    last_label = ""
    for row in rows:
        if row["label"]:
            last_label = row["label"]
        else:
            row["label"] = last_label
    regions: dict[str, dict] = {}
    for row in rows:
        bucket = regions.setdefault(row["region"], {"rows": [], "matched": 0, "total": 0})
        bucket["rows"].append(row)
    for bucket in regions.values():
        bucket["matched"] = sum(r["matched"] for r in bucket["rows"] if r["label"].lower() != "total")
        bucket["total"] = sum(r["total"] for r in bucket["rows"] if r["label"].lower() != "total")
        bucket["percent"] = (100.0 * bucket["matched"] / bucket["total"]) if bucket["total"] else 0.0
        bucket["complete"] = bool(bucket["total"]) and bucket["matched"] == bucket["total"]
    return {"source": "README.md", "regions": regions, "badges": badges}


def identity() -> dict:
    """What the tree says it is: the regions it targets and the binaries it expects to reproduce."""
    out = {"regions": [], "hashes": []}
    # USA first, the one region order every other list in the launcher follows.
    for region, name in (("USA", "SLUS_014.36"), ("Europe", "SLES_039.36")):
        sha = ROOT / "config" / region[:2].lower() / f"{name}.sha1"
        if sha.is_file():
            first = _read(sha).split()
            digest = next((t for t in first if re.fullmatch(r"[0-9a-f]{40}", t)), "")
            out["regions"].append({"region": region, "binary": name, "sha1": digest})
            if digest:
                out["hashes"].append(f"{name} {digest[:12]}")
    return out


def read_lines(rel: str, cap: int = LINE_CAP) -> tuple[list[str], bool]:
    """Lines of one file, read-only, capped. `truncated` says whether there were more."""
    path = ROOT / rel
    if not path.is_file():
        raise FileNotFoundError(rel)
    text = _read(path)
    lines = text.splitlines()
    return (lines[:cap], len(lines) > cap)


def search(needle: str, regex: bool = False, hits: int = SEARCH_HITS) -> list[tuple[str, int, str]]:
    """Literal (or regex) search across the tree's text files. Read-only, capped."""
    if not needle:
        return []
    pattern = None
    if regex:
        try:
            pattern = re.compile(needle)
        except re.error:
            return []
    out: list[tuple[str, int, str]] = []
    visited = 0
    for info in sorted(scan().files, key=lambda f: f.rel):
        if info.suffix not in TEXT_SUFFIXES or info.size > MAX_READ:
            continue
        visited += 1
        if visited > SEARCH_FILE_CAP or len(out) >= hits:
            break
        try:
            text = _read(ROOT / info.rel)
        except OSError:
            continue
        for number, line in enumerate(text.splitlines(), 1):
            found = pattern.search(line) if pattern else (needle in line)
            if found:
                out.append((info.rel, number, line.strip()[:240]))
                if len(out) >= hits:
                    return out
    return out
