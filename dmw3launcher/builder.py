"""Build a native recompilation of the player's own disc, on their machine, with their toolchain.

The launcher ships no game code and no compiled game binary. What it ships is the engine's own
recompiler (`engine/psxrecomp.exe` + its `framework/`), the bundled MIT OpenBIOS image, and the
small MIT `retcomm-rbengine` library the runtime links for rewind. Given the player's disc, this
module:

  1. PRE-FLIGHTS the C/C++ toolchain and says precisely what is missing if anything is, before a
     recompile that takes 10-15 minutes is ever started;
  2. runs the recompiler CLI over the player's disc to generate the game's C sources;
  3. compiles that generated project locally with CMake + Ninja + the player's compiler;
  4. lands the finished, self-contained build in `Builds/EUR/` or `Builds/USA/` beside the
     launcher, where the two regional Play buttons run it.

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
    """The bundled MIT OpenBIOS image — the one BIOS that is legal to ship, so the player only
    ever has to supply a disc."""
    return framework_dir() / "bios" / "openbios.bin"


def rbengine_dir() -> pathlib.Path:
    return engine_dir() / "lib" / "retcomm-rbengine"


def engine_present() -> bool:
    return cli_path().is_file() and bios_path().is_file()


def work_root() -> pathlib.Path:
    """Where generated sources and the CMake build tree go — under the root `build/`, which the
    tree's .gitignore excludes."""
    return paths.launcher_root() / "build"


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
        return ("Install what is missing and try again:\n"
                "  • a C/C++ compiler: 'Visual Studio Build Tools' with the\n"
                "    'Desktop development with C++' workload, or LLVM/clang;\n"
                "  • CMake (https://cmake.org/download/);\n"
                "  • Ninja (ships with the VS Build Tools, or https://ninja-build.org/).")


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
    cands.append(r"C:\Program Files\CMake\bin\cmake.exe")
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
    """(compiler path, kind, label, vcvars) — MSVC first, then clang, then gcc."""
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
    project_dir.parent.mkdir(parents=True, exist_ok=True)
    argv = [str(cli_path()), "build", "--disc", str(image), "--bios", str(bios_path()),
            "--output", str(project_dir), "--name", name]
    code = _run_stream(argv, engine_dir(), log, timeout=3600)
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
           f'-DRECOMP_RBENGINE_ROOT="{rbengine_dir()}"']
    if tc.ninja:
        cfg += ["-DCMAKE_MAKE_PROGRAM=" + f'"{tc.ninja}"']
    lines += [" ".join(cfg), "if errorlevel 1 exit /b 11"]
    lines += [f'"{cmake}" --build "{cmake_build}" --config Release --parallel',
              "exit /b %errorlevel%"]
    script.write_text("\r\n".join(lines) + "\r\n", encoding="utf-8")


def compile_project(project_dir: pathlib.Path, cmake_build: pathlib.Path, tc: ToolChain,
                    script_dir: pathlib.Path, log) -> bool:
    if not tc.ok:
        log("!! " + tc.summary())
        return False
    script = script_dir / "build.cmd"
    script.parent.mkdir(parents=True, exist_ok=True)
    if tc.compiler_kind == "msvc" and sys.platform == "win32":
        _write_build_script(script, tc, project_dir, cmake_build)
        argv = [os.environ.get("ComSpec", "cmd.exe"), "/c", str(script)]
        code = _run_stream(argv, script_dir, log)
    else:
        cmake = tc.cmake
        argv = [cmake, "-S", str(project_dir), "-B", str(cmake_build), "-G", "Ninja",
                "-DCMAKE_BUILD_TYPE=Release", "-DPSX_RECOMP_UI=OFF",
                f"-DRECOMP_RBENGINE_ROOT={rbengine_dir()}"]
        if tc.ninja:
            argv.append(f"-DCMAKE_MAKE_PROGRAM={tc.ninja}")
        if tc.compiler and tc.compiler_kind in ("clang", "gcc"):
            argv += [f"-DCMAKE_C_COMPILER={tc.compiler}"]
            if tc.compiler_kind == "clang":
                cxx = _which("clang++") or tc.compiler
                argv += [f"-DCMAKE_CXX_COMPILER={cxx}"]
        code = _run_stream(argv, script_dir, log)
        if code == 0:
            code = _run_stream([cmake, "--build", str(cmake_build), "--config", "Release",
                                "--parallel"], script_dir, log)
    if code != 0:
        log(f"!! the compiler stopped (exit {code})")
        return False
    return True


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


def assemble_into(folder: str, exe_name: str | None, project_dir: pathlib.Path,
                  cmake_build: pathlib.Path, log) -> pathlib.Path | None:
    """Copy the finished, self-contained runtime into `Builds/<folder>/` beside the launcher.

    The runtime resolves game.toml, bios/ and its input files from its working directory, so the
    destination carries the exe, its game.toml and the bundled BIOS. Everything runtime-generated
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
    buttons find them). Any other disc is still attempted — the recompiler is the authority on
    whether it can be built — and, if it builds, lands in `Builds/OTHER` with an honest note that
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
    log("   stage 2/3: compiling — this is the long one")
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
    log("   note: this disc is not Europe or USA, so the two Play buttons do not list it — "
        "run the exe from its folder, or move it under Builds/ where you want it.")
    return BuildResult(True, region, exe,
                       f"Built a {label} disc, but it is not on a Play button.")

