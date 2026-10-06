"""Build a native recompilation of the player's own disc, on their machine, with their toolchain.

The launcher ships no game code and no compiled game binary. What it ships is the engine's own
recompiler (`engine/psxrecomp.exe` + its `framework/`), the bundled MIT OpenBIOS image, and the
small MIT `retcomm-rbengine` library the runtime links for rewind. Given the player's disc, this
module:

  1. PRE-FLIGHTS the C/C++ toolchain and says precisely what is missing if anything is, before a
     recompile that takes 10 to 20 minutes is ever started;
  2. runs the recompiler CLI over the player's disc to generate the game's C sources;
  3. compiles that generated project locally with CMake + Ninja + the player's compiler;
  4. lands the finished, self-contained build in `Builds/EUR/` or `Builds/USA/` beside the
     launcher, where the two regional Play buttons run it;
  5. bundles the overlay compiler (TinyCC + embedded CPython) beside that build, so the runtime
     compiles dynamically-loaded overlay code instead of falling back to its interpreter
     (`assemble_overlay_toolchain`).

Everything is derived from `paths.launcher_root()`: no machine path is baked into this module, and
the generated sources, the intermediate build tree and the disc image all live OUTSIDE version
control (the tree's `.gitignore` excludes `build/`, `Builds/`, `Discs/` and the generated C).
"""
from __future__ import annotations

import glob
import os
import pathlib
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field

from . import builds, disc, paths

# ---------------------------------------------------------------- what the engine package holds

ENGINE_DIRNAME = "engine"


def engine_dir() -> pathlib.Path:
    return paths.launcher_root() / ENGINE_DIRNAME


def cli_path() -> pathlib.Path:
    return engine_dir() / "psxrecomp.exe"


def framework_dir() -> pathlib.Path:
    return engine_dir() / "framework"


def bios_path() -> pathlib.Path:
    """The bundled MIT OpenBIOS image, the one BIOS that is legal to ship, so the player only
    ever has to supply a disc."""
    return framework_dir() / "bios" / "openbios.bin"


def rbengine_dir() -> pathlib.Path:
    return engine_dir() / "lib" / "retcomm-rbengine"


# ---------------------------------------------------------------- the overlay compiler it bundles

# The runtime compiles dynamically-loaded overlay code with its `tcc` tier, but only when it finds
# a self-contained toolchain beside its own exe. These are the two redistributable inputs that
# bundle is assembled from, kept under the engine package so they travel with the launcher. Their
# SHA-256s are recorded in the generated THIRD_PARTY_NOTICES.txt.
OVERLAY_TCC_ZIP = "tcc-0.9.27-win64-bin.zip"           # TinyCC 0.9.27, LGPL-2.1
OVERLAY_PYTHON_ZIP = "python-3.12.10-embed-amd64.zip"  # embedded CPython 3.12, PSF-2.0


def overlay_inputs_dir() -> pathlib.Path:
    return engine_dir() / "overlay_toolchain_inputs"


def overlay_packager() -> pathlib.Path:
    """The engine's own packager, which also writes the licence + source-offer notice."""
    return engine_dir() / "tools" / "package_overlay_toolchain.py"


def overlay_licenses_dir() -> pathlib.Path:
    return engine_dir() / "packaging" / "licenses"


def overlay_dir(folder: str) -> pathlib.Path:
    return builds.builds_dir() / folder / "overlay_toolchain"


def overlay_ready(folder: str) -> bool:
    """True when that build carries a usable bundled toolchain (what the runtime probes for)."""
    return (overlay_dir(folder) / "python" / "python.exe").is_file()


def engine_present() -> bool:
    return cli_path().is_file() and bios_path().is_file()


def work_root() -> pathlib.Path:
    """Where generated sources and the CMake build tree go - under the root `build/`, which the
    tree's .gitignore excludes."""
    return paths.launcher_root() / "build"


# ---------------------------------------------------------------- a CLI-safe path

# The bundled recompiler builds its helper command lines with a shell and does NOT survive a space
# (or a bracket) anywhere in a path it touches: given a folder like "Digimon World 3 Launcher
# [public]", the BIOS helper is handed everything up to the first space ("...\Digimon") and dies
# with "config file not found". The fix is to hand the CLI - and only the CLI - the SAME directory
# under its 8.3 short name (a "DI23A3~1"-style alias), which carries neither character.
_UNSAFE = " \t[](){}!&^<>|\"'"


def _is_cli_safe(p: pathlib.Path) -> bool:
    return not any(ch in _UNSAFE for ch in str(p))


_SHORT_CACHE: dict[str, pathlib.Path] = {}


def _short_path(p: pathlib.Path) -> pathlib.Path:
    """Windows 8.3 form of `p` (space- and bracket-free), or `p` when the volume gives none."""
    if sys.platform != "win32":
        return p
    key = str(p)
    if key in _SHORT_CACHE:
        return _SHORT_CACHE[key]
    out = p
    try:
        import ctypes
        buf = ctypes.create_unicode_buffer(32768)
        n = ctypes.windll.kernel32.GetShortPathNameW(str(p), buf, 32768)
        cand = pathlib.Path(buf.value) if n else p
        if str(cand) != str(p) and _is_cli_safe(cand) and cand.is_dir():
            out = cand
    except Exception:  # noqa: BLE001
        out = p
    _SHORT_CACHE[key] = out
    return out


def _junction_alias(real: pathlib.Path) -> pathlib.Path | None:
    """A last resort for volumes with 8.3 disabled: a directory junction under a clean path.

    Junctions need no elevation. Best effort - returns None when no clean writable base exists.
    """
    try:
        import hashlib
        base = os.environ.get("ProgramData") or tempfile.gettempdir()
        b = pathlib.Path(base)
        if not b.is_dir() or not _is_cli_safe(b):
            return None
        link = b / ("DW3RecompiledPlus-" + hashlib.sha1(str(real).encode()).hexdigest()[:8])
        if link.is_dir():
            return link
        res = subprocess.run(["cmd", "/c", "mklink", "/J", str(link), str(real)],
                             capture_output=True, text=True, errors="replace")
        return link if res.returncode == 0 and link.is_dir() else None
    except Exception:  # noqa: BLE001
        return None


def cli_root() -> pathlib.Path:
    """The root to hand the bundled CLI: the launcher's own folder, or its short form when the
    folder's own name contains a character the CLI's shell-built command lines cannot carry."""
    real = paths.launcher_root()
    if _is_cli_safe(real):
        return real
    short = _short_path(real)
    if _is_cli_safe(short):
        return short
    return _junction_alias(real) or real


def _via(p: pathlib.Path) -> pathlib.Path:
    """`p` expressed under `cli_root()`, for the paths the CLI and its CMake build are given."""
    root = paths.launcher_root()
    croot = cli_root()
    try:
        return croot / p.relative_to(root)
    except ValueError:
        return _short_path(p)


# ---------------------------------------------------------------- the toolchain pre-flight

@dataclass
class ToolChain:
    """What was found on the machine, in the tool's own terms.

    `compiler_kind` is one of "msvc", "clang", "gcc" or "" (nothing found). `vcvars` is the VS
    developer environment script MSVC needs before cl.exe/INCLUDE/LIB are usable.
    """
    cmake: str | None = None
    ninja: str | None = None
    compiler: str | None = None
    compiler_kind: str = ""
    compiler_label: str = ""
    vcvars: str | None = None
    is_windows: bool = field(default_factory=lambda: sys.platform == "win32")

    @property
    def ok(self) -> bool:
        return bool(self.cmake and self.ninja and self.compiler)

    def missing(self) -> list[str]:
        out = []
        if not self.cmake:
            out.append("CMake")
        if not self.ninja:
            out.append("Ninja")
        if not self.compiler:
            out.append("a C/C++ compiler (MSVC Build Tools or clang)")
        return out

    def summary(self) -> str:
        if not self.ok:
            return "Missing: " + ", ".join(self.missing())
        comp = self.compiler_label or self.compiler_kind
        parts = [comp, f"CMake {_cmake_version(self.cmake)}", f"Ninja {_ninja_version(self.ninja)}"]
        return "Ready: " + ", ".join(p for p in parts if p)

    def hint(self) -> str:
        return ("Install what is missing and try again. The launcher needs all three.\n"
                "  • a C/C++ compiler - 'Visual Studio Build Tools' with the\n"
                "    'Desktop development with C++' workload, or LLVM/clang;\n"
                "  • CMake (cmake.org);\n"
                "  • Ninja (ships with the VS Build Tools, or ninja-build.org).")


def _cmake_version(path: str | None) -> str:
    return _first_version_line([path, "--version"]) if path else ""


def _ninja_version(path: str | None) -> str:
    return _first_version_line([path, "--version"]) if path else ""


def _first_version_line(argv: list[str]) -> str:
    try:
        res = subprocess.run(argv, capture_output=True, text=True, errors="replace", timeout=20)
    except Exception:  # noqa: BLE001
        return ""
    line = (res.stdout or "").strip().splitlines()
    if not line:
        return ""
    # "cmake version 4.2.3" -> "4.2.3"; "1.13.1" stays
    toks = line[0].split()
    return toks[-1] if toks else ""


def _which(name: str) -> str | None:
    return shutil.which(name)


def _prefer_native(paths_found: list[str]) -> str | None:
    """Pick the first candidate that is not a devkitPro/cygwin copy.

    On this class of machine `cmake` on PATH can be devkitPro's MSYS2 cmake, which is a POSIX
    build that a plain Windows toolchain cannot use. A native copy is always preferred when one
    exists.
    """
    real = [p for p in paths_found if p and pathlib.Path(p).is_file()]
    native = [p for p in real if "devkitpro" not in str(p).lower()
              and "msys" not in str(p).lower()]
    return (native or real or [None])[0]


def find_cmake() -> str | None:
    cands: list[str] = []
    for env in ("ProgramFiles", "ProgramFiles(x86)"):
        base = os.environ.get(env)
        if base:
            cands.append(str(pathlib.Path(base) / "CMake" / "bin" / "cmake.exe"))
    w = _which("cmake")
    if w:
        cands.append(w)
    return _prefer_native(cands)


def find_ninja() -> str | None:
    cands: list[str] = []
    w = _which("ninja")
    if w:
        cands.append(w)
    for env in ("ProgramFiles", "ProgramFiles(x86)"):
        base = os.environ.get(env)
        if base:
            cands.append(str(pathlib.Path(base) / "CMake" / "bin" / "ninja.exe"))
    return _prefer_native(cands)


def _vswhere() -> str | None:
    for env in ("ProgramFiles(x86)", "ProgramFiles"):
        base = os.environ.get(env)
        if base:
            p = pathlib.Path(base) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
            if p.is_file():
                return str(p)
    return None


def find_msvc() -> tuple[str | None, str]:
    """(vcvars64.bat, label) for the newest installed Visual Studio / Build Tools."""
    roots: list[str] = []
    vswhere = _vswhere()
    if vswhere:
        try:
            res = subprocess.run(
                [vswhere, "-latest", "-products", "*",
                 "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
                 "-property", "installationPath"],
                capture_output=True, text=True, errors="replace", timeout=30)
            roots += [ln.strip() for ln in (res.stdout or "").splitlines() if ln.strip()]
        except Exception:  # noqa: BLE001
            pass
    for env in ("ProgramFiles", "ProgramFiles(x86)"):
        base = os.environ.get(env)
        if not base:
            continue
        for pat in ("Microsoft Visual Studio/*/*", "Microsoft Visual Studio/*"):
            roots += glob.glob(str(pathlib.Path(base) / pat))
    seen: set[str] = set()
    for root in roots:
        if root in seen:
            continue
        seen.add(root)
        vcvars = pathlib.Path(root) / "VC" / "Auxiliary" / "Build" / "vcvars64.bat"
        if not vcvars.is_file():
            continue
        vers = sorted(glob.glob(str(pathlib.Path(root) / "VC" / "Tools" / "MSVC" / "*"
                                  / "bin" / "Hostx64" / "x64" / "cl.exe")))
        label = f"MSVC {pathlib.Path(vers[-1]).parts[-5]}" if vers else "MSVC"
        return str(vcvars), label
    return None, ""


def find_compiler() -> tuple[str | None, str, str, str | None]:
    """(compiler path, kind, label, vcvars), MSVC first, then clang, then gcc."""
    if sys.platform == "win32":
        vcvars, label = find_msvc()
        if vcvars:
            root = pathlib.Path(vcvars).parents[3]
            cl = sorted(glob.glob(str(root / "VC" / "Tools" / "MSVC" / "*"
                                      / "bin" / "Hostx64" / "x64" / "cl.exe")))
            return (cl[-1] if cl else vcvars), "msvc", (label or "MSVC"), vcvars
    for name, kind in (("clang", "clang"), ("clang-cl", "clang"), ("gcc", "gcc"), ("cc", "gcc")):
        w = _which(name)
        if w:
            return w, kind, name, None
    return None, "", "", None


_CACHE: ToolChain | None = None


def preflight(refresh: bool = False) -> ToolChain:
    """Detect the toolchain. Cached for the session so the UI can ask cheaply; `refresh=True`
    (the tab's 'Re-check' button) re-probes after the player installs something."""
    global _CACHE
    if _CACHE is not None and not refresh:
        return _CACHE
    compiler, kind, label, vcvars = find_compiler()
    _CACHE = ToolChain(cmake=find_cmake(), ninja=find_ninja(), compiler=compiler,
                       compiler_kind=kind, compiler_label=label, vcvars=vcvars)
    return _CACHE


def reset_cache() -> None:
    global _CACHE
    _CACHE = None


# ---------------------------------------------------------------- running a tool, streamed

def _run_stream(argv: list[str], cwd: pathlib.Path, log, timeout: int = 7200,
                env: dict[str, str] | None = None) -> int:
    """Run a command, feeding every output line to `log`, and return its exit code.

    A long compile produces a lot of output; streaming it is what makes the progress honest
    instead of a spinner that hides whether anything is happening.
    """
    log("$ " + " ".join(argv))
    proc = subprocess.Popen(argv, cwd=str(cwd), stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, text=True, errors="replace",
                            env=env, bufsize=1)
    assert proc.stdout is not None
    try:
        for line in proc.stdout:
            log(line.rstrip("\n"))
        proc.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        proc.kill()
        log(f"!! timed out after {timeout}s")
        return 124
    finally:
        try:
            proc.stdout.close()
        except Exception:  # noqa: BLE001
            pass
    return proc.returncode


# ---------------------------------------------------------------- stage 1: generate the C

def generate(image: pathlib.Path, project_dir: pathlib.Path, name: str, log) -> bool:
    """Run the bundled recompiler CLI over the player's disc.

    The CLI copies its build framework out of its own package, so this is where the patched
    framework actually reaches the generated project.
    """
    if not engine_present():
        log(f"!! the bundled engine is incomplete at {engine_dir()}")
        return False
    if project_dir.exists():
        shutil.rmtree(project_dir, ignore_errors=True)
    project_dir.mkdir(parents=True, exist_ok=True)
    if not _is_cli_safe(paths.launcher_root()):
        log(f"   the launcher folder's name needs the CLI's short form: {cli_root()}")
    argv = [str(_via(cli_path())), "build", "--disc", str(image),
            "--bios", str(_via(bios_path())), "--output", str(_via(project_dir)),
            "--name", name]
    code = _run_stream(argv, _via(engine_dir()), log, timeout=3600)
    if code != 0 or not (project_dir / "game.toml").is_file():
        log(f"!! the recompiler stopped (exit {code})")
        return False
    return True


# ---------------------------------------------------------------- stage 2: compile it

def _write_build_script(script: pathlib.Path, tc: ToolChain, project_dir: pathlib.Path,
                        cmake_build: pathlib.Path) -> None:
    """A small .cmd so the VS developer environment is applied in the same shell as CMake.

    MSVC is unusable from a bare process: vcvars64.bat is what puts cl.exe, INCLUDE and LIB on
    the environment, and it only affects the shell it runs in, so the configure and the build
    have to share that shell. Quoting `cmd /c "... && ..."` is genuinely fragile, hence a file.
    """
    cmake = tc.cmake or "cmake"
    lines = ["@echo off", "setlocal"]
    if tc.vcvars:
        lines.append(f'call "{tc.vcvars}" >nul')
        lines.append("if errorlevel 1 exit /b 10")
    cfg = [f'"{cmake}"', "-S", f'"{project_dir}"', "-B", f'"{cmake_build}"',
           "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release", "-DPSX_RECOMP_UI=OFF",
           f'-DRECOMP_RBENGINE_ROOT="{_via(rbengine_dir())}"']
    if tc.ninja:
        cfg += ["-DCMAKE_MAKE_PROGRAM=" + f'"{tc.ninja}"']
    lines += [" ".join(cfg), "if errorlevel 1 exit /b 11"]
    lines += [f'"{cmake}" --build "{cmake_build}" --config Release --parallel',
              "exit /b %errorlevel%"]
    script.write_text("\r\n".join(lines) + "\r\n", encoding="utf-8")


def _flatten_fetched_deps(cmake_build: pathlib.Path) -> list[str]:
    """Unwrap a dependency FetchContent left inside its archive's top-level folder.

    CMake's URL download extracts an archive as-is, so a release tarball that ships a single
    enclosing directory (SDL3-3.4.10/, libchdr-<sha>/, zlib-<sha>/) leaves the real source one
    level down. FetchContent then finds no CMakeLists.txt at the source root, adds no target, and
    the configure dies with "SDL3 3.4+ was not found" (or a later "no linkable target"). Moving
    that folder's contents up makes the fetched tree the flat layout the project expects.

    Returns the dependency names it flattened, so the caller can decide to retry the configure.
    """
    deps = cmake_build / "_deps"
    if not deps.is_dir():
        return []
    changed: list[str] = []
    for src in sorted(deps.glob("*-src")):
        try:
            if (src / "CMakeLists.txt").is_file():
                continue
            entries = list(src.iterdir())
        except OSError:
            continue
        kids = [p for p in entries if p.is_dir()]
        files = [p for p in entries if p.is_file()]
        if len(kids) != 1 or files:
            continue
        inner = kids[0]
        try:
            if not (inner / "CMakeLists.txt").is_file() and not any(inner.iterdir()):
                continue
            for item in list(inner.iterdir()):
                shutil.move(str(item), str(src / item.name))
            inner.rmdir()
        except OSError:
            continue
        changed.append(src.name)
    return changed


def compile_project(project_dir: pathlib.Path, cmake_build: pathlib.Path, tc: ToolChain,
                    script_dir: pathlib.Path, log) -> bool:
    if not tc.ok:
        log("!! " + tc.summary())
        return False
    script = script_dir / "build.cmd"
    script.parent.mkdir(parents=True, exist_ok=True)
    if tc.compiler_kind == "msvc" and sys.platform == "win32":
        _write_build_script(script, tc, _via(project_dir), _via(cmake_build))

        def _run_once() -> int:
            return _run_stream([os.environ.get("ComSpec", "cmd.exe"), "/c", str(_via(script))],
                               script_dir, log)
    else:
        cmake = tc.cmake
        argv = [cmake, "-S", str(_via(project_dir)), "-B", str(_via(cmake_build)), "-G", "Ninja",
                "-DCMAKE_BUILD_TYPE=Release", "-DPSX_RECOMP_UI=OFF",
                f"-DRECOMP_RBENGINE_ROOT={_via(rbengine_dir())}"]
        if tc.ninja:
            argv.append(f"-DCMAKE_MAKE_PROGRAM={tc.ninja}")
        if tc.compiler and tc.compiler_kind in ("clang", "gcc"):
            argv += [f"-DCMAKE_C_COMPILER={tc.compiler}"]
            if tc.compiler_kind == "clang":
                cxx = _which("clang++") or tc.compiler
                argv += [f"-DCMAKE_CXX_COMPILER={cxx}"]

        def _run_once() -> int:
            code = _run_stream(argv, script_dir, log)
            if code == 0:
                code = _run_stream([cmake, "--build", str(_via(cmake_build)), "--config",
                                    "Release", "--parallel"], script_dir, log)
            return code

    # A configure that dies because a fetched dependency stayed wrapped can be repaired and
    # retried; each pass unwraps whatever the failed pass had just downloaded (SDL3 and libchdr
    # together, then zlib), so it converges in a couple of attempts and is a no-op afterwards.
    code = 1
    for _attempt in range(4):
        code = _run_once()
        if code == 0:
            return True
        flattened = _flatten_fetched_deps(cmake_build)
        if not flattened:
            break
        log("   a fetched dependency stayed inside its archive folder; unwrapped "
            + ", ".join(flattened) + " and retrying the configure")
    log(f"!! the compiler stopped (exit {code})")
    return False


def _find_exe(out_dir: pathlib.Path, expected: str) -> pathlib.Path | None:
    cands = sorted(out_dir.glob("*.exe"))
    for c in cands:
        if c.name == expected:
            return c
    main = [c for c in cands if not c.name.endswith("_oracle.exe")
            and "_pgxp" not in c.name and "ctest" not in c.name.lower()]
    if main:
        return main[0]
    return cands[0] if cands else None


# ---------------------------------------------------------------- stage 3: land the build

def _sanitize_identifier(text: str) -> str:
    """CMake's `string(MAKE_C_IDENTIFIER ...)`, which the runtime uses as the exe's name."""
    out = "".join(ch if (ch.isalnum() or ch == "_") else "_" for ch in text)
    return ("_" + out) if out[:1].isdigit() else out


def expected_exe_name(title: str) -> str:
    """The executable the runtime will produce for a window title: `<title> Recompiled`."""
    return _sanitize_identifier(f"{title} Recompiled")


def _python_interpreter() -> str | None:
    """The interpreter to run the engine's stdlib-only packager with.

    `sys.executable` is the launcher's own interpreter, which is right for a source checkout but is
    the frozen app itself under PyInstaller, so only accept it when it really is a `python*`.
    """
    exe = sys.executable or ""
    if exe and pathlib.Path(exe).name.lower().startswith("python"):
        return exe
    for name in ("python", "python3", "py"):
        found = shutil.which(name)
        if found:
            return found
    return None


def assemble_overlay_toolchain(out_dir: pathlib.Path, project_dir: pathlib.Path, log) -> bool:
    """Bundle the overlay compiler (TinyCC + embedded CPython) beside a finished build.

    The runtime probes `<exe_dir>/overlay_toolchain/python/python.exe` at startup. When it is
    there, dynamically-loaded overlay code is compiled natively instead of being left on the
    dirty-RAM interpreter, which stalls the frame timer for about a second on a first visit. The
    engine's own packager assembles it, so the TinyCC LGPL-2.1 licence text and the written
    source offer are written with it.

    Best effort by design: a build with no overlay toolchain still runs, only slower, so a failure
    is reported and never fails the recompile that produced the build.
    """
    packager = overlay_packager()
    tcc_zip = overlay_inputs_dir() / OVERLAY_TCC_ZIP
    py_zip = overlay_inputs_dir() / OVERLAY_PYTHON_ZIP
    game_exe = engine_dir() / "libexec" / "psxrecomp-game.exe"
    # The include dir must be the one the GAME was built against: its generated
    # overlay_codegen_hash.h is what the exe's compiled-in hash is checked against.
    cand = [project_dir / "psxrecomp" / "runtime" / "include",
            framework_dir() / "runtime" / "include"]
    inc = next((p for p in cand if (p / "overlay_codegen_hash.h").is_file()), None) \
        or next((p for p in cand if p.is_dir()), None)
    python = _python_interpreter()
    missing = [str(p) for p in (packager, tcc_zip, py_zip, game_exe) if not p.is_file()]
    if python is None:
        missing.append("a Python interpreter to run the packager")
    if inc is None:
        missing.append("the runtime include dir")
    if missing:
        log("   note: no overlay toolchain bundled (missing " + ", ".join(missing) + ")")
        log("   overlay gaps will fall back to the interpreter, which stalls about a second on a "
            "first visit")
        return False
    log("   bundling the overlay compiler (TinyCC + CPython) beside the build")
    argv = [python, str(packager), "--exe-dir", str(out_dir),
            "--tcc-zip", str(tcc_zip), "--python-zip", str(py_zip),
            "--recompiler", str(game_exe), "--runtime-include", str(inc),
            "--licenses-dir", str(overlay_licenses_dir()), "--force"]
    code = _run_stream(argv, engine_dir(), log, timeout=1200)
    ok = code == 0 and overlay_ready(out_dir.name)
    if not ok:
        log(f"   note: the overlay toolchain could not be assembled (packager exit {code}); "
            f"overlay gaps will use the interpreter")
    return ok


def assemble_into(folder: str, exe_name: str | None, project_dir: pathlib.Path,
                  cmake_build: pathlib.Path, log) -> pathlib.Path | None:
    """Copy the finished, self-contained runtime into `Builds/<folder>/` beside the launcher.

    The runtime resolves game.toml, bios/ and its input files from its working directory, so the
    destination carries the exe, its game.toml, the bundled BIOS and - via
    `assemble_overlay_toolchain` - the bundled overlay compiler. Everything runtime-generated
    (input.ini, cards, caches) the game writes itself on first run.
    """
    out = builds.builds_dir() / folder
    out.mkdir(parents=True, exist_ok=True)
    exe_src = _find_exe(cmake_build, exe_name or "")
    if exe_src is None:
        log(f"!! no executable was produced under {cmake_build}")
        return None
    exe_dst = out / (exe_name or exe_src.name)
    shutil.copy2(exe_src, exe_dst)
    gt_src = project_dir / "game.toml"
    if gt_src.is_file():
        shutil.copy2(gt_src, out / "game.toml")
    bios_out = out / "bios"
    bios_out.mkdir(parents=True, exist_ok=True)
    for f in (framework_dir() / "bios").glob("*"):
        if f.is_file():
            shutil.copy2(f, bios_out / f.name)
    assemble_overlay_toolchain(out, project_dir, log)
    log(f"== placed {exe_dst.name} in {out}")
    return exe_dst


def assemble(region: str, project_dir: pathlib.Path, cmake_build: pathlib.Path, log) -> pathlib.Path | None:
    spec = builds.spec(region)
    return assemble_into(spec.folder, spec.exe_name, project_dir, cmake_build, log)


# ---------------------------------------------------------------- the whole job

@dataclass
class BuildResult:
    ok: bool
    region: str | None
    exe: pathlib.Path | None
    message: str


def build(region: str, image: pathlib.Path, log) -> BuildResult:
    """Recompile `image` into a native build, start to finish. Nothing is refused for its region.

    Europe and USA are first-class (they land in `Builds/EUR` / `Builds/USA`, where the two Play
    buttons find them). Any other disc is still attempted, the recompiler is the authority on
    whether it can be built, and, if it builds, lands in `Builds/OTHER` with an honest note that
    the two regional Play buttons do not cover it.

    `log` receives one line at a time; the caller runs this off the UI thread.
    """
    if not image.is_file():
        return BuildResult(False, region, None, f"Disc image not found: {image}")
    if not engine_present():
        return BuildResult(False, region, None,
                           f"The bundled engine is incomplete at {engine_dir()} "
                           f"(psxrecomp.exe and framework/bios/openbios.bin are both required).")
    tc = preflight()
    if not tc.ok:
        return BuildResult(False, region, None, tc.summary() + "\n\n" + tc.hint())
    known = region in (builds.REGION_EU, builds.REGION_US)
    serial = disc.serial_of_image(image)
    if known:
        spec = builds.spec(region)
        folder, title, exe_name, label = spec.folder, spec.title, spec.exe_name, spec.label
    else:
        folder = "OTHER"
        title = f"Digimon World 3 {serial}".strip()
        exe_name = None
        label = f"unlisted region ({serial or 'no serial'})"

    root = work_root() / folder
    project = root / "project"
    cmake_build = project / "build"
    log(f"== Recompiling {image.name} for {label} ==")
    log(f"   disc serial: {serial or '(none readable)'}   engine: {engine_dir()}")
    log("   stage 1/3: generating the game's C sources (several minutes)")
    if not generate(image, project, title, log):
        return BuildResult(False, region, None, "The recompiler failed; see the log above.")
    log("   stage 2/3: compiling, this is the long one")
    if not compile_project(project, cmake_build, tc, root, log):
        return BuildResult(False, region, None, "The compile failed; see the log above.")
    log("   stage 3/3: placing the build beside the launcher")
    exe = assemble_into(folder, exe_name, project, cmake_build, log)
    if exe is None:
        return BuildResult(False, region, None, "The build produced no executable.")
    if known:
        try:
            builds.set_disc(region, image)
        except Exception as exc:  # noqa: BLE001
            log(f"   note: could not record the disc path in game.toml ({exc})")
        log(f"== {label} build ready: {exe} ==")
        return BuildResult(True, region, exe, f"{label} build ready.")
    log(f"== build ready at {exe} ==")
    log("   note: this disc is not Europe or USA, so the two Play buttons do not list it, "
        "run the exe from its folder, or move it under Builds/ where you want it.")
    return BuildResult(True, region, exe,
                       f"Built a {label} disc, but it is not on a Play button.")

