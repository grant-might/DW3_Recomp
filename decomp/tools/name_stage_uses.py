#!/usr/bin/env python3
"""
Names a stage's data after the function that uses it.

What the stage's tables don't name (tools/name_stage_data.py) is mostly the
data that its functions read: animation frames, spots and the like. This
names a datum that one named function alone uses (not a func_XXXXXXXX), or
one update function and others of its task, after it, by kind:

    AnimFrame, StageTileFrame, ...Frame       <function>Frames
    StageEffectSpot, ...Spot                  <function>Spots
    StageSeqStep                              <function>Steps
    StageQuad, StageQuadTexture               <function>Quads, <function>Textures
    StageFallParams                           <function>Params

numbered in the order the function first uses them (<function>Frames0,
<function>Frames1...) when it uses several of a kind, after those it has
already; and a datum that only one named table holds (and functions may
use), after the table and its (first) place in it: <table>_<i>, or
<table>_<i>_<k> for the k-th of several in one item.
A few numbers no kind names have names given by reading their code
(GIVEN). A datum that only one version of a stage has, or whose name the
stage already has, keeps its address name. The names go into the C and both
versions' config/<version>/stages/<stage>.txt (then `make regenerate` each
version). Only splat's names (D_XXXXXXXX) are renamed, so it can be run
again at any time:

    tools/name_stage_uses.py [wstag200 ...]
"""
import argparse
import re
import sys

from name_stage_data import (
    AUTO, CONFIG, SRC, VERSIONS, definitions, elf_addresses, preprocess, stage_list, stage_sources, update_symbols,
)
from name_stage_funcs import functions, included_names

HEADER = "// named by the function or table that uses them (tools/name_stage_uses.py)"
KINDS = (
    (re.compile(r"^(AnimFrame|Stage\w*Frame)$"), "Frames"),
    (re.compile(r"^Stage\w*Spot$"), "Spots"),
    (re.compile(r"^StageSeqStep$"), "Steps"),
    (re.compile(r"^StageQuad$"), "Quads"),
    (re.compile(r"^StageQuadTexture$"), "Textures"),
    (re.compile(r"^StageFallParams$"), "Params"),
)
TOKEN = re.compile(r"\b\w+\b")

# Names given by reading the code, for the numbers no kind above names
# (the stages are those whose data it is)
GIVEN = {
    # the menus' sprite frame for their count of options (5 to 8)
    ("wstag210",): {"D_800A6DE8": "updateListMenuCountFrames"},
    ("wstag924",): {"D_800A7A34": "updateEvent1602CountFrames", "D_800A7A44": "updateEvent1604CountFrames"},
    ("wstag935",): {"D_800A7B64": "updateEvent1616CountFrames", "D_800A7B74": "updateEvent1618CountFrames"},
    # the records of a tile effect: their y and sort y from its y, its places
    # by map (and the sound of each)
    ("wstag212",): {"D_800A7034": "tileEffectSortY", "D_800A7038": "tileEffectPlaces", "D_800A7044": "tileEffectSounds"},
    ("wstag225",): {"D_800A54F4": "tileEffectSortY", "D_800A54F8": "tileEffectY", "D_800A54FC": "tileEffectPlaces"},
    ("wstag226",): {"D_800A5788": "tileEffectSortY", "D_800A578C": "tileEffectY", "D_800A5790": "tileEffectPlaces"},
    ("wstag750",): {"D_800A610C": "tileEffectSortY", "D_800A6110": "tileEffectY", "D_800A6114": "tileEffectPlaces"},
    ("wstag925",): {"D_800A67C0": "tileEffectSortY", "D_800A67C4": "tileEffectY"},
    # how far the lift shakes each frame, 0x3E8 ending it
    ("wstag261",): {"D_800A555C": "tileLiftShake"},
    ("wstag934",): {"D_800A65B8": "tileLiftShake"},
    # u8 animations of frame and duration pairs
    ("wstag205",): {"D_800A551C": "spriteFieldAnims"},
    ("wstag780",): {
        "D_800A726C": "flyerFrames",  # by time, way up and animation
        "D_800A7274": "flyerStarts",  # x and y by way up, right and still
        "D_800A7CC0": "byteAnims",  # a duration of 0 jumps to the frame's index
    },
    # the actors of the three party slots
    ("wstag740",): {"D_800A598C": "partyActorIds"},
    ("wstag741",): {"D_800A59A0": "partyActorIds"},
    # the frames of the two records, running and fading
    ("wstag460",): {"D_800A6294": "tileDuoRecordFrames", "D_800A62F4": "wandererSpeeds", "D_800A62FC": "angleTangents"},
    ("wstag526",): {"D_800A6728": "wanderPairSpeeds", "D_800A6730": "angleTangents"},
    ("wstag375",): {"D_800A5284": "farSpriteSpots"},
    ("wstag949",): {"D_800A6174": "farSpriteSpots"},
    # the x of the timer's three digits
    ("wstag795",): {"D_800A5938": "timerDigitX"},
    ("wstag800",): {"D_800A65B4": "timerDigitX"},
    # when each floater of the chain moves, 0x1000 ending it
    ("wstag805",): {"D_800A609C": "floaterChainTimes"},
}


def kind(type_name):
    for pattern, suffix in KINDS:
        if pattern.match(type_name):
            return suffix
    return None


def leaves(init):
    """The top-level items of an initializer, each flattened to its text"""
    def flat(item):
        return item if isinstance(item, str) else " ".join(flat(i) for i in item)

    return [flat(i) for i in init] if isinstance(init, list) else [init]


def use_names(text, given=None):
    """[(datum, name)] that the stage's functions and tables give its data"""
    defs = definitions(text)
    funcs = functions(text)
    users = {}  # datum: set of (kind, user)
    first_use = {}  # (function, datum): the order of its first use in the function
    for f, (_, body) in funcs.items():
        order = 0
        for line in body:
            for tok in TOKEN.findall(line):
                if tok in defs:
                    users.setdefault(tok, set()).add(("function", f))
                    if (f, tok) not in first_use:
                        first_use[(f, tok)] = order
                        order += 1
    holders = {}  # datum: [(table, index)]
    for table, (_, _, init) in defs.items():
        for i, leaf in enumerate(leaves(init)):
            held = []
            for tok in TOKEN.findall(leaf):
                if tok in defs and tok != table and tok not in held:
                    held.append(tok)
            for k, tok in enumerate(held):
                users.setdefault(tok, set()).add(("table", table))
                # the k-th of several data in one item: <i>_<k>
                holders.setdefault(tok, []).append((table, f"{i}_{k}" if len(held) > 1 else str(i)))

    out = {d: n for d, n in (given or {}).items() if d in defs}
    by_function = {}
    for datum, (type_name, _, _) in defs.items():
        u = users.get(datum, set())
        suffix = kind(type_name)
        if datum in out or not AUTO.match(datum) or suffix is None or any(what != "function" for what, _ in u):
            continue
        named = sorted(f for _, f in u)
        if any(f.startswith("func_") for f in named):
            continue
        updates = [f for f in named if f.startswith("update")]
        if len(named) == 1:
            f = named[0]
        elif len(updates) == 1:
            # the data of a task that its other functions restart
            f = updates[0]
        else:
            continue
        by_function.setdefault((f, suffix), []).append(datum)
    for (f, suffix), data in by_function.items():
        data.sort(key=lambda d: first_use[(f, d)])
        # after the names the function's data has already
        had = [m.group(1) for d in defs for m in [re.fullmatch(re.escape(f + suffix) + r"(\d*)", d)] if m]
        start = max((int(n) + 1 for n in had if n), default=0)
        for n, datum in enumerate(data):
            out[datum] = f + suffix + (str(start + n) if len(data) > 1 or had else "")

    # then the data that only one table holds, once, after the table
    changed = True
    while changed:
        changed = False
        for datum in defs:
            if datum in out or not AUTO.match(datum):
                continue
            held = holders.get(datum, [])
            if not held or len({table for table, _ in held}) != 1:
                continue
            table, i = held[0]
            name = out.get(table, table)
            if AUTO.match(name) or name.startswith("func_"):
                continue
            out[datum] = f"{name}_{i}"
            changed = True
    return list(out.items())


def stage_names(stage, text, versions):
    """{datum: name} for a stage, agreed by every version that builds it"""
    per_version = []
    taken = included_names(text)
    given = {}
    for stages, names in GIVEN.items():
        if stage in stages:
            given.update(names)
    for v in versions:
        source = preprocess(text, v)
        taken |= set(functions(source)) | set(definitions(source))
        per_version.append(dict(use_names(source, given)))
    first = per_version[0]
    out = {d: n for d, n in first.items()
           if n not in taken and all(names.get(d) == n for names in per_version[1:])}
    counts = {}
    for n in out.values():
        counts[n] = counts.get(n, 0) + 1
    return {d: n for d, n in out.items() if counts[n] == 1}


def main():
    parser = argparse.ArgumentParser(description="Names a stage's data after the function that uses it.")
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
        path.write_text(re.sub(r"\bD_[0-9A-F]{8}\b", lambda m: renames.get(m.group(0), m.group(0)), text))
        for v in versions:
            # A D_ name is us's address, or the version's own in a stage us hasn't;
            # else the version's symbol file has it, or its last link does
            if v == "us" or "us" not in versions:
                addresses = {d: int(AUTO.match(d).group(1), 16) for d in renames}
            else:
                elf = elf_addresses(v, stage)
                addresses = {d: elf.get(d, elf.get(n)) for d, n in renames.items()}
                addresses = {d: a for d, a in addresses.items() if a is not None}
            update_symbols(CONFIG / v / "stages" / f"{stage}.txt", renames, addresses, HEADER)
        total += len(renames)
    print(f"{total} data named", file=sys.stderr)


if __name__ == "__main__":
    main()
