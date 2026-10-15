#!/usr/bin/env python3
"""
Names a stage's functions after what calls them.

splat named the stages' functions by address (func_800A67B0). This names
each one by the place a table or a call gives it, and nothing else:

    stageEvents[k].start, .end        startEvent<id>, endEvent<id>  (the event's id;
                                        startEvents<a>To<b> for a run of events)
    FIELDSTG_scriptCommands[k]        createCommand<id>, handleCommand<id>
      .create, .handle                  (the command's id, for the stage's functions;
                                        handleCommands<a>To<b> for a run of commands)
    the update a function gives       update<Task>, and create<Task> for the function
      createTask(WithId)                that creates it (the task's struct, without
                                        Stage; not for StageTask or Task)
    the update of a StageTask or Task  updateEvent<id>, updateCommand<id>
      that an event's start or a
      command's create function creates
      (or of a task that several
      updates of the stage have)

A few functions no table or call names have names given by reading their
code (GIVEN). An event's or a command's name comes before the name of the
task it creates.
A function that two tables (or two calls) name differently, two functions given one name,
a name the stage already has (or includes from src/field/stages/common), and a
function only one version of a stage has keep their address names. The names
go into the C, both versions' config/<version>/stages/<stage>.txt and, for a
function FIELDSTG links against, its WSTAGnnn_ name in include/field/stages.h and
src/field/field_mode/ (and docs/) (then `make regenerate` each version). Only splat's names
(func_XXXXXXXX) are renamed, so it can be run again at any time:

    tools/name_stage_funcs.py [wstag200 ...]
"""
import argparse
import re
import sys

from name_stage_data import (
    CONFIG, ROOT, SRC, VERSIONS, definitions, elf_addresses, preprocess, stage_list, stage_sources, target, update_symbols,
)

AUTO = re.compile(r"^func_([0-9A-F]{8})$")
FUNCTION = re.compile(r"^(?:static\s+)?[\w\s\*]+?\b(\w+)\(([^;]*)\)\s*\{\s*$")
COMMANDS = ROOT / "src" / "field" / "field_mode" / "data" / "fieldstg.c"
LINKED = ([ROOT / "include" / "field" / "stages.h"] + sorted((ROOT / "src" / "field" / "field_mode").rglob("*.c"))
          + sorted((ROOT / "docs").glob("*.md")))
HEADER = "// functions named by what calls them (tools/name_stage_funcs.py)"
GENERIC_TASKS = {"StageTask", "Task"}

# Names given by reading the code, where no table or call names a function
# (the stages are those that share it, src/field/stages/common)
GIVEN = {
    ("wstag745", "wstag746"): {
        "func_800A4CA4": "updateDarkness",  # draws the screen black in TASK_DONE
        "func_800A4EB8": "createDarkness",
    },
    ("wstag480", "wstag481"): {"func_800A4CBC": "stepTileSequences"},
    ("wstag740", "wstag741"): {"func_800A5130": "drawPairSprite"},
    # the records that wander about their place, and fade out when told
    ("wstag460",): {
        "func_800A51D0": "fadeOutTileSolo",  # mode 2, wait 20
        "func_800A521C": "createTileSoloTask",  # createTileSolo takes an id
        "func_800A5630": "fadeOutTileDuo",
        "func_800A567C": "createTileDuoTask",
        "func_800A57C8": "isFarFromHome",  # more than dist from home, in x + y
        "func_800A5804": "getAngle",  # of (x, y), 0x100 a turn, from a table of tangents
        "func_800A5974": "moveWanderer",
        "func_800A5DCC": "fadeOutWanderer",
        "func_800A5E18": "createWandererTask",
    },
    ("wstag526",): {
        "func_800A503C": "updateClutTile",  # cycles the CLUT row of record 0x15
        "func_800A5228": "fadeOutClutTile",
        "func_800A5244": "createClutTile",
        "func_800A5274": "createClutTileTask",
        "func_800A5668": "fadeOutTileDuo",
        "func_800A56B4": "createTileDuoTask",
        "func_800A5800": "updateStillTile",  # shows record 0x16 with one frame
        "func_800A59B8": "fadeOutStillTile",
        "func_800A59D4": "createStillTile",
        "func_800A5A04": "createStillTileTask",
        "func_800A5B50": "isFarFromHome",
        "func_800A5B8C": "getAngle",
        "func_800A5CFC": "moveWanderPair",
        "func_800A6210": "fadeOutWanderPair",
        "func_800A625C": "createWanderPairTask",
    },
    # wstag310's two sets of six records, which wstag311 has unused
    ("wstag311",): {
        "func_800A5094": "updateTileSix",
        "func_800A5458": "restartTileSix",  # its first two records' animations, for mode 1 or 3
        "func_800A54C8": "createTileSixWithId",
        "func_800A54F8": "createTileSix",
        "func_800A5644": "updateTileSixW",
        "func_800A5A08": "restartTileSixW",
        "func_800A5A78": "createTileSixWWithId",
        "func_800A5AA8": "createTileSixW",
    },
    # the three records that a map object's event plays at a place
    ("wstag212",): {"func_800A4D80": "resetTileEffect"},
    ("wstag225", "wstag226", "wstag750"): {"func_800A4D7C": "resetTileEffect"},
    ("wstag925",): {"func_800A60D8": "resetTileEffect"},
    ("wstag226",): {
        "func_800A5028": "startTileEffect",  # at the place of map 0 or 1
        "func_800A50C0": "createTileEffectTask",  # createTileEffect takes an id
    },
    ("wstag934",): {"func_800A6218": "handleTileLift"},  # wstag261's handleCommand827, for maps 0x348 and 0x349
    # 36 sprites scrolled at 1/8 of the map, far behind it
    ("wstag375",): {"func_800A4CA8": "updateFarSprites", "func_800A4DFC": "createFarSprites"},
    ("wstag949",): {"func_800A5DE4": "updateFarSprites", "func_800A5F38": "createFarSprites"},
    # the timer of GAME.countdown, and the tile pair started in TASK_DONE
    ("wstag795",): {
        "func_800A50C4": "createTilePairDone",
        "func_800A50F8": "drawTimer",
        "func_800A5240": "updateTimer",
        "func_800A5410": "createTimer",
    },
    ("wstag800",): {
        "func_800A50B4": "createTilePair16Done",
        "func_800A5404": "drawTimer",
        "func_800A554C": "updateTimer",
        "func_800A571C": "createTimer",
    },
    ("wstag810",): {
        "func_800A5088": "createTilePairN1",  # of animations 1 and 2
        "func_800A50C4": "createTilePairN3",
        "func_800A5100": "createTilePairN5",
        "func_800A513C": "createTilePairNDone",
    },
    ("wstag805",): {
        "func_800A4E90": "stepTileLoop",
        "func_800A5310": "stepFloaterPart",
        "func_800A5528": "drawFloaterPart",
    },
    ("wstag785",): {"func_800A50CC": "drawActorMark"},
    ("wstag415",): {
        "func_800A4CB8": "stepFallBody",
        "func_800A4DAC": "drawFaller",
        "func_800A5320": "stepMoverBody",
        "func_800A59C8": "drawScroller",
    },
    ("wstag545",): {"func_800A4CDC": "stepHeldAnimation"},  # holds its last frame
    ("wstag680",): {"func_800A4DA0": "stepRiser"},
    ("wstag780",): {
        "func_800A4CA4": "drawFlyer",
        "func_800A4FF4": "drawGlow",
        "func_800A5344": "drawEffect",
        "func_800A5574": "stepTileLoopAnimation",
        "func_800A5804": "updateEffectSet",  # the six effects and the looping records
        "func_800A589C": "createEffectSet",
        "func_800A58C8": "releaseFlyers",
        "func_800A5A10": "drawFlyerGate",
        "func_800A5E80": "drawByteAnims",
    },
    ("wstag310",): {"func_800A5DBC": "drawEffect"},
    # the background drawn at 2/3 of the map's scroll
    ("wstag815",): {
        "func_800A4CA4": "drawBackground",
        "func_800A4E08": "updateBackground",
        "func_800A4F10": "createBackground",
    },
    ("wstag820",): {"func_800A5638": "createStageEffectWithId"},
}


def functions(text):
    """{name: (parameters, body lines)} of a file's functions"""
    out = {}
    lines = text.split("\n")
    name = None
    for line in lines:
        m = FUNCTION.match(line)
        if m and not line.startswith(" "):
            name = m.group(1)
            out[name] = (m.group(2), [])
            continue
        if line == "}":
            name = None
        elif name is not None:
            out[name][1].append(line)
    return out


def script_commands():
    """{stage: [(function, name)]} of FIELDSTG_scriptCommands"""
    out = {}
    text = COMMANDS.read_text()
    m = re.search(r"^ScriptCommand FIELDSTG_scriptCommands\[\] = \{\n(.*?)^\};", text, re.M | re.S)
    if not m:
        return out
    ids = {}
    for row in re.finditer(r"\{(\d+), (\w+), (\w+)\}", m.group(1)):
        command = int(row.group(1))
        for leaf, kind in ((row.group(2), "create"), (row.group(3), "handle")):
            f = re.fullmatch(r"(WSTAG\d+)_(\w+)", leaf)
            if f:
                ids.setdefault((f.group(1).lower(), f.group(2), kind), []).append(command)
    for (stage, function, kind), numbers in ids.items():
        first, last = min(numbers), max(numbers)
        if len(numbers) == 1:
            out.setdefault(stage, []).append((function, f"{kind}Command{first}"))
        elif sorted(numbers) == list(range(first, last + 1)):
            # the function of a run of commands
            out.setdefault(stage, []).append((function, f"{kind}Commands{first}To{last}"))
    return out


def structural_names(text, commands, stage):
    """[(function, name, from a table)] that the stage's tables and calls give its functions"""
    out = []
    funcs = functions(text)
    defs = definitions(text)

    events = defs.get("stageEvents")
    if events and isinstance(events[2], list):
        ids = {}
        for event in events[2]:
            if isinstance(event, list) and len(event) == 5 and re.fullmatch(r"\d+", event[0]):
                for leaf, kind in ((event[3], "start"), (event[4], "end")):
                    f = target(leaf)
                    if f in funcs:
                        ids.setdefault((f, kind), []).append(int(event[0]))
        for (f, kind), numbers in ids.items():
            first, last = min(numbers), max(numbers)
            if len(numbers) == 1:
                out.append((f, f"{kind}Event{first}", True))
            elif sorted(numbers) == list(range(first, last + 1)):
                # the start or the end of a run of events
                out.append((f, f"{kind}Events{first}To{last}", True))

    out.extend((f, name, True) for f, name in commands if f in funcs)
    for stages, given in GIVEN.items():
        if stage in stages:
            out.extend((f, name, True) for f, name in given.items() if f in funcs)

    tables = {f: name for f, name, _ in out}
    tasks = []
    for creator, (_, body) in funcs.items():
        calls = [m.group(1) for line in body for m in re.finditer(r"createTask(?:WithId)?\((\w+),", line)]
        if len(set(calls)) != 1 or calls[0] not in funcs:
            continue
        update = calls[0]
        m = re.match(r"(?:const\s+)?(\w+)\s*\*", funcs[update][0])
        if not m:
            continue
        if m.group(1) in GENERIC_TASKS:
            # the task of an event or a command
            starter = re.fullmatch(r"(?:start|create)(Event|Command)(\d+)", tables.get(creator, creator))
            if starter:
                out.append((update, f"update{starter.group(1)}{starter.group(2)}", False))
            continue
        task = re.sub(r"^Stage(?=[A-Z])", "", m.group(1))
        starter = re.fullmatch(r"(?:start|create)(Event|Command)(\d+)", tables.get(creator, creator))
        tasks.append((update, creator, task, starter))
    # A task that several updates of the stage have is named after the event
    # or the command whose start creates it
    for update, creator, task, starter in tasks:
        if starter and sum(other == task for _, _, other, _ in tasks) > 1:
            out.append((update, f"update{starter.group(1)}{starter.group(2)}", False))
            continue
        out.append((update, f"update{task}", False))
        out.append((creator, f"create{task}", False))
    return out


def included_names(text):
    """The functions and data the stage takes from src/field/stages/common"""
    names = set()
    for m in re.finditer(r'^#include "(common/\w+\.inc\.c)"', text, re.M):
        inc = (SRC / m.group(1)).read_text()
        names |= set(functions(inc)) | set(definitions(inc))
    return names


def stage_names(stage, text, versions, commands):
    """{function: name} for a stage, agreed by every version that builds it"""
    per_version = []
    taken = included_names(text)
    for v in versions:
        source = preprocess(text, v)
        taken |= set(functions(source)) | set(definitions(source))
        names = {}
        clash = set()
        for f, new, table in structural_names(source, commands, stage):
            if f in names and names[f][0] != new:
                if names[f][1] == table:
                    clash.add(f)
                continue
            names.setdefault(f, (new, table))
        per_version.append({f: n for f, (n, _) in names.items() if f not in clash})
    first = per_version[0]
    out = {f: n for f, n in first.items()
           if AUTO.match(f) and n not in taken and all(names.get(f) == n for names in per_version[1:])}
    counts = {}
    for n in out.values():
        counts[n] = counts.get(n, 0) + 1
    return {f: n for f, n in out.items() if counts[n] == 1}


def rename_linked(stage, renames):
    """Renames FIELDSTG's WSTAGnnn_ names of a stage's functions"""
    prefix = stage.upper() + "_"
    pattern = re.compile(r"\b" + prefix + r"(func_[0-9A-F]{8})\b")
    for path in LINKED:
        text = path.read_text()
        new = pattern.sub(lambda m: prefix + renames.get(m.group(1), m.group(1)), text)
        if new != text:
            path.write_text(new)


def main():
    parser = argparse.ArgumentParser(description="Names a stage's functions after what calls them.")
    parser.add_argument("stages", nargs="*")
    args = parser.parse_args()
    has = {v: stage_list(v) for v in VERSIONS}
    sources = stage_sources()
    stages = args.stages or sorted(sources)
    commands = script_commands()
    total = 0
    for stage in stages:
        path = sources[stage]
        text = path.read_text()
        versions = [v for v in VERSIONS if stage in has[v]]
        if not versions:
            continue
        renames = stage_names(stage, text, versions, commands.get(stage, []))
        if not renames:
            continue
        path.write_text(re.sub(r"\bfunc_[0-9A-F]{8}\b", lambda m: renames.get(m.group(0), m.group(0)), text))
        rename_linked(stage, renames)
        for v in versions:
            # A func_ name is us's address, or the version's own in a stage us hasn't;
            # else the version's symbol file has it, or its last link does
            if v == "us" or "us" not in versions:
                addresses = {f: int(AUTO.match(f).group(1), 16) for f in renames}
            else:
                elf = elf_addresses(v, stage)
                addresses = {f: elf.get(f, elf.get(n)) for f, n in renames.items()}
                addresses = {f: a for f, a in addresses.items() if a is not None}
            update_symbols(CONFIG / v / "stages" / f"{stage}.txt", renames, addresses, HEADER, " // type:func")
        total += len(renames)
    print(f"{total} functions named", file=sys.stderr)


if __name__ == "__main__":
    main()
