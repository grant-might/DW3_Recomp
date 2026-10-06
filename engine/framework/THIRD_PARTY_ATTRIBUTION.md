# Third-Party Attribution

## OpenBIOS — PCSX-Redux's free PS1 BIOS

[OpenBIOS](https://github.com/grumpycoders/pcsx-redux) (src/mips/openbios) by
the PCSX-Redux authors, licensed **MIT** (notice: `bios/OpenBIOS.LICENSE`;
the binary also links permissively-licensed code from
[uC-sdk](https://github.com/grumpycoders/uC-sdk), noted there too). Vendored
as the prebuilt image `bios/openbios.bin` (pin and build recipe recorded in
`bios/OpenBIOS.toml`) and statically recompiled by
`psxrecomp-bios --config bios/OpenBIOS.toml` exactly like the retail BIOS.
Normal runtime builds stage it automatically so players supply only a disc;
`bios/OpenBIOS.LICENSE` always rides alongside the shipped image.


## TinyCC (TCC) — toolchain-free overlay compiler shipped to players

[TinyCC](https://bellard.org/tcc/) by Fabrice Bellard and contributors, licensed
**LGPL-2.1**. Not vendored in this repository — no TinyCC source or binary is
tracked here. It is invoked as a **separate subprocess** by
`tools/compile_overlays.py` (`--compiler tcc`, `--tcc &lt;binary&gt;`) to build overlay
shards into a DLL, so players need no compiler of their own. Nothing in the
runtime links against libtcc, so this is aggregation with a separate program
rather than LGPL linkage.

The runtime expects an end-user bundle at
`&lt;exe_dir&gt;/overlay_toolchain/{python/, tcc/tcc.exe, compile_overlays.py, …}`
(`runtime/src/main.cpp`). The TinyCC license notice is shipped alongside the
binary in that bundle; `tools/package_overlay_toolchain.py` (below) is the
supported way to build it.

`tools/package_overlay_toolchain.py` now assembles that directory from inputs
you already have (the TinyCC win64 zip, the CPython embeddable zip, and this
tree's `psxrecomp-game.exe` + `runtime/include`). It copies the TinyCC LGPL-2.1
text (`packaging/licenses/LGPL-2.1.txt`) to `tcc/COPYING`, the CPython
`LICENSE.txt` (PSF-2.0) to `python/`, and writes `THIRD_PARTY_NOTICES.txt` with
each input's SHA-256 so a release can be audited. Pass `--tcc-zip` and
`--python-zip` (nothing is downloaded for you). The exact upstream artefacts
the shipped bundle was built from are:
`tcc-0.9.27-win64-bin.zip` (sha256
`34a721949a2583fdff725312da092fa0f5f1f284b702e6f811c6954714faabb2`) and
`python-3.12.10-embed-amd64.zip` (sha256
`4acbed6dd1c744b0376e3b1cf57ce906f9dc9e95e68824584c8099a63025a3c3`).

Developers with `gcc` on `PATH` use the gcc tier instead; the bundled tcc matters
only for end-user release packages (`docs/BUILDING.md`).

## CPython — embedded interpreter inside the overlay toolchain bundle

[Python](https://www.python.org/) (CPython), licensed **PSF-2.0** (permissive;
the licence text ships as `python/LICENSE.txt` in the bundle, from the embeddable
distribution itself). The end-user `overlay_toolchain/` bundle carries an
*embedded* CPython so the runtime can run `tools/compile_overlays.py` without the
player installing Python — `runtime/src/main.cpp` probes for
`overlay_toolchain/python/python.exe` and spawns it as a subprocess. Nothing in
the runtime links against libpython. `tools/package_overlay_toolchain.py` copies
the licence next to the interpreter. No CPython source or binary is tracked in
this repository.

## JRickey / gba-recomp — verified-enhancement shadow + screen color science

The verified-enhancement QoL layer (`feat/shadow-enhancements`) reuses two
engine-agnostic pieces originally authored by Jrickey in
[JRickey/gba-recomp](https://github.com/JRickey/gba-recomp), licensed
**MIT OR Apache-2.0**, used with permission:

- **`ShadowVerifier`** — the envelope-correlation differential self-check,
  probation auto-gain calibration, and prove/strike/pause state machine.
  Original: `crates/gba-core/src/shadow.rs`.
  This repo: `runtime/src/audio_shadow.c`, `runtime/include/audio_shadow.h`
  (C re-implementation, via the gbarecomp C++ port `src/gba/audio_shadow.*`
  and the snesrecomp C port `runner/src/snes/audio_shadow.*`; the algorithm is
  unchanged).

- **Color-science core** (xyY→XYZ, primaries→matrix, Bradford chromatic
  adaptation, sRGB OETF) used to bake the present-time screen-color LUT.
  Original: `crates/screen/src/{color,profile,lut}.rs`.
  This repo: `runtime/src/color_lut.c`, `runtime/include/color_lut.h`
  (C re-implementation, via the gbarecomp C++ port `src/runtime/color_lut.*`).

### PSX-specific work (ours)

- The **CRT / composite / Trinitron** display panel models in `color_lut.c`
  (the GBA port modelled a handheld LCD; a console scanned out to a TV needs a
  CRT/composite model instead) — SMPTE-C / Trinitron-class phosphor gamuts,
  CRT gamma, black-lift.
- The **SPU float shadow render** (`runtime/src/spu_shadow.c`,
  `runtime/include/spu_shadow.h`): 4-point cubic resampling + float headroom
  re-render of the PS1 SPU ADPCM voice mix, driven from a read-only tap on the
  canon `spu.c` voice state. This is console-specific (the SNES analog re-renders
  the S-DSP; the GBA analog re-renders the MP2K software mixer).
- The tap plumbing in `runtime/src/spu.c` and `runtime/include/spu.h`.

All reuse keeps the original copyright and dual MIT/Apache-2.0 license.
