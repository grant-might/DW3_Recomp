# Editor support, included by the Makefile. Nothing here is part of the
# build or changes what it writes.

# compile_commands.json for clangd: every C file the version builds, with the
# build's own preprocessor flags (tools/compile_commands.py). It is for one
# version at a time; make VERSION=us compile_commands switches to the other.
compile_commands: export CC_CPPFLAGS = $(CPPFLAGS)
compile_commands: export CC_SRC = $(ALL_C_SRC)
compile_commands:
	$(PYTHON) tools/compile_commands.py

# The C's formatter (tools/format.sh, with .clang-format): format rewrites
# the files of src/ and include/ (or FILES), format-check lists the ones it
# would change and fails if there are any. CLANG_FORMAT is clang-format 18.
CLANG_FORMAT ?= clang-format
format format-check: export CLANG_FORMAT := $(CLANG_FORMAT)
format:
	tools/format.sh $(FILES)
format-check:
	tools/format.sh --check $(FILES)

.PHONY: compile_commands format format-check
