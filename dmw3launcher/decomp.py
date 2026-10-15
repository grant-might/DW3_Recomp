"""Read-only view of the matching decompilation tree.

Nothing here writes: it lists, counts, parses and reads. The launcher shows the 100% complete tree
and nothing else - no root picker, because there is only one decompilation worth opening.

Progress is NOT recomputed from the sources. A function counter over the C files is a guess
(measured: 2,866 functions in `src/`, ~20% short of the published 3,606, because stage overlays and
oddly formatted signatures defeat the pattern), and publishing a 20%-short figure as "matches" would
be a lie with a decimal point. The authoritative numbers are the ones the project and decomp.dev
publish, and they live in the tree: `README.md`'s progress table. That is what `published()`
parses. The per-file facts shown next to it - lines, size, and stub count - are counted exactly.

Three versions are in the tree: USA, Europe and Japan. Every one of them is first-class here.
Japan has its own `config/jp/` (SLPS_034.46.sha1, overlays.sha1, stages.sha1), so the tab lists it
as a region it targets. The README's numeric table carries no row for it, but the project does
publish its figures - in the machine-readable report decomp.dev serves for the version
(`<version page>.json`) and, more coarsely, in the badges its README row links. Both are fetched by
`published_figures`, which prefers the report (it carries the counts AND the per-category
breakdown), reads every number out of what the site serves, and reports a version's figures as
unavailable rather than showing one nobody measured. See `regions()`, `parse_report()`,
`report_parts()`.
"""

from __future__ import annotations

import dataclasses
import json
import os
import pathlib
import re
import time
import urllib.request

from . import paths

# The decompilation tree this launcher opens. It is not part of the public repository (it is a
# research tree the player supplies), so it is discovered relatively first: a `decomp/` folder
# beside the launcher, or the DW3_DECOMP_ROOT override. The developer's own checkout is only a
# last-resort fallback, never how a shipped copy finds it.
DECOMP_ROOT_ENV = "DW3_DECOMP_ROOT"
_ROOT_CANDIDATES = (
    lambda: os.environ.get(DECOMP_ROOT_ENV),
    lambda: paths.launcher_root() / "decomp",
    lambda: r"C:\Users\space\Downloads\Videogame decomp\complete\Digimon World 3 [DECOMP]\dw3_decomp-main",
    lambda: r"D:\AGENT\NEWEST DIGIMON WORLD 3 COMPLETE PROJECT\Digimon World 3 [DECOMP] 99.62%\dw3_decomp-main",
)


def _looks_like_tree(p: pathlib.Path) -> bool:
    """A decompilation tree carries the sources and the README whose table is the progress."""
    return (p / "src").is_dir() and (p / "README.md").is_file()


def _find_root() -> pathlib.Path:
    cands: list[pathlib.Path] = []
    for get in _ROOT_CANDIDATES:
        raw = get()
        if raw:
            cands.append(pathlib.Path(raw))
    for c in cands:
        if _looks_like_tree(c):
            return c
    return cands[1]              # the relative decomp/ folder: absent until a player drops one in


ROOT = _find_root()

# The versions this tree targets, USA first (the one order every list in the launcher follows),
# each with the binary it reproduces and the `config/<folder>/` its own checklists live in. All
# three folders exist in the tree, Japan's included: config/jp/ holds SLPS_034.46.sha1 plus the
# overlay and stage lists for the Japanese release, exactly like us/ and eu/ do.
REGION_TARGETS = (
    ("USA", "SLUS_014.36", "us"),
    ("Europe", "SLES_039.36", "eu"),
    ("Japan", "SLPS_034.46", "jp"),
)
REGION_ORDER = tuple(name for name, _binary, _folder in REGION_TARGETS)

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

# The README's VERSION table, one row per release: `| 🇯🇵 Japan (`SLPS_034.46`) | ... |`. The serial
# sits in markdown code formatting (backticks), so the pattern accepts it with or without them. A
# version row carries either the project's own prose or decomp.dev badges - Japan has carried both
# upstream (a plain sentence in one revision, three badge images in the next), so the parser accepts
# either shape and never turns an image into a number. `MEASURE_RE` reads which measures a row's
# badges cover; it is the ONLY thing taken from a badge cell, never a value out of the image.
VERSION_ROW_RE = re.compile(r"^\|\s*[^|]*?\(`?(?P<serial>SL[A-Z]{2}_\d{3}\.\d{2})`?\)\s*\|"
                            r"(?P<cells>.*)\|$")
VERSION_PAGE_RE = re.compile(r"https://decomp\.dev/(?P<repo>[\w.\-]+/[\w.\-]+)/(?P<serial>SL[A-Z]{2}_\d{3}\.\d{2})")
MEASURE_RE = re.compile(r"[?&]measure=(?P<measure>[a-z0-9_-]+)")

# The badge IMAGES a version row links: a decomp.dev SVG with its own query string. These are read
# out of the README row (never written down here), fetched, and the percentage is read from the SVG
# the URL serves - see `published_figures`. They are the FALLBACK for a version the tree's numeric
# table has no row for (Japan today): the same site publishes a machine-readable report of the same
# figures, which is preferred because it carries the counts and the per-category breakdown as data
# (`REPORT_SUFFIX`, `parse_report`).
BADGE_URL_RE = re.compile(r"https://decomp\.dev/[\w.\-]+/[\w.\-]+\.svg\?[^)\s\"'<>]+")
SVG_TEXT_RE = re.compile(r">([^<>]*)<")                 # what an SVG node renders as text
SVG_PERCENT_RE = re.compile(r"^(\d+(?:\.\d+)?)\s*%$")
SVG_COUNTS_RE = re.compile(r"^(\d[\d,]*)\s*/\s*(\d[\d,]*)$")

# A decomp.dev version PAGE (`https://decomp.dev/<owner>/<repo>/<serial>`) serves the report
# decomp.dev generated for that version - the same data its badges are drawn from - as `<page>.json`.
# Nothing is written down here beyond the suffix: the page comes out of the README's own row.
REPORT_SUFFIX = ".json"

# A short timeout on purpose: the tab shows a placeholder that resolves, so an answer that takes
# longer than this is told to be unavailable rather than freezing the window on it.
FIGURE_TIMEOUT = 5.0
FIGURE_CACHE_TTL = 6 * 3600        # seconds a fetched badge stays usable
MAX_FIGURE_BYTES = 128 * 1024      # a badge SVG is ~1.3 KB; this is a guard, not a size
FIGURE_USER_AGENT = "DMW3Launcher/decomp-tab"



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


def table_labels() -> list[str]:
    """The part rows the README's numeric table publishes, in its own order: labels only.

    `| Executable, game code | Europe | 346 / 346 | …` - the rows a version's breakdown is shown
    under. Read out of the README rather than written down here, so a version whose table has no row
    is still broken down under the SAME labels the versions that do are (see `report_parts`). Deduped
    (every label appears once per release) and without the Total row.
    """
    out: list[str] = []
    for bucket in published().get("regions", {}).values():
        for row in bucket.get("rows", []):
            label = str(row.get("label", "")).strip()
            if label and label.lower() != "total" and label not in out:
                out.append(label)
    return out


def report_url(page: str) -> str:
    """The machine-readable report behind a decomp.dev version page, or '' for no page."""
    page = (page or "").strip().rstrip("/")
    return f"{page}{REPORT_SUFFIX}" if page else ""


def _sha1_of(path: pathlib.Path) -> str:
    """The digest a `.sha1` file carries, or ''.

    The files are `<40 hex>  build/<region>/<binary>`; anything else in the file is ignored.
    """
    if not path.is_file():
        return ""
    first = _read(path).split()
    return next((token for token in first if re.fullmatch(r"[0-9a-f]{40}", token)), "")


def _count_lines(path: pathlib.Path) -> int:
    """Entries named by a checklist file (its non-empty lines), 0 when the file is absent."""
    if not path.is_file():
        return 0
    return sum(1 for line in _read(path).splitlines() if line.strip())


def regions() -> list[dict]:
    """Everything the tree and its README say about each version it targets.

    One entry per target, in the app's order (USA, Europe, Japan):

        region, binary, config, sha1, overlays, stages, present   <- facts about THIS tree
        page, report, measures, note                              <- what the README offers for it

    There is deliberately NO percentage here. The README's numeric progress table is the only place
    the project publishes figures, and it may or may not carry a version's row (see `published()`).
    Japan's row carries decomp.dev BADGE IMAGES - or, in an earlier revision, the sentence "the
    shared C, not on decomp.dev yet" - so `page`, `report` and `measures` are read from the row's
    LINKS and query strings, and `note` only ever holds prose. `report` is the machine-readable
    report behind that page (`<page>.json`), which is what the panel fetches when there is one.
    A figure scraped out of a badge image would be a guess dressed as a measurement, which is the
    one thing this module must not produce.
    """
    readme_rows: dict[str, dict] = {}
    readme = ROOT / "README.md"
    if readme.is_file():
        for line in _read(readme).splitlines():
            row = VERSION_ROW_RE.match(line.strip())
            if not row:
                continue
            serial = row.group("serial")
            cells = [cell.strip() for cell in row.group("cells").split("|")]
            pages = [m.group(0) for m in VERSION_PAGE_RE.finditer(line)]
            readme_rows[serial] = {
                "page": next((p for p in pages if p.endswith("/" + serial)), pages[0] if pages else ""),
                "measures": sorted(set(MEASURE_RE.findall(line))),
                "note": next((cell for cell in cells if cell and "http" not in cell), ""),
            }

    out: list[dict] = []
    for region, binary, folder in REGION_TARGETS:
        conf = ROOT / "config" / folder
        sha = conf / f"{binary}.sha1"
        row = readme_rows.get(binary, {})
        out.append({
            "region": region,
            "binary": binary,
            "config": f"config/{folder}",
            "sha1": _sha1_of(sha),
            "present": sha.is_file(),
            "overlays": _count_lines(conf / "overlays.sha1"),
            "stages": _count_lines(conf / "stages.sha1"),
            "page": row.get("page", ""),
            "report": report_url(row.get("page", "")),
            "measures": list(row.get("measures", [])),
            "note": row.get("note", ""),
        })
    return out


def badge_urls(serial: str) -> dict[str, str]:
    """{measure: svg url} taken from the README's own version row for `serial`.

    The row is the README's `| 🇯🇵 Japan (`SLPS_034.46`) | <badges> |` line; every decomp.dev badge
    SVG it links is kept under the `measure=` it asks for. Nothing is written down here, so a row
    whose badges change changes what is fetched, and a version with no row has nothing to fetch.
    """
    out: dict[str, str] = {}
    readme = ROOT / "README.md"
    if not readme.is_file():
        return out
    for line in _read(readme).splitlines():
        row = VERSION_ROW_RE.match(line.strip())
        if not row or row.group("serial") != serial:
            continue
        for url in BADGE_URL_RE.findall(line):
            measure = next(iter(MEASURE_RE.findall(url)), "")
            if measure and measure not in out:
                out[measure] = url
    return out


def parse_badge_svg(text: str) -> dict:
    """What one decomp.dev/shields badge SVG actually renders, as data.

    `{"percent": 100.0}` for a percentage badge, `{"counts": (3606, 3606)}` for a counts badge (the
    functions badge for SLPS_034.46 renders `3,606 / 3,606`), `{}` when this SVG renders neither.

    Read, never computed: the value is the badge's own text node (the label is not a number). Two
    DIFFERENT percentages in one badge is ambiguous and yields `{}` - no choice between them is ours
    to make, and a number that was not published must never reach the panel.
    """
    percents: set[float] = set()
    counts: list[tuple[int, int]] = []
    for raw in SVG_TEXT_RE.findall(text):
        token = raw.strip()
        pct = SVG_PERCENT_RE.match(token)
        if pct:
            percents.add(float(pct.group(1)))
            continue
        both = SVG_COUNTS_RE.match(token)
        if both:
            pair = (int(both.group(1).replace(",", "")), int(both.group(2).replace(",", "")))
            if pair not in counts:
                counts.append(pair)
    if len(percents) == 1:
        return {"percent": percents.pop()}
    if percents:
        return {}
    if len(counts) == 1:
        return {"counts": counts[0]}
    return {}


def _count_pair(measures: dict, stem: str) -> tuple[int, int] | None:
    """`(matched, total)` for one measure of a decomp.dev report, or None when it carries neither."""
    matched, total = measures.get(f"matched_{stem}"), measures.get(f"total_{stem}")
    pair: list[int] = []
    for value in (matched, total):
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            return None
        if float(value) != int(value):
            return None
        pair.append(int(value))
    return pair[0], pair[1]


def _measure_entry(measures: dict, stem: str) -> dict:
    """One measure as the panel's figure shape: `{"counts": (matched, total), "percent": float}`.

    The same shape the badge path renders, so the panel draws either source with one code path. The
    percentage is the report's OWN `matched_<stem>_percent` field; it is not computed here. A report
    that carries the counts but no percentage yields the counts alone - a number nobody published is
    never filled in.
    """
    pair = _count_pair(measures, stem)
    if pair is None:
        return {}
    entry: dict = {"counts": pair}
    percent = measures.get(f"matched_{stem}_percent")
    if isinstance(percent, (int, float)) and not isinstance(percent, bool):
        entry["percent"] = float(percent)
    return entry


def _config_folder(serial: str) -> str:
    """The `config/<folder>/` this tree keeps a version's own checklists in, or ''."""
    return next((folder for _region, binary, folder in REGION_TARGETS if binary == serial), "")


def _overlay_ids(folder: str) -> list[str]:
    """The overlays THIS tree names, lowercased the way a report names its categories.

    `config/<folder>/overlays.sha1` is the very list the tab counts its "21 overlays" from. Each line
    is `<sha>  build/<region>/AAA/PRO/<NAME>.PRO`, so the unit is that file's stem, lowercased:
    `build/jp/AAA/PRO/STAGSLCT.PRO` -> `stagslct`.
    """
    path = ROOT / "config" / folder / "overlays.sha1"
    if not folder or not path.is_file():
        return []
    out: list[str] = []
    for line in _read(path).splitlines():
        tokens = line.split()
        if not tokens:
            continue
        stem = pathlib.PurePosixPath(tokens[-1]).stem
        if stem:
            out.append(stem.lower())
    return out


def report_groups(serial: str, categories: dict) -> dict[str, list[str]]:
    """Which of a report's categories each of the panel's part rows is made of, read from THIS tree.

    `{"executable": [...], "overlays": [...], "stages": [...]}` - category ids, in the tree's order.
    Nothing here is written down as a guess:

    * the OVERLAYS are the units `config/<folder>/overlays.sha1` names (the file the tab counts its
      "21 overlays" from), and every one of them must be a category the report carries;
    * the STAGES are the category whose id is the tree's own stage list (`config/<folder>/
      stages.sha1`) AND, where the report publishes a unit count for it, whose unit count is exactly
      that list's length - the same number the tab reports as the version's stages;
    * the EXECUTABLE is the single category left over once those are claimed.

    {} when anything disagrees - a unit the report does not carry, a stage count that does not match,
    more or fewer than one category left over. A row that cannot be derived from the tree is left
    out; it is never guessed at.
    """
    folder = _config_folder(serial)
    if not folder or not categories:
        return {}
    overlays = _overlay_ids(folder)
    if not overlays or not set(overlays) <= set(categories):
        return {}
    stage_list = ROOT / "config" / folder / "stages.sha1"
    stage_count = _count_lines(stage_list)
    rest = [cid for cid in categories if cid not in set(overlays)]
    stages: list[str] = []
    for cid in rest:
        if cid != stage_list.stem.lower():
            continue
        units = categories[cid].get("measures", {}).get("total_units")
        if isinstance(units, (int, float)) and not isinstance(units, bool) and units != stage_count:
            continue                       # the report's own unit count contradicts the tree's list
        stages.append(cid)
    executable = [cid for cid in rest if cid not in stages]
    if len(stages) != 1 or len(executable) != 1:
        return {}
    return {"executable": executable, "overlays": overlays, "stages": stages}


def _label_with_count(label: str, count: int) -> str:
    """A table label whose TRAILING parenthetical is restated with this version's own count.

    The stage row's label carries a per-version count upstream ("The stages (293 and 238)"), so for a
    version the table has no row for it has to be restated from this tree's own stage list. A label
    with no parenthetical, or a count of zero, is left exactly as it is.
    """
    if not count or " (" not in label or not label.rstrip().endswith(")"):
        return label
    return f"{label.split(' (')[0]} ({count})"


def report_parts(serial: str, report: dict) -> list[dict]:
    """The panel's per-part rows for a report: the same rows the other versions show, filled in.

    One row per part - the executable, the overlays, the stages - under the README's own table
    labels (`table_labels`), carrying the report's own function counts summed over the categories
    each row is made of (`report_groups`). The panel divides those two numbers for its percentage,
    exactly as it does for the rows it reads out of the table itself.

    [] when the rows cannot be built honestly: a table without three part labels, a grouping the tree
    does not support, a category with no function counts, or groups that do not add up to the
    report's own total (which is what proves every category is counted, once). No row is invented,
    and no count is estimated.
    """
    categories = {c["id"]: c for c in report.get("report_categories", [])
                  if isinstance(c, dict) and isinstance(c.get("id"), str)}
    groups = report_groups(serial, categories)
    labels = table_labels()
    if not groups or len(labels) != 3:
        return []
    rows: list[dict] = []
    for label, key in zip(labels, ("executable", "overlays", "stages")):
        matched = total = 0
        for cid in groups[key]:
            entry = _measure_entry(categories[cid].get("measures", {}), "functions")
            if not entry:
                return []
            matched += entry["counts"][0]
            total += entry["counts"][1]
        rows.append({"label": label, "matched": matched, "total": total})
    folder = _config_folder(serial)
    rows[2]["label"] = _label_with_count(rows[2]["label"],
                                         _count_lines(ROOT / "config" / folder / "stages.sha1"))
    overall = _measure_entry(report.get("measures", {}), "functions")
    if not overall or sum(r["matched"] for r in rows) != overall["counts"][0] \
            or sum(r["total"] for r in rows) != overall["counts"][1]:
        return []
    return rows


def parse_report(text: str, serial: str) -> dict:
    """A decomp.dev report (the JSON behind a version page) as the panel's figures; {} for anything else.

    The shape is the badge path's own - `code`, `data` and `functions`, each `{"counts": (matched,
    total), "percent": …}` - so the panel draws either source identically; `parts` carries the
    per-part rows (`report_parts`), and `commit` the upstream revision the report was generated from,
    which the panel quotes as provenance.

    Every number is READ: the percentages are the report's `matched_*_percent` fields and the counts
    its `matched_*`/`total_*` pairs. Nothing is counted or estimated here, and text that is not a
    report with measures yields {} rather than a plausible-looking zero.
    """
    try:
        raw = json.loads(text)
    except (ValueError, TypeError):
        return {}
    if not isinstance(raw, dict):
        return {}
    measures = raw.get("measures")
    categories = raw.get("report_categories")
    if not isinstance(measures, dict) or not isinstance(categories, list):
        return {}
    out: dict = {}
    for measure in ("code", "data", "functions"):
        entry = _measure_entry(measures, measure)
        if entry:
            out[measure] = entry
    if not out:
        return {}
    parts = report_parts(serial, raw)
    if parts:
        out["parts"] = parts
    commit = raw.get("commit")
    if isinstance(commit, dict) and commit.get("sha"):
        out["commit"] = str(commit["sha"])
    return out


def fetch_text(url: str, timeout: float = FIGURE_TIMEOUT) -> str:
    """One plain GET, text only. Raises on any failure; the caller decides what to say about it."""
    request = urllib.request.Request(url, headers={"User-Agent": FIGURE_USER_AGENT})
    with urllib.request.urlopen(request, timeout=timeout) as response:  # noqa: S310 - https only
        return response.read(MAX_FIGURE_BYTES).decode("utf-8", "replace")


# Badges fetched in this process, by URL. A second read of the same version (a click back and
# forth) costs nothing; the file below carries them across runs.
_FIGURE_MEMORY: dict[str, dict] = {}


def figure_cache_file() -> pathlib.Path:
    """Where fetched badges are kept between runs.

    The tree itself is read-only and never written, so this lives in the launcher's own per-user
    data folder (`paths.user_data_dir()`).
    """
    return paths.user_data_dir() / "decomp_figures.json"


def _fresh_entries(path: pathlib.Path, now: float) -> dict:
    """The usable entries of the on-disk cache: `{url: {"at": ts, "rendered": {...}}}`."""
    try:
        raw = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}
    if not isinstance(raw, dict):
        return {}
    out: dict = {}
    for url, entry in raw.items():
        if not isinstance(entry, dict) or not isinstance(entry.get("rendered"), dict):
            continue
        try:
            age = now - float(entry.get("at", 0))
        except (TypeError, ValueError):
            continue
        if 0 <= age < FIGURE_CACHE_TTL:
            out[url] = {"at": float(entry["at"]), "rendered": entry["rendered"]}
    return out


def _write_cache(path: pathlib.Path, known: dict, fetched: dict, now: float) -> None:
    """Merge what was just fetched into the file. A cache that cannot be written is not a failure."""
    payload = {url: entry for url, entry in known.items() if now - entry["at"] < FIGURE_CACHE_TTL}
    payload.update(fetched)
    try:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(payload, indent=1, sort_keys=True), encoding="utf-8")
    except OSError:
        pass


def read_figures(urls: dict[str, str], fetch=None, timeout: float = FIGURE_TIMEOUT,
                 now: float | None = None, cache_file: pathlib.Path | None = None,
                 parse=None) -> dict:
    """{measure: what its URL renders} for the URLs given; {} when nothing could be read.

    Every URL is fetched at most once per FIGURE_CACHE_TTL: a copy that fresh is reused (this
    process's memory first, then the small JSON file beside the launcher's per-user data), an older
    one is fetched again, and a fetch that FAILS contributes nothing - the stale copy is not served
    in its place, because a figure from an unknown moment is not what the project publishes now. A
    fetch that raises is therefore an honest "unavailable", never an old or invented number.

    `parse` turns the text one URL serves into that measure's rendered value, and defaults to the
    badge SVG reader; the decomp.dev report path passes `parse_report` instead, so both sources share
    this one cache, timeout and failure story rather than growing a second one.

    `fetch` and `cache_file` are injectable, so this is exercised without a network.
    """
    fetch = fetch or fetch_text
    parse = parse or parse_badge_svg
    now = time.time() if now is None else now
    cache_file = cache_file if cache_file is not None else figure_cache_file()
    on_disk = _fresh_entries(cache_file, now)
    out: dict = {}
    fetched: dict = {}
    for measure, url in urls.items():
        entry = _FIGURE_MEMORY.get(url) or on_disk.get(url)
        rendered = entry["rendered"] if entry else None
        if not rendered:
            try:
                text = fetch(url, timeout)
            except Exception:            # noqa: BLE001 - any failure is the same answer: unavailable
                text = ""
            rendered = parse(text) if text else {}
            if rendered:
                _FIGURE_MEMORY[url] = {"at": now, "rendered": rendered}
                fetched[url] = {"at": now, "rendered": rendered}
        if rendered:
            out[measure] = dict(rendered)
    if fetched:
        _write_cache(cache_file, on_disk, fetched, now)
    return out


def page_for(serial: str) -> str:
    """The decomp.dev version page the README's own row links for `serial`, or ''."""
    return next((t.get("page", "") for t in regions() if t["binary"] == serial), "")


def read_report(serial: str, fetch=None, timeout: float = FIGURE_TIMEOUT,
                now: float | None = None, cache_file: pathlib.Path | None = None) -> dict:
    """The version's decomp.dev JSON report, read through the same fetch and cache the badges use.

    {} when the README's row links no page, when nothing could be fetched, or when what came back is
    not a report. The URL is the version's own page with `.json` on it, so a row that moves changes
    what is fetched - nothing is written down here. `report_url` is added to the result, so a caller
    can quote where the figures came from.
    """
    url = report_url(page_for(serial))
    if not url:
        return {}
    rendered = read_figures({"report": url}, fetch=fetch, timeout=timeout, now=now,
                            cache_file=cache_file,
                            parse=lambda text: parse_report(text, serial))
    rendered = rendered.get("report") or {}
    return {**rendered, "report_url": url} if rendered else {}


def published_figures(serial: str, fetch=None, timeout: float = FIGURE_TIMEOUT,
                      now: float | None = None, cache_file: pathlib.Path | None = None) -> dict:
    """A version's decomp.dev figures: its machine-readable report where there is one, badges otherwise.

    The report is preferred - it carries the counts (`3,606 / 3,606`) and the per-part breakdown as
    data, which is what lets a version the README's table has no row for show the same four rows the
    other versions show. Where there is no report (no page in the row, or nothing readable came
    back), the badge SVGs the row links are read instead, exactly as before.

    {} when the row carries neither, or nothing could be read: the caller says "unavailable", and no
    number of ours ever stands in for the project's.
    """
    report = read_report(serial, fetch=fetch, timeout=timeout, now=now, cache_file=cache_file)
    if report:
        return report
    return read_figures(badge_urls(serial), fetch=fetch, timeout=timeout, now=now,
                        cache_file=cache_file)


def identity() -> dict:
    """What the tree says it is: the regions it targets and the binaries it expects to reproduce."""
    out = {"regions": [], "hashes": []}
    # USA first, the one region order every other list in the launcher follows.
    for region, binary, folder in REGION_TARGETS:
        sha = ROOT / "config" / folder / f"{binary}.sha1"
        if sha.is_file():
            digest = _sha1_of(sha)
            out["regions"].append({"region": region, "binary": binary, "sha1": digest})
            if digest:
                out["hashes"].append(f"{binary} {digest[:12]}")
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
