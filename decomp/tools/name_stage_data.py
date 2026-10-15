#!/usr/bin/env python3
"""
Names a stage's data after where its tables put it.

The tables setupStage gives FIELDSTG (include/field/stage.h) point at the rest of a
stage's data, which splat named by address (D_800A5424). This names each
datum by the place the structure gives it, and nothing else:

    stageActors[i]                    actor<i>        (FieldActorEntry)
      .conditions                     actor<i>Conditions
      .talks                          actor<i>Talks   (FieldTalk[])
        [j].conditions, [j].actions   actor<i>Talk<j>Conditions, ...Actions
    stageBattles[p].battles[a]        area<a>Battles  (BattleList)
      .battles[b]                     area<a>Battle<b>
    the FieldBattles tables of a      stageBattles<k>, and battles<k>Area<a>...
      stage without stageBattles
    the StageSlot tables of a stage   stageSlots<k>
      without stageSlots
    stageEvents[k].script             script<id>      (the event's id)
    placePoints[k], ids a and b       placePoints<a>_<b>  (StagePoints)
      .points, then each next         placePoints<a>_<b>Point<n> (a chain that
                                        several places share: the first's)
    the FieldTalk tables left (the    talks<k>, talks<k>Talk<j>Conditions...
      characters differ between       and actorConditions<k> for the
      versions, or share them)        characters' conditions, in order

A stage with several places in stageBattles names their battles after the
place's id: place<id>Area<a>Battles. A datum that two of these name
differently, or that only one version of a stage has, keeps its address
name. The names go into the C, config/us/stages/<stage>.txt and
config/eu/stages/<stage>.txt, so that splat's disassembly of the original
has them too (then `make regenerate` each version); a datum the European
symbol file doesn't list yet gets the address of build/eu/<stage>.elf. Only splat's names
(D_XXXXXXXX) are renamed, so it can be run again at any time:

    tools/name_stage_data.py [wstag200 ...]
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src" / "field" / "stages"
CONFIG = ROOT / "config"
VERSIONS = ("us", "eu")
AUTO = re.compile(r"^D_([0-9A-F]{8})$")
DEFINITION = re.compile(
    r"^(?:static\s+)?(?:const\s+)?(\w+)\s*(\**)\s*(\w+)\s*((?:\[[^\]]*\])*)\s*=\s*",
    re.M,
)


def stage_sources():
    """{stage: its C file}: src/field/stages/<area>/wstag###.c (tools/stage_areas.py),
    or src/field/stages/wstag###.c for a stage in no area"""
    paths = sorted(SRC.glob("wstag[0-9][0-9][0-9].c")) + sorted(SRC.glob("*/wstag[0-9][0-9][0-9].c"))
    return {p.stem: p for p in paths}


def stage_list(version):
    """The stages a version has (config/<version>/stages.txt)"""
    out = set()
    for line in (CONFIG / version / "stages.txt").read_text().splitlines():
        words = line.split("#")[0].split()
        if words:
            out.add(words[0].lower())
    return out


def preprocess(text, version):
    """The text with the #if VERSION_* blocks of other versions blanked"""
    out = []
    stack = []  # (this branch taken, a branch was taken)
    for line in text.split("\n"):
        s = line.strip()
        m = re.match(r"#(if|elif)\s+(.*)$", s)
        if m:
            cond = eval_condition(m.group(2), version)
            if m.group(1) == "if":
                stack.append([cond, cond])
            else:
                top = stack[-1]
                top[0] = cond and not top[1]
                top[1] = top[1] or cond
            out.append("")
            continue
        if s.startswith("#else"):
            top = stack[-1]
            top[0] = not top[1]
            top[1] = True
            out.append("")
            continue
        if s.startswith("#endif"):
            stack.pop()
            out.append("")
            continue
        out.append(line if all(t[0] for t in stack) else "")
    return "\n".join(out)


def eval_condition(cond, version):
    expr = cond.replace("||", " or ").replace("&&", " and ").replace("!", " not ")
    expr = re.sub(r"VERSION_(\w+)", lambda m: str(m.group(1).lower() == version), expr)
    return bool(eval(expr, {}, {}))


def parse_initializer(text, pos):
    """Parses a C initializer at pos: a nested list of leaf strings, and the end"""
    def value(i):
        while text[i].isspace():
            i += 1
        if text[i] == "{":
            items = []
            i += 1
            while True:
                while text[i].isspace():
                    i += 1
                if text[i] == "}":
                    return items, i + 1
                item, i = value(i)
                items.append(item)
                while text[i].isspace():
                    i += 1
                if text[i] == ",":
                    i += 1
        start = i
        depth = 0
        while True:
            c = text[i]
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
            elif depth == 0 and c in ",};":
                return text[start:i].strip(), i
            i += 1

    return value(pos)


def definitions(text):
    """{name: (type, is a pointer, initializer)} of a file's data"""
    out = {}
    for m in DEFINITION.finditer(text):
        try:
            init, _ = parse_initializer(text, m.end())
        except IndexError:
            continue
        out[m.group(3)] = (m.group(1), m.group(2) != "", init)
    return out


def target(leaf):
    """The symbol a pointer initializer names (&name or name), or None"""
    if not isinstance(leaf, str):
        return None
    m = re.fullmatch(r"&?\s*(\w+)", leaf)
    if m and m.group(1) != "NULL":
        return m.group(1)
    return None


def structural_names(defs):
    """[(symbol, name)] that the stage's tables give its data, in walk order"""
    out = []

    def name(sym, new):
        if sym is not None and sym in defs:
            out.append((sym, new))

    actors = defs.get("stageActors")
    if actors and isinstance(actors[2], list):
        for i, leaf in enumerate(actors[2]):
            actor = target(leaf)
            if actor is None or actor not in defs:
                continue
            name(actor, f"actor{i}")
            entry = defs[actor][2]
            if defs[actor][0] != "FieldActorEntry" or not isinstance(entry, list) or len(entry) < 2:
                continue
            name(target(entry[0]), f"actor{i}Conditions")
            talks = target(entry[1])
            if talks is None or talks not in defs:
                continue
            name(talks, f"actor{i}Talks")
            for j, talk in enumerate(defs[talks][2]):
                if isinstance(talk, list) and len(talk) >= 2:
                    name(target(talk[0]), f"actor{i}Talk{j}Conditions")
                    name(target(talk[1]), f"actor{i}Talk{j}Actions")

    def name_battles(table, base):
        places = [p for p in table if isinstance(p, list) and len(p) == 4 and isinstance(p[3], list)]
        ids = [p[1] for p in places]
        if len(places) != len(table) or (len(places) > 1 and len(set(ids)) != len(ids)):
            return
        for place in places:
            prefix = base + ("area" if len(places) == 1 else f"place{int(place[1], 0)}Area")
            if base:
                prefix = prefix[:len(base)] + prefix[len(base)].upper() + prefix[len(base) + 1:]
            for a, leaf in enumerate(place[3]):
                lst = target(leaf)
                if lst is None or lst not in defs or defs[lst][0] != "BattleList":
                    continue
                name(lst, f"{prefix}{a}Battles")
                init = defs[lst][2]
                if isinstance(init, list) and len(init) == 2 and isinstance(init[1], list):
                    for b, bleaf in enumerate(init[1]):
                        name(target(bleaf), f"{prefix}{a}Battle{b}")

    battles = defs.get("stageBattles")
    if battles and isinstance(battles[2], list):
        name_battles(battles[2], "")
    else:
        # A stage that picks one of several tables (by progress) names them in order
        tables = [s for s, d in defs.items() if d[0] == "FieldBattles" and isinstance(d[2], list)]
        for k, table in enumerate(tables):
            name(table, f"stageBattles{k}")
            name_battles(defs[table][2], f"battles{k}")

    if "stageSlots" not in defs:
        # A stage that picks one of several slot tables names them in order
        tables = [s for s, d in defs.items() if d[0] == "StageSlot" and isinstance(d[2], list)]
        for k, table in enumerate(tables):
            name(table, f"stageSlots{k}")

    events = defs.get("stageEvents")
    if events and isinstance(events[2], list):
        for event in events[2]:
            if isinstance(event, list) and len(event) == 5 and re.fullmatch(r"\d+", event[0]):
                name(target(event[1]), f"script{int(event[0])}")

    points = defs.get("placePoints")
    if points and isinstance(points[2], list):
        places = [target(leaf) for leaf in points[2]]
        places = [p for p in places if p in defs and defs[p][0] == "StagePoints"
                  and isinstance(defs[p][2], list) and len(defs[p][2]) == 3]
        keys = [f"{int(defs[p][2][0], 0)}_{int(defs[p][2][1], 0)}" for p in places]
        # a chain that several places share is named after the first of them
        seen = set()
        for place, key in zip(places, keys):
            if keys.count(key) > 1:
                continue
            name(place, f"placePoints{key}")
            point = target(defs[place][2][2])
            n = 0
            while point is not None and point in defs and point not in seen:
                seen.add(point)
                name(point, f"placePoints{key}Point{n}")
                n += 1
                pinit = defs[point][2]
                point = target(pinit[6]) if isinstance(pinit, list) and len(pinit) == 7 else None
    return out


def shared_lists(defs, named):
    """The characters' lists that the stage's tables don't name (a stage whose
    characters differ between versions, or share them): its FieldTalk tables
    and its characters' conditions, in order"""
    talks = [s for s, d in defs.items() if d[0] == "FieldTalk" and AUTO.match(s) and s not in named]
    conditions = set()
    for d in defs.values():
        if d[0] == "FieldActorEntry" and isinstance(d[2], list) and d[2]:
            sym = target(d[2][0])
            if sym in defs and defs[sym][0] == "u16" and AUTO.match(sym) and sym not in named:
                conditions.add(sym)
    return talks, [s for s in defs if s in conditions]


def shared_names(defs, talks, conditions):
    """[(symbol, name)] for those lists that every version has: talks<k> for
    the FieldTalk tables, with talks<k>Talk<j>Conditions and ...Actions, and
    actorConditions<k> for the characters' conditions"""
    out = []
    for k, table in enumerate(talks):
        out.append((table, f"talks{k}"))
        for j, talk in enumerate(defs[table][2] if isinstance(defs[table][2], list) else []):
            if isinstance(talk, list) and len(talk) >= 2:
                for leaf, part in ((talk[0], "Conditions"), (talk[1], "Actions")):
                    sym = target(leaf)
                    if sym in defs and AUTO.match(sym):
                        out.append((sym, f"talks{k}Talk{j}{part}"))
    out.extend((sym, f"actorConditions{k}") for k, sym in enumerate(conditions))
    return out


def stage_names(stage, text, versions):
    """{symbol: name} for a stage, agreed by every version that builds it"""
    per_version = []
    for v in versions:
        defs = definitions(preprocess(text, v))
        names = {}
        clash = set()
        for sym, new in structural_names(defs):
            if sym in names and names[sym] != new:
                clash.add(sym)
            names.setdefault(sym, new)
        for sym in clash:
            del names[sym]
        per_version.append((defs, names))
    out = {}
    taken = set()
    for v in versions:
        taken |= set(definitions(preprocess(text, v)))
    first_defs, first = per_version[0]
    for sym, new in first.items():
        if not AUTO.match(sym):
            continue
        if any(names.get(sym) != new for _, names in per_version[1:]):
            continue
        if new in taken:
            continue
        out[sym] = new
    # then the characters' lists the tables leave, if every version names them alike
    shared = []
    lists = [shared_lists(defs, out) for defs, _ in per_version]
    talks = [s for s in lists[0][0] if all(s in t for t, _ in lists[1:])]
    conditions = [s for s in lists[0][1] if all(s in c for _, c in lists[1:])]
    for defs, _ in per_version:
        names = {}
        for sym, new in shared_names(defs, talks, conditions):
            names.setdefault(sym, new)
        shared.append(names)
    for sym, new in shared[0].items():
        if all(names.get(sym) == new for names in shared[1:]) and new not in taken:
            out[sym] = new
    # Two symbols given one name (a table pointed at twice) keep theirs
    counts = {}
    for new in out.values():
        counts[new] = counts.get(new, 0) + 1
    return {s: n for s, n in out.items() if counts[n] == 1}


SYMBOL_LINE = re.compile(r"^(\s*)(\w+)(\s*=\s*)(0x[0-9A-Fa-f]+)(.*)$")
HEADER = "// named by their place in the stage's tables (tools/name_stage_data.py)"


def elf_addresses(version, stage):
    """{symbol: address} of a stage's last link, or {} if it hasn't been built"""
    elf = ROOT / "build" / version / f"{stage}.elf"
    if not elf.exists():
        return {}
    out = subprocess.run(["mipsel-linux-gnu-nm", str(elf)], capture_output=True, text=True).stdout
    return {w[2]: int(w[0], 16) for w in (l.split() for l in out.splitlines()) if len(w) == 3}


def update_symbols(path, renames, addresses, header=HEADER, suffix=""):
    """Renames symbols in a stage's symbol file, adding those it hasn't at addresses
    under header (with suffix, such as // type:func)"""
    lines = path.read_text().split("\n") if path.exists() else []
    if lines and lines[-1] == "":
        lines.pop()
    present = set()
    for i, line in enumerate(lines):
        m = SYMBOL_LINE.match(line)
        if m and m.group(2) in renames:
            present.add(m.group(2))
            lines[i] = m.group(1) + renames[m.group(2)] + m.group(3) + m.group(4) + m.group(5)
    added = sorted((addresses[s], renames[s]) for s in renames if s not in present and s in addresses)
    if added:
        if header not in lines:
            if lines:
                lines.append("")
            lines.append(header)
        at = len(lines)
        for i, line in enumerate(lines):
            if line == header:
                at = i + 1
                while at < len(lines) and SYMBOL_LINE.match(lines[at]):
                    at += 1
                break
        block = [f"{new} = 0x{addr:08X};{suffix}" for addr, new in added]
        lines[at:at] = block
        # keep the block in address order
        start = lines.index(header) + 1
        end = start
        while end < len(lines) and SYMBOL_LINE.match(lines[end]):
            end += 1
        lines[start:end] = sorted(lines[start:end], key=lambda l: int(SYMBOL_LINE.match(l).group(4), 16))
    path.write_text("\n".join(lines) + "\n")


def main():
    parser = argparse.ArgumentParser(description="Names a stage's data after where its tables put it.")
    parser.add_argument("stages", nargs="*")
    args = parser.parse_args()
    has = {v: stage_list(v) for v in VERSIONS}
    sources = stage_sources()
    stages = args.stages or sorted(sources)
    total = 0
    for stage in stages:
        path = sources[stage]
        text = path.read_text()
        versions = [v for v in VERSIONS if stage in has[v]]
        if not versions:
            continue
        renames = stage_names(stage, text, versions)
        if not renames:
            continue
        new_text = re.sub(r"\bD_[0-9A-F]{8}\b", lambda m: renames.get(m.group(0), m.group(0)), text)
        path.write_text(new_text)
        for v in versions:
            # A D_ name is us's address, or the version's own in a stage us hasn't;
            # else the version's symbol file has it, or its last link does
            if v == "us" or "us" not in versions:
                addresses = {s: int(AUTO.match(s).group(1), 16) for s in renames}
            else:
                elf = elf_addresses(v, stage)
                addresses = {s: elf.get(s, elf.get(n)) for s, n in renames.items()}
                addresses = {s: a for s, a in addresses.items() if a is not None}
            update_symbols(CONFIG / v / "stages" / f"{stage}.txt", renames, addresses)
        total += len(renames)
    print(f"{total} data named", file=sys.stderr)


if __name__ == "__main__":
    main()
