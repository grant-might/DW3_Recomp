r"""Regression checks for the launcher's non-UI logic. Ad-hoc verification, not a suite.

Worth keeping because it already caught a real bug: `disc.verify` used to complain about a
missing DiscTool before it complained about the player's .iso, which is the less useful order.

Run it after touching settings.py / disc.py / saves.py / runtime.py / paths.py / theme.py:

    .\.venv\Scripts\python.exe verify_launcher.py
"""
from __future__ import annotations

import pathlib
import re
import shutil
import struct
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))

from dmw3launcher import disc, paths, runtime, saves      # noqa: E402
from dmw3launcher import settings as st                   # noqa: E402
from dmw3launcher.ui import theme                         # noqa: E402

results: list[tuple[bool, str]] = []


def check(ok: bool, label: str, detail: str = "") -> None:
    results.append((bool(ok), label))
    print(f"[{'PASS' if ok else 'FAIL'}] {label}" + (f"  -- {detail}" if detail else ""))


# --- A. writing settings.toml must never damage it -----------------------------
root = paths.resolve_runtime()
check(root is not None, "A0 runtime located", str(root))
if root:
    real = paths.settings_toml(root)
    if real.is_file():
        tmpdir = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-dmw3-"))
        work = tmpdir / "settings.toml"
        shutil.copy(real, work)
        before = work.read_text(encoding="utf-8-sig")
        comments_before = [l for l in before.splitlines() if l.lstrip().startswith("#")]
        snap_before = st.snapshot(work)
        untouched = {
            ("launcher", "skip_launcher"): snap_before.get("launcher", {}).get("skip_launcher"),
            ("localization", "load_sectors_per_frame"):
                snap_before.get("localization", {}).get("load_sectors_per_frame"),
            ("controller", "p2_device"): snap_before.get("controller", {}).get("p2_device"),
        }
        st.set_values(work, {("video", "window_width"): 1600,
                             ("video", "antialiasing"): False})
        after = work.read_text(encoding="utf-8-sig")
        comments_after = [l for l in after.splitlines() if l.lstrip().startswith("#")]
        snap_after = st.snapshot(work)

        check(comments_after == comments_before, "A1 every comment line survives the write",
              f"{len(comments_before)} before, {len(comments_after)} after")
        check(snap_after.get("video", {}).get("window_width") == 1600,
              "A2 the intended int was written")
        check(snap_after.get("video", {}).get("antialiasing") is False,
              "A3 the intended bool was written")
        check(all(snap_after.get(s, {}).get(k) == v for (s, k), v in untouched.items()),
              "A4 keys the launcher does not manage are untouched")
        check(snap_after.get("video", {}).get("renderer") == "opengl",
              "A5 an unrelated video key kept its value")
        check(set(snap_before) == set(snap_after), "A6 section set unchanged",
              f"{sorted(snap_before)} -> {sorted(snap_after)}")
        shutil.rmtree(tmpdir, ignore_errors=True)
    else:
        check(True, "A settings.toml absent - writer checks skipped",
              str(real))

# --- B. save discovery against the real install --------------------------------
if root:
    slots = [f for f in saves.discover(root) if not f.is_card]
    check(len(slots) >= 4, "B1 slot files discovered", f"{len(slots)} slots")
    check(all(f.size == 10060 for f in slots), "B2 slot payload size is 10060 B",
          str(sorted({f.size for f in slots})))

# --- C. region classification (the honest-USA-refusal path) ---------------------
check(disc.region_from_output("ES EL DISCO CORRECTO ... SLES-03936") == disc.REGION_EU,
      "C1 EU disc classified EU")
check(disc.region_from_output("su disco arranca con SLUS-01436") == disc.REGION_US,
      "C2 USA disc classified US")
check(disc.region_from_output("no la edicion europea, hace falta SLES-03936") == disc.REGION_EU,
      "C3 DiscTool's EU-refusal text still yields a region")
check(disc.region_from_output("nonsense") == disc.REGION_UNKNOWN, "C4 garbage yields UNKNOWN")

# --- D. the player's own file is judged before the tool ------------------------
T = pathlib.Path(tempfile.gettempdir())
NO_TOOL = T / "hermes-verify-no-such-DiscTool.exe"
iso, nocue, goodcue, weird = (T / "hermes-verify-fake.iso", T / "hermes-verify-nobin.cue",
                              T / "hermes-verify-good.cue", T / "hermes-verify-game.txt")
iso.write_bytes(b"\0" * 32)
nocue.write_text('FILE "nothing.txt" BINARY\n', encoding="utf-8")
goodcue.write_text('FILE "game.bin" BINARY\n  TRACK 01 MODE2/2352\n', encoding="utf-8")
weird.write_text("nope", encoding="utf-8")
try:
    chk = disc.verify(NO_TOOL, iso)
    check((not chk.ok) and "raw sectors" in chk.message,
          "D1 a .iso is refused with the raw-sector reason", chk.message[:52])
    chk = disc.verify(NO_TOOL, nocue)
    check((not chk.ok) and "does not point at a .bin" in chk.message,
          "D2 a .cue with no .bin is refused", chk.message[:52])
    chk = disc.verify(NO_TOOL, goodcue)
    check((not chk.ok) and "DiscTool.exe not found" in chk.message,
          "D3 a good .cue passes the file checks and fails only on the missing tool",
          chk.message[:52])
    chk = disc.verify(NO_TOOL, weird)
    check((not chk.ok) and "Unsupported file type" in chk.message,
          "D4 an unrelated file type is refused", chk.message[:52])
    chk = disc.verify(NO_TOOL, T / "hermes-verify-nope.cue")
    check((not chk.ok) and "not found" in chk.message,
          "D5 a missing image is reported as missing", chk.message[:52])
finally:
    for p in (iso, nocue, goodcue, weird):
        p.unlink(missing_ok=True)

# --- E. stylesheet colours (regression guard for a bad token) -------------------
sheet = theme.stylesheet()
colour_toks = re.findall(
    r"(?:^|[\s;{])(?:background|background-color|color|border|border-color"
    r"|selection-background-color|selection-color)\s*:\s*(#[0-9a-zA-Z]{2,8})", sheet)
bad = [t for t in colour_toks
       if not re.fullmatch(r"#(?:[0-9a-fA-F]{3,4}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})", t)]
check(bool(colour_toks), "E0 colour values were found to check", f"{len(colour_toks)} found")
check(not bad, "E1 no malformed colour token in the stylesheet", str(bad[:5]))
check("primary" in sheet and "danger" in sheet, "E2 styled button roles present")

# --- F. a bogus install must fail loudly ---------------------------------------
tmp = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-dmw3-"))
try:
    raised = False
    try:
        runtime.launch(tmp)
    except Exception:
        raised = True
    check(raised, "F1 launching a non-install raises instead of failing silently")
finally:
    shutil.rmtree(tmp, ignore_errors=True)

# --- G. config round-trip (on a patched path, real config untouched) ------------
tmpcfg = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-dmw3-")) / "launcher.json"
orig = paths.config_path
paths.config_path = lambda: tmpcfg  # type: ignore[assignment]
try:
    cfg = paths.load_config()
    cfg["runtime_root"] = str(root) if root else "x"
    paths.save_config(cfg)
    reloaded = paths.load_config()
    check(reloaded.get("runtime_root") == cfg["runtime_root"], "G1 config round-trips")
    check(paths.resolve_runtime(reloaded) == root, "G2 runtime re-resolves from config")
finally:
    paths.config_path = orig  # type: ignore[assignment]
    shutil.rmtree(tmpcfg.parent, ignore_errors=True)

# --- H. the art layer: the ICO container, art present, art absent ---------------
from PySide6.QtGui import QIcon                        # noqa: E402
from PySide6.QtWidgets import QApplication             # noqa: E402

_app = QApplication.instance() or QApplication([])     # needed to build widgets below

ico = paths.assets_dir() / "icon.ico"
if ico.is_file():
    raw = ico.read_bytes()
    reserved, kind, count = struct.unpack("<HHH", raw[:6])
    sizes, in_bounds, png = [], True, True
    for i in range(count):
        off = 6 + i * 16
        w, _h, _c, _r, _p, _b, nbytes, imgoff = struct.unpack("<BBBBHHII", raw[off:off + 16])
        sizes.append(256 if w == 0 else w)
        in_bounds &= imgoff + nbytes <= len(raw)
        png &= raw[imgoff:imgoff + 8] == b"\x89PNG\r\n\x1a\n"
    check(reserved == 0 and kind == 1 and count == 7, "H1 icon.ico is a valid 7-size icon",
          f"type={kind} count={count}")
    check(sorted(sizes) == [16, 24, 32, 48, 64, 128, 256], "H2 icon.ico sizes are the required set",
          f"{sizes} (order does not matter; the set does)")
    check(in_bounds and png, "H3 icon.ico entries are in-bounds PNG payloads")
    check(not QIcon(str(ico)).isNull(), "H4 Qt loads icon.ico")
else:
    check(True, "H1-H4 skipped: no icon.ico on disk (fallback icon in use)")


def _resolves(fn) -> bool:
    v = fn()
    return v is not None and not (hasattr(v, "isNull") and v.isNull())


LOADERS = (theme.background, theme.logo, theme.header_bar, theme.play_banner,
           lambda: theme.tab_image("play"), lambda: theme.tab_image("memcard"),
           lambda: theme.tab_image("mods"), lambda: theme.tab_image("settings"))
# background.png and header_bar.png are optional by design: with no file the window paints the live
# theme colour and the header falls back to a flat panel (section P), so only the art the user has
# actually supplied must resolve.
REQUIRED_ART = (theme.logo, theme.play_banner,
                lambda: theme.tab_image("play"), lambda: theme.tab_image("memcard"),
                lambda: theme.tab_image("mods"), lambda: theme.tab_image("settings"))
check(all(_resolves(f) for f in REQUIRED_ART),
      "H5 with art on disk every supplied loader resolves",
      str(sum(_resolves(f) for f in REQUIRED_ART)) + " of " + str(len(REQUIRED_ART)))

# Art ABSENT. Point the loader at an empty directory rather than renaming the real assets
# folder: same code path, and a check can never leave the player's art moved.
_empty = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-dmw3-"))
_orig_assets_dir = paths.assets_dir
paths.assets_dir = lambda: _empty  # type: ignore[assignment]
try:
    raised = False
    for f in LOADERS:
        try:
            f()
        except Exception as exc:  # noqa: BLE001
            raised = True
            print(f"    loader raised with no art: {exc}")
    check(not raised, "H6 with no art, every loader falls back instead of raising")

    try:
        from dmw3launcher.ui.main_window import TAB_ORDER
        _expected = len(TAB_ORDER)
    except Exception:  # noqa: BLE001
        _expected = 5
    try:
        from dmw3launcher.ui.main_window import MainWindow
        _win = MainWindow(paths.load_config())
        _tabs = _win.tabs.count()
    except Exception as exc:  # noqa: BLE001
        _tabs = 0
        print(f"    MainWindow raised with no art: {exc}")
    check(_tabs == _expected, "H7 every tab builds with no art", f"tabs={_tabs} of {_expected}")
finally:
    paths.assets_dir = _orig_assets_dir  # type: ignore[assignment]
    shutil.rmtree(_empty, ignore_errors=True)

# --- I. the Memory Card tab: the editor must actually embed, and the tab must never
#        advertise a file the editor cannot read ---------------------------------
from PySide6.QtCore import Qt                          # noqa: E402

try:
    from dmw3launcher.ui.memcard_tab import MemcardTab
    _mc_tab = MemcardTab(paths.load_config())
    check(_mc_tab._editor is not None, "I1 save editor embeds in-process",
          _mc_tab._editor_error or "")
    check(_mc_tab._payload_size == 32768, "I2 editor payload size discovered",
          str(_mc_tab._payload_size))

    _rows = []
    for _i in range(_mc_tab.list_saves.count()):
        _it = _mc_tab.list_saves.item(_i)
        _kind, _p, _editable, _note = _it.data(Qt.ItemDataRole.UserRole)
        _rows.append((_kind, pathlib.Path(_p), _editable, _note))
    _slots = [r for r in _rows if r[0] == "slot"]
    _cards = [r for r in _rows if r[0] == "card"]
    check(not any(r[2] for r in _slots),
          "I3 recomp .sav slots are not advertised as editable",
          f"{len(_slots)} slot(s) listed")
    check(all(r[2] for r in _cards),
          "I4 memory cards are advertised as editable", f"{len(_cards)} card(s) listed")

    # Opening a locked slot must explain itself on the status line, not raise: a raised error
    # became a modal dialog, so under test it looked like a hang rather than a bug.
    if _slots:
        _mc_tab.open_path("slot", _slots[0][1])
        check("cannot be opened" in _mc_tab.lbl_status.text(),
              "I5 opening a recomp slot explains instead of raising",
              _mc_tab.lbl_status.text()[:80])

    # The Appearance chooser lives in Settings now (section N, N8); the Memory Card tab must not
    # carry a second one. The embedded editor used to expose its own "APPEARANCE > Themes" page,
    # which was the duplicate the owner had removed - assert it is really gone from this tab while
    # the editor still embeds.
    if _mc_tab._editor is not None:
        _nav = [b.text() for b in getattr(_mc_tab._editor, "_nav_buttons", [])]
        _keys = list(getattr(_mc_tab._editor, "_keys", []))
        _nav_ok = (len(_keys) == len(_nav)
                   and _mc_tab._editor.stack.count() == len(_nav))
        check("Themes" not in _nav and "themes" not in _keys and _nav_ok
              and getattr(_mc_tab._editor, "themes_tab", None) is None,
              "I6 the Memory Card tab carries no theme chooser of its own (the Appearance "
              "dropdown lives in Settings, and the editor's page is gone from this embed)",
              f"{len(_nav)} nav entries, stack {_mc_tab._editor.stack.count()}")
except Exception as _exc:  # noqa: BLE001
    check(False, "I1-I5 memory card tab checks raised", f"{type(_exc).__name__}: {_exc}")

# --- J. one theme, one app: the embedded editor themes the QApplication, and the launcher must
#        follow it WITHOUT its own sheet reaching back into the editor ----------------------
try:
    from PySide6.QtGui import QPalette
    from dmw3launcher.ui import theme as lt
    from dmw3launcher.ui.main_window import MainWindow as _LW

    _win = _LW(paths.load_config())
    _win.resize(1240, 940)
    _win.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)   # lay out, don't display
    _win.show()
    _app.processEvents()

    _ed = _win.memcard._editor
    check(_ed is not None, "J1 the editor embeds in the launcher window",
          _win.memcard._editor_error or "")
    if _ed is not None:
        _edt = _win.memcard._ed_theme
        check(len(_edt.PALETTES) == 5, "J2 the editor exposes all 5 themes",
              str(list(_edt.PALETTES)))

        _before = (lt.BG, lt.ACCENT)
        _edt.apply(_app, "guilmon-black")
        _app.processEvents()
        _want = _edt.PALETTES["guilmon-black"]

        check((lt.BG, lt.ACCENT) == (_want["BG"], _want["ACCENT"]),
              "J3 a theme picked INSIDE the editor recolours the launcher",
              f"{_before} -> {(lt.BG, lt.ACCENT)}")
        check(_want["BG"] in _app.styleSheet(), "J4 the theme is app-wide (on the QApplication)")
        # UPDATED with the seamless-surface change: the panel sheet no longer paints the PAGE
        # colour on every widget (that is what stamped a darker box behind every label inside a
        # panel), so `BG in panel.styleSheet()` is the wrong proxy now. What proves the panels
        # still follow the theme is that they carry the theme's SURFACES - the panel/card tone,
        # the control tone and the border - and that a container actually paints them rather than
        # the page colour. Both halves are asserted, so a regression that puts the inset boxes
        # back fails here instead of silently looking wrong.
        from PySide6.QtWidgets import QGroupBox as _QGBj                      # noqa: PLC0415
        _sheet = _win.play.styleSheet()
        _missing = [k for k in ("SURFACE", "SURFACE_2", "BORDER_STRONG") if _want[k] not in _sheet]
        _probe_gb = _QGBj("probe")
        _probe_gb.setStyleSheet(_sheet)
        _probe_gb.resize(220, 90)
        _fill = _probe_gb.grab().toImage().pixelColor(24, 60).name()
        check(not _missing and _fill not in (lt.BG.lower(), "#000000"),
              "J5 the launcher's own panels carry the theme's surface colours and paint them "
              "behind their text (no page-coloured box inside a panel)",
              f"missing {_missing}; container fill {_fill} (panel {lt.PANEL}, page {lt.BG})")
        check(_win.styleSheet() == "",
              "J6 the launcher's sheet is off the editor's ancestor chain",
              "Qt merges rule sets by specificity, so a host rule would leak inside")
        # The property that matters: after a theme switch the editor must FOLLOW the app. An
        # explicit setPalette() marks the widget WA_SetPalette and it then keeps the OLD colours
        # while its children update, which is exactly what made half the window the old theme.
        _pal_before = _ed.palette().color(QPalette.Window).name()
        _edt.apply(_app, "gargomon-green")
        _app.processEvents()
        _pal_after = _ed.palette().color(QPalette.Window).name()
        _want2 = _edt.PALETTES["gargomon-green"]["BG"]
        check(_pal_after == _want2 and _pal_after != _pal_before,
              "J7 the embedded editor follows the app palette across a switch",
              f"{_pal_before} -> {_pal_after}, theme BG {_want2}")
        check((lt.BG, lt.ACCENT) == (_want2, _edt.PALETTES["gargomon-green"]["ACCENT"]),
              "J8 the launcher follows the same switch", f"BG={lt.BG}")

        # A reparented QMainWindow lays out when shown; activate explicitly so this runs without
        # putting a window on screen (an unlaid-out one reads statusBar height 480).
        _win.layout().activate()
        _ed.layout().activate()
        _app.processEvents()
        check(0 < _ed.menuBar().height() < 60 and 0 < _ed.statusBar().height() < 60,
              "J9 the embedded QMainWindow is really laid out",
              f"menu={_ed.menuBar().height()} status={_ed.statusBar().height()}")

        _edt.apply(_app, _edt.DEFAULT_THEME)     # leave the app on the default theme
        _app.processEvents()
except Exception as _exc:  # noqa: BLE001
    check(False, "J1-J9 theme integration raised", f"{type(_exc).__name__}: {_exc}")

# --- K. layout invariants: one gutter, one card padding, nothing squeezed -------------------
# The numbers are the point. "Properly spaced" shows up as a single left/right edge per page and
# no child below its minimum; a squeezed page is what collapsed the padding before.
try:
    from dmw3launcher.ui import theme as _lt
    from dmw3launcher.ui.main_window import MainWindow as _KW

    _kw = _KW(paths.load_config())
    _kw.resize(1200, 880)
    _kw.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
    _kw.show()
    _app.processEvents()

    _tb = _kw.tabs.tabBar()          # the tab bar's geometry is relative to the QTabWidget
    _label_x = _tb.mapTo(_kw, _tb.tabRect(0).topLeft()).x()
    check(_label_x == _lt.PAGE_MARGIN,
          "K1 the tab bar's first label sits on the page gutter",
          f"x={_label_x} expected {_lt.PAGE_MARGIN}")

    for _name, _page in (("Play", _kw.play), ("MemoryCard", _kw.memcard),
                         ("Mods", _kw.mods), ("Settings", _kw.settings)):
        _kw.tabs.setCurrentWidget(_page)
        for _ in range(2):
            _app.processEvents()
        _page.layout().activate()     # geometry stays stale until the layout is activated
        _kw.layout().activate()
        for _ in range(3):
            _app.processEvents()
        _lay = _page.layout()
        _m = _lay.contentsMargins()
        _lefts, _rights, _squeezed = set(), set(), []
        for _i in range(_lay.count()):
            _w = _lay.itemAt(_i).widget()
            if _w is None or not _w.isVisible():   # hidden widgets keep stale default geometry
                continue
            _g = _w.geometry()
            _lefts.add(_g.x())
            _rights.add(_g.x() + _g.width())
            if _g.height() < _w.minimumSizeHint().height():
                _squeezed.append(type(_w).__name__)
        check(_lefts == {_m.left()} and _rights == {_page.width() - _m.right()},
              f"K2 {_name}: every card shares one gutter",
              f"lefts={sorted(_lefts)} rights={sorted(_rights)}")
        check(not _squeezed, f"K3 {_name}: nothing squeezed below its minimum",
              ", ".join(_squeezed))

    _kw.tabs.setCurrentWidget(_kw.memcard)
    for _ in range(3):
        _app.processEvents()
    _ked = _kw.memcard._editor
    if _ked is not None:
        check(_ked.width() >= _ked.minimumWidth() and _ked.height() >= _ked.minimumHeight(),
              "K4 the embedded editor is at least its own minimum size (not clipped)",
              f"{_ked.width()}x{_ked.height()} "
              f"(min {_ked.minimumWidth()}x{_ked.minimumHeight()})")
except Exception as _exc:  # noqa: BLE001
    check(False, "K1-K4 layout invariant checks raised", f"{type(_exc).__name__}: {_exc}")

# --- L. English only: DiscTool speaks Spanish, the UI must not ---------------------------------
try:
    from dmw3launcher import tooltext as _tt
    _usa_report = ("DiscTool ver     SLUS_014.36.bin\r\n"
                   "volumen      : DMW3\r\n"
                   "sistema      : PLAYSTATION\r\n"
                   "ejecutable   : SLUS_014.36\r\n"
                   "sectores     : 330000 (el disco dice 330000)\r\n"
                   "fallo: Es Digimon World 3/2003, pero no la edicion europea. Su disco arranca "
                   "con SLUS-01436 y este port esta hecho sobre SLES-03936. Hace falta la "
                   "version europea (SLES-03936).\r\n")
    _why = _tt.english(disc._first_meaningful(_usa_report))
    check("European edition" in _why and "SLES-03936" in _why
          and "SLUS-01436" in _why and not _tt.still_spanish(_why),
          "L1 a USA disc's refusal reads as English and keeps both region codes", _why[:96])
    check(not _tt.still_spanish(_tt.english(_usa_report)),
          "L2 nothing Spanish survives the tool's whole report",
          ", ".join(_tt.still_spanish(_tt.english(_usa_report))))

    _ps = _tt.english("No es un disco de PlayStation (pone \"GAME\" donde deberia poner "
                      "PLAYSTATION).")
    check("GAME" in _ps and "PLAYSTATION" in _ps and not _tt.still_spanish(_ps),
          "L3 a non-PlayStation disc keeps its quoted value, in English", _ps)

    _lbl = _tt.english("FALTA   AAA/DAT/X.BIN")
    check("MISSING" in _lbl and not _tt.still_spanish(_lbl),
          "L4 report labels are translated", _lbl)

    _short = _tt.english("La imagen es demasiado corta: no llega ni al descriptor del disco. "
                         "Puede que la copia se cortara a medias.")
    check("too short" in _short and not _tt.still_spanish(_short),
          "L5 a truncated image is explained in English", _short)

    _inc = _tt.english("El disco es el correcto, pero la imagen esta incompleta: tiene 100 "
                       "sectores y deberia tener 200. Vuelve a copiarla.")
    check("100" in _inc and "200" in _inc and "incomplete" in _inc and not _tt.still_spanish(_inc),
          "L6 an incomplete image keeps its sector counts, in English", _inc)

    _usage = _tt.english("DiscTool sacar   <imagen.bin|.cue> <destino>")
    check(_usage.startswith("DiscTool extract") and not _tt.still_spanish(_usage),
          "L7 its usage lines are translated too", _usage)

    # The real report from a broken .cue, verbatim: the reason lives on the `veredicto :` line,
    # and the sentence after that label is the whole message.
    _real = ('volumen      : \n'
             'sistema      : \n'
             'ejecutable   : (ninguno)\n'
             'sectores     : 0 (el disco dice 0)\n'
             'AAA oculto   : no\n'
             'veredicto    : No puedo abrir "Digimon World 2003 (Europe)".bin".\n')
    _r = disc._first_meaningful(_real)
    check(_r.startswith("No puedo abrir") and not _r.startswith("volumen"),
          "L8 the refusal is taken from DiscTool's veredicto line, not a context label", _r[:70])
    check(_tt.english(_r).startswith('Cannot open "') and not _tt.still_spanish(_tt.english(_r)),
          "L9 ...and it reads as English", _tt.english(_r)[:70])

    # Region must come from the disc's own boot code: the refusal names both regions.
    check(disc.region_from_output(_usa_report) == disc.REGION_US,
          "L10 a USA refusal that also mentions SLES-03936 is still US",
          disc.region_from_output(_usa_report))
    check(disc.region_from_output("ejecutable   : SLES_039.36") == disc.REGION_EU,
          "L11 the reported boot executable decides the region")
    check(disc.region_from_output("ejecutable   : (ninguno)") == disc.REGION_UNKNOWN,
          "L12 no boot executable does not invent a region",
          disc.region_from_output("ejecutable   : (ninguno)"))
except Exception as _exc:  # noqa: BLE001
    check(False, "L1-L12 DiscTool translation checks raised", f"{type(_exc).__name__}: {_exc}")

# --- M. the user's own art: the logo and the tab word-images -----------------------------------
try:
    from PySide6.QtGui import QImage as _QImage
    from PySide6.QtGui import QPixmap as _QPixmap
    from dmw3launcher.ui.main_window import (LOGO_ART_H, TAB_LABELS, TAB_ORDER,  # noqa: PLC0415
                                             MainWindow as _MW)

    _art = theme.tab_images(theme.TAB_ART_H)
    check(set(_art) == set(TAB_ORDER),
          "M1 every tab slot loads art", ", ".join(sorted(_art)))
    check({pm.height() for pm in _art.values()} == {theme.TAB_ART_H},
          "M2 one bar height governs the whole row",
          str({pm.height() for pm in _art.values()}))
    check({round(pm.width() / pm.height(), 2) for pm in _art.values()} == {4.0},
          "M3 the word-image aspect survives scaling (320x80 -> 4:1)",
          str({round(pm.width() / pm.height(), 2) for pm in _art.values()}))

    def _dominant_opaque(img) -> str | None:
        counts: dict[str, int] = {}
        for _y in range(0, img.height(), 2):
            for _x in range(0, img.width(), 2):
                _c = img.pixelColor(_x, _y)
                if _c.alpha() >= 250:
                    counts[_c.name()] = counts.get(_c.name(), 0) + 1
        return max(counts, key=counts.get) if counts else None

    for _k in ("play", "memcard", "mods", "settings"):
        _dom = _dominant_opaque(_art[_k].toImage())
        check(_dom == "#ffffff", f"M4 the drawn word on '{_k}' is white on the dark chrome", str(_dom))
    check(theme._ink_contrast(_QPixmap(str(paths.assets_dir() / "tab_memcard.png")))
          < theme.MIN_ART_CONTRAST,
          "M5 the supplied art really is unreadable on this chrome (so the copies are not cosmetic)")

    # The baked copy must change the word and NOTHING else: the icon is what a blanket recolour
    # ruins, because MODS' outline and centre are dark too.
    for _k in ("play", "mods"):
        _src_p, _whi_p = (paths.assets_dir() / f"tab_{_k}.png",
                          paths.assets_dir() / f"tab_{_k}_white.png")
        check(_whi_p.is_file(), f"M6 a baked white copy exists for '{_k}'")
        if not _whi_p.is_file():
            continue
        _src, _whi = _QImage(str(_src_p)), _QImage(str(_whi_p))
        _icols = theme._coloured_columns(_src)
        _icon_diff = _word_diff = 0
        for _y in range(_src.height()):
            for _x in range(_src.width()):
                if _src.pixelColor(_x, _y).getRgb() == _whi.pixelColor(_x, _y).getRgb():
                    continue
                if _x in _icols:
                    _icon_diff += 1
                else:
                    _word_diff += 1
        check(_icon_diff == 0 and _word_diff > 0,
              f"M7 '{_k}': the icon is untouched and only the word was repainted",
              f"icon px changed={_icon_diff} (in cols {min(_icols)}-{max(_icols)}), "
              f"word px changed={_word_diff}")

    # ...and what the bar DRAWS must match the supplied icon, scaled exactly the same way.
    for _k in ("play", "mods"):
        _sp = _QPixmap(str(paths.assets_dir() / f"tab_{_k}.png"))
        _w = max(1, round(_sp.width() * theme.TAB_ART_H / _sp.height()))
        _ref = _sp.scaled(_w, theme.TAB_ART_H, Qt.AspectRatioMode.IgnoreAspectRatio,
                          Qt.TransformationMode.SmoothTransformation).toImage()
        _cols = {round(_x * _w / _sp.width()) for _x in theme._coloured_columns(_sp.toImage())}
        _drawn = _art[_k].toImage()
        _diff = sum(1 for _y in range(_drawn.height()) for _x in range(_drawn.width())
                    if _x in _cols and _drawn.pixelColor(_x, _y).getRgb()
                    != _ref.pixelColor(_x, _y).getRgb())
        check(_diff == 0, f"M8 the bar draws '{_k}''s icon exactly as supplied", f"{_diff} px differ")

    # The lift is the fallback for art with no baked copy: it must not repaint an icon's columns.
    _src_pm = _QPixmap(str(paths.assets_dir() / "tab_mods.png"))
    _icols = theme._coloured_columns(_src_pm.toImage())
    _lifted = theme._lift_dark_ink(_src_pm, theme.TEXT).toImage()
    _before = _src_pm.toImage()
    _icon_diff = sum(1 for _y in range(_before.height()) for _x in range(_before.width())
                     if _x in _icols and _before.pixelColor(_x, _y).getRgb()
                     != _lifted.pixelColor(_x, _y).getRgb())
    check(_icon_diff == 0, "M9 the fallback lift leaves icon columns alone too",
          f"{_icon_diff} px differ inside x{min(_icols)}-{max(_icols)}")

    _win2 = _MW(paths.load_config())
    _win2.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
    _win2.show()
    for _ in range(3):
        _app.processEvents()
    _texts = [_win2.tabs.tabText(_i) for _i in range(_win2.tabs.count())]
    _tips = [_win2.tabs.tabToolTip(_i) for _i in range(_win2.tabs.count())]
    _null = [_win2.tabs.tabIcon(_i).isNull() for _i in range(_win2.tabs.count())]
    check(all(_t == "" for _t in _texts) and not any(_null),
          "M10 the tabs draw the art instead of text", f"texts={_texts} nullIcons={_null}")
    check(_tips == [TAB_LABELS[k] for k in TAB_ORDER],
          "M11 each tab keeps its name as a tooltip", str(_tips))

    # With no art at all the tabs must fall back to their plain labels, not go blank.
    _orig_assets = paths.assets_dir
    _noart = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-noart-"))
    paths.assets_dir = lambda: _noart      # type: ignore[assignment]
    try:
        _win2._apply_tab_art()
        _fallback = [_win2.tabs.tabText(_i) for _i in range(_win2.tabs.count())]
        check(_fallback == [TAB_LABELS[k] for k in TAB_ORDER],
              "M12 with no art the tabs fall back to their text labels", str(_fallback))
    finally:
        paths.assets_dir = _orig_assets      # type: ignore[assignment]
        _win2._apply_tab_art()
        shutil.rmtree(_noart, ignore_errors=True)

    _logo = theme.logo()
    check(_logo is not None and _logo.width() > _logo.height(),
          "M13 the logo is a wide wordmark, so the header drops the duplicate title text",
          f"{_logo.width()}x{_logo.height()}" if _logo else "none")
    check(_win2.header.height() >= LOGO_ART_H,
          "M14 the header is tall enough for the logo at its drawn height",
          f"{_win2.header.height()} >= {LOGO_ART_H}")
except Exception as _exc:  # noqa: BLE001
    check(False, "M1-M14 user-art checks raised", f"{type(_exc).__name__}: {_exc}")

# --- N. the Play/Stop pair, the header, and the Settings grid ----------------------------------
try:
    from PySide6.QtWidgets import QComboBox as _QComboBox
    from PySide6.QtWidgets import QFormLayout as _QFL
    from PySide6.QtWidgets import QGroupBox as _QGB
    from PySide6.QtWidgets import QLabel as _QLabel

    _win3 = _MW(paths.load_config())
    _win3.resize(1200, 880)
    _win3.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
    _win3.show()
    for _ in range(4):
        _app.processEvents()

    _p, _s = _win3.play.btn_play, _win3.play.btn_stop
    check(_p.size() == _s.size(), "N1 Play and Stop are the same size",
          f"{_p.width()}x{_p.height()} vs {_s.width()}x{_s.height()}")

    _hdr = [w.text() for w in _win3.header.findChildren(_QLabel) if w.text()]
    check(not _hdr, "N2 the header carries no text next to the logo", str(_hdr))

    _win3.tabs.setCurrentWidget(_win3.settings)
    for _ in range(4):
        _app.processEvents()
    _win3.settings.layout().activate()

    # one field column across every group: the thing that made the page look unaligned
    _field_x, _label_w = set(), set()
    for _box in _win3.settings.findChildren(_QGB):
        _fl = _box.layout()
        if not isinstance(_fl, _QFL):
            continue
        for _r in range(_fl.rowCount()):
            _it = _fl.itemAt(_r, _QFL.ItemRole.FieldRole)
            _w = _it.widget() if _it else None
            if _w is not None and _w.isVisible() and not isinstance(_w, _QLabel):
                _field_x.add(_w.x())
            _li = _fl.itemAt(_r, _QFL.ItemRole.LabelRole)
            _lw = _li.widget() if _li else None
            if _lw is not None and _lw.isVisible():
                _label_w.add(_lw.minimumWidth())
    check(len(_field_x) == 1, "N3 every settings group starts its fields in ONE column",
          f"field x values: {sorted(_field_x)}")
    check(_label_w == {190}, "N4 the shared label column is one width", str(sorted(_label_w)))

    _combo_w = {c.width() for c in _win3.settings.findChildren(_QComboBox) if c.isVisible()}
    check(len(_combo_w) == 1, "N5 the settings dropdowns share one width", str(sorted(_combo_w)))

    # the size dropdown shows a readable size but stores the bare width the runtime wants
    _size = _win3.settings._widgets[("video", "window_width")]
    check(isinstance(_size.currentData(), int) and "×" in _size.currentText()
          and "custom" not in _size.currentText(),
          "N6 Window size reads as a size and stores the width",
          f"{_size.currentText()!r} -> {_size.currentData()!r}")
    check(len(_size.preset_values) >= 4 and _size.count() == len(_size.preset_values),
          "N7 the size presets loaded without a stale custom entry",
          f"{_size.count()} items, {len(_size.preset_values)} presets")

    # Appearance (moved here from the Memory Card tab): one entry per editor palette, applied
    # app-wide the moment it changes, and remembered across launches. The player's own choice is
    # read back and restored, because a check must never change what they see next launch.
    _look = _win3.settings.look_combo
    _keys = [_look.itemData(_i) for _i in range(_look.count())]
    try:
        from dmw3editor.ui import theme as _edt_n                       # noqa: PLC0415
        _want_keys = list(_edt_n.PALETTES)
    except Exception:  # noqa: BLE001
        _want_keys = []
    check(bool(_keys) and _keys == _want_keys and _look.isEnabled(),
          "N8 Settings carries the Appearance dropdown, one entry per editor theme",
          f"{len(_keys)} entries: {', '.join(str(_k) for _k in _keys)}")
    if _want_keys:
        from PySide6.QtCore import QSettings as _QSettingsN              # noqa: PLC0415
        _store = _QSettingsN("DMW3SaveEditor", "DMW3SaveEditor")
        _was = _store.value("theme")
        _current = _look.currentData()
        _other = next((_k for _k in _want_keys if _k != _current), _current)
        _then = (theme.BG, theme.ACCENT, theme.TEXT)
        _look.setCurrentIndex(_look.findData(_other))
        for _ in range(4):
            _app.processEvents()
        _now = (theme.BG, theme.ACCENT, theme.TEXT)
        _saved = _store.value("theme")
        # "Remembered" has to mean the NEXT LAUNCH comes up on it, not merely that the colours
        # moved: build a second window from the same config and assert it adopts the stored theme
        # (the Memory Card tab reads saved_theme() as it constructs the embedded editor, which is
        # exactly what a restart does).
        _reborn = _MW(paths.load_config())
        _reborn.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
        _reborn.show()
        for _ in range(3):
            _app.processEvents()
        _want_bg = (_edt_n.PALETTES.get(_other) or {}).get("BG")
        check(_now != _then and _saved == _other and _want_bg is not None and theme.BG == _want_bg,
              "N9 picking a theme there recolours the app and a FRESH window comes back on it "
              "(the choice is remembered across a restart, not just applied once)",
              f"{_then[0]} -> {_now[0]}, saved {_saved!r}, fresh window BG {theme.BG} "
              f"expected {_want_bg}")
        _reborn.close()
        _look.setCurrentIndex(_look.findData(_current))
        for _ in range(3):
            _app.processEvents()
        if _was is None:
            _store.remove("theme")
        else:
            _store.setValue("theme", _was)
        check((_store.value("theme") is None) if _was is None else (_store.value("theme") == _was),
              "N10 and the theme set before is put back afterwards",
              f"restored {_store.value('theme')!r}")

        # The picker shows what it is offering: an icon per entry, and the selected theme's whole
        # palette as a strip of chips underneath.
        from PySide6.QtGui import QColor as _QColorN                    # noqa: PLC0415
        _labels = [_look.itemText(_i) for _i in range(_look.count())]
        _strip = _win3.settings.look_swatches
        _pal_now = _edt_n.PALETTES.get(_look.currentData()) or {}
        _tokens = [_t for _t in _strip.TOKENS if _pal_now.get(_t)]
        _img = _strip.grab().toImage()
        _matched = 0
        for _i, _token in enumerate(_tokens):
            _want = _QColorN(_pal_now[_token])
            _px = _img.pixelColor(_i * (_strip.CHIP_W + _strip.GAP) + _strip.CHIP_W // 2,
                                  _strip.CHIP_H // 2)
            if (abs(_px.red() - _want.red()) + abs(_px.green() - _want.green())
                    + abs(_px.blue() - _want.blue())) <= 12:
                _matched += 1
        _null_icons = [_i for _i in range(_look.count()) if _look.itemIcon(_i).isNull()]
        check(not _null_icons and bool(_tokens) and _matched == len(_tokens),
              "N11 every entry carries its colours and the strip paints the selected palette",
              f"{_look.count()} entries, {len(_null_icons)} without an icon, "
              f"{_matched}/{len(_tokens)} chips matched")
        _want_labels = {"Kumamon", "Guilmon", "Terriermon", "Beelzemon", "Gargomon"}
        _stale = [n for n in ("Digimon Blue", "Guilmon Black", "Terriermon Green",
                              "Beelzemon Purple", "Gargomon Green", "Eggshell Green")
                  if n in _labels]
        check(set(_labels) == _want_labels and not _stale,
              "N12 the five themes carry only Digimon names",
              " / ".join(_labels) + (f"  |  stale: {_stale}" if _stale else ""))
    else:
        check(True, "N11-N12 palette-preview checks skipped (the save editor is not importable)")
except Exception as _exc:  # noqa: BLE001
    check(False, "N1-N12 shell/settings checks raised", f"{type(_exc).__name__}: {_exc}")

# --- O. the Settings page's WRITE path, through the widgets -----------------------------------
# The riskiest thing this launcher does is rewrite the player's commented settings.toml, so exercise
# the real widgets against a COPY of the real file: choose a preset, Apply, and check the file comes
# back with a number, every comment, and every other key intact.
try:
    from PySide6.QtWidgets import QMessageBox as _QMB
    from dmw3launcher.ui.settings_tab import SettingsTab as _STab

    _real = _QMB.information
    for _n in ("information", "warning", "critical", "question"):
        setattr(_QMB, _n, staticmethod(lambda *a, **k: None))   # modal boxes block offscreen
    _inst: pathlib.Path | None = None
    try:
        _real_toml = root and paths.settings_toml(root)
        if _real_toml and _real_toml.is_file():
            _inst = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-stwrite-"))
            (_inst / "Binaries").mkdir(parents=True)
            (_inst / "Engine" / "Config").mkdir(parents=True)
            (_inst / "Binaries" / "DigimonWorld2003.exe").write_bytes(b"stub")
            (_inst / "Engine" / "Config" / "game.toml").write_text("[game]\n", encoding="utf-8")
            _copy = _inst / "Binaries" / "settings.toml"
            shutil.copy2(_real_toml, _copy)
            _src_text = _copy.read_text(encoding="utf-8")
            _before = st.snapshot(_copy)

            _tab = _STab({"runtime_root": str(_inst)})
            _size = _tab._widgets[("video", "window_width")]
            check(_size.currentData() == 1280,
                  "O1 the page loads the file's width into the preset",
                  f"{_size.currentText()!r} -> {_size.currentData()!r}")
            _i = _size.findData(1920)
            _size.setCurrentIndex(_i)
            _tab.apply()
            _after = st.snapshot(_copy)
            check(int(_after["video"]["window_width"]) == 1920,
                  "O2 Apply writes the chosen width", repr(_after["video"]["window_width"]))
            check(not isinstance(_after["video"]["window_width"], str),
                  "O3 written as a number, not the dropdown's label",
                  type(_after["video"]["window_width"]).__name__)
            check(sum(1 for l in _src_text.splitlines() if l.lstrip().startswith("#"))
                  == sum(1 for l in _copy.read_text(encoding="utf-8").splitlines()
                         if l.lstrip().startswith("#")),
                  "O4 every comment in the player's file survives")
            check(_after["video"].get("renderer") == _before["video"].get("renderer")
                  and _after["audio"] == _before["audio"],
                  "O5 keys the launcher does not manage are untouched")

            st.set_values(_copy, {("video", "window_width"): 1024})
            for _ in range(3):
                _tab.reload()
            check(_size.currentData() == 1024, "O6 an off-preset width still reads back",
                  f"{_size.currentText()!r} -> {_size.currentData()!r}")
            check(sum(1 for _j in range(_size.count())
                      if "custom" in _size.itemText(_j)) == 1,
                  "O7 repeated refreshes leave exactly one custom entry")
            _tab.apply()
            check(int(st.snapshot(_copy)["video"]["window_width"]) == 1024,
                  "O8 and it writes back as the number, not the label")
        else:
            check(True, "O1-O8 settings write checks skipped (no real settings.toml)")
    finally:
        _QMB.information = _real
        # in a finally so a failing check cannot leave the temp install behind
        if _inst is not None:
            shutil.rmtree(_inst, ignore_errors=True)
except Exception as _exc:  # noqa: BLE001
    check(False, "O1-O8 settings write checks raised", f"{type(_exc).__name__}: {_exc}")

# --- P. the app background: no art means the live theme colour --------------------------------
try:
    _win4 = _MW(paths.load_config())
    _win4.resize(1000, 700)
    _win4.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
    _win4.show()
    for _ in range(3):
        _app.processEvents()
    _win4.tabs.setCurrentWidget(_win4.settings)   # a page with margin beside its cards
    for _ in range(3):
        _app.processEvents()
    if (paths.assets_dir() / "background.png").is_file():
        check(True, "P1-P2 background art is present, so the theme-colour path does not apply")
    else:
        check(_win4._bg is None, "P1 no background art is loaded when the file is absent")
        _shot = _win4.grab().toImage()
        _page = _shot.pixelColor(6, _shot.height() // 2).name()
        check(_page == theme.BG.lower(),
              "P2 with no background art the page background is the live theme colour",
              f"{_page} vs {theme.BG}")
    if (paths.assets_dir() / "header_bar.png").is_file():
        check(True, "P3-P5 header art is present, so the flat-panel path does not apply")
    else:
        check(_win4.header._pm is None, "P3 with no header art the header holds no pixmap")
        _pt = _win4.header.mapTo(_win4, _win4.header.rect().center())   # middle: logo is left,
        _band = _win4.grab().toImage().pixelColor(_pt).name()           # buttons are right
        check(_band == theme.BG.lower(),
              "P4 the blank header paints the PAGE colour, so header and page never seam",
              f"{_band} vs {theme.BG}")

        # ...and that has to hold for every theme, not just the active one: the point of the
        # seamless header is that it survives a theme swap.
        try:
            from dmw3editor.ui import theme as _edt3
            _pals = dict(_edt3.PALETTES)
        except Exception:  # noqa: BLE001
            _pals = {}
        if _pals:
            _mismatched = []
            for _pname, _pal in _pals.items():
                theme.use_editor_theme(_pal)
                theme.set_editor_active(True)
                _win4._restyle()
                for _ in range(2):
                    _app.processEvents()
                _img = _win4.grab().toImage()
                if (_img.pixelColor(6, _img.height() // 2).name()
                        != _img.pixelColor(_win4.header.mapTo(
                            _win4, _win4.header.rect().center())).name()):
                    _mismatched.append(_pname)
            check(not _mismatched,
                  "P5 header and page are the same colour in all 5 themes",
                  ", ".join(_mismatched) or f"{len(_pals)} themes matched")
            theme.use_editor_theme(_pals.get(getattr(_edt3, "DEFAULT_THEME", ""), list(_pals.values())[0]))
            _win4._restyle()
except Exception as _exc:  # noqa: BLE001
    check(False, "P1-P5 background/header checks raised", f"{type(_exc).__name__}: {_exc}")

# --- Q. the Decompilation tab: five tabs, published progress, and a READ-ONLY tree ------------
try:
    import hashlib                                                        # noqa: PLC0415

    from PySide6.QtCore import QPoint as _QPoint                          # noqa: PLC0415
    from PySide6.QtCore import Qt as _QtQ                                # noqa: PLC0415
    from PySide6.QtWidgets import (QApplication as _QApp,  # noqa: PLC0415
                                   QFrame as _QFrame, QHeaderView as _QHeaderView)

    from dmw3launcher import decomp as _dc                               # noqa: PLC0415
    from dmw3launcher.ui.decomp_tab import CodeView as _CodeView         # noqa: PLC0415

    _app5 = _QApp.instance() or _QApp([])
    check(_dc.available(), "Q0 the decomp tree is where the launcher expects it",
          str(_dc.ROOT))

    # Snapshot the REAL tree before touching anything: the read-only claim is only worth something
    # if it is checked against every byte of every file, not against the absence of an edit button.
    _tree_before = {}
    for _p in _dc.ROOT.rglob("*"):
        if _p.is_file():
            _st = _p.stat()
            _tree_before[_p.relative_to(_dc.ROOT).as_posix()] = (
                _st.st_size, _st.st_mtime_ns,
                hashlib.sha256(_p.read_bytes()).hexdigest())

    _win5 = _MW(paths.load_config())
    _win5.resize(1200, 880)
    _win5.setAttribute(_QtQ.WidgetAttribute.WA_DontShowOnScreen, True)
    _win5.show()
    for _ in range(4):
        _app5.processEvents()

    _labels = [_win5.tabs.tabText(i) or _win5.tabs.tabToolTip(i) for i in range(_win5.tabs.count())]
    check(_labels == ["Play", "Memory Card", "Mods", "Decompilation", "Settings"],
          "Q1 five tabs, in order, with the decompilation before Settings", str(_labels))
    check(_win5.tabs.indexOf(_win5.decomp) < _win5.tabs.indexOf(_win5.settings),
          "Q2 the new tab sits BEFORE Settings")

    _art = theme.tab_image("decomp")
    check(_art is not None, "Q3 the new tab's word-image is installed")
    if _art is not None:
        _others = {theme.tab_image(k).width() for k in ("play", "memcard", "mods", "settings")
                   if theme.tab_image(k) is not None}
        check(_art.height() == theme.TAB_ART_H and _others == {_art.width()},
              "Q4 it is drawn at the same art height and width as the other four",
              f"{_art.width()}x{_art.height()} vs {_others}")
        _img = _art.toImage()
        _ink = [theme._luma(_img.pixelColor(x, y))
                for y in range(0, _img.height(), 3) for x in range(0, _img.width(), 3)
                if _img.pixelColor(x, y).alpha() > 32]
        check(_ink and sum(_ink) / len(_ink) > 0.5,
              "Q5 its word reads light on the dark chrome (the white copy is the one drawn)",
              f"mean ink luma {sum(_ink) / max(1, len(_ink)):.2f}")

    _bar = _win5.tabs.tabBar()
    _win5.resize(_win5.minimumWidth(), 640)
    for _ in range(3):
        _app5.processEvents()
    check(_bar.sizeHint().width() <= _bar.width(),
          "Q6 five word-images fit the tab bar at the window's minimum width",
          f"needs {_bar.sizeHint().width()}px of {_bar.width()}px at {_win5.minimumWidth()}px wide")

    _win5.resize(1200, 880)
    _win5.tabs.setCurrentWidget(_win5.decomp)
    for _ in range(6):
        _app5.processEvents()
    _d = _win5.decomp

    # The page obeys the same layout rules as every other: nothing squeezed, at the default size
    # AND at the window's minimum, which is what the artist and the user will actually resize to.
    def _squeezed_at(width: int, height: int) -> list[str]:
        _win5.resize(width, height)
        for _ in range(3):
            _app5.processEvents()
        _d.layout().activate()
        for _ in range(3):
            _app5.processEvents()
        out: list[str] = []
        for _child in _d.findChildren(_QFrame):
            if not _child.isVisible():
                continue
            # A view's header is a fixture of that view, not page content: Qt reports a large
            # minimumSizeHint for it (58px) while rendering it at the style's own height (32px), so
            # it is not evidence of a squeezed layout. Everything else must fit its minimum.
            if isinstance(_child, _QHeaderView):
                continue
            _geo = _child.geometry()
            if _geo.width() <= 0:
                continue
            # An explicit minimumHeight is a decision this code made on purpose (the tree may be
            # 80px tall); the hint is what the style would ask for on its own. Flag only shrinking
            # below the floor the widget actually declares, which is the involuntary case.
            _floor = _child.minimumHeight() or _child.minimumSizeHint().height()
            if _floor > _geo.height() + 1:
                out.append(f"{type(_child).__name__} {_geo.height()} < {_floor}")
        return out

    _default_squeezed = _squeezed_at(1200, 880)
    _min_squeezed = _squeezed_at(_win5.minimumWidth(), _win5.minimumHeight())
    _win5.resize(1200, 880)
    for _ in range(3):
        _app5.processEvents()
    check(not _default_squeezed and not _min_squeezed,
          "Q7 nothing on the decomp page is squeezed, at the default size or at the minimum",
          "; ".join(_default_squeezed + [f"minimum: {t}" for t in _min_squeezed])
          or f"clean at 1200x880 and at {_win5.minimumWidth()}x{_win5.minimumHeight()}")

    if _dc.available():
        _names = {_d.tree.topLevelItem(i).text(0) for i in range(_d.tree.topLevelItemCount())}
        check({"src", "include", "config", "tools", "docs"} <= _names and "external" not in _names,
              "Q8 the tree is the real project, and vendored external/ is not listed",
              ", ".join(sorted(_names)))
        _src_files = [f for f in _dc.scan().files if f.rel.startswith("src/")]
        check(sum(1 for f in _src_files if f.suffix == ".c") == 377,
              "Q9 the tree holds the project's 377 C files",
              f"{sum(1 for f in _src_files if f.suffix == '.c')}")
        check(sum(f.stubs for f in _src_files) == 0,
              "Q10 no stubs remain in src/ (the complete tree has nothing left to match)")

        _pub = _dc.published()
        # The panel opens on USA — the one build order the whole UI follows now — so its numbers
        # come from the USA bucket and the USA segment is the one checked. (Europe here publishes
        # different totals, so this also proves the pane is not silently showing the wrong region.)
        _us = _pub.get("regions", {}).get("USA", {})
        _shown = f"{_us.get('matched', 0):,} / {_us.get('total', 0):,}"
        check(_d.progress.percent.text() == f"{_us.get('percent', 0):.2f}%"
              and _d.progress.counts.text().startswith(_shown)
              and _d.progress._buttons["USA"].isChecked(),
              "Q11 the panel shows the project's own published numbers, not a count of ours "
              "(USA leads, as everywhere else)",
              f"{_d.progress.percent.text()} · {_d.progress.counts.text()}")

        # Every code view the tab can make must be read-only, so open one first and ask the CLASS:
        # before a file is opened there is no view at all, and asking the current widget would have
        # checked nothing while reporting a pass.
        _d.open_path("src/stgdglab/stgdglab_4.c")
        for _ in range(2):
            _app5.processEvents()
        _views = _d.findChildren(_CodeView)
        _writable = [w for w in _views if not w.isReadOnly() or w.isUndoRedoEnabled()]
        check(_views and not _writable and _d.view is not None,
              "Q12 every code view is read-only and undo/redo is off, with no writable sibling",
              f"{len(_views)} views, {len(_writable)} writable")
        _view = _d.view                  # the view the checks below read from

        _rel = "src/stgdglab/stgdglab_4.c"
        _opened = _d.open_path(_rel)
        for _ in range(3):
            _app5.processEvents()
        _real_lines = (_dc.ROOT / _rel).read_text(encoding="utf-8", errors="replace").splitlines()
        check(_opened and _view.blockCount() == len(_real_lines) and _view.highlighter._rules,
              "Q13 opening a file shows exactly its lines, highlighted",
              f"{_view.blockCount()} blocks vs {len(_real_lines)} lines, "
              f"{len(_view.highlighter._rules)} rules")

        _d.find_box.setText("STGDGLAB_openMenu")
        _d.find()
        for _ in range(3):
            _app5.processEvents()
        _hits = [_d.results.item(i).data(_QtQ.ItemDataRole.UserRole)
                 for i in range(_d.results.count())]
        _hits = [h for h in _hits if isinstance(h, tuple)]
        check(_hits, "Q14 searching the tree returns file/line hits", f"{len(_hits)} hits")
        if _hits:
            _hit_file, _hit_line = _hits[0]
            _d._open_result(_d.results.item(0))
            for _ in range(3):
                _app5.processEvents()
            _actual = (_dc.ROOT / _hit_file).read_text(encoding="utf-8",
                                                       errors="replace").splitlines()[_hit_line - 1]
            check(_d.view.textCursor().blockNumber() + 1 == _hit_line
                  and "STGDGLAB_openMenu" in _actual,
                  "Q15 a hit jumps to the line that really contains it",
                  f"{_hit_file}:{_hit_line}")

        _d.filter_box.setText("stgdglab")
        for _ in range(3):
            _app5.processEvents()
        _visible = {_d.tree.topLevelItem(i).text(0) for i in range(_d.tree.topLevelItemCount())
                    if not _d.tree.topLevelItem(i).isHidden()}
        check("src" in _visible and _visible != _names,
              "Q16 the file filter narrows the tree to matching branches", ", ".join(sorted(_visible)))
        _d.filter_box.setText("")

    # The editor's tabs: one per open file, reused when a file is opened again, and closable.
    # Start from no tabs: the search above already opened one, and the counts below are absolute.
    while _d._order:
        _d._close_tab(0)
    for _ in range(3):
        _app5.processEvents()
    _first = "src/stgdglab/stgdglab_4.c"
    _second = "src/main/system.c"
    _d.open_path(_first)
    _d.open_path(_second)
    for _ in range(3):
        _app5.processEvents()
    check(_d.tabs.count() == 2 and _d.view is _d._docs.get(_second),
          "Q17 opening a second file adds a tab and shows it",
          f"{_d.tabs.count()} tabs, showing {_d.file_path.text()!r}")
    _d.tabs.setCurrentIndex(0)
    for _ in range(3):
        _app5.processEvents()
    _want = len((_dc.ROOT / _first).read_text(encoding="utf-8",
                                              errors="replace").splitlines())
    check(_d.view is _d._docs.get(_first) and _d.view.blockCount() == _want,
          "Q18 switching tabs brings the other file back, still loaded",
          f"{_d.view.blockCount()} blocks vs {_want} lines")
    _d.open_path(_first, 40)
    for _ in range(3):
        _app5.processEvents()
    check(_d.tabs.count() == 2,
          "Q19 opening the same file again reuses its tab instead of duplicating it",
          f"{_d.tabs.count()} tabs")
    check("›" in _d.file_path.text() and "read-only" in _d.editor_status.text()
          and _d.view.textCursor().blockNumber() + 1 == 40,
          "Q20 the breadcrumb, the status line and a jump-to-line all land together",
          f"{_d.file_path.text()!r} / {_d.editor_status.text()!r} / "
          f"line {_d.view.textCursor().blockNumber() + 1}")
    _d._close_tab(0)
    for _ in range(3):
        _app5.processEvents()
    check(_d.tabs.count() == 1 and _d.view is _d._docs.get(_second)
          and _d.file_path.text().endswith("system.c"),
          "Q21 closing a tab drops it and shows the one that is left",
          f"{_d.tabs.count()} tabs, showing {_d.file_path.text()!r}")

    # The close glyph is painted by the tab bar, so its hit box is ours: a click on it must close
    # THAT tab and must not be swallowed by the tab underneath.
    from PySide6.QtTest import QTest as _QTest                            # noqa: PLC0415
    _d.open_path(_first)
    for _ in range(3):
        _app5.processEvents()
    _target = _d._order[0]
    _box = _d.tabs.close_rect(0)
    _QTest.mouseClick(_d.tabs, _QtQ.MouseButton.LeftButton,
                      _QtQ.KeyboardModifier.NoModifier, _box.center())
    for _ in range(3):
        _app5.processEvents()
    check(_d.tabs.count() == 1 and _target not in _d._docs,
          "Q22 clicking a tab's close glyph closes that tab",
          f"{_d.tabs.count()} tab left, closed {_target.rsplit('/', 1)[-1]}")

    # The hit box is ours, so the other half matters too: a click on the tab BODY must select the
    # tab and close nothing, and a click just inside the tab but outside the glyph must do nothing.
    _d.open_path(_second)
    for _ in range(3):
        _app5.processEvents()
    _count = _d.tabs.count()
    _body = _QPoint(_d.tabs.tabRect(1).left() + 5, _d.tabs.tabRect(1).center().y())
    _QTest.mouseClick(_d.tabs, _QtQ.MouseButton.LeftButton, _QtQ.KeyboardModifier.NoModifier, _body)
    for _ in range(3):
        _app5.processEvents()
    _box1 = _d.tabs.close_rect(1)
    _outside = _QPoint(_box1.center().x() - _box1.width() // 2 - 3, _box1.center().y())
    _QTest.mouseClick(_d.tabs, _QtQ.MouseButton.LeftButton, _QtQ.KeyboardModifier.NoModifier,
                      _outside)
    for _ in range(3):
        _app5.processEvents()
    check(_d.tabs.count() == _count and _d.tabs.currentIndex() == 1,
          "Q23 a click on a tab's body selects it, and just outside the glyph closes nothing",
          f"{_d.tabs.count()} tabs, current {_d.tabs.currentIndex()}")

    # ...and the read-only proof: the same tree, byte for byte, after all of that.
    _tree_after = {}
    for _p in _dc.ROOT.rglob("*"):
        if _p.is_file():
            _st = _p.stat()
            _tree_after[_p.relative_to(_dc.ROOT).as_posix()] = (
                _st.st_size, _st.st_mtime_ns,
                hashlib.sha256(_p.read_bytes()).hexdigest())
    _changed = sorted(k for k in set(_tree_before) | set(_tree_after)
                      if _tree_before.get(k) != _tree_after.get(k))
    check(not _changed,
          "Q24 the decomp tree is untouched: every file's size, mtime and hash is unchanged",
          f"{len(_changed)} changed of {len(_tree_before)}" + (f": {_changed[:3]}" if _changed else ""))
    _win5.close()
except Exception as _exc:  # noqa: BLE001
    check(False, "Q1-Q24 decomp-tab checks raised", f"{type(_exc).__name__}: {_exc}")

# --- R. the decomp data layer: pure functions on synthetic inputs -------------------------------
# Section Q drives the REAL tree; these pin the parts no real-tree run can isolate, so a regression
# in the counter or the README parser is caught by shape instead of by the numbers still happening
# to look plausible. Synthetic inputs, a temp tree, and ROOT restored either way.
try:
    import shutil as _shutilR                                              # noqa: PLC0415
    import tempfile as _tempfileR                                          # noqa: PLC0415

    from dmw3launcher import decomp as _dcR                                # noqa: PLC0415

    _shapes = {
        "signature with its brace on the same line": ("void f(void) {\n  g();\n}\n", 1),
        "brace on the next line": ("static s32 h(s32 a)\n{\n  return a;\n}\n", 1),
        "a whole body on one line": ("void k(void) {}\n", 1),
        "a for-loop and its calls": ("void m(void) {\n  for (i = 0; i < n; i++) {\n    g();\n  }\n}\n", 1),
        "braces inside comments": ("/* { */\nvoid p(void) {\n  // }\n}\n", 1),
        "a prototype with no brace": ("s32 q(void);\n", 0),
        "nested blocks": ("void r(void) {\n  if (a) {\n    while (b) {\n      c();\n    }\n  }\n}\n", 1),
    }
    _wrong = [name for name, (snippet, want) in _shapes.items()
              if _dcR.count_code(snippet)[0] != want]
    check(not _wrong, "R1 the definition counter handles every shape it will meet",
          ", ".join(_wrong) or f"{len(_shapes)} shapes")

    check(_dcR.count_code('INCLUDE_ASM("main/nonmatchings/graphics", convertText);\n') == (0, 1),
          "R2 a stub macro counts as a stub and not as a function")

    check(_dcR.search("(unclosed", regex=True) == [],
          "R3 an invalid regex returns no hits instead of raising (it used to raise)")

    _rroot = pathlib.Path(_tempfileR.mkdtemp(prefix="hermes-verify-dmw3-layer-"))
    _real_root = _dcR.ROOT
    try:
        (_rroot / "src").mkdir()
        (_rroot / "src" / "a.c").write_text("void a(void) {}\n", encoding="utf-8")
        (_rroot / "external" / "vendored").mkdir(parents=True)
        (_rroot / "external" / "vendored" / "v.c").write_text("void v(void) {}\n", encoding="utf-8")
        _dcR.ROOT = _rroot
        _rels = {f.rel for f in _dcR.scan().files}
        check(_rels == {"src/a.c"},
              "R4 the walk skips vendored external/ and dot-directories", ", ".join(sorted(_rels)))
        _dcR.ROOT = _rroot / "gone"
        _dcR.scan()                     # must not raise out of a tab's first paint
        check(_dcR.published() == {},
              "R5 a tree that vanished reports nothing and raises nothing")
    finally:
        _dcR.ROOT = _real_root
        _shutilR.rmtree(_rroot, ignore_errors=True)
except Exception as _exc:  # noqa: BLE001
    check(False, "R1-R5 decomp data-layer checks raised", f"{type(_exc).__name__}: {_exc}")

# --- S. the editor pane's pixels: the IDE claims a widget walk cannot confirm --------------------
# A widget walk proves the widgets exist; it cannot prove the caret's line is banded, that the
# gutter emphasises the right number, or that a role colour is not sitting on the background.
try:
    from PySide6.QtCore import QPointF as _QPointFS                       # noqa: PLC0415
    from PySide6.QtCore import Qt as _QtS                                 # noqa: PLC0415
    from PySide6.QtGui import QColor as _QColorS                          # noqa: PLC0415
    from PySide6.QtGui import QMouseEvent as _QMouseEventS                # noqa: PLC0415
    from PySide6.QtWidgets import QApplication as _QAppS                  # noqa: PLC0415

    from dmw3launcher.ui.main_window import MainWindow as _MWS            # noqa: PLC0415

    _app6 = _QAppS.instance() or _QAppS([])
    _win6 = _MWS(paths.load_config())
    _win6.resize(1280, 1000)
    _win6.setAttribute(_QtS.WidgetAttribute.WA_DontShowOnScreen, True)
    _win6.show()
    for _ in range(5):
        _app6.processEvents()
    _win6.tabs.setCurrentWidget(_win6.decomp)
    _win6.decomp.layout().activate()
    for _ in range(5):
        _app6.processEvents()
    _d6 = _win6.decomp
    _d6.open_path("src/stgdglab/stgdglab_4.c", 690)
    for _ in range(5):
        _app6.processEvents()
    _v = _d6.view
    _shot = _win6.grab().toImage()

    def _near(a, b, tol: int = 26) -> bool:
        return (abs(a.red() - b.red()) + abs(a.green() - b.green())
                + abs(a.blue() - b.blue())) <= tol

    # Both the code and the numbers start at the VIEWPORT's top: the sheet's padding insets them, and
    # measuring from the view lands in the blank gap between rows, where a correct feature reads as
    # background. Rows count from the FIRST VISIBLE block, because the view is scrolled.
    _vp = _v.viewport().mapTo(_win6, _v.viewport().rect().topLeft())
    _gut = _v._gutter
    _gp = _gut.mapTo(_win6, _gut.rect().topLeft())
    _lh = _v.fontMetrics().height()
    _caret_row = int(_vp.y() + _v.blockBoundingGeometry(_v.firstVisibleBlock())
                     .translated(_v.contentOffset()).top()
                     + (_v.textCursor().blockNumber()
                        - _v.firstVisibleBlock().blockNumber()) * _lh)
    _pal6 = theme.code_palette()

    _band = _shot.pixelColor(int(_vp.x() + _v.viewport().width() - 10), _caret_row + _lh // 2)
    _above = _shot.pixelColor(int(_vp.x() + _v.viewport().width() - 10), _caret_row - _lh // 2)
    check(_near(_band, _QColorS(theme.PANEL_HI)) and not _near(_band, _above),
          "S1 the caret's line is banded in the panel colour and the line above it is not",
          f"{_band.name()} vs {_above.name()} (PANEL_HI {theme.PANEL_HI})")

    _ink: dict[str, object] = {}
    for _which, _row in (("active", _caret_row), ("neighbour", _caret_row - _lh)):
        _best = None
        for _y in range(_row + 1, _row + _lh - 1):
            for _x in range(int(_gp.x()), int(_gp.x() + _gut.width())):
                _px = _shot.pixelColor(_x, _y)
                if _best is None or theme._luma(_px) > theme._luma(_best):
                    _best = _px
        _ink[_which] = _best
    check(_ink["active"] != _ink["neighbour"]
          and _near(_ink["active"], _QColorS(_pal6["gutter_active"]), 90)
          and _near(_ink["neighbour"], _QColorS(_pal6["gutter_fg"]), 90),
          "S2 the gutter draws the caret's number brighter than its neighbour's",
          f"{_ink['active'].name()} vs {_ink['neighbour'].name()}")

    # One screenful of a long file holds no type declaration and few comments, so walk several
    # screens; and a role colour within a hair of the background is invisible however it is set.
    _bg6 = _shot.pixelColor(int(_vp.x()) + 4, int(_vp.y()) + 4)
    _roles6 = {k: _pal6[k] for k in ("keyword", "type", "number", "comment", "string")}
    _seen6: dict[str, int] = {}
    for _line in (1, 200, 420, 690, 900, 1150):
        _v.goto_line(_line)
        for _ in range(2):
            _app6.processEvents()
        _page = _win6.grab().toImage()
        for _y in range(int(_vp.y()), int(_vp.y() + _v.viewport().height()), 2):
            for _x in range(int(_vp.x()), int(_vp.x() + _v.viewport().width()), 2):
                _px = _page.pixelColor(_x, _y)
                if _px.alpha() < 200:
                    continue
                for _role, _value in _roles6.items():
                    if _near(_px, _QColorS(_value), 18):
                        _seen6[_role] = _seen6.get(_role, 0) + 1
    check(len([r for r, n in _seen6.items() if n >= 8]) >= 4,
          "S3 at least four syntax roles reach the pixels",
          ", ".join(f"{r}={n}" for r, n in sorted(_seen6.items())))
    check(all(not _near(_QColorS(v), _bg6, 30) for v in _roles6.values()),
          "S4 no role colour is close enough to the editor background to vanish into it",
          f"background {_bg6.name()}")

    # The tab's close glyph: the user's complaint was that it sat against the tab's border, so the
    # spacing around it is checked as a number. Ink runs inside a tab are the name's glyphs, then the
    # X, then the tab's own border, so the gaps are the last name glyph to the X, and the X to the
    # border. They should read as evenly spaced, not one squeezed against an edge.
    _d6.open_path("src/main/system.c")
    for _ in range(4):
        _app6.processEvents()
    _bar6 = _d6.tabs
    _shot6 = _win6.grab().toImage()
    _bo = _bar6.mapTo(_win6, _bar6.rect().topLeft())
    _gaps: list[str] = []
    _even: list[bool] = []
    for _i in range(min(2, _bar6.count())):
        _r = _bar6.tabRect(_i)
        _x0, _x1 = _bo.x() + _r.left(), _bo.x() + _r.right()
        _y0, _y1 = _bo.y() + _r.top(), _bo.y() + _r.bottom()

        def _ink(_x: int) -> bool:
            return any(theme._luma(_shot6.pixelColor(_x, _y)) > 0.18
                       for _y in range(_y0 + 3, _y1 - 2))

        # Measure the two gaps in the windows the design puts them in rather than by classifying
        # ink runs: the X's two strokes can read as one run or two depending on antialiasing, and a
        # run-based classifier then mistakes a stroke for the file name (it reported 2px once).
        _edge = _x1 - 1                       # the tab's last column, whose border is not the glyph
        _window = 24                          # the X lives in the tab's right 24px
        _x_cols = [_x for _x in range(max(_x0, _edge - _window + 1), _edge - 1) if _ink(_x)]
        _n_cols = [_x for _x in range(_x0, max(_x0, _edge - _window)) if _ink(_x)]
        if not _x_cols or not _n_cols:
            continue
        _before = min(_x_cols) - max(_n_cols) - 1
        _after = _edge - 1 - max(_x_cols)
        _gaps.append(f"tab {_i}: name->X {_before}px, X->edge {_after}px")
        _even.append(abs(_before - _after) <= 3 and _after >= 6 and _before >= 6)
    check(_even and all(_even),
          "S5 a tab's close glyph is evenly spaced, not squeezed against the tab's border",
          "; ".join(_gaps) or "no tab had ink to measure")

    # The glyph's hover: without mouse tracking Qt delivers NO move events while no button is held,
    # so both the highlight and the cursor were dead until this was checked.
    check(_bar6.hasMouseTracking(),
          "S6 the tab bar tracks the mouse, without which none of the hover below can fire")
    _boxes = _bar6.close_rect(0), _bar6.close_rect(1)
    _event = _QMouseEventS(_QMouseEventS.Type.MouseMove, _QPointFS(_boxes[0].center()),
                           _QPointFS(_bar6.mapToGlobal(_boxes[0].center())),
                           _QtS.MouseButton.NoButton, _QtS.MouseButton.NoButton,
                           _QtS.KeyboardModifier.NoModifier)
    _QAppS.sendEvent(_bar6, _event)
    for _ in range(4):
        _app6.processEvents()
    _page6 = _win6.grab().toImage()
    _bo6 = _bar6.mapTo(_win6, _bar6.rect().topLeft())

    def _box_pixels(box, image):
        return [image.pixelColor(_bo6.x() + _x, _bo6.y() + _y).name()
                for _y in range(box.top(), box.bottom(), 2) for _x in range(box.left(),
                                                                        box.right(), 2)]

    _hovered = _box_pixels(_boxes[0], _page6) != _box_pixels(_boxes[0], _shot6)
    _untouched = _box_pixels(_boxes[1], _page6) == _box_pixels(_boxes[1], _shot6)
    check(_hovered and _untouched,
          "S7 hovering a close glyph highlights that glyph and leaves its neighbour alone")
    check(_bar6.cursor().shape() == _QtS.CursorShape.PointingHandCursor,
          "S8 the cursor is a pointing hand over a close glyph", str(_bar6.cursor().shape()))
    _win6.close()
except Exception as _exc:  # noqa: BLE001
    check(False, "S1-S8 editor pixel checks raised", f"{type(_exc).__name__}: {_exc}")

# --- T. the app's identity: the name, the icon, the Windows taskbar id ---------------------------
try:
    import sys as _sysT                                                   # noqa: PLC0415

    from PySide6.QtGui import QIcon as _QIconT                            # noqa: PLC0415
    from PySide6.QtWidgets import QApplication as _QAppT                  # noqa: PLC0415

    import main as _mainT                                                 # noqa: PLC0415

    from dmw3launcher import paths as _pathsT                             # noqa: PLC0415
    from dmw3launcher.ui.main_window import MainWindow as _MWT            # noqa: PLC0415

    _app7 = _QAppT.instance() or _QAppT([])
    _win7 = _MWT(paths.load_config())
    _win7.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
    _win7.show()
    for _ in range(4):
        _app7.processEvents()

    check(_win7.windowTitle() == _pathsT.APP_NAME == "DW3 Recompiled+",
          "T1 the app is named DW3 Recompiled+ in its title bar",
          f"{_win7.windowTitle()!r}")

    _mine = _win7.windowIcon().pixmap(64, 64).toImage()
    _fallback = theme.generated_icon().scaled(64, 64).toImage()
    _same = all(_mine.pixelColor(x, y) == _fallback.pixelColor(x, y)
                for x in range(0, 64, 4) for y in range(0, 64, 4))
    check(not _win7.windowIcon().isNull() and not _same,
          "T2 the window carries the supplied icon, not the generated fallback",
          f"icon.ico installed: {(paths.assets_dir() / 'icon.ico').is_file()}")

    _ico = paths.assets_dir() / "icon.ico"
    _sizes = ({(s.width(), s.height()) for s in _QIconT(str(_ico)).availableSizes()}
              if _ico.is_file() else set())
    check({16, 32, 48, 256} <= {w for w, _h in _sizes},
          "T3 the ICO carries the frames Windows asks for (16/32/48/256)",
          ", ".join(f"{w}x{h}" for w, h in sorted(_sizes)) or "no icon.ico")

    # The taskbar button uses the window icon only for a process that claims its own identity;
    # otherwise Windows groups it under python.exe. Claim it and read it back, freeing the string
    # the way the API requires.
    _claimed = _mainT.set_windows_app_id()
    _readback = ""
    if _sysT.platform == "win32":
        import ctypes as _ctypesT                                         # noqa: PLC0415
        _buf = _ctypesT.c_wchar_p()
        try:
            if _ctypesT.windll.shell32.GetCurrentProcessExplicitAppUserModelID(
                    _ctypesT.byref(_buf)) == 0:
                _readback = _buf.value or ""
        finally:
            if _buf:
                _ctypesT.windll.ole32.CoTaskMemFree(_buf)
    check((_claimed and _readback == _mainT.APP_USER_MODEL_ID)
          if _sysT.platform == "win32" else True,
          "T4 the process claims its own taskbar identity, so the taskbar shows our icon",
          f"claimed={_claimed}, read back {_readback!r}")
    _win7.close()
except Exception as _exc:  # noqa: BLE001
    check(False, "T1-T4 identity checks raised", f"{type(_exc).__name__}: {_exc}")

# --- U: DiscTool's report reaches the player in English -------------------------------
# Fixtures are the tool's real output, captured verbatim from DiscTool.exe: the European disc it
# accepts, and the USA disc it refuses.
try:
    from dmw3launcher import tooltext as _ttU, disc as _discU                     # noqa: PLC0415

    _euU = ("volumen      : DMW3\nsistema      : PLAYSTATION\nejecutable   : SLES_039.36\n"
            "sectores     : 294280 (el disco dice 294280)\nAAA oculto   : si\n"
            "veredicto    : ES EL DISCO CORRECTO")
    _usU = ("volumen      : DMW3\nsistema      : PLAYSTATION\nejecutable   : SLUS_014.36\n"
            "sectores     : 275309 (el disco dice 275309)\nAAA oculto   : si\n"
            "veredicto    : Es Digimon World 3/2003, pero no la edicion europea. Su disco arranca "
            "con SLUS_014.36 y este port esta hecho sobre SLES_039.36. Hace falta la version "
            "europea (SLES-03936).")
    _leftU = _ttU.still_spanish(_ttU.english(_euU)) + _ttU.still_spanish(_ttU.english(_usU))
    check(not _leftU, "U1 both real DiscTool reports translate with no Spanish left",
          ", ".join(_leftU) or "clean")
    _eu_lines = _discU.DiscCheck(True, _discU.REGION_EU, "ok", _euU).lines
    _wantU = [l.strip() for l in _ttU.english(_euU).splitlines() if l.strip()]
    check(_eu_lines == _wantU and len(_eu_lines) == 6,
          "U2 DiscCheck.lines is the whole report in English, one entry per line",
          " | ".join(_eu_lines))
    check("verdict" in _eu_lines[-1].lower() and "correct disc" in _eu_lines[-1].lower(),
          "U3 the success verdict reads as English", _eu_lines[-1])
    _us_lines = _discU.DiscCheck(False, _discU.REGION_US, "no", _usU).lines
    check(not _ttU.still_spanish("\n".join(_eu_lines + _us_lines)),
          "U4 nothing Spanish survives on the path the log walks")
    check("SLUS_014.36" in "\n".join(_us_lines) and "European version" in "\n".join(_us_lines),
          "U5 the refusal keeps its facts in English", _us_lines[-1][:88])
    check(_discU.DiscCheck(True, "eu", "ok").lines == [] and _ttU.english("") == "",
          "U6 an empty report shows nothing instead of raising")
    _playU = (paths.launcher_root() / "dmw3launcher/ui/play_tab.py").read_text(encoding="utf-8")
    check("chk.raw" not in _playU and "chk.lines" in _playU,
          "U7 the Play tab shows lines(), never the untranslated report")
except Exception as _exc:  # noqa: BLE001
    check(False, "U1-U7 disc-report language checks raised", f"{type(_exc).__name__}: {_exc}")

# --- V. the two native builds: buttons, folders, the disc rewrite, the router -------------------
# The Play tab no longer installs a port. It runs our own regional recompilations, each from its own
# Builds/<region>/ folder, with that build's own game.toml pointed at the matching image. These
# checks pin that behaviour against the real layout on disk and the real Play tab.
try:
    import tomlkit as _vtoml                                                      # noqa: PLC0415
    from dmw3launcher import builds as _vb                                        # noqa: PLC0415

    _us, _eu = _vb.BUILDS[0], _vb.BUILDS[1]      # named so a future reorder cannot swap them

    _vwin = _MW(paths.load_config())
    _vwin.resize(1200, 880)
    _vwin.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
    _vwin.show()
    for _ in range(4):
        _app.processEvents()
    _vplay = _vwin.play

    check("Europe" in _vplay.btn_play_eu.text() and "USA" in _vplay.btn_play_us.text(),
          "V1 the Play tab offers one button per regional build",
          f"{_vplay.btn_play_eu.text()!r} / {_vplay.btn_play_us.text()!r}")
    _vsizes = {(b.width(), b.height())
               for b in (_vplay.btn_play_us, _vplay.btn_play_eu, _vplay.btn_stop)}
    check(len(_vsizes) == 1, "V2 the two Play buttons and Stop share one size", str(sorted(_vsizes)))

    # Wider, not just equal: the old fixed 150px clipped "Play (Europe)". The width is derived from
    # the label's own metric at the #primary sheet's font plus the sheet's padding, so a Play label
    # must be COVERED by its button. Europe is the widest label, so covering it proves USA fits too.
    _vpad = 2 * getattr(_vplay, "PRIMARY_PAD_X", 28)
    _veu = _vplay.btn_play_eu
    _vneed = _veu.fontMetrics().horizontalAdvance(_veu.text()) + _vpad
    _vclipped = [(b.text(), b.width(), b.sizeHint().width())
                 for b in (_vplay.btn_play_us, _vplay.btn_play_eu, _vplay.btn_stop)
                 if b.width() < b.sizeHint().width()]
    check(not _vclipped and _veu.width() >= _vneed
          and _vplay.btn_play_us.width() == _veu.width(),
          "V3 each Play label (Europe widest) fits its button with the sheet's own padding",
          f"clipped={_vclipped}; Europe needs {_vneed} of {_veu.width()}px")
    check(_vplay.btn_play_us.x() < _vplay.btn_play_eu.x()
          and _vplay.btn_play is _vplay.btn_play_us,
          "V4 the Play tab lays the USA button before Europe, and btn_play is the USA button",
          f"us.x={_vplay.btn_play_us.x()} eu.x={_vplay.btn_play_eu.x()}")
    check([b.region for b in _vb.BUILDS] == [_vb.REGION_US, _vb.REGION_EU],
          "V5 the build list leads with USA, Europe second (one order for the whole UI)",
          str([b.region for b in _vb.BUILDS]))
    check(_vb.build_dir(_vb.REGION_EU) == _vb.builds_dir() / "EUR"
          and _vb.build_dir(_vb.REGION_US) == _vb.builds_dir() / "USA"
          and _vb.exe_path(_vb.REGION_EU).name == "Digimon_World_2003_Recompiled.exe"
          and _vb.exe_path(_vb.REGION_US).name == "Digimon_World_3_Recompiled.exe",
          "V6 each region resolves its own folder and executable",
          f"{_vb.build_dir(_vb.REGION_EU).name}/{_vb.build_dir(_vb.REGION_US).name} "
          f"{_vb.exe_path(_vb.REGION_EU).name}/{_vb.exe_path(_vb.REGION_US).name}")

    # V7-V9 the disc line is rewritten through tomlkit, on a synthetic file that HAS comments
    _vd = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-builds-"))
    _vt = _vd / "game.toml"
    _vt.write_text('# keep me\n[game]\nname = "x"\ndisc = "old.bin"\n\n'
                   '[runtime]\n# and me\nbios_hle = false\n', encoding="utf-8")
    _vorig_gt = _vb.game_toml
    try:
        _vb.game_toml = lambda region: _vt                            # type: ignore[assignment]
        _vb.set_disc(_vb.REGION_EU, _vd / "new image.bin")
        _vtext = _vt.read_text(encoding="utf-8")
        _vdoc = _vtoml.parse(_vtext)
        check("# keep me" in _vtext and "# and me" in _vtext, "V7 the rewrite keeps every comment")
        _vwritten = pathlib.Path(str(_vdoc["game"]["disc"]))
        check(_vwritten.is_absolute() and _vwritten.name == "new image.bin",
              "V8 the disc line is the absolute path of the chosen image", str(_vdoc["game"]["disc"]))
        check(str(_vdoc["game"]["name"]) == "x" and _vdoc["runtime"]["bios_hle"] is False,
              "V9 every other key survives the rewrite")
    finally:
        _vb.game_toml = _vorig_gt                                     # type: ignore[assignment]
        shutil.rmtree(_vd, ignore_errors=True)

    # V10-V15 the router: serial prefix -> region, on synthetic bytes and the real reader
    _vr = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-router-"))

    def _vbin(prefix: bytes) -> pathlib.Path:
        p = _vr / (prefix.decode() + "bin")
        p.write_bytes(b"\x00" * 64 + prefix + b"\x00" * 64)
        return p

    check(disc.region_of_image(_vbin(b"SCES_123.45")) == disc.REGION_EU
          and disc.region_of_image(_vbin(b"SLES_039.36")) == disc.REGION_EU,
          "V10 SCES*/SLES* images route to PAL/Europe")
    check(disc.region_of_image(_vbin(b"SCUS_999.11")) == disc.REGION_US
          and disc.region_of_image(_vbin(b"SLUS_014.36")) == disc.REGION_US,
          "V11 SCUS*/SLUS* images route to NTSC-U")
    check(disc.region_of_image(_vbin(b"SLPM_65000")) == disc.REGION_JP,
          "V12 an NTSC-J serial does not masquerade as a playable build")
    check(disc.region_of_image(_vbin(b"HOMEBREW")) == disc.REGION_UNKNOWN,
          "V13 an image with no serial is UNKNOWN, not misrouted")
    _vcue = _vr / "t.cue"
    _vcue.write_text('FILE "SCES_123.45bin" BINARY\n', encoding="utf-8")
    check(disc.region_of_image(_vcue) == disc.REGION_EU,
          "V14 a .cue is routed through the .bin it points at")
    shutil.rmtree(_vr, ignore_errors=True)
    check(disc.region_of_serial("SLUS_014.36") == _vb.REGION_US
          and disc.region_of_serial("SLES_039.36") == _vb.REGION_EU
          and disc.region_of_serial("nonsense") == disc.REGION_UNKNOWN,
          "V15 a bare licence code maps to a region, and garbage to UNKNOWN")

    # V16 nothing refuses a USA disc any more
    check("compiled from the European disc only"
          not in (ROOT / "dmw3launcher/disc.py").read_text(encoding="utf-8"),
          "V16 nothing refuses a USA disc any more")

    # V17 the new code carries no absolute path: everything is derived from the launcher folder
    for _vname in ("dmw3launcher/builds.py", "dmw3launcher/builder.py",
                   "dmw3launcher/ui/play_tab.py"):
        _vabs = re.findall(r"[A-Za-z]:[\\/]", (ROOT / _vname).read_text(encoding="utf-8"))
        check(not _vabs, f"V17 {_vname} carries no absolute path literal", str(_vabs))

    # V18-V20 a launch: cwd is the build folder, PSX_DEV_INPUT=1, and the console is teed
    _vcap: dict = {}

    class _VFakePopen:
        def __init__(self, argv, cwd=None, env=None, creationflags=0, close_fds=True,
                     stdout=None, stderr=None, startupinfo=None):
            _vcap.update(argv=argv, cwd=cwd, env=env, stdout=stdout, stderr=stderr)
            self.pid = 0

    _vld = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-launch-"))
    (_vld / "fake.exe").write_bytes(b"MZ")
    _vorig_popen = _vb.subprocess.Popen
    _vorig_exe, _vorig_dir, _vorig_log = _vb.exe_path, _vb.build_dir, _vb.run_log_path
    _vb.subprocess.Popen = _VFakePopen                               # type: ignore[assignment]
    _vb.exe_path = lambda region: _vld / "fake.exe"                  # type: ignore[assignment]
    _vb.build_dir = lambda region: _vld                              # type: ignore[assignment]
    _vb.run_log_path = lambda region: _vld / "logs" / "runtime.log"  # type: ignore[assignment]
    try:
        _vb.launch(_vb.REGION_US, log_path=_vb.run_log_path(_vb.REGION_US))
    finally:
        _vb.subprocess.Popen = _vorig_popen                          # type: ignore[assignment]
        _vb.exe_path, _vb.build_dir, _vb.run_log_path = _vorig_exe, _vorig_dir, _vorig_log
    check(_vcap.get("argv") == [str(_vld / "fake.exe")] and _vcap.get("cwd") == str(_vld),
          "V18 a launch runs the exe with cwd set to its own build folder",
          f"argv={_vcap.get('argv')} cwd={_vcap.get('cwd')}")
    check((_vcap.get("env") or {}).get(_vb.DEV_INPUT_VAR) == "1"
          and _vb.DEV_INPUT_VAR == "PSX_DEV_INPUT",
          "V19 the launch sets PSX_DEV_INPUT=1 (pad merged onto Player 1)")
    check(_vcap.get("stdout") is not None and (_vld / "logs" / "runtime.log").is_file(),
          "V20 the launch tees the runtime's console to a log the tab can show",
          f"stdout={_vcap.get('stdout')}")
    shutil.rmtree(_vld, ignore_errors=True)

    # V21 the honest 'it really booted' signal
    check(_vb.boots_to_bios(["psxrecomp: main() entered",
                             "psxrecomp: loaded game config (SLUS-01436)",
                             "psxrecomp runtime: executing from PC=0xBFC00000"])
          and not _vb.boots_to_bios(["psxrecomp: main() entered"]) and not _vb.boots_to_bios([]),
          "V21 boots_to_bios recognises the BIOS entry line and nothing weaker")
    _vwin.close()
except Exception as _exc:  # noqa: BLE001
    check(False, "V1-V21 native-build checks raised", f"{type(_exc).__name__}: {_exc}")

# --- W. the builder: the pre-flight, and the shipped engine's portability fixes ----------------
# The public Play tab is a BUILDER. The two things that matter most are that it never starts a
# 10-15 minute recompile it cannot finish, and that the framework it ships does not reintroduce the
# five MSVC build blockers a vanilla CLI copy carries.
try:
    from PySide6.QtWidgets import QMessageBox as _QMBW                            # noqa: PLC0415

    from dmw3launcher import builder as _bd                                       # noqa: PLC0415
    from dmw3launcher import builds as _wbuilds                                   # noqa: PLC0415

    _wus, _weu = _wbuilds.BUILDS[0], _wbuilds.BUILDS[1]

    # the report shape and its two states, in the tool's own terms
    _wempty = _bd.ToolChain()
    _wfull = _bd.ToolChain(cmake="cmake", ninja="ninja", compiler="cl", compiler_kind="msvc",
                           compiler_label="MSVC 1")
    check(not _wempty.ok and len(_wempty.missing()) == 3
          and _wempty.summary().startswith("Missing:"),
          "W1 an empty toolchain report names exactly CMake, Ninja and a compiler",
          _wempty.summary())
    check(_wfull.ok and not _wfull.missing() and _wfull.summary().startswith("Ready:"),
          "W2 a full toolchain report is Ready with nothing missing", _wfull.summary())

    # The probe must never pick devkitPro's cygwin cmake over a native one.
    _wtc = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-tc-"))
    (_wtc / "devkitpro").mkdir()
    (_wtc / "native").mkdir()
    _wdk = _wtc / "devkitpro" / "cmake.exe"
    _wnv = _wtc / "native" / "cmake.exe"
    _wdk.write_bytes(b"x")
    _wnv.write_bytes(b"x")
    _wpick = _bd._prefer_native([str(_wdk), str(_wnv)])
    check(_wpick == str(_wnv),
          "W3 the probe prefers a native cmake over devkitPro's cygwin copy (it cannot build MSVC)",
          str(_wpick))
    shutil.rmtree(_wtc, ignore_errors=True)

    _wtc_live = _bd.preflight(refresh=True)
    check(isinstance(_wtc_live, _bd.ToolChain) and _bd.find_cmake() is not None,
          "W4 a CMake is found on this machine for the live pre-flight", _wtc_live.summary())
    check(_bd.engine_present() and _bd.cli_path().is_file() and _bd.bios_path().is_file(),
          "W5 the launcher ships a complete bundled engine (recompiler + OpenBIOS)",
          str(_bd.cli_path()))
    check(_bd.work_root() == paths.launcher_root() / "build",
          "W6 the builder's generated sources live under the launcher's build/ (which git ignores)",
          str(_bd.work_root()))
    check(_bd._is_cli_safe(_bd.cli_root()) and _bd.cli_root().is_dir(),
          "W6b the bundled CLI is handed a short path its shell-built helper commands survive "
          "(no space, no bracket)",
          f"{_bd.cli_root()} (launcher at {paths.launcher_root()})")
    _wprobe = _bd._via(paths.launcher_root())
    check(_wprobe.samefile(paths.launcher_root()),
          "W6c ...and that short path is the SAME directory, not a copy", str(_wprobe))
    check(_bd.expected_exe_name(_wus.title) == _wus.exe_name.rsplit(".", 1)[0]
          and _bd.expected_exe_name(_weu.title) == _weu.exe_name.rsplit(".", 1)[0],
          "W7 the builder predicts the runtime's exe name the way CMake does",
          f"{_bd.expected_exe_name(_wus.title)} / {_bd.expected_exe_name(_weu.title)}")

    # PRE-FLIGHT GATES THE BUILD. With a disc selected but no compiler, Build is disabled and
    # start_build() refuses instead of launching a long job. This is the whole point of the check.
    from dmw3launcher.ui.play_tab import PlayTab as _WTab                        # noqa: PLC0415
    _wsyn = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-build-")) / "USA disc.bin"
    _wsyn.write_bytes(b"\x00" * 64 + b"SLUS_014.36" + b"\x00" * 64)
    _wtab = _WTab(paths.load_config(),
                  toolchain=_bd.ToolChain(cmake="cmake", ninja="ninja"))
    _wtab.setAttribute(Qt.WidgetAttribute.WA_DontShowOnScreen, True)
    _wtab._extra = [_wsyn]
    _wtab._scan_discs()
    _wtab.toolchain = _bd.ToolChain(cmake="cmake", ninja="ninja")
    _wtab._update_toolchain_label()
    _wtab._update_build_button()
    check(_wtab.list_discs.count() == 1 and "NTSC-U" in _wtab.list_discs.item(0).text(),
          "W8 a synthetic USA image is listed and routed as NTSC-U/USA",
          _wtab.list_discs.item(0).text() if _wtab.list_discs.count() else "none")
    check(not _wtab.btn_build.isEnabled(),
          "W9 with a disc selected but no compiler, Build is disabled (pre-flight gates the build)",
          _wtab.lbl_progress.text()[:90])
    _wreal_mb = _QMBW.critical
    _worig_pf = _bd.preflight
    _QMBW.critical = staticmethod(lambda *a, **k: None)      # a modal box would hang offscreen
    _bd.preflight = lambda refresh=False: _bd.ToolChain(cmake="cmake", ninja="ninja")
    try:
        _wtab.start_build()
    finally:
        _bd.preflight = _worig_pf
        _QMBW.critical = _wreal_mb
    check(_wtab._worker is None,
          "W10 start_build refuses (starts no worker) when the toolchain is incomplete")
    check("FAILED" in _wtab.log.toPlainText(),
          "W11 ...and the refusal is stated in the Activity log",
          _wtab.log.toPlainText().splitlines()[-1] if _wtab.log.toPlainText() else "")

    if _bd.preflight().ok and _bd.engine_present():
        _wtab.toolchain = _bd.preflight()
        _wtab._update_toolchain_label()
        _wtab._update_build_button()
        check(_wtab.btn_build.isEnabled(),
              "W12 with a complete toolchain and the engine present, Build is enabled",
              _wtab.lbl_progress.text()[:100])
    else:
        check(True, "W12 skipped: this machine has no complete toolchain")
    _wtab.deleteLater()

    # W13-W19 the shipped framework carries all five MSVC portability fixes (by content, not by
    # trusting that it was copied). Each one was a real build blocker.
    _ef = _bd.framework_dir()
    _sio = (_ef / "runtime/src/sio.c").read_text(encoding="utf-8", errors="replace")
    check("#if PSX_MAX_PLAYERS != 2" in _sio and "#error" in _sio,
          "W13 sio.c (1/5) has the portable initialisers and the player-count guard")
    check(re.search(r"=\s*\{[^}]*\b\d+\s*\.\.\.\s*\d+", _sio) is None,
          "W14 ...and no GCC range designator remains in sio.c")
    _mdec = (_ef / "runtime/src/mdec.c").read_text(encoding="utf-8", errors="replace")
    check("_Alignas(" in _mdec and "__attribute__((aligned" not in _mdec,
          "W15 mdec.c (2/5) uses C11 _Alignas, not the GNU aligned attribute")
    _wlink = (_ef / "runtime/include/psx_c_linkage.h")
    _wmain = (_ef / "runtime/src/main.cpp").read_text(encoding="utf-8", errors="replace")
    check(_wlink.is_file() and 'extern "C"' in _wlink.read_text(encoding="utf-8", errors="replace")
          and "psx_c_linkage.h" in _wmain,
          "W16 psx_c_linkage.h (3/5) declares the C globals and main.cpp includes it")
    _wrt = (_ef / "runtime/runtime.cmake").read_text(encoding="utf-8", errors="replace")
    check(re.search(r"if\s*\(\s*MSVC\s*\)[\s\S]{0,300}?/utf-8", _wrt) is not None,
          "W17 runtime.cmake (4/5) adds /utf-8 under if(MSVC)")
    check((_ef / "runtime/src/main.cpp").read_bytes().startswith(b"\xef\xbb\xbf"),
          "W18 main.cpp (4/5) keeps its UTF-8 BOM at byte 0")
    for _wrel in ("runtime/src/main.cpp", "runtime/src/beetle_main.cpp",
                  "runtime/src/psx_netplay.c"):
        _wtxt = (_ef / _wrel).read_text(encoding="utf-8", errors="replace")
        check("_IONBF" in _wtxt,
              f"W19 {_wrel} (5/5) sets an unbuffered std stream under MSVC (_IONBF)")
    check(re.search(r"#if(?:def)?\s+(?:defined\s*\(\s*)?_MSC_VER[\s\S]{0,400}?_IONBF",
                    _wmain) is not None,
          "W19b ...and in main.cpp the _IONBF choice is guarded by _MSC_VER, not applied blindly")

    # the licenses the tree must ship
    check("PolyForm" in (ROOT / "LICENSE.md").read_text(encoding="utf-8", errors="replace"),
          "W20 the tree root ships the engine's PolyForm Noncommercial license (LICENSE.md)")
    check((ROOT / "THIRD_PARTY_ATTRIBUTION.md").is_file()
          and (ROOT / "THIRD_PARTY_ATTRIBUTION.md").stat().st_size > 0,
          "W21 the tree root ships THIRD_PARTY_ATTRIBUTION.md")
    check((ROOT / "OpenBIOS.LICENSE").is_file()
          and "MIT" in (ROOT / "OpenBIOS.LICENSE").read_text(encoding="utf-8", errors="replace"),
          "W22 the tree root ships the OpenBIOS MIT notice")

    # ...and build() refuses the cheapest mistakes before touching a toolchain at all
    _wres = _bd.build(_wbuilds.REGION_US,
                      pathlib.Path(tempfile.gettempdir()) / "hermes-verify-no-such-disc.bin",
                      lambda s: None)
    check(not _wres.ok and "not found" in _wres.message.lower(),
          "W23 build() refuses a missing disc image before doing anything", _wres.message[:80])

    # W24 the shipped CLI is the CLI's WHOLE package, and keeps the one file its project-root
    # discovery depends on. The engine's config_loader walks up from a profile looking for a
    # .gitignore/.git/CMakeLists.txt marker; the framework carries a .gitignore precisely so a
    # generated project's psxrecomp/ is found as the framework root. Drop it and the BIOS profile
    # resolves its seeds against the game project instead, and the BIOS step dies with
    # "cannot open seed file". That is a functional file, not decoration.
    _eng = _bd.engine_dir()
    _missing_cli = [str(p.relative_to(_eng)) for p in (
        _eng / "psxrecomp.exe", _eng / "libexec" / "psxrecomp-bios.exe",
        _eng / "libexec" / "psxrecomp-game.exe", _eng / "libexec" / "psxrecomp-toml.exe",
        _eng / "framework" / ".gitignore", _eng / "framework" / "bios" / "OpenBIOS.toml",
        _eng / "framework" / "recompiler" / "seeds" / "openbios_elf_seeds.json",
        _eng / "framework" / "runtime" / "runtime.cmake",
        _eng / "share" / "phase2_ghidra_seeds.json")
        if not p.is_file()]
    check(not _missing_cli,
          "W24 the bundled CLI is its complete package (exe, libexec helpers, framework, seeds, "
          "share) - and keeps the framework/.gitignore marker its project-root walk needs",
          str(_missing_cli))

    # W25 a dependency FetchContent leaves inside its archive's top-level folder must be
    # unwrapped, or CMake finds no CMakeLists.txt and the configure dies ("SDL3 3.4+ was not
    # found"). Synthetic tree, real helper.
    _wfd = pathlib.Path(tempfile.mkdtemp(prefix="hermes-verify-deps-"))
    try:
        _src = _wfd / "_deps" / "demo-src" / "Demo-1.0"
        _src.mkdir(parents=True)
        (_src / "CMakeLists.txt").write_text("project(demo)\n", encoding="utf-8")
        (_src / "lib").mkdir()
        _flat = _bd._flatten_fetched_deps(_wfd)
        check(_flat == ["demo-src"]
              and (_wfd / "_deps" / "demo-src" / "CMakeLists.txt").is_file()
              and not (_wfd / "_deps" / "demo-src" / "Demo-1.0").exists()
              and _bd._flatten_fetched_deps(_wfd) == [],
              "W25 a wrapped FetchContent source is unwrapped once, and the second pass is a no-op",
              f"flattened={_flat}")
    finally:
        shutil.rmtree(_wfd, ignore_errors=True)
except Exception as _exc:  # noqa: BLE001
    check(False, "W1-W22 builder checks raised", f"{type(_exc).__name__}: {_exc}")

# --- X. the public tree ships no game code, and the ignore rules say so ------------------------
# The legal guardrail: recompiled output, extracted executables and disc images must live OUTSIDE
# version control. The pattern list is asserted AND, in a real checkout, git is asked directly.
try:
    import subprocess as _subX                                                    # noqa: PLC0415

    _gi = (ROOT / ".gitignore").read_text(encoding="utf-8")
    _want = ("Builds/", "Discs/", "dist/", "build/", ".venv/", "__pycache__/",
             "*.bin", "*.cue", "*.iso", "*.chd")
    _gmiss = [p for p in _want if p not in _gi]
    check(not _gmiss, "X1 .gitignore excludes every game-data and build path it must",
          str(_gmiss))
    check("openbios.bin" in _gi,
          "X2 ...with the one legal BIOS exception spelled out (OpenBIOS is MIT)")

    def _git(*args):
        return _subX.run(["git", "-C", str(ROOT), *args], capture_output=True,
                         text=True, errors="replace")

    if _git("rev-parse", "--is-inside-work-tree").stdout.strip() == "true":
        _gign = [p for p in ("Builds/USA/x.exe", "Discs/a.bin", "build/tmp/gen/a_full.c",
                             "dist/x", "loose.bin", "loose.cue", "loose.iso", "loose.chd",
                             ".venv/x", "__pycache__/x.pyc")
                 if _git("check-ignore", "-q", p).returncode != 0]
        check(not _gign, "X3 git really ignores every build/, Discs/ and generated path",
              str(_gign))

        _gtracked = _git("ls-files").stdout.splitlines()
        _gbad = [f for f in _gtracked
                 if f.startswith(("Builds/", "Discs/", "build/", "dist/"))
                 or f.lower().endswith((".cue", ".iso", ".chd", ".mcr", ".mcd"))
                 or f.lower().endswith(("_full.c", "_dispatch.c"))
                 or (f.lower().endswith(".bin") and "openbios.bin" not in f.lower())]
        check(not _gbad, "X4 no game content is tracked",
              f"{len(_gtracked)} files tracked; offenders={_gbad[:5]}")
        _gbins = [f for f in _gtracked if f.lower().endswith(".bin")]
        check(all("openbios.bin" in f.lower() for f in _gbins),
              "X5 the only tracked *.bin is the shipped OpenBIOS image", str(_gbins))

        _gstatus = _git("status", "--porcelain").stdout.splitlines()
        _gsbad = []
        for _ln in _gstatus:
            _path = _ln[3:].strip().strip('"')
            if (_path.startswith(("Builds/", "Discs/", "build/", "dist/"))
                    or _path.lower().endswith((".cue", ".iso", ".chd"))
                    or (_path.lower().endswith(".bin") and "openbios.bin" not in _path.lower())):
                _gsbad.append(_ln)
        check(not _gsbad, "X6 nothing game-derived is tracked or staged in the working tree",
              f"{len(_gstatus)} status lines; offenders={_gsbad[:5]}")
    else:
        check(True, "X3-X6 skipped: the tree is not a git checkout")
except Exception as _exc:  # noqa: BLE001
    check(False, "X1-X6 git-hygiene checks raised", f"{type(_exc).__name__}: {_exc}")

failed = [lbl for ok, lbl in results if not ok]
print("\n" + "=" * 60)
print(f"{len(results) - len(failed)}/{len(results)} checks passed")
if failed:
    print("FAILED: " + "; ".join(failed))
sys.exit(1 if failed else 0)
