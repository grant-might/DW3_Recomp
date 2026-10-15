#!/bin/sh
# Format the game's C with clang-format and .clang-format, or check it:
#
#   tools/format.sh [--check] [FILE...]
#
# Without files it takes every .c and .h file of src/ and include/ that git
# has. --check changes nothing: it lists the files that formatting would
# change and fails if there are any. CLANG_FORMAT names the program
# (clang-format by default); .clang-format is written for clang-format 18.
#
# Two things are left as they are:
# - the files of IGNORE, below;
# - the data: a definition at the top of a file with an initializer
#   ("TYPE NAME = ..."), to its ";". clang-format would put every element
#   of a list that ends in a comma on a line of its own, and the tables
#   keep the rows they were written in.

set -e

CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"

# Files clang-format 18 would change for the worse:
# - include/engine/task.h: it indents the comments of TASK_HEADER's fields
#   although SkipMacroDefinitionBody leaves a macro's body alone
# - battle_mode/hud.c, field_mode/actor_icon.c, text/text_tools.c: it reads
#   a pointer to an array, "u8 (*anim)[2]", or a cast to a function pointer,
#   "(s32 (*)())", as a call and drops the space
# - battle_mode/enemy_attack.c: it indents the European version's "if"
#   after "else" and "#endif" as the else's body
# - field_mode/commands.c: it writes a case range as "case A... B:"
IGNORE='
include/engine/task.h
src/battle/battle_mode/enemy_attack.c
src/battle/battle_mode/hud.c
src/engine/text/text_tools.c
src/field/field_mode/actor_icon.c
src/field/field_mode/commands.c
'

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"
cd "$TOP"

check=
if [ "$1" = --check ]; then
	check=1
	shift
fi

if ! command -v "$CLANG_FORMAT" > /dev/null; then
	echo "$CLANG_FORMAT not found: install clang-format 18, or set CLANG_FORMAT" >&2
	exit 1
fi

[ $# -gt 0 ] || set -- $(git ls-files 'src/*.c' 'src/*.h' 'include/*.h')

# The lines of FILE outside its top-level definitions with an initializer,
# as --lines=N:M: such a definition starts in the first column with a name
# and has an "=", and ends with the ";" after its braces close
code_lines() {
	awk '
	function flush(end) {
		if (end >= from)
			printf " --lines=%d:%d", from, end
	}
	# the line without its strings, characters and comments
	function code(line) {
		gsub(/"([^"\\]|\\.)*"/, "", line)
		gsub(/'"'"'([^'"'"'\\]|\\.)*'"'"'/, "", line)
		gsub(/\/\*([^*]|\*+[^*\/])*\*+\//, "", line)
		sub(/\/\*.*/, "", line)
		return line
	}
	BEGIN { from = 1 }
	!data && /^[A-Za-z_]/ && code($0) ~ /=/ {
		flush(NR - 1)
		data = 1
		depth = 0
	}
	data {
		line = code($0)
		depth += gsub(/\{/, "", line)
		depth -= gsub(/\}/, "", line)
		if (depth <= 0 && line ~ /;/) {
			data = 0
			from = NR + 1
		}
		next
	}
	END {
		if (!data)
			flush(NR)
	}' "$1"
}

status=0
for file in "$@"; do
	case "$IGNORE" in
	*"
$file
"*) continue ;;
	esac
	lines=$(code_lines "$file")
	# a file that is all data
	[ -n "$lines" ] || continue
	if [ -n "$check" ]; then
		if ! "$CLANG_FORMAT" --style=file $lines "$file" | cmp -s - "$file"; then
			echo "$file"
			status=1
		fi
	else
		"$CLANG_FORMAT" --style=file -i $lines "$file"
	fi
done
if [ -n "$check" ] && [ $status != 0 ]; then
	echo "these files aren't formatted: run tools/format.sh (make format)" >&2
fi
exit $status
