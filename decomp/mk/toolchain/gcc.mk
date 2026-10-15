# make TOOLCHAIN=gcc: the game's C built with a modern GCC, the binutils'
# own (CROSS, mipsel-linux-gnu-gcc, GCC 12 on Ubuntu 24.04), into
# build/<version>/gcc/. It doesn't match: it links the same objects' code and
# data, in the same order and with the same memory map, as the matching build,
# which is what makes it a working game (docs/toolchain.md). The PsyQ
# libraries and the header are the same assembly.

CC := $(CROSS)gcc

# The PlayStation's R3000A: MIPS I, o32, little-endian, no FPU. Ubuntu's GCC
# targets Linux, so it would make position-independent code that calls
# through the GOT, and protect the stack with a guard the game hasn't. The
# assembler leaves the sections their size, as in ASFLAGS.
GCC_ARCHFLAGS := -march=r3000 -mtune=r3000 -mabi=32 -EL -msoft-float -mfp32 \
	-mno-abicalls -fno-pic -fno-pie -fno-stack-protector -Wa,-no-pad-sections

# -Os: each binary must fit in its file on the disc, as the CD reader loads
# the number of sectors that the executable's file table gives
# (getFileSectorCount), and an overlay in its memory (tools/fitcheck.py).
# -G0: nothing goes through $gp, so no small data needs a place of its own.
# What GCC 2.8.1 did that GCC 12 doesn't by default:
# -std=gnu89: the C dialect, with its old-style definitions.
# -fsigned-char, -fno-builtin: as the matching build (docs/toolchain.md); the
#   game's memcpy, strcpy and the like are PsyQ's.
# -fno-strict-aliasing: the game reads the same memory through different
#   types, which GCC 2.8.1 never took as a reason to reorder.
# -fwrapv: signed arithmetic wraps, as the R3000's addu does.
# -fno-delete-null-pointer-checks: address 0 is RAM, the kernel's, so a
#   pointer that was read through may still be checked against NULL.
# -fno-aggressive-loop-optimizations: GCC 12 bounds loops by the arrays they
#   index; some of the game's array sizes are only as large as what is known.
# -mno-check-zero-division: the game's divisions have no divide-by-zero
#   check (maspsx runs without --expand-div), GCC 12's would stop the game.
# -fno-zero-initialized-in-bss: a variable initialized to 0 stays in .data,
#   as GCC 2.8.1 put it: an overlay's .bss is neither loaded nor cleared.
# -fno-common: a variable is defined once, in its unit; maspsx's commons
#   only fix addresses (src/engine/data/game_bss.c).
# -fno-toplevel-reorder: each unit's functions and data in the order of the
#   C, as GCC 2.8.1 emitted them.
GCC_CFLAGS := $(GCC_ARCHFLAGS) -Os -G0 -std=gnu89 -fsigned-char -fno-builtin \
	-fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks \
	-fno-aggressive-loop-optimizations -mno-check-zero-division \
	-fno-zero-initialized-in-bss -fno-common -fno-toplevel-reorder \
	-Wall -Wno-unused

# the matching build's preprocessor flags, but for those that make the
# binutils' cpp pass for GCC 2.8.1's
GCC_CPPFLAGS = $(filter-out -undef -D__GNUC__=% -D__GNUC_MINOR__=%,$(CPPFLAGS))

$(BUILDDIR)/%.c.o: %.c mk/toolchain/gcc.mk
	@mkdir -p $(dir $@)
	$(CC) -c $(GCC_CFLAGS) $(GCC_CPPFLAGS) -MMD -MP -MT $@ -MF $(@:.o=.d) -o $@ $<

# splat's linker scripts name the matching build's objects, with each one's
# .text, .rodata, .data and .bss: these name this build's, and take the
# sections GCC 12 splits from those too (.text.startup for main,
# .rodata.str1.4 for strings), which would be discarded. The units that only
# have small data (.sdata, src/engine/data/heap_start.c and the like) have it
# in .data at -G0, which goes where their .sdata went.
# The executable's tail, the boot image, follows its .bss, which ends at
# OVERLAY_VRAM in the original: main() reads the image at STAGE_VRAM
# (SUB_OVERLAY_ADDRESS, src/engine/system/main.c), so the tail stays at
# OVERLAY_VRAM, after zeros in the file up to there.
LDSCRIPT_DIR := $(BUILDDIR)/ld
$(LDSCRIPT_DIR)/%.ld: $(GENDIR)/%.ld mk/toolchain/gcc.mk
	@mkdir -p $(dir $@)
	sed -e 's|^\( *\)build/$(VERSION)/|\1$(BUILDDIR)/|' -e 's|\.o(\(\.[a-z]*\));|.o(\1 \1.*);|' \
	    -e 's|\.o(\.sdata \.sdata\.\*);|.o(.sdata .sdata.* .data .data.*);|' \
	    -e 's|^\( *\)tail_ROM_START = __romPos;|\1__romPos += OVERLAY_VRAM - main_VRAM_END;\n&|' \
	    -e 's|\.tail main_VRAM_END :|.tail OVERLAY_VRAM :|' $< > $@

# The CD reader loads each overlay as large as the original's file, which
# may be larger than this build's: the heap begins after that
LINK_HEAP_ARGS := --disc $(DISK_DIR)

# This build's binaries are their own sizes: each one must still fit
all: fitcheck
