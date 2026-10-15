# Digimon World 3 decomp

| Version | Code | Data | Functions |
|---|---|---|---|
| 🇪🇺 Europe (`SLES_039.36`) | [![Code](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=code&version=SLES_039.36&label=Code)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLES_039.36) | [![Data](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=data&version=SLES_039.36&label=Data)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLES_039.36) | [![Functions](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=functions&version=SLES_039.36&label=Functions)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLES_039.36) |
| 🇺🇸 USA (`SLUS_014.36`) | [![Code](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=code&version=SLUS_014.36&label=Code)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLUS_014.36) | [![Data](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=data&version=SLUS_014.36&label=Data)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLUS_014.36) | [![Functions](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=functions&version=SLUS_014.36&label=Functions)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLUS_014.36) |
| 🇯🇵 Japan (`SLPS_034.46`) | the shared C, not on decomp.dev yet | | |

[![Executable](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=code&version=SLES_039.36&category=game&label=Europe%20executable)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLES_039.36?category=game)
[![Stages](https://decomp.dev/ReGame-Labs/dw3_decomp.svg?mode=shield&measure=code&version=SLES_039.36&category=stages&label=Europe%20stages)](https://decomp.dev/ReGame-Labs/dw3_decomp/SLES_039.36?category=stages)

[![Build](https://github.com/ReGame-Labs/dw3_decomp/actions/workflows/build.yaml/badge.svg)](https://github.com/ReGame-Labs/dw3_decomp/actions/workflows/build.yaml)
[![Docs](https://github.com/ReGame-Labs/dw3_decomp/actions/workflows/docs.yaml/badge.svg)](https://regame-labs.github.io/dw3_decomp/)
[![Platform](https://img.shields.io/badge/platform-PlayStation-003791)](docs/binaries.md#the-games-binaries)
[![Versions](https://img.shields.io/badge/versions-USA%20%7C%20Europe%20%7C%20Japan-blue)](docs/binaries.md#how-the-versions-are-organised)
[![Compiler](https://img.shields.io/badge/compiler-GCC%202.8.1-orange)](docs/toolchain.md#toolchain)
[![Fake matches | hacks](https://img.shields.io/badge/fake%20matches%20%7C%20hacks-0%20%7C%20183-yellow)](docs/status.md#fake-matches-and-hacks)
[![License](https://img.shields.io/github/license/ReGame-Labs/dw3_decomp)](LICENSE)

A work in progress matching decompilation of **Digimon World 3** for the
PlayStation: C source that compiles back into byte-identical copies of the
game's executable, its overlays and its stages. The European release,
*Digimon World 2003* (`SLES_039.36`), the most complete one, is the main
version: the build's default and the one decomp.dev shows first. The USA
release, *Digimon World 3* (`SLUS_014.36`), builds from the same C and
matches byte for byte too. The Japanese release, *Digimon World 3:
Aratanaru Bouken no Tobira* (`SLPS_034.46`), builds byte for byte from
splat's assembly so far, split and named after the USA version.

This repository does not contain any game data. You need your own copy of the
game to build it.

[![Progress map](https://decomp.dev/ReGame-Labs/dw3_decomp.png)](https://decomp.dev/ReGame-Labs/dw3_decomp)

<sub>Each rectangle is a unit, sized by its code; green means it matches.
Click it for the details on decomp.dev.</sub>

## Status

Measured with `make report` on the European and USA versions; the badges
above are always current:

| Part | Version | Functions in C | Code | Data |
|---|---|---|---|---|
| Executable, game code | Europe | 346 / 346 | 100.00 % | 100.00 % |
| | USA | 346 / 346 | 100.00 % | 100.00 % |
| The 21 overlays | Europe | 1,670 / 1,670 | 100.00 % | 100.00 % |
| | USA | 1,665 / 1,665 | 100.00 % | 100.00 % |
| The stages (293 and 238) | Europe | 1,590 / 1,590 | 100.00 % | 100.00 % |
| | USA | 1,369 / 1,369 | 100.00 % | 100.00 % |
| **Total** | **Europe** | **3,606 / 3,606** | **100.00 %** | **100.00 %** |
| | **USA** | **3,380 / 3,380** | **100.00 %** | **100.00 %** |

- The game's code and data are all C in the European and USA versions: the
  executable, the 21 overlays and all the stages.
- The Japanese version builds from splat's assembly so far, split into the
  USA version's files and named after it: its C is the next step.
- The PsyQ libraries are Sony's code, not the game's: the build takes them
  from the original as splat's disassembly and the progress leaves them out.
- [docs/status.md](docs/status.md) has the details of each part, how the
  progress is measured, and the fake matches and hacks the badge counts.

## Quick start

### Prerequisites

On Debian or Ubuntu (the CI uses Ubuntu 24.04 and Python 3.12), install:
```
binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu git make python3 python3-venv unzip wget
```
Or build in Docker instead ([below](#building-with-docker)).

Clone with the submodules (maspsx, m2c, decomp-permuter and the PsyQ
headers), create the Python environment, which must be active whenever you
run `make` or the scripts in `tools/`, and download the prebuilt tools (GCC
2.8.1 for the PSX, objdiff-cli and mkpsxiso) into `bin/`, checked against
`tools/deps.sha256`:
```
git clone --recursive https://github.com/ReGame-Labs/dw3_decomp.git
cd dw3_decomp            # in an existing clone: git submodule update --init --recursive
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt
tools/dl_deps.sh
```

### Getting the game files

Each disc goes in its own `disks/<version>/`. Extract it with
`tools/extract_disc.py`, which also reaches the `AAA/` directories, and check
the executable:
```
python3 tools/extract_disc.py "/path/to/Digimon World 3 (USA).bin" disks/us
sha1sum disks/us/SLUS_014.36   # 444653259f78ddb483fd22af72cce9276f42f214

python3 tools/extract_disc.py "/path/to/Digimon World 2003 (Europe).bin" disks/eu
sha1sum disks/eu/SLES_039.36   # d1b7e4d646e3a9c2b88fdb25d20b5f7116bbb06d

python3 tools/extract_disc.py "/path/to/Digimon World 3 - Aratanaru Bouken no Tobira (Japan).bin" disks/jp
sha1sum disks/jp/SLPS_034.46   # 0e5b6b9ee2f99a804ed8c4117c805482c10e260b
```

Only `disks/<version>/<executable>` and `disks/<version>/AAA/PRO/` are
needed, and only for the versions you build. `config/<version>/` holds the
SHA-1 of every original binary, which `make compare` checks the build
against: `<executable>.sha1`, `overlays.sha1` and `stages.sha1`.

### Build

The same steps build each version, with `VERSION` set to `eu`, `us` or `jp`
(`eu` when it is left out):
```
# Split the executable, the overlays and the stages with splat
# (asm/<version>/, build/<version>/generated/)
make VERSION=eu generate

# Build build/<version>/<executable> and build/<version>/AAA/PRO/*.PRO
make VERSION=eu -j$(nproc)

# Check the executable, every overlay and every stage against the originals
make VERSION=eu compare
```

`make compare` prints one `OK` per binary, and a change only counts once
every line still says `OK`: 261 lines for `us` (the executable, 21 overlays
and 239 stages, `WSTAG260` among them), 316 for `eu` and for `jp` (the
executable, 21 overlays and 294 stages):
```
build/eu/SLES_039.36: OK
build/eu/AAA/PRO/CNTY_SEL.PRO: OK
...
```
The CI builds and compares every version on every push.

`make VERSION=<version> regenerate` deletes `asm/<version>/`,
`build/<version>/`, `expected/<version>/` and `assets/<version>/`, and splits
the binaries again. Run it after changing a `config/<version>/*.yaml`, a symbol file or
`stages.txt`, so that no stale files stay behind in `asm/<version>/`.

### Building with Docker

`tools/docker.sh` builds the CI's build environment from the `Dockerfile`
(Ubuntu 24.04, the MIPS binutils, Python and the prebuilt tools, no game
data) and runs a command in it, as your own user; without a command it opens
a shell. No `bin/` or `.venv` is needed on the host. With the submodules and
the disc files in `disks/` as above:
```
tools/docker.sh make generate
tools/docker.sh sh -c 'make -j$(nproc)'
tools/docker.sh make compare
VERSION=us tools/docker.sh make generate
```
[docs/toolchain.md](docs/toolchain.md#docker) has the details.

### Checks

The CI runs these on every push; run them before opening a pull request:
```
make VERSION=eu report        # build/<version>/report.json, the progress
make VERSION=eu shiftcheck    # every address in the code and data is a symbol
make VERSION=eu padcheck      # linked higher, words change only at relocations
make VERSION=eu lint          # a modern GCC checks the C
python3 tools/hacks.py --check README.md docs/status.md   # the hacks badge and table
python3 tools/check_names.py  # every version uses the USA version's names
```
[docs/shifting.md](docs/shifting.md) explains `shiftcheck` and `padcheck`,
[docs/toolchain.md](docs/toolchain.md#lint) `lint`, and
[docs/status.md](docs/status.md#progress) the report.

## Documentation

- [The API documentation](https://regame-labs.github.io/dw3_decomp/): the European version's functions,
  types, globals and macros, with their source and graphs, and these pages
  (`make docs` builds it locally).
- [docs/status.md](docs/status.md): the status of each part, how the
  progress is measured, and the fake matches and hacks.
- [docs/binaries.md](docs/binaries.md): the game's executable, overlays and
  stages, their memory maps, and how the versions are organised.
- [docs/toolchain.md](docs/toolchain.md): the compiler and its flags, the
  declaration check, the Docker image, the API documentation (`make docs`),
  the repository's layout and its tools.
- [docs/shifting.md](docs/shifting.md): the shiftable build and the checks
  that keep it (`shiftcheck`, `padcheck`, `inputcheck`).
- [CONTRIBUTING.md](CONTRIBUTING.md): the matching workflow and the rules on
  matching, versions, code style, names and pull requests.
- [TODO.md](TODO.md): what is left to do.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md) before opening a pull request. In
short: every function starts as an `INCLUDE_ASM` line in `src/`. To
decompile one, get a first draft from m2c, make it match with
`tools/try_match.py`, objdiff and, for a near miss, the permuter, then replace
the `INCLUDE_ASM` with it and check that `make compare` still says `OK` for
every binary of every version.

The PsyQ functions were named from the
[PsyQ 4.7 signatures](https://github.com/lab313ru/psx_psyq_signatures).

## Links

- [decomp.dev: Digimon World 3](https://decomp.dev/ReGame-Labs/dw3_decomp)
- Inspired by these projects:
  [Digimon World decomp](https://github.com/jype0/dw_decomp),
  [Digimon World 2 decomp](https://github.com/Wyrelade/Digimon-World-2-Decomp),
  [Digimon Digital Card Battle decomp](https://github.com/ReGame-Labs/dcb_decomp),
  which links the same PsyQ 4.7 libraries.
- Tools: [splat](https://github.com/ethteck/splat),
  [maspsx](https://github.com/mkst/maspsx),
  [objdiff](https://github.com/encounter/objdiff),
  [m2c](https://github.com/matt-kempster/m2c),
  [decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
  [old-gcc](https://github.com/decompals/old-gcc) (the PSX GCC builds),
  [mkpsxiso](https://github.com/Lameguy64/mkpsxiso),
  [psyq_headers](https://github.com/jype0/psyq_headers).
