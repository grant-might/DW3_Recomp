#!/usr/bin/env python3
"""
Renames fields of the structs that the stages and FIELDSTG share, where
the code reads them through a pointer of that type.

A field such as StageTile.unk2 is read as object->unk2, and unk2 is a field
of many structs, so a plain rename can't change it. This renames OLD to NEW
after a pointer that the same function (or file scope) declares with the
struct's type, StageTile *object, in src/field/stages/ and src/field/field_mode/; the
struct's declaration is changed by hand. It changes no bytes, and can be run
again at any time:

    tools/stage_struct_fields.py
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
FILES = sorted((ROOT / "src" / "field").rglob("*.c"))

# struct: {old field: new field}, in include/field/field_map.h and include/field/stage.h
FIELDS = {
    "StageTile": {"unk2": "margin", "unkE": "sortY"},
    "StageActor": {"unk64": "z", "unk74": "hasShadow"},
}

# a line that ends a function or a file-scope definition
END = re.compile(r"^\}", re.M)


def scopes(text):
    """The text cut after each line that starts with }: one function each"""
    out, start = [], 0
    for m in END.finditer(text):
        end = text.find("\n", m.end())
        end = len(text) if end < 0 else end + 1
        out.append(text[start:end])
        start = end
    out.append(text[start:])
    return out


def rename(scope):
    for struct, fields in FIELDS.items():
        names = set(re.findall(rf"\b{struct}\s*\*\s*(\w+)", scope))
        for name in names:
            for old, new in fields.items():
                scope = re.sub(rf"\b({re.escape(name)}->){old}\b", rf"\g<1>{new}", scope)
    return scope


def main():
    changed = 0
    for path in FILES:
        text = path.read_text()
        new = "".join(rename(s) for s in scopes(text))
        if new != text:
            path.write_text(new)
            changed += 1
    print(f"{changed} files changed", file=sys.stderr)


if __name__ == "__main__":
    main()
