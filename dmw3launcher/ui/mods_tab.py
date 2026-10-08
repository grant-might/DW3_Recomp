"""The Mods tab: mod packages, and the relink that puts their code into a build.

A mod for these builds is a PACKAGE: a ZIP whose archive root holds `manifest.toml`. Installing it
unpacks the archive to `<build>/mods/packages/<id>/<version>/`, and `<build>/mods/state.toml` is
what switches a feature on or off. That half is data only, and `dmw3launcher.mods` implements it
faithfully to the file contract the runtime's own provider documents.

That is half the job, and it is not the half a player should have to do by hand:

  * a plugin mod ships native code (`plugin/<name>.c`) that has to be COMPILED INTO the build. The
    runtime only activates plugin code whose constructor is already in the executable - a
    manifest's `[[plugin]]` block is an id reference, not a loader - so unpacking the ZIP alone
    changes nothing, and a launch with that package selected is refused
    (`trusted plugin is unavailable`).
  * the two Rebuild buttons do that step per region: they stage the package's source into the
    build's `mods_src/`, set the region define for that build, rebuild and relink, then deploy the
    executable beside the mods folder. On a build that already exists this is incremental and takes
    seconds; from nothing it is a 10 to 20 minute build, which is the Play tab's job, and the tab
    says so before the button is pressed.

The old folder-drop mods (a folder in `mods/`, moved to `mods/.disabled/` to switch off) are still
listed at the bottom. Those are data-only mods and need no relink.

Every failure is reported in the tab's own status line and Activity log rather than in a modal
dialog: the message sits next to the button that was pressed, and a modal cannot be dismissed by a
headless check.
"""
from __future__ import annotations

import pathlib
import shutil

from PySide6.QtCore import Qt, QThread, Signal
from PySide6.QtWidgets import (QComboBox, QFrame, QGroupBox, QHBoxLayout, QLabel, QListWidget,
                               QListWidgetItem, QPlainTextEdit, QPushButton, QScrollArea,
                               QTextBrowser, QVBoxLayout, QWidget)

from .. import builder, builds, mods, paths, runtime
from . import theme

RULES_SUMMARY = (
    "• A mod lives in one folder under the build's mods/ and must not write outside the build.\n"
    "• Mods may replace or add files under the build tree; they must never touch the memory "
    "cards (card1.mcd / card2.mcd), which are the player's data and belong to the Memory Card tab.\n"
    "• Ship the original file alongside a replacement as <name>.orig-stock so the mod can be "
    "reverted, the project already uses that convention.\n"
    "• A mod must not require a network connection at run time. A plugin mod's code is linked into "
    "the executable by the Rebuild buttons, so it is never patched by hand.\n"
    "• Saves made with a mod active may not load without it. Say so in the mod's README.\n"
)

COST_TEXT = (
    "A rebuild compiles the mod's code into that region's build and replaces its executable. If "
    "that build already exists this is an incremental relink and takes seconds. If it does not "
    "exist yet, build the disc on the Play tab first (10 to 20 minutes, about 3 GB): a rebuild "
    "cannot make a build from nothing."
)


class _RelinkWorker(QThread):
    """Runs the relink off the UI thread, forwarding every line of the build output."""

    line = Signal(str)
    done = Signal(object)      # builder.ModRelinkResult

    def __init__(self, region: str, sources: list[pathlib.Path], parent=None) -> None:
        super().__init__(parent)
        self.region = region
        self.sources = list(sources)

    def run(self) -> None:  # noqa: D102 (QThread entry point)
        try:
            res = builder.relink_mod(self.region, self.sources, self.line.emit)
        except Exception as exc:  # noqa: BLE001 - a worker must never take the app down
            res = builder.ModRelinkResult(False, self.region, None,
                                          f"{type(exc).__name__}: {exc}")
        self.done.emit(res)


class ModsTab(QWidget):
    def __init__(self, cfg: dict) -> None:
        super().__init__()
        self.cfg = cfg
        self.region: str = builds.default_region()
        self.package: mods.ModPackage | None = None
        self._worker: _RelinkWorker | None = None

        # The page scrolls rather than squeezes: a package card, the rebuild card, the file list
        # and the rules need more height than the window gives them, and a squeezed layout
        # collapses its padding. Same shape as the Play tab, so the page gutter is identical.
        outer = QVBoxLayout(self)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.setSpacing(0)
        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setFrameShape(QFrame.Shape.NoFrame)
        content = QWidget()
        scroll.setWidget(content)
        outer.addWidget(scroll)

        lay = QVBoxLayout(content)
        lay.setContentsMargins(theme.PAGE_MARGIN, theme.CARD_GAP,
                               theme.PAGE_MARGIN, theme.CARD_GAP)
        lay.setSpacing(theme.CARD_GAP)

        self.head = QLabel()
        self.head.setWordWrap(True)
        self.head.setObjectName("hint")
        lay.addWidget(self.head)

        pick = QHBoxLayout()
        pick.setSpacing(theme.ROW_GAP)
        pick.addWidget(QLabel("Build"))
        self.combo_region = QComboBox()
        for b in builds.BUILDS:
            self.combo_region.addItem(b.label, b.region)
        self.combo_region.currentIndexChanged.connect(self._pick_region)
        pick.addWidget(self.combo_region)
        pick.addStretch(1)
        lay.addLayout(pick)

        # ---- the package -----------------------------------------------------
        pbox = QGroupBox("Mod package")
        pl = QVBoxLayout(pbox)
        pl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        pl.setSpacing(theme.ROW_GAP)
        hint = QLabel("A mod package is a ZIP whose root holds manifest.toml. Loading one shows "
                      "what it is, what it targets and what it can switch on; installing unpacks "
                      "it into this build's mods/packages/<id>/<version>/ and writes "
                      "mods/state.toml.")
        hint.setObjectName("hint")
        hint.setWordWrap(True)
        pl.addWidget(hint)

        row1 = QHBoxLayout()
        row1.setSpacing(theme.ROW_GAP)
        self.btn_load = QPushButton("Load mod package…")
        self.btn_load.clicked.connect(self.load_package)
        self.btn_open_packages = QPushButton("Open packages folder")
        self.btn_open_packages.clicked.connect(self._open_packages)
        row1.addWidget(self.btn_load)
        row1.addWidget(self.btn_open_packages)
        row1.addStretch(1)
        pl.addLayout(row1)

        self.card = QTextBrowser()
        self.card.setOpenExternalLinks(True)
        self.card.setMinimumHeight(110)
        self.card.setMaximumHeight(190)
        pl.addWidget(self.card)

        row2 = QHBoxLayout()
        row2.setSpacing(theme.ROW_GAP)
        self.btn_install = QPushButton("Install")
        self.btn_install.clicked.connect(lambda: self.install())
        self.btn_reinstall = QPushButton("Reinstall")
        self.btn_reinstall.clicked.connect(lambda: self.install(replace=True))
        self.btn_remove = QPushButton("Remove")
        self.btn_remove.clicked.connect(self.remove_package)
        row2.addWidget(self.btn_install)
        row2.addWidget(self.btn_reinstall)
        row2.addWidget(self.btn_remove)
        row2.addStretch(1)
        pl.addLayout(row2)

        row3 = QHBoxLayout()
        row3.setSpacing(theme.ROW_GAP)
        self.combo_feature = QComboBox()
        self.combo_feature.setMinimumWidth(220)
        row3.addWidget(QLabel("Feature"))
        row3.addWidget(self.combo_feature)
        self.btn_enable = QPushButton("Enable")
        self.btn_enable.clicked.connect(lambda: self.set_enabled(True))
        self.btn_disable = QPushButton("Disable")
        self.btn_disable.clicked.connect(lambda: self.set_enabled(False))
        row3.addWidget(self.btn_enable)
        row3.addWidget(self.btn_disable)
        row3.addStretch(1)
        pl.addLayout(row3)

        self.lbl_state = QLabel()
        self.lbl_state.setWordWrap(True)
        self.lbl_state.setObjectName("hint")
        pl.addWidget(self.lbl_state)
        lay.addWidget(pbox)

        # ---- the rebuild -----------------------------------------------------
        rbox = QGroupBox("Rebuild with this mod")
        rl = QVBoxLayout(rbox)
        rl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        rl.setSpacing(theme.ROW_GAP)
        self.lbl_cost = QLabel(COST_TEXT)
        self.lbl_cost.setObjectName("hint")
        self.lbl_cost.setWordWrap(True)
        rl.addWidget(self.lbl_cost)

        row4 = QHBoxLayout()
        row4.setSpacing(theme.CARD_GAP)
        self.btn_rebuild_us = QPushButton("Rebuild (USA)")
        self.btn_rebuild_us.clicked.connect(lambda: self.rebuild(builds.REGION_US))
        self.btn_rebuild_eu = QPushButton("Rebuild (EUR)")
        self.btn_rebuild_eu.clicked.connect(lambda: self.rebuild(builds.REGION_EU))
        self._rebuild_buttons = (self.btn_rebuild_us, self.btn_rebuild_eu)
        for b in self._rebuild_buttons:
            b.setMinimumWidth(170)      # both labels are short; one width reads as a pair
            row4.addWidget(b)
        row4.addStretch(1)
        rl.addLayout(row4)

        self.lbl_rebuild = QLabel()
        self.lbl_rebuild.setWordWrap(True)
        self.lbl_rebuild.setObjectName("hint")
        rl.addWidget(self.lbl_rebuild)

        self.log = QPlainTextEdit()
        self.log.setReadOnly(True)
        self.log.setPlaceholderText("Installing a package and rebuilding report here.")
        self.log.setMinimumHeight(96)
        self.log.setMaximumHeight(150)
        rl.addWidget(self.log)
        lay.addWidget(rbox)

        # ---- folder mods (data only, no relink) ------------------------------
        box = QGroupBox("Mod files in this build")
        bl = QVBoxLayout(box)
        bl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        bl.setSpacing(theme.ROW_GAP)
        self.list_mods = QListWidget()
        self.list_mods.setMinimumHeight(120)
        bl.addWidget(self.list_mods)
        row5 = QHBoxLayout()
        row5.setSpacing(theme.ROW_GAP)
        self.btn_toggle = QPushButton("Enable / disable")
        self.btn_toggle.clicked.connect(self.toggle)
        self.btn_folder = QPushButton("Open mods folder")
        self.btn_folder.clicked.connect(self._open_mods)
        self.btn_refresh = QPushButton("Refresh")
        self.btn_refresh.clicked.connect(self.refresh)
        row5.addWidget(self.btn_toggle)
        row5.addWidget(self.btn_folder)
        row5.addWidget(self.btn_refresh)
        row5.addStretch(1)
        bl.addLayout(row5)
        note = QLabel("Folders here are data-only mods: they need no rebuild, and enable/disable "
                      "just moves them in or out of the way.")
        note.setObjectName("hint")
        note.setWordWrap(True)
        bl.addWidget(note)
        lay.addWidget(box)

        # ---- rules -----------------------------------------------------------
        gbox = QGroupBox("Rules for mods")
        gl = QVBoxLayout(gbox)
        gl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        gl.setSpacing(theme.ROW_GAP)
        self.guide = QTextBrowser()
        self.guide.setOpenExternalLinks(True)
        self.guide.setMaximumHeight(150)
        gl.addWidget(self.guide)
        row6 = QHBoxLayout()
        row6.setSpacing(theme.ROW_GAP)
        self.btn_guide = QPushButton("Open full guide (docs/MODS.md)")
        self.btn_guide.clicked.connect(self._open_guide)
        row6.addWidget(self.btn_guide)
        self.btn_mkmod = QPushButton("Create empty mod skeleton…")
        self.btn_mkmod.clicked.connect(self._skeleton)
        row6.addWidget(self.btn_mkmod)
        row6.addStretch(1)
        gl.addLayout(row6)
        lay.addWidget(gbox)
        lay.addStretch(1)

        self.guide.setPlainText(RULES_SUMMARY)
        index = self.combo_region.findData(self.region)
        if index >= 0:
            self.combo_region.blockSignals(True)
            self.combo_region.setCurrentIndex(index)
            self.combo_region.blockSignals(False)
        self.refresh()

    # ------------------------------------------------------------------ helpers
    def _exe_dir(self) -> pathlib.Path:
        """The folder a build's `mods/` sits in: the runtime resolves it next to its exe."""
        return builds.build_dir(self.region)

    def _say(self, text: str) -> None:
        self.log.appendPlainText(text)

    def _set_label(self, label: QLabel, state: str, text: str) -> None:
        label.setText(text)
        if label.objectName() != state:
            label.setObjectName(state)
            label.style().unpolish(label)
            label.style().polish(label)

    def _pick_region(self) -> None:
        self.region = self.combo_region.currentData() or builds.default_region()
        self.refresh()

    def _simplify_path(self, p: pathlib.Path) -> str:
        """A short, readable form of a path inside the launcher (the tree is deeply nested)."""
        try:
            return str(p.relative_to(paths.launcher_root()))
        except ValueError:
            return str(p)

    # ------------------------------------------------------------------ refresh
    def refresh(self) -> None:
        ready = builds.build_status(self.region)[0]
        label = builds.spec(self.region).label
        if ready:
            self.head.setText(f"{label} build. Mods for it live in "
                              f"{self._simplify_path(self._exe_dir() / 'mods')}")
        else:
            self.head.setText(f"The {label} build is not on disk yet, so there is nothing to "
                              f"install a mod into. Build it on the Play tab first.")
        for b in (self.btn_toggle, self.btn_folder, self.btn_mkmod, self.btn_install,
                  self.btn_reinstall, self.btn_remove, self.btn_open_packages):
            b.setEnabled(ready)
        self._refresh_feature_combo()
        self._refresh_mod_list()
        self._refresh_package_card()
        self._refresh_rebuild_state()

    def _refresh_mod_list(self) -> None:
        self.list_mods.clear()
        if not builds.build_status(self.region)[0]:
            return
        on = builds.mods_dir(self.region)
        off = on / ".disabled"
        on.mkdir(parents=True, exist_ok=True)
        for base, state in ((on, "enabled"), (off, "disabled")):
            if not base.is_dir():
                continue
            for p in sorted(base.iterdir()):
                if p.name.startswith("."):
                    continue
                it = QListWidgetItem(f"[{state}]  {p.name}"
                                     + ("  (folder)" if p.is_dir() else ""))
                it.setData(Qt.ItemDataRole.UserRole, (state, str(p)))
                self.list_mods.addItem(it)
        if self.list_mods.count() == 0:
            self.list_mods.addItem("No mod files in this build yet.")

    def _refresh_feature_combo(self) -> None:
        self.combo_feature.blockSignals(True)
        self.combo_feature.clear()
        pkg = self.package
        ready = builds.build_status(self.region)[0]
        if pkg is None or not pkg.features:
            self.combo_feature.addItem("(no package loaded)", None)
            self.combo_feature.setEnabled(False)
            self.btn_enable.setEnabled(False)
            self.btn_disable.setEnabled(False)
        else:
            states = mods.feature_states(self._exe_dir(), pkg) if ready else {}
            self.combo_feature.addItem("All features", None)
            for f in pkg.features:
                mark = ""
                if f.id in states:
                    mark = "  [on]" if states[f.id] else "  [off]"
                self.combo_feature.addItem(f"{f.id}  ({f.name}){mark}" if f.name
                                           else f"{f.id}{mark}", f.id)
            self.combo_feature.setEnabled(True)
            can = ready and mods.is_installed(self._exe_dir(), pkg)
            self.btn_enable.setEnabled(can)
            self.btn_disable.setEnabled(can)
        self.combo_feature.blockSignals(False)

    def _refresh_package_card(self) -> None:
        pkg = self.package
        ready = builds.build_status(self.region)[0]
        if pkg is None:
            self.card.setPlainText(
                "No package loaded.\n\n"
                "Load a mod package (a .zip whose root holds manifest.toml) to see its id, "
                "version, targets and features, then install it and press a Rebuild button so its "
                "code is linked into that region's build.")
            self._set_label(self.lbl_state, "hint", "")
            return
        lines = [f"{pkg.name}  {pkg.version}",
                 f"id        {pkg.id}",
                 f"format    {pkg.format_version}",
                 "targets   " + (", ".join(pkg.targets) if pkg.targets else "none declared")]
        if pkg.targets and not pkg.targets_region(self.region):
            lines.append(f"          ! this package does not list "
                         f"{builds.spec(self.region).label} as a target")
        lines.append("plugin    " + (", ".join(pkg.plugins) + "  (native code: needs a rebuild)"
                                     if pkg.plugins else
                                     "none, this is a data-only mod, no rebuild needed"))
        if ready:
            lines.append("installed " + mods.state_summary(self._exe_dir(), pkg)
                         + f"  ({self._simplify_path(mods.install_dir(self._exe_dir(), pkg))})")
        else:
            lines.append("installed no (this build is not on disk yet)")
        if pkg.features:
            states = mods.feature_states(self._exe_dir(), pkg) if ready else {}
            lines += ["", "Features"]
            for f in pkg.features:
                mark = ""
                if f.id in states:
                    mark = "  [on]" if states[f.id] else "  [off]"
                lines.append(f"  * {f.id}{' - ' + f.name if f.name else ''}{mark}")
                if f.description:
                    lines.append(f"      {f.description}")
        if pkg.description:
            lines += ["", "Description", pkg.description]
        self.card.setPlainText("\n".join(lines))

        state = (mods.state_summary(self._exe_dir(), pkg) if ready
                 else "not installed here (build this region on the Play tab first)")
        self._set_label(self.lbl_state, "ok" if "enabled" in state else "warn",
                        f"{pkg.id} {pkg.version}: {state}")

    def _refresh_rebuild_state(self) -> None:
        """The two rebuild buttons: off until a package is loaded, and honest about each region."""
        busy = self._worker is not None
        loaded = self.package is not None
        for b in self._rebuild_buttons:
            b.setEnabled(loaded and not busy)
        if busy:
            self._set_label(self.lbl_rebuild, "hint", "Rebuilding… see the log below.")
            return
        if not loaded:
            self._set_label(self.lbl_rebuild, "hint",
                            "Rebuild is off until a mod package is loaded.")
            return
        if not self.package.has_plugin:
            self._set_label(self.lbl_rebuild, "hint",
                            f"{self.package.id} ships no plugin code, so it needs no rebuild. "
                            f"Install it above and play.")
            return
        parts = []
        for region in (builds.REGION_US, builds.REGION_EU):
            spec = builds.spec(region)
            if builder.has_project(region):
                parts.append(f"{spec.label}: build tree ready, relink is incremental")
            elif builds.build_status(region)[0]:
                parts.append(f"{spec.label}: executable present but no project tree, rebuild it "
                             f"from disc on the Play tab first")
            else:
                parts.append(f"{spec.label}: not built yet, build it on the Play tab first")
        self._set_label(self.lbl_rebuild, "hint",
                        "Press the button for the region you play.   " + "   ".join(parts))

    # ------------------------------------------------------------------ the package
    def _pick_package_file(self) -> str:
        """The package file picker, in one place so the load path can be driven without a modal."""
        from PySide6.QtWidgets import QFileDialog
        path, _ = QFileDialog.getOpenFileName(self, "Load a mod package", "",
                                             "Mod packages (*.zip);;All files (*)")
        return path or ""

    def load_package(self) -> None:
        path = self._pick_package_file()
        if not path:
            return
        try:
            pkg = mods.read_package(pathlib.Path(path))
        except mods.ModPackageError as exc:
            self.package = None
            self._say(f"FAILED to load {pathlib.Path(path).name}: {exc}")
            self._refresh_feature_combo()
            self._refresh_package_card()
            self._refresh_rebuild_state()
            self._set_label(self.lbl_state, "err", str(exc))
            return
        self.package = pkg
        self._say(f"loaded package {pkg.id} {pkg.version} from {pathlib.Path(path).name}")
        self._say(f"   targets: {', '.join(pkg.targets) or '(none declared)'}; "
                  f"features: {', '.join(f.id for f in pkg.features) or '(none)'}; "
                  f"plugin: {', '.join(pkg.plugins) or '(none, data only)'}")
        self._refresh_feature_combo()
        self._refresh_package_card()
        self._refresh_rebuild_state()

    def install(self, replace: bool = False) -> bool:
        """Unpack the loaded package into this build. A fresh install switches its features on."""
        pkg = self.package
        if pkg is None:
            return False
        if not builds.build_status(self.region)[0]:
            self._set_label(self.lbl_state, "err",
                            f"The {builds.spec(self.region).label} build is not on disk yet. "
                            f"Build it on the Play tab first.")
            return False
        exe_dir = self._exe_dir()
        try:
            dest = mods.install(pkg, exe_dir, replace=replace)
        except mods.ModPackageError as exc:
            self._say(f"FAILED to install {pkg.id} {pkg.version}: {exc}")
            self._set_label(self.lbl_state, "err", str(exc))
            return False
        self._say(f"installed {pkg.id} {pkg.version} -> {self._simplify_path(dest)}")
        if pkg.features:
            # Loading and installing a package is the player asking for that mod, so a fresh
            # install switches its features ON; the manifest default only governs the case where no
            # state file exists at all. Disable is one press away and the state line always says
            # which way it is.
            try:
                mods.set_features(exe_dir, pkg, {f.id: True for f in pkg.features})
                self._say("   enabled: " + ", ".join(f.id for f in pkg.features))
            except OSError as exc:
                self._say(f"   note: could not write state.toml ({exc})")
        self._refresh_feature_combo()
        self._refresh_package_card()
        self._refresh_rebuild_state()
        return True

    def remove_package(self) -> None:
        pkg = self.package
        if pkg is None:
            return
        try:
            mods.remove(pkg, self._exe_dir())
        except OSError as exc:
            self._set_label(self.lbl_state, "err", f"Could not remove {pkg.id}: {exc}")
            return
        self._say(f"removed {pkg.id} {pkg.version} from this build and dropped it from state.toml")
        self._refresh_feature_combo()
        self._refresh_package_card()
        self._refresh_rebuild_state()

    def set_enabled(self, enabled: bool) -> None:
        pkg = self.package
        if pkg is None or not pkg.features:
            return
        if not mods.is_installed(self._exe_dir(), pkg):
            self._set_label(self.lbl_state, "err",
                            f"{pkg.id} {pkg.version} is not installed in this build yet. "
                            f"Press Install first.")
            return
        fid = self.combo_feature.currentData()
        wanted = {f.id: enabled for f in pkg.features} if fid is None else {fid: enabled}
        try:
            mods.set_features(self._exe_dir(), pkg, wanted)
        except OSError as exc:
            self._set_label(self.lbl_state, "err", f"Could not write state.toml: {exc}")
            return
        what = "all features" if fid is None else str(fid)
        self._say(f"{'enabled' if enabled else 'disabled'} {what} in "
                  f"{self._simplify_path(mods.state_path(self._exe_dir()))}")
        self._refresh_feature_combo()
        self._refresh_package_card()

    def _open_packages(self) -> None:
        d = mods.packages_dir(self._exe_dir())
        d.mkdir(parents=True, exist_ok=True)
        runtime.open_path(d)

    def _open_mods(self) -> None:
        if not builds.build_status(self.region)[0]:
            return
        on = builds.mods_dir(self.region)
        on.mkdir(parents=True, exist_ok=True)
        runtime.open_path(on)

    def _open_guide(self) -> None:
        runtime.open_path(paths.docs_dir() / "MODS.md")

    def _skeleton(self) -> None:
        if not builds.build_status(self.region)[0]:
            return
        on = builds.mods_dir(self.region)
        base = on / "my-mod"
        n = 1
        while base.exists():
            n += 1
            base = on / f"my-mod-{n}"
        base.mkdir(parents=True, exist_ok=True)
        (base / "README.md").write_text(
            f"# {base.name}\n\nWhat this mod changes, and how to uninstall it.\n\n"
            f"## Files it replaces\n\n- `path/inside/build/modified.file` "
            f"(original kept as `modified.file.orig-stock`)\n\n"
            f"## Does it affect saves?\n\nNo / Yes, explain.\n", encoding="utf-8")
        self._refresh_mod_list()
        runtime.open_path(base)

    def toggle(self) -> None:
        it = self.list_mods.currentItem()
        if not it:
            return
        data = it.data(Qt.ItemDataRole.UserRole)
        if not data:
            return
        state, path = data
        src = pathlib.Path(path)
        on = builds.mods_dir(self.region)
        off = on / ".disabled"
        dst_dir = off if state == "enabled" else on
        dst_dir.mkdir(parents=True, exist_ok=True)
        try:
            shutil.move(str(src), str(dst_dir / src.name))
        except OSError as exc:
            self._say(f"FAILED to move {src.name}: {exc}")
        self._refresh_mod_list()

    # ------------------------------------------------------------------ the rebuild
    def rebuild(self, region: str) -> None:
        """Install the loaded package for `region`, relink it into that build, deploy the exe."""
        pkg = self.package
        if pkg is None or self._worker is not None:
            return
        spec = builds.spec(region)
        exe_dir = builds.build_dir(region)
        if not builds.build_status(region)[0] or not builder.has_project(region):
            self._set_label(
                self.lbl_rebuild, "err",
                f"There is no finished {spec.label} build to relink. Build the {spec.label} disc "
                f"on the Play tab first (10 to 20 minutes, about 3 GB); once that build exists a "
                f"mod relink takes seconds.")
            self._say(f"rebuild ({spec.label}) refused: no build at {exe_dir}")
            return
        if not pkg.has_plugin:
            self._set_label(self.lbl_rebuild, "hint",
                            f"{pkg.id} ships no plugin code, so there is nothing to link in. "
                            f"Install it and play.")
            return
        # The code half needs the package's source and the data half the packages/ folder plus
        # state.toml inside that region's mods folder, so install first when it is not there yet.
        try:
            if not mods.is_installed(exe_dir, pkg):
                dest = mods.install(pkg, exe_dir)
                self._say(f"installed {pkg.id} {pkg.version} -> {self._simplify_path(dest)}")
                if pkg.features:
                    mods.set_features(exe_dir, pkg, {f.id: True for f in pkg.features})
                    self._say("   enabled: " + ", ".join(f.id for f in pkg.features))
        except mods.ModPackageError as exc:
            self._set_label(self.lbl_rebuild, "err", str(exc))
            self._say(f"FAILED: {exc}")
            return
        sources = mods.plugin_sources(mods.install_dir(exe_dir, pkg))
        if not sources:
            self._set_label(self.lbl_rebuild, "err",
                            f"{pkg.id} declares a plugin but ships no source under plugin/, so "
                            f"there is nothing to compile in this package.")
            return
        self._worker = _RelinkWorker(region, sources, self)
        self._worker.line.connect(self._say)
        self._worker.done.connect(self._on_relink_done)
        self._refresh_rebuild_state()
        self._say(f"== Rebuild ({spec.label}) with {pkg.id} {pkg.version} ==")
        self._worker.start()

    def _on_relink_done(self, res) -> None:
        self._worker = None
        ok = bool(getattr(res, "ok", False))
        message = str(getattr(res, "message", "Rebuild finished."))
        # Refresh FIRST: it re-enables the buttons and rebuilds the package card. Setting the
        # outcome after it is what keeps the outcome on screen - the refresh recomputes the
        # rebuild line from scratch and would otherwise wipe the message the player needs.
        self.refresh()
        if ok:
            self._set_label(self.lbl_rebuild, "ok", message)
            self._say("REBUILD OK: " + message)
        else:
            self._set_label(self.lbl_rebuild, "err", message)
            self._say("FAILED: " + message)
