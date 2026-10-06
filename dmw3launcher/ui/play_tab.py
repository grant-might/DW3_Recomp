"""The Play tab: build the game from the player's own disc, then run the regional builds.

The launcher ships no game code. Its Play tab is therefore a BUILDER plus two launch buttons:

  * the player points at their own Digimon World 3 disc image (Europe OR USA - both are
    first-class, nothing is refused for its region);
  * the tab reads the disc's own serial to name the region, pre-flights the C/C++ toolchain and
    says precisely what is missing BEFORE starting a 10 to 20 minute recompile;
  * it runs the bundled recompiler and compiles the result with the player's own CMake + Ninja +
    compiler, streaming honest progress into the Activity log;
  * the finished build lands in Builds/EUR or Builds/USA beside the launcher, where the two Play
    buttons (USA first, Europe second - the house order) run it.

Each build is self-contained (exe + game.toml + bios/ + its input files), and the runtime resolves
those from its working directory, so a launch sets cwd to that folder. The disc panel is a ROUTER,
not a gate: it reads each image's serial (SLES/SCES = PAL Europe, SLUS/SCUS = NTSC-U) and lights
the matching Play button when that region's build exists.
"""
from __future__ import annotations

import pathlib

from PySide6.QtCore import Qt, QEvent, QThread, QTimer, Signal
from PySide6.QtWidgets import (QFileDialog, QFrame, QGroupBox, QHBoxLayout, QLabel,
                               QListWidget, QListWidgetItem, QMessageBox, QPlainTextEdit,
                               QPushButton, QScrollArea, QVBoxLayout, QWidget)

from .. import builder, builds, disc, runtime
from . import theme

# One size for the three control buttons, so the two Play buttons read as a pair with Stop. The
# width is DERIVED from the sheet, never read from a cached sizeHint: the #primary sheet renders a
# 15px font and pads 28px a side, so "Play (Europe)" needs its label's own metric plus 2 *
# PRIMARY_PAD_X of padding - a fixed width would clip a wider label. Deriving it from QFontMetrics
# means a stylesheet that arrives after construction cannot leave the width stale. PAIR_PAD_X is
# extra breathing room; the height is the primary button's own natural height.
PRIMARY_PAD_X = 28        # the #primary sheet's own horizontal padding (12px 28px)
PRIMARY_PAD_Y = 12        # its vertical padding, for the natural height
PAIR_BTN_H = 56
PAIR_PAD_X = 16

_REGION_LABEL = {
    disc.REGION_EU: "PAL / Europe",
    disc.REGION_US: "NTSC-U / USA",
    disc.REGION_JP: "NTSC-J / Japan",
    disc.REGION_UNKNOWN: "region not recognised",
}


def _region_label(region: str) -> str:
    return _REGION_LABEL.get(region, _REGION_LABEL[disc.REGION_UNKNOWN])


class _BuildWorker(QThread):
    """Runs the whole recompile off the UI thread, forwarding every line of output."""

    line = Signal(str)
    done = Signal(object)      # builder.BuildResult

    def __init__(self, region: str, image: pathlib.Path, parent=None) -> None:
        super().__init__(parent)
        self.region = region
        self.image = image

    def run(self) -> None:  # noqa: D102 (QThread entry point)
        try:
            res = builder.build(self.region, self.image, self.line.emit)
        except Exception as exc:  # noqa: BLE001 - a worker must never take the app down
            res = builder.BuildResult(False, self.region, None, f"{type(exc).__name__}: {exc}")
        self.done.emit(res)


class PlayTab(QWidget):
    def __init__(self, cfg: dict, toolchain=None) -> None:
        super().__init__()
        self.cfg = cfg
        self._proc = None
        self._proc_region: str | None = None
        self._routes: dict[str, pathlib.Path | None] = {}
        self._extra: list[pathlib.Path] = []      # images the player added this session
        self._worker: _BuildWorker | None = None
        self._booted = False
        # The pre-flight is done once per session (it probes the machine) and can be re-run from
        # the tab; a caller may inject a known report so the UI can be exercised without touching
        # the real toolchain.
        self.toolchain = toolchain if toolchain is not None else builder.preflight()

        # The page scrolls rather than squeezes: the cards plus the hero banner need more height
        # than the window gives them, and a squeezed layout collapses its padding.
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

        # ---- optional hero art (takes no space when absent) ----------------
        self.banner = theme.Banner(theme.play_banner(), 220)
        if self.banner.isVisible():
            lay.addWidget(self.banner)

        # ---- build from your own disc --------------------------------------
        box = QGroupBox("Build from your own disc")
        bl = QVBoxLayout(box)
        bl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        bl.setSpacing(theme.ROW_GAP)
        hint = QLabel("Point at your own Digimon World 3 disc image (.cue or .bin). The launcher "
                      "reads its serial (SLES/SCES = PAL Europe, SLUS/SCUS = NTSC-U), recompiles "
                      "it locally with the bundled engine, then drops the finished build beside "
                      "the launcher. Europe and USA are both supported.")
        hint.setObjectName("hint")
        hint.setWordWrap(True)
        bl.addWidget(hint)

        self.list_discs = QListWidget()
        self.list_discs.setMinimumHeight(108)
        self.list_discs.setMaximumHeight(140)
        self.list_discs.currentRowChanged.connect(lambda _r: self._on_disc_selected())
        bl.addWidget(self.list_discs)

        row2 = QHBoxLayout()
        row2.setSpacing(theme.ROW_GAP)
        self.btn_add = QPushButton("Add disc image…")
        self.btn_add.clicked.connect(self.add_disc)
        self.btn_check = QPushButton("Check disc")
        self.btn_check.clicked.connect(self.check_selected)
        self.btn_rescan = QPushButton("Rescan discs")
        self.btn_rescan.clicked.connect(self.refresh)
        row2.addWidget(self.btn_add)
        row2.addWidget(self.btn_check)
        row2.addWidget(self.btn_rescan)
        row2.addStretch(1)
        bl.addLayout(row2)

        self.lbl_disc = QLabel()
        self.lbl_disc.setWordWrap(True)
        bl.addWidget(self.lbl_disc)

        self.lbl_toolchain = QLabel()
        self.lbl_toolchain.setWordWrap(True)
        bl.addWidget(self.lbl_toolchain)

        row4 = QHBoxLayout()
        row4.setSpacing(theme.ROW_GAP)
        self.btn_recheck = QPushButton("Re-check toolchain")
        self.btn_recheck.clicked.connect(self.recheck_toolchain)
        self.btn_build = QPushButton("Build selected disc")
        self.btn_build.clicked.connect(self.start_build)
        row4.addWidget(self.btn_recheck)
        row4.addWidget(self.btn_build)
        row4.addStretch(1)
        bl.addLayout(row4)

        self.lbl_progress = QLabel()
        self.lbl_progress.setObjectName("hint")
        self.lbl_progress.setWordWrap(True)
        bl.addWidget(self.lbl_progress)
        lay.addWidget(box)

        # ---- play ----------------------------------------------------------
        pbox = QGroupBox("Play")
        pl = QVBoxLayout(pbox)
        pl.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        pl.setSpacing(theme.ROW_GAP)
        row3 = QHBoxLayout()
        row3.setSpacing(theme.CARD_GAP)
        # USA first, Europe second: the same order the build list and the disc router walk, so the
        # tab, its labels and the router cannot disagree about which region leads.
        self.btn_play_us = QPushButton("▶  Play (USA)")
        self.btn_play_us.clicked.connect(self.play_usa)
        self.btn_play_eu = QPushButton("▶  Play (Europe)")
        self.btn_play_eu.clicked.connect(self.play_europe)
        self.btn_stop = QPushButton("Stop")
        self.btn_stop.setObjectName("danger")
        self.btn_stop.clicked.connect(self.stop)
        self.btn_stop.setEnabled(False)
        # btn_play names the leading Play button: the control pairing (and the verifier's Play/Stop
        # size check) names one lead button, and USA leads.
        self.btn_play = self.btn_play_us
        self._controls = (self.btn_play_us, self.btn_play_eu, self.btn_stop)
        self._size_control_buttons()
        row3.addWidget(self.btn_play_us)
        row3.addWidget(self.btn_play_eu)
        row3.addWidget(self.btn_stop)
        row3.addStretch(1)
        pl.addLayout(row3)

        self.lbl_input = QLabel("Input: keyboard and every connected controller are merged onto "
                                "Player 1 - the launcher starts each build with PSX_DEV_INPUT=1.")
        self.lbl_input.setObjectName("hint")
        self.lbl_input.setWordWrap(True)
        pl.addWidget(self.lbl_input)
        self.lbl_run = QLabel()
        self.lbl_run.setObjectName("hint")
        self.lbl_run.setWordWrap(True)
        pl.addWidget(self.lbl_run)
        lay.addWidget(pbox)

        # ---- builds --------------------------------------------------------
        bbox = QGroupBox("Builds")
        bl2 = QVBoxLayout(bbox)
        bl2.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        bl2.setSpacing(theme.ROW_GAP)
        self.lbl_builds = QLabel()
        self.lbl_builds.setWordWrap(True)
        bl2.addWidget(self.lbl_builds)
        row5 = QHBoxLayout()
        row5.setSpacing(theme.ROW_GAP)
        self.btn_open_builds = QPushButton("Open Builds folder")
        self.btn_open_builds.clicked.connect(self._open_builds)
        row5.addWidget(self.btn_open_builds)
        row5.addStretch(1)
        bl2.addLayout(row5)
        lay.addWidget(bbox)

        # ---- activity log --------------------------------------------------
        lbox = QGroupBox("Activity")
        ll = QVBoxLayout(lbox)
        ll.setContentsMargins(theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD, theme.CARD_PAD)
        ll.setSpacing(theme.ROW_GAP)
        self.log = QPlainTextEdit()
        self.log.setReadOnly(True)
        self.log.setPlaceholderText("Disc routing, toolchain checks and builds report here.")
        self.log.setMinimumHeight(96)
        self.log.setMaximumHeight(150)
        ll.addWidget(self.log)
        lay.addWidget(lbox)
        lay.addStretch(1)

        self.refresh()

        self._timer = QTimer(self)
        self._timer.setInterval(1500)
        self._timer.timeout.connect(self._tick)
        self._timer.start()

    # ------------------------------------------------------------------ state
    def _say(self, text: str) -> None:
        self.log.appendPlainText(text)

    @staticmethod
    def _repolish(widget) -> None:
        widget.style().unpolish(widget)
        widget.style().polish(widget)

    def _size_control_buttons(self) -> None:
        """Give the two Play buttons and Stop one size, derived from the sheet's own metrics.

        The width is measured against the LIVE stylesheet on a throwaway probe, never read from a
        cached sizeHint: the #primary sheet renders a 15px font and pads 28px a side, so
        "Play (Europe)" needs its label's own font metric plus 2 * PRIMARY_PAD_X of padding. A
        QPushButton caches sizeHint for whatever sheet was in force when it was last polished, so a
        sheet that arrives after construction can leave the width short. The probe wears the
        current sheet, so its metric cannot be stale.
        """
        probe = QPushButton()
        probe.setObjectName("primary")
        probe.setStyleSheet(theme.stylesheet())
        probe.ensurePolished()
        fm = probe.fontMetrics()
        need = max(fm.horizontalAdvance(b.text()) for b in self._controls)
        text_h = fm.height()
        probe.deleteLater()
        width = need + 2 * PRIMARY_PAD_X + PAIR_PAD_X
        height = max(PAIR_BTN_H, text_h + 2 * PRIMARY_PAD_Y)
        for b in self._controls:
            b.setFixedSize(width, height)

    def showEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        super().showEvent(event)
        self._size_control_buttons()

    def changeEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        super().changeEvent(event)
        if event.type() == QEvent.Type.StyleChange and getattr(self, "_controls", None):
            self._size_control_buttons()

    # ------------------------------------------------------------------ refresh
    def refresh(self) -> None:
        rows = []
        for s in builds.BUILDS:
            ok, exe = builds.build_status(s.region)
            rows.append(f"{s.label}: {'ready' if ok else 'not built yet'}   -   {exe}")
        other = builds.builds_dir() / "OTHER"
        if other.is_dir() and any(other.glob("*.exe")):
            rows.append(f"Other discs: built into {other}")
        self.lbl_builds.setText("\n".join(rows))
        self._scan_discs()
        self._update_toolchain_label()
        self._update_build_button()
        if self._proc is None:
            self.lbl_run.clear()

    def _images(self) -> list[pathlib.Path]:
        out: list[pathlib.Path] = []
        for p in builds.images():
            if p not in out:
                out.append(p)
        for p in self._extra:
            if p not in out:
                out.append(p)
        return out

    def _scan_discs(self) -> None:
        self.list_discs.blockSignals(True)
        self.list_discs.clear()
        self._routes = {s.region: builds.disc_for_region(s.region) for s in builds.BUILDS}
        for img in self._images():
            region = disc.region_of_image(img)
            it = QListWidgetItem(f"{disc.describe_disc_name(img)}   "
                                 f"[{img.suffix.lower()}]   ->  {_region_label(region)}")
            it.setData(Qt.ItemDataRole.UserRole, (str(img), region))
            self.list_discs.addItem(it)
        if self.list_discs.count() and self.list_discs.currentRow() < 0:
            self.list_discs.setCurrentRow(0)
        self.list_discs.blockSignals(False)
        self._update_route_label()
        self._light_buttons()

    def _update_route_label(self) -> None:
        if not self.list_discs.count():
            _us = builds.spec(builds.REGION_US)
            _eu = builds.spec(builds.REGION_EU)
            self._set_disc_label(
                "err",
                f"No disc image found in {builds.discs_dir()}. Add your own with the "
                f"\"Add disc image...\" button, or drop \"{_us.disc_name}\" / "
                f"\"{_eu.disc_name}\" in there.")
            return
        sel = self._selected_disc()
        if sel is None:
            return
        img, region = sel
        if region == disc.REGION_UNKNOWN:
            self._set_disc_label(
                "err",
                f"\"{img.name}\" carries no PAL or NTSC-U serial. Pick the .bin/.cue that came "
                f"off your own Digimon World 3 disc. You can still try to build it; the "
                f"recompiler is the judge.")
        else:
            self._set_disc_label("ok", f"\"{img.name}\" -> {_region_label(region)}")

    def _set_disc_label(self, state: str, text: str) -> None:
        self.lbl_disc.setText(text)
        if self.lbl_disc.objectName() != state:
            self.lbl_disc.setObjectName(state)
            self._repolish(self.lbl_disc)

    def _light_buttons(self) -> None:
        """Enable the Play button whose region has a finished build beside the launcher."""
        for btn, region in ((self.btn_play_us, builds.REGION_US),
                            (self.btn_play_eu, builds.REGION_EU)):
            ready = builds.build_status(region)[0]
            btn.setEnabled(ready)
            name = "primary" if ready else ""
            if btn.objectName() != name:
                btn.setObjectName(name)
                self._repolish(btn)

    # ------------------------------------------------------------------ toolchain
    def _update_toolchain_label(self) -> None:
        if not builder.engine_present():
            self._set_toolchain_label(
                "err", f"The bundled engine is missing from {builder.engine_dir()} - "
                       f"psxrecomp.exe and framework/bios/openbios.bin are both required to "
                       f"build.")
        elif self.toolchain.ok:
            self._set_toolchain_label("ok", "Toolchain " + self.toolchain.summary())
        else:
            self._set_toolchain_label(
                "err", "Toolchain " + self.toolchain.summary()
                + " - install it and press \"Re-check toolchain\".")

    def _set_toolchain_label(self, state: str, text: str) -> None:
        self.lbl_toolchain.setText(text)
        if self.lbl_toolchain.objectName() != state:
            self.lbl_toolchain.setObjectName(state)
            self._repolish(self.lbl_toolchain)

    def recheck_toolchain(self) -> None:
        self.toolchain = builder.preflight(refresh=True)
        self._say("toolchain: " + self.toolchain.summary())
        self._update_toolchain_label()
        self._update_build_button()

    def _update_build_button(self) -> None:
        sel = self._selected_disc()
        busy = self._worker is not None
        self.btn_build.setEnabled(bool(sel is not None and not busy and self.toolchain.ok
                                       and builder.engine_present()))
        if busy:
            self.lbl_progress.setText("Building... see the Activity log.")
        elif sel is None:
            self.lbl_progress.setText("Select a disc image to build.")
        elif not builder.engine_present():
            self.lbl_progress.setText("Cannot build: the bundled engine is missing.")
        elif not self.toolchain.ok:
            self.lbl_progress.setText("Cannot build until the missing toolchain is installed.")
        else:
            img, region = sel
            where = (builds.spec(region).folder
                     if region in (builds.REGION_EU, builds.REGION_US) else "OTHER")
            self.lbl_progress.setText(
                f"Ready to build {img.name} -> Builds/{where}. A recompile takes 10 to 20 minutes.")
        self.btn_recheck.setEnabled(not busy)

    # ------------------------------------------------------------------ disc router
    def _selected_disc(self) -> tuple[pathlib.Path, str] | None:
        it = self.list_discs.currentItem()
        if not it:
            return None
        path, region = it.data(Qt.ItemDataRole.UserRole)
        return pathlib.Path(path), region

    def _on_disc_selected(self) -> None:
        sel = self._selected_disc()
        if not sel:
            return
        img, region = sel
        serial = disc.serial_of_image(img)
        self._say(f"selected {img.name}: {_region_label(region)}"
                  + (f"  -  serial {serial}" if serial else "  -  no serial found"))
        self._update_route_label()
        self._update_build_button()

    def add_disc(self) -> None:
        path, _ = QFileDialog.getOpenFileName(
            self, "Select your disc image", "",
            "Disc images (*.cue *.bin *.iso *.chd);;All files (*)")
        if not path:
            return
        img = pathlib.Path(path)
        if img not in self._extra:
            self._extra.append(img)
        self._scan_discs()
        self.list_discs.setCurrentRow(self.list_discs.count() - 1)

    def check_selected(self) -> bool:
        """Report the selected image's own region and whether it looks usable.

        This never gates a build or a launch: it reads the image's own serial and says what it
        found. The recompiler is the real judge of whether the disc builds.
        """
        sel = self._selected_disc()
        if not sel:
            QMessageBox.information(self, "No disc", "Add or select a disc image first.")
            return False
        img, _region = sel
        self._say(f"- checking {img.name} -")
        chk = disc.verify(img)
        self._say(f"   region: {_region_label(chk.region)}")
        for line in chk.lines:
            self._say("   " + line)
        self.lbl_disc.setText(f"{img.name}: {_region_label(chk.region)}")
        return chk.ok

    def _open_builds(self) -> None:
        d = builds.builds_dir()
        d.mkdir(parents=True, exist_ok=True)
        runtime.open_path(d)

    # ------------------------------------------------------------------ building
    def start_build(self) -> None:
        sel = self._selected_disc()
        if sel is None:
            QMessageBox.information(self, "No disc", "Add or select a disc image first.")
            return
        img, region = sel
        # PRE-FLIGHT: never start a 10 to 20 minute recompile that cannot finish. Say precisely what
        # is missing instead.
        self.toolchain = builder.preflight(refresh=True)
        self._update_toolchain_label()
        if not builder.engine_present():
            self._fail(f"The bundled engine is missing from {builder.engine_dir()}.")
            return
        if not self.toolchain.ok:
            self.lbl_progress.setText("Cannot build until the missing toolchain is installed.")
            self._fail(self.toolchain.summary() + "\n\n" + self.toolchain.hint())
            return
        self._booted = False
        self._worker = _BuildWorker(region, img, self)
        self._worker.line.connect(self._say)
        self._worker.done.connect(self._on_build_done)
        self.btn_build.setEnabled(False)
        self.btn_recheck.setEnabled(False)
        self.lbl_progress.setText("Building... see the Activity log.")
        self._worker.start()

    def _on_build_done(self, res) -> None:
        self._worker = None
        self.refresh()
        if getattr(res, "ok", False):
            self.lbl_progress.setText(getattr(res, "message", "Build finished."))
            self._say("BUILD OK: " + getattr(res, "message", ""))
        else:
            self.lbl_progress.setText(getattr(res, "message", "Build failed."))
            self._fail(getattr(res, "message", "Build failed."))

    # ------------------------------------------------------------------ launching
    def play_europe(self) -> None:
        self._play(builds.REGION_EU)

    def play_usa(self) -> None:
        self._play(builds.REGION_US)

    def _fail(self, message: str) -> None:
        self._say("FAILED: " + message)
        QMessageBox.critical(self, "Could not start the game", message)

    def _play(self, region: str) -> None:
        s = builds.spec(region)
        exe = builds.exe_path(region)
        if not exe.is_file():
            self._fail(f"The {s.label} build is not here yet: {exe}\n\nBuild it from your own "
                       f"{s.label} disc first.")
            return
        # Prefer an image in Discs/ (so a moved install keeps working), else the one this build
        # was made from; either way the build's own game.toml is pointed at it before launch.
        img = builds.disc_for_region(region)
        if img is not None:
            try:
                builds.set_disc(region, img)
            except Exception as exc:  # noqa: BLE001
                self._fail(f"Could not point {builds.game_toml(region)} at {img}: "
                           f"{type(exc).__name__}: {exc}")
                return
        else:
            img = builds.baked_disc(region)
            if img is None:
                self._fail(f"No {s.label} disc image found. Put \"{s.disc_name}\" in "
                           f"{builds.discs_dir()}, or rebuild with \"Add disc image...\".")
                return
        self._say(f"disc -> {img}")
        try:
            self._proc = builds.launch(region, log_path=builds.run_log_path(region))
            self._proc_region = region
        except Exception as exc:  # noqa: BLE001
            self._fail(f"Could not start the {s.label} build: {type(exc).__name__}: {exc}")
            return
        self.btn_play_eu.setEnabled(False)
        self.btn_play_us.setEnabled(False)
        self.btn_stop.setEnabled(True)
        self.lbl_run.setText(f"Running: {s.label} build   -   pid {self._proc.pid}")
        self._say(f"launched the {s.label} build (pid {self._proc.pid}, cwd "
                  f"{builds.build_dir(region)}, {builds.DEV_INPUT_VAR}=1)")

    def stop(self) -> None:
        if self._proc and self._proc.poll() is None:
            self._proc.terminate()
            self._say("stop requested")
        self._proc = None
        self._proc_region = None
        self.btn_stop.setEnabled(False)
        self._light_buttons()
        self.lbl_run.clear()

    def _tick(self) -> None:
        if self._proc is not None and self._proc.poll() is None:
            if not self._booted and self._proc_region:
                if builds.boots_to_bios(builds.tail_log(self._proc_region)):
                    self._booted = True
                    self._say("runtime reached its recompiled BIOS entry "
                              "(executing from PC=0xBFC00000)")
            return
        if self._proc is not None and self._proc.poll() is not None:
            code = self._proc.returncode
            label = builds.spec(self._proc_region).label if self._proc_region else "build"
            self._proc = None
            self._proc_region = None
            self.btn_stop.setEnabled(False)
            self._light_buttons()
            self.lbl_run.clear()
            self._say(f"{label} build exited (code {code})")
