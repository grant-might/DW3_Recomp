"""The mod package contract, implemented on the launcher side.

A mod package is a **ZIP whose archive root holds `manifest.toml`**. Installing one maps the
archive root onto

    <EXE_DIR>/mods/packages/<manifest id>/<manifest version>/

and the mod's features are switched on or off by `<EXE_DIR>/mods/state.toml`
(`format_version = 2`, one `[[package]]` block per installed package and one `[[feature]]` block
per feature with an `enabled` flag).

`EXE_DIR` is the folder the build's executable lives in, which is exactly what the launcher calls a
build folder (`Builds/EUR`, `Builds/USA`): the runtime resolves `mods` next to itself, so a package
installed here is the one that build runs with.

Nothing here calls the runtime's own C++ provider - the launcher is Python, and the provider is
compiled into the game. What the provider does with a package is a documented FILE contract, so this
module implements that contract directly: it unpacks the archive to the right place and writes the
state file. The one thing a file cannot do is load a plugin, because plugin callbacks are registered
by constructors compiled into the executable (the manifest's `[[plugin]]` block is only an id
reference). That is why a plugin package also needs the relink the Mods tab's Rebuild buttons do
(see `builder.relink_mod`), and this module reports whether a package carries plugin source at all.
"""
from __future__ import annotations

import pathlib
import shutil
import zipfile
from dataclasses import dataclass, field

try:
    import tomllib  # Python 3.11+
except ImportError:  # pragma: no cover - only on an older interpreter
    tomllib = None  # type: ignore[assignment]

# The state file's own format version, as the runtime writes and reads it.
FORMAT_VERSION = 2

MANIFEST_NAME = "manifest.toml"
PACKAGES_DIRNAME = "packages"
STATE_NAME = "state.toml"
SOURCE_SUFFIXES = (".c", ".cpp", ".cc")


class ModPackageError(Exception):
    """A package that cannot be read, installed, or trusted. The message is player-facing."""


@dataclass(frozen=True)
class ModFeature:
    """One switchable part of a package (`[[feature]]` in the manifest)."""

    id: str
    name: str = ""
    description: str = ""
    group: str = ""
    default_enabled: bool = False


@dataclass
class ModPackage:
    """A package's manifest, read from a ZIP or from an installed package directory."""

    id: str
    version: str
    name: str
    description: str = ""
    targets: tuple[str, ...] = ()
    features: tuple[ModFeature, ...] = ()
    plugins: tuple[str, ...] = ()          # plugin ids the manifest references
    format_version: int = 0
    manifest: dict = field(default_factory=dict)
    zip_path: pathlib.Path | None = None   # set when it came from a package file

    @property
    def has_plugin(self) -> bool:
        """True when this package ships native code that has to be linked into a build."""
        return bool(self.plugins)

    def feature(self, feature_id: str) -> ModFeature | None:
        return next((f for f in self.features if f.id == feature_id), None)

    def targets_region(self, region: str) -> bool:
        """True when the package's own `[[target]]` list names this region's disc id."""
        from . import disc
        wanted = {disc.REGION_EU: "SLES-03936", disc.REGION_US: "SLUS-01436"}.get(region)
        return bool(wanted) and wanted in self.targets

    def summary(self) -> str:
        return f"{self.name} {self.version} ({self.id})"


# ---------------------------------------------------------------- reading a manifest

def _as_bool(value, default: bool = False) -> bool:
    if isinstance(value, bool):
        return value
    if isinstance(value, (int, float)):
        return bool(value)
    if isinstance(value, str):
        return value.strip().lower() in ("1", "true", "yes", "on")
    return default


def parse_manifest(text: str, origin: str = "") -> ModPackage:
    """Parse a `manifest.toml` into a `ModPackage`, refusing one that is not usable."""
    if tomllib is None:  # pragma: no cover
        raise ModPackageError("This Python cannot read TOML (needs 3.11+).")
    where = f" in {origin}" if origin else ""
    try:
        doc = tomllib.loads(text)
    except Exception as exc:  # noqa: BLE001
        raise ModPackageError(f"{MANIFEST_NAME}{where} is not valid TOML: {exc}") from None

    missing = [k for k in ("format_version", "id", "version", "name") if k not in doc]
    if missing:
        raise ModPackageError(
            f"{MANIFEST_NAME}{where} is missing {', '.join(missing)}. "
            f"id, version, name and format_version are required.")

    features = tuple(
        ModFeature(
            id=str(f.get("id", "")),
            name=str(f.get("name", "")),
            description=str(f.get("description", "")),
            group=str(f.get("group", "")),
            default_enabled=_as_bool(f.get("default_enabled", False)),
        )
        for f in (doc.get("feature") or []) if isinstance(f, dict) and f.get("id")
    )
    plugins = tuple(
        str(p.get("id", "")) for p in (doc.get("plugin") or [])
        if isinstance(p, dict) and p.get("id")
    )
    targets = tuple(
        str(t.get("game_id", "")) for t in (doc.get("target") or [])
        if isinstance(t, dict) and t.get("game_id")
    )
    return ModPackage(
        id=str(doc["id"]),
        version=str(doc["version"]),
        name=str(doc["name"]),
        description=str(doc.get("description", "")),
        targets=targets,
        features=features,
        plugins=plugins,
        format_version=int(doc.get("format_version", 0) or 0),
        manifest=doc,
    )


def read_package(zip_path: pathlib.Path) -> ModPackage:
    """Read a package file. The archive ROOT must hold `manifest.toml`."""
    path = pathlib.Path(zip_path)
    if not path.is_file():
        raise ModPackageError(f"No such package file: {path}")
    if not zipfile.is_zipfile(path):
        raise ModPackageError(f"{path.name} is not a ZIP archive. A package is a ZIP whose "
                              f"root holds {MANIFEST_NAME}.")
    try:
        with zipfile.ZipFile(path) as zf:
            names = [n for n in zf.namelist() if not n.endswith("/")]
            if MANIFEST_NAME not in names:
                root_manifests = [n for n in names if n.lower().endswith("/" + MANIFEST_NAME)]
                hint = (f" It is at {root_manifests[0]!r}, so the archive wraps it in a folder. "
                        f"Repack so {MANIFEST_NAME} is at the root.") if root_manifests else ""
                raise ModPackageError(
                    f"{path.name} has no {MANIFEST_NAME} at its archive root.{hint}")
            text = zf.read(MANIFEST_NAME).decode("utf-8-sig", errors="replace")
    except ModPackageError:
        raise
    except (OSError, zipfile.BadZipFile) as exc:
        raise ModPackageError(f"Could not read {path.name}: {exc}") from None
    pkg = parse_manifest(text, f"{path.name}:{MANIFEST_NAME}")
    pkg.zip_path = path
    return pkg


# ---------------------------------------------------------------- paths inside a build

def mods_dir(exe_dir: pathlib.Path) -> pathlib.Path:
    return pathlib.Path(exe_dir) / "mods"


def packages_dir(exe_dir: pathlib.Path) -> pathlib.Path:
    return mods_dir(exe_dir) / PACKAGES_DIRNAME


def state_path(exe_dir: pathlib.Path) -> pathlib.Path:
    return mods_dir(exe_dir) / STATE_NAME


def install_dir(exe_dir: pathlib.Path, package: ModPackage) -> pathlib.Path:
    return packages_dir(exe_dir) / package.id / package.version


def installed_versions(exe_dir: pathlib.Path, package_id: str) -> list[str]:
    d = packages_dir(exe_dir) / package_id
    if not d.is_dir():
        return []
    return sorted(p.name for p in d.iterdir() if p.is_dir())


def is_installed(exe_dir: pathlib.Path, package: ModPackage) -> bool:
    return (install_dir(exe_dir, package) / MANIFEST_NAME).is_file()


def installed_packages(exe_dir: pathlib.Path) -> list[ModPackage]:
    """Every package version installed in a build, newest manifest read from disk."""
    out: list[ModPackage] = []
    root = packages_dir(exe_dir)
    if not root.is_dir():
        return out
    for manifest in sorted(root.glob(f"*/*/{MANIFEST_NAME}")):
        try:
            text = manifest.read_text(encoding="utf-8-sig", errors="replace")
            pkg = parse_manifest(text, str(manifest))
        except (OSError, ModPackageError):
            continue
        out.append(pkg)
    return out


# ---------------------------------------------------------------- installing

def plugin_sources(package_dir: pathlib.Path) -> list[pathlib.Path]:
    """The plugin source files a package ships (`plugin/**/*.c`). Empty for a data-only mod."""
    d = pathlib.Path(package_dir) / "plugin"
    if not d.is_dir():
        return []
    return sorted(p for p in d.rglob("*")
                  if p.is_file() and p.suffix.lower() in SOURCE_SUFFIXES)


def _extract_archive(zf: zipfile.ZipFile, dest: pathlib.Path) -> None:
    """Extract a whole archive into `dest`, refusing a member that escapes it."""
    dest = dest.resolve()
    for info in zf.infolist():
        name = info.filename.replace("\\", "/")
        if not name or name.endswith("/"):
            continue
        target = (dest / name).resolve()
        if dest != target and dest not in target.parents:
            raise ModPackageError(f"The package contains a path outside the mods folder: "
                                  f"{info.filename!r}")
        target.parent.mkdir(parents=True, exist_ok=True)
        with zf.open(info) as src, open(target, "wb") as out:
            shutil.copyfileobj(src, out)


def install(package: ModPackage, exe_dir: pathlib.Path, replace: bool = False) -> pathlib.Path:
    """Unpack a package into `<exe_dir>/mods/packages/<id>/<version>/` and return that path.

    The runtime's own installer refuses a package/version directory that already exists, and so
    does this - `replace=True` (the tab's Reinstall button) removes the old copy first, which is
    the documented way to put a package back.
    """
    if package.zip_path is None:
        raise ModPackageError("This package was not read from a file, so it cannot be installed. "
                              "Use \"Load mod package\" and pick the ZIP.")
    dest = install_dir(exe_dir, package)
    if dest.exists():
        if not replace:
            raise ModPackageError(
                f"{package.id} {package.version} is already installed in this build. "
                f"Use Reinstall to replace it. ({dest})")
        shutil.rmtree(dest, ignore_errors=True)
    dest.mkdir(parents=True, exist_ok=True)
    try:
        with zipfile.ZipFile(package.zip_path) as zf:
            _extract_archive(zf, dest)
    except ModPackageError:
        shutil.rmtree(dest, ignore_errors=True)
        raise
    except (OSError, zipfile.BadZipFile) as exc:
        shutil.rmtree(dest, ignore_errors=True)
        raise ModPackageError(f"Could not unpack {package.zip_path.name}: {exc}") from None
    if not (dest / MANIFEST_NAME).is_file():
        shutil.rmtree(dest, ignore_errors=True)
        raise ModPackageError(f"{package.zip_path.name} did not unpack a {MANIFEST_NAME}.")
    return dest


def remove(package: ModPackage, exe_dir: pathlib.Path) -> None:
    """Delete an installed package version and drop it from the state file."""
    d = install_dir(exe_dir, package)
    if d.is_dir():
        shutil.rmtree(d, ignore_errors=True)
    parent = d.parent
    if parent.is_dir() and not any(parent.iterdir()):
        try:
            parent.rmdir()
        except OSError:
            pass
    drop_package(exe_dir, package.id)


# ---------------------------------------------------------------- the state file

@dataclass
class StatePackage:
    """One `[[package]]` plus its features' switches, as the state file holds them."""

    id: str
    version: str
    features: dict[str, bool] = field(default_factory=dict)


def _q(text: str) -> str:
    return '"' + str(text).replace("\\", "\\\\").replace('"', '\\"') + '"'


def _read_state(exe_dir: pathlib.Path) -> dict[str, StatePackage]:
    """Read `mods/state.toml` into packages. An unreadable or other-version file reads as empty,
    which is the SAFE direction: every feature then falls back to its manifest default (off for
    this project's mods) rather than to something we guessed at."""
    out: dict[str, StatePackage] = {}
    p = state_path(exe_dir)
    if not p.is_file() or tomllib is None:
        return out
    try:
        doc = tomllib.loads(p.read_text(encoding="utf-8-sig", errors="replace"))
    except Exception:  # noqa: BLE001
        return out
    if int(doc.get("format_version", 0) or 0) != FORMAT_VERSION:
        return out
    for entry in doc.get("package") or []:
        if isinstance(entry, dict) and entry.get("id"):
            out[str(entry["id"])] = StatePackage(str(entry["id"]), str(entry.get("version", "")), {})
    for entry in doc.get("feature") or []:
        if not isinstance(entry, dict):
            continue
        pid = str(entry.get("package_id", ""))
        if pid in out and entry.get("id"):
            out[pid].features[str(entry["id"])] = _as_bool(entry.get("enabled", False))
    return out


def _render_state(pkgs: dict[str, StatePackage]) -> str:
    """The canonical file: format_version, then a [[package]] and its [[feature]]s per package."""
    lines = [f"format_version = {FORMAT_VERSION}", ""]
    for s in pkgs.values():
        lines += ["[[package]]", f"id = {_q(s.id)}", f"version = {_q(s.version)}", ""]
        for fid, enabled in s.features.items():
            lines += ["[[feature]]", f"package_id = {_q(s.id)}", f"id = {_q(fid)}",
                      f"enabled = {'true' if enabled else 'false'}", ""]
    return "\n".join(lines).rstrip("\n") + "\n"


def read_state(exe_dir: pathlib.Path) -> list[StatePackage]:
    return list(_read_state(exe_dir).values())


def write_state(exe_dir: pathlib.Path, packages: list[StatePackage]) -> pathlib.Path:
    """Write the state file from scratch. The runtime rewrites it canonically after a mod commit,
    so producing canonical content is safe (it is documented as safe to rewrite between runs)."""
    d = mods_dir(exe_dir)
    d.mkdir(parents=True, exist_ok=True)
    p = state_path(exe_dir)
    tmp = p.with_suffix(".toml.tmp")
    tmp.write_text(_render_state({s.id: s for s in packages}), encoding="utf-8")
    tmp.replace(p)
    return p


def set_features(exe_dir: pathlib.Path, package: ModPackage,
                 features: dict[str, bool]) -> pathlib.Path:
    """Merge one package's feature switches into the state file, keeping every other package."""
    pkgs = _read_state(exe_dir)
    entry = pkgs.get(package.id) or StatePackage(package.id, package.version, {})
    entry.version = package.version
    for fid, enabled in features.items():
        entry.features[fid] = bool(enabled)
    pkgs[package.id] = entry
    return write_state(exe_dir, list(pkgs.values()))


def set_feature(exe_dir: pathlib.Path, package: ModPackage,
                feature_id: str, enabled: bool) -> pathlib.Path:
    return set_features(exe_dir, package, {feature_id: enabled})


def drop_package(exe_dir: pathlib.Path, package_id: str) -> pathlib.Path | None:
    pkgs = _read_state(exe_dir)
    if package_id in pkgs:
        del pkgs[package_id]
    if not pkgs and not state_path(exe_dir).is_file():
        return None
    return write_state(exe_dir, list(pkgs.values()))


def feature_states(exe_dir: pathlib.Path, package: ModPackage) -> dict[str, bool]:
    """The effective switch per feature: the state file's value, else the manifest default."""
    entry = _read_state(exe_dir).get(package.id)
    out: dict[str, bool] = {}
    for f in package.features:
        if entry is not None and f.id in entry.features:
            out[f.id] = entry.features[f.id]
        else:
            out[f.id] = f.default_enabled
    return out


def state_summary(exe_dir: pathlib.Path, package: ModPackage) -> str:
    """A one-line description of the installed + enabled state, or why it is neither."""
    if not is_installed(exe_dir, package):
        return "not installed in this build"
    states = feature_states(exe_dir, package)
    if not states:
        return "installed (the manifest declares no features)"
    on = [fid for fid, en in states.items() if en]
    off = [fid for fid, en in states.items() if not en]
    parts = []
    if on:
        parts.append("enabled: " + ", ".join(on))
    if off:
        parts.append("off: " + ", ".join(off))
    return "installed; " + "; ".join(parts)
