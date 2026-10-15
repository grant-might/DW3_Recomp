#!/usr/bin/env python3
"""
Writes the stages' sound ids, story points and flag codes by name: MUSIC and
the SOUND_ names (include/engine/sound.h, include/field/stage.h), the FIELD_PROGRESS_
names (include/field/field_map.h), and the condition codes
(include/engine/game_state.h).

- a field's or a battle's music (FieldState.music, the third word of a
  AreaBattle) becomes MUSIC(bank, n);
- a sound the stages play, given to SOUND.playSound, stopSound or
  fadeOutSound or in a table of them, becomes its SOUND_ name from
  include/engine/sound.h or include/field/stage.h;
- a story point that include/field/field_map.h names (FIELD_PROGRESS_), compared
  with or stored in GAME.progress or in a PROGRESS code, becomes its name;
- the codes of the FLAGS_00 conditions and actions (the lists of the
  characters and their talks, a StageSlot's conditions, and the codes given
  to FLAGS_00.checkCondition and applyAction) become FLAG(group, id),
  PROGRESS(n), ITEM(kind, id)... and CODES_END; a list too long for a line
  gets a pair a line.

It changes no bytes, and can be run again at any time:

    tools/stage_constants.py [src/field/stages/central_sector/wstag200.c ...]
"""
import argparse
import functools
import glob
import re
import sys

HEADERS = ("include/engine/sound.h", "include/field/stage.h")
NUMBER = r"0x[0-9A-Fa-f]+|\d+"
VERSIONS = {"US", "EU"}


def number(n):
    """n as the data is written: decimal under 10, else hex"""
    return str(n) if n < 10 else f"0x{n:X}"


def music(value):
    """MUSIC(bank, n) for a music id, or None"""
    v = int(value, 0)
    if v & 0xFE00FF00 != 0x60000000:
        return None
    return f"MUSIC({number(v >> 18 & 0x7F)}, {number(v & 0xFF)})"


@functools.lru_cache(maxsize=None)
def progress_names():
    """{value: FIELD_PROGRESS_ name} of include/field/field_map.h"""
    text = open("include/field/field_map.h").read()
    return {int(m.group(2), 0): m.group(1)
            for m in re.finditer(r"^#define (FIELD_PROGRESS_\w+) (" + NUMBER + r")\b", text, re.M)}


def sound_names():
    """{id: SOUND_ name} of the headers"""
    out = {}
    for path in HEADERS:
        for m in re.finditer(r"^#define (SOUND_\w+) (0x[0-9A-Fa-f]+)\b", open(path).read(), re.M):
            out.setdefault(int(m.group(2), 16), m.group(1))
    return out


FLAG_GROUPS = {0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x18, 0x1A, 0x1C, 0x20, 0x40}
SIMPLE_CODES = {0x60: "PROGRESS", 0x70: "SPECIAL", 0x72: "PARTY_STAT", 0x74: "EVENT_BATTLE",
                0x7E: "WARP_ARG", 0x90: "START_EVENT", 0x92: "CARD"}


def code(word):
    """A condition or action code by name (include/engine/game_state.h), or the word as it is"""
    if not re.fullmatch(NUMBER, word):
        return word
    v = int(word, 0)
    if v == 0xFFFF:
        return "CODES_END"
    if v > 0xFFFF:
        return word
    group = v >> 8 & 0xFE
    n = v & 0x1FF
    if group in FLAG_GROUPS:
        return f"FLAG({number(group)}, {number(n)})"
    if group == 0x60 and n in progress_names():
        return f"PROGRESS({progress_names()[n]})"
    if group in SIMPLE_CODES:
        return f"{SIMPLE_CODES[group]}({number(n)})"
    if group in (0x76, 0x78):
        return f"CARD_BATTLE({number(n)}, {(group - 0x76) // 2})"
    if 0x80 <= group <= 0x8E:
        return f"ITEM({v >> 9 & 7}, {number(n)})"
    return word


def split_words(body):
    """The comma-separated words of an initializer, commas in parentheses kept"""
    words = []
    depth = 0
    word = ""
    for c in body:
        if c == "," and depth == 0:
            words.append(word.strip())
            word = ""
            continue
        depth += (c == "(") - (c == ")")
        word += c
    words.append(word.strip())
    return [w for w in words if w]


def code_list(m):
    """A u16 list of (code, value) pairs ending with 0xFFFF, by name"""
    head, body = m.group(1), m.group(2)
    if "/*" in body:
        return m.group(0)
    if "#" in body:
        return versioned_code_list(m)
    words = split_words(body.replace("\n", " "))
    if len(words) % 2 != 1 or code(words[-1]) != "CODES_END":
        return m.group(0)
    pairs = [f"{code(words[i])}, {words[i + 1]}" for i in range(0, len(words) - 1, 2)]
    line = head + "{ " + ", ".join(pairs + ["CODES_END"]) + " };"
    if len(line) <= 100:
        return line
    return head + "{\n" + "".join(f"    {p},\n" for p in pairs) + "    CODES_END,\n};"


def versioned_code_list(m):
    """A code list with #if VERSION blocks, by name a line at a time"""
    head, body = m.group(1), m.group(2)
    lines = []
    parity = 0
    stack = []  # (parity at the #if, parity at the end of each branch, the versions the branches cover)
    for line in body.strip("\n").split("\n"):
        s = line.strip()
        if s.startswith("#if"):
            stack.append((parity, [], set(re.findall(r"VERSION_(\w+)", s))))
        elif s.startswith(("#elif", "#else", "#endif")):
            start, ends, covered = stack[-1]
            ends.append(parity)
            parity = start
            covered.update(re.findall(r"VERSION_(\w+)", s) if s.startswith("#elif") else [])
            if s.startswith("#else"):
                covered.update(VERSIONS)
            if s.startswith("#endif"):
                stack.pop()
                if not VERSIONS <= covered:
                    ends.append(start)  # a version that has none of the branches
                if len(set(ends)) != 1:
                    return m.group(0)
                parity = ends[0]
        if s.startswith("#"):
            lines.append(s)
            continue
        words = split_words(s)
        lines.append("    " + ", ".join(code(w) if (parity + i) % 2 == 0 else w for i, w in enumerate(words)) + ",")
        parity = (parity + len(words)) % 2
    if stack or parity != 1:
        return m.group(0)
    return head + "{\n" + "".join(line + "\n" for line in lines) + "};"


def code_lists(text):
    """The names of the u16 lists the characters and their talks point at"""
    names = set()
    for m in re.finditer(r"^FieldActorEntry \w+ = \{ (\w+),", text, re.M):
        names.add(m.group(1))
    for m in re.finditer(r"^FieldTalk \w+\[\] = \{\n((?:(?:    \{[^\n]*\},|#[^\n]*)\n)*)\};", text, re.M):
        for row in re.finditer(r"\{ (\w+), (\w+),", m.group(1)):
            names.update(row.groups())
    names.discard("NULL")
    return names


def rewrite(text, sounds):
    def music_store(m):
        new = music(m.group(2))
        return m.group(1) + new if new else m.group(0)

    text = re.sub(r"(\.music = )(" + NUMBER + r")(?=;)", music_store, text)

    def battle(m):
        new = music(m.group(2))
        return m.group(1) + new + m.group(3) if new else m.group(0)

    text = re.sub(r"(^AreaBattle \w+ = \{ [^,]+, [^,]+, )(" + NUMBER + r")( \};)", battle, text, flags=re.M)

    def call(m):
        v = int(m.group(2), 0)
        return m.group(1) + sounds[v] + ")" if v in sounds else m.group(0)

    text = re.sub(r"((?:playSound|stopSound|fadeOutSound)\()(" + NUMBER + r")\)", call, text)

    points = progress_names()

    def progress(m):
        v = int(m.group(2), 0)
        return m.group(1) + points[v] if v in points else m.group(0)

    text = re.sub(r"(GAME\.progress (?:==|!=|=) )(" + NUMBER + r")\b", progress, text)
    text = re.sub(r"(\bPROGRESS\()(" + NUMBER + r")(?=\))", progress, text)

    # a table of sounds that a function plays: s32 D_[] = { ids }
    def table(m):
        body = m.group(2)
        ids = [w.strip() for w in body.split(",") if w.strip()]
        if not ids or any(not re.fullmatch(NUMBER, w) or int(w, 0) not in sounds for w in ids):
            return m.group(0)
        return m.group(1) + ", ".join(sounds[int(w, 0)] for w in ids) + ",\n};"

    text = re.sub(r"(^s32 \w+\[\] = \{\n    )([^}]*?),\n\};", table, text, flags=re.M)

    lists = code_lists(text)
    if lists:
        pattern = r"^(u16 (?:" + "|".join(sorted(lists)) + r")\[\] = )\{\s*([^}]*?),?\s*\};"
        text = re.sub(pattern, code_list, text, flags=re.M)

    # a StageSlot's two conditions
    def slot(m):
        return "{ { { %s, %s }, { %s, %s } }," % (code(m.group(1)), m.group(2), code(m.group(3)), m.group(4))

    def slots(m):
        return re.sub(r"\{ \{ \{ (\w+), (\w+) \}, \{ (\w+), (\w+) \} \},", slot, m.group(0))

    text = re.sub(r"^StageSlot \w+\[\] = \{\n.*?^\};", slots, text, flags=re.M | re.S)

    def flag_call(m):
        return m.group(1) + code(m.group(2)) + ","

    text = re.sub(r"(FLAGS_00\.(?:checkCondition|applyAction)\()(" + NUMBER + r"),", flag_call, text)
    return text


def main():
    parser = argparse.ArgumentParser(description="Writes the stages' sound ids and flag codes by name.")
    parser.add_argument("files", nargs="*")
    args = parser.parse_args()
    files = args.files or sorted(glob.glob("src/field/stages/**/*.c", recursive=True))
    sounds = sound_names()
    changed = 0
    for path in files:
        text = open(path).read()
        new = rewrite(text, sounds)
        if new != text:
            open(path, "w").write(new)
            changed += 1
    print(f"{changed} files changed", file=sys.stderr)


if __name__ == "__main__":
    main()
