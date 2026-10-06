"""Art and palette.

The launcher must look finished BEFORE the custom art exists, and must pick the art up the
moment it is dropped into assets/. Every slot therefore falls back to a generated look, and
the asset manifest in docs/ASSETS.md names each file and its exact size.

The MEMORY CARD tab embeds the save editor, whose 5 themes are applied to the whole QApplication.
Adopting one therefore restyles the launcher too, deliberately: one theme, one app.
`use_editor_theme()` maps the editor's semantic tokens onto the launcher's and notifies every
registered widget, so nothing keeps a stale colour. Every token below is read at call time, never
captured at import, which is what makes a live swap possible.
"""
from __future__ import annotations

import pathlib
from typing import Callable

from PySide6.QtCore import QPointF, QRectF, Qt
from PySide6.QtGui import (QColor, QFont, QIcon, QImage, QLinearGradient, QPainter, QPen, QPixmap,
                           QPolygonF)
from PySide6.QtWidgets import QComboBox, QFrame, QSizePolicy, QWidget

from .. import paths

# Design-space size for full-bleed art. Everything is scaled from this, so a replacement
# image can be authored at exactly this size and nothing drifts.
BASE_W, BASE_H = 1280, 800

# Palette (used when no art is present, and for widget chrome that sits over art).
BG = "#12141c"
BG_ALT = "#191d28"
PANEL = "#1e2230"
PANEL_HI = "#262b3c"
BORDER = "#333a52"
TEXT = "#e8ecf7"
TEXT_DIM = "#9aa3bd"
ACCENT = "#f2a33c"        # DW3's orange
ACCENT_HOVER = "#ffb44f"
ACCENT_DIM = "#a86d1f"
ON_ACCENT = "#171a22"
OK = "#57c98b"
WARN = "#e2b93b"
ERR = "#e2595f"

_TOKENS = ("BG", "BG_ALT", "PANEL", "PANEL_HI", "BORDER", "TEXT", "TEXT_DIM", "ACCENT",
           "ACCENT_HOVER", "ACCENT_DIM", "ON_ACCENT", "OK", "WARN", "ERR")
_DEFAULTS = {t: globals()[t] for t in _TOKENS}

# One spacing scale for every page, so no page invents its own rhythm. Every margin and gap in
# the UI comes from these four numbers.
PAGE_MARGIN = 20      # page edge -> content
CARD_GAP = 12         # between cards
CARD_PAD = 16         # inside a card
ROW_GAP = 8           # between controls sitting on one row

_LISTENERS: list[Callable[[], None]] = []


def on_change(fn: Callable[[], None]) -> None:
    """Register a callback fired whenever the palette is swapped."""
    if fn not in _LISTENERS:
        _LISTENERS.append(fn)


def _notify() -> None:
    for fn in list(_LISTENERS):
        try:
            fn()
        except RuntimeError:          # the widget behind it was destroyed
            _LISTENERS.remove(fn)


def use_editor_theme(pal: dict | None) -> str:
    """Adopt the save editor's active palette. None restores the launcher's own defaults.

    The editor's themes carry more tokens than the launcher needs, so this is a mapping rather
    than a copy; `BORDER_STRONG` becomes our border because the editor's hairline `BORDER` is
    nearly invisible against its own surfaces, where the launcher draws real frames. Returns the
    theme's label, for status text.
    """
    if not pal:
        globals().update(_DEFAULTS)
        _notify()
        return "Launcher default"
    mapping = {
        "BG": pal.get("BG"),
        "BG_ALT": pal.get("SIDEBAR"),
        "PANEL": pal.get("SURFACE"),
        "PANEL_HI": pal.get("SURFACE_2"),
        "BORDER": pal.get("BORDER_STRONG"),
        "TEXT": pal.get("TEXT"),
        "TEXT_DIM": pal.get("TEXT_MUTED"),
        "ACCENT": pal.get("ACCENT"),
        "ACCENT_HOVER": pal.get("ACCENT_HOVER"),
        "ACCENT_DIM": pal.get("ACCENT_DIM"),
        "ON_ACCENT": pal.get("ON_ACCENT"),
        "OK": pal.get("OK"),
        "WARN": pal.get("WARN"),
        "ERR": pal.get("DANGER"),
    }
    globals().update({k: v for k, v in mapping.items() if v})
    _notify()
    return str(pal.get("label", "theme"))


def _maybe(name: str) -> pathlib.Path | None:
    p = paths.assets_dir() / name
    return p if p.is_file() else None


def background() -> QPixmap | None:
    p = _maybe("background.png")
    if not p:
        return None
    pm = QPixmap(str(p))
    return pm if not pm.isNull() else None


def logo() -> QPixmap | None:
    for n in ("logo.png", "logo.webp"):
        p = _maybe(n)
        if p:
            pm = QPixmap(str(p))
            if not pm.isNull():
                return pm
    return None


# Height the tab art is drawn at. The user's art is the tab NAME as a word-image (320x80), not a
# square glyph, so the bar scales it by height and the aspect follows from the source.
TAB_ART_H = 40
MIN_ART_CONTRAST = 3.0


def _luma(c: QColor) -> float:
    def lin(v: float) -> float:
        v /= 255.0
        return v / 12.92 if v <= 0.03928 else ((v + 0.055) / 1.055) ** 2.4
    return 0.2126 * lin(c.red()) + 0.7152 * lin(c.green()) + 0.0722 * lin(c.blue())


def _ink_contrast(pm: QPixmap) -> float:
    """WCAG contrast between the art's ink and the page background it will sit on."""
    img = pm.toImage()
    steps_x = max(1, img.width() // 40)
    steps_y = max(1, img.height() // 40)
    total, n = 0.0, 0
    for y in range(0, img.height(), steps_y):
        for x in range(0, img.width(), steps_x):
            c = img.pixelColor(x, y)
            if c.alpha() > 32:                      # ignore the transparent padding
                total += _luma(c)
                n += 1
    if not n:
        return 21.0
    ink, bg = total / n, _luma(QColor(BG))
    hi, lo = max(ink, bg), min(ink, bg)
    return (hi + 0.05) / (lo + 0.05)


DARK_INK = 0.25   # below this luminance a pixel counts as the black word-text


def _coloured_columns(img: QImage, chroma: int = 25) -> frozenset[int]:
    """Columns containing colour: where an icon lives, and where ink must never be repainted."""
    cols = set()
    for x in range(img.width()):
        for y in range(img.height()):
            c = img.pixelColor(x, y)
            if c.alpha() > 8 and (max(c.red(), c.green(), c.blue())
                                  - min(c.red(), c.green(), c.blue())) > chroma:
                cols.add(x)
                break
    return frozenset(cols)


def _lift_dark_ink(pm: QPixmap, colour: str) -> QPixmap:
    """Repaint the near-black ink, leaving colour alone and never touching an icon.

    Only used for art with no pre-baked white copy (see tab_images). The tab art separates its icon
    from its word with a transparent gap, and the icon's own outline and centre are dark too - which
    is why a blanket recolour flattened the MODS icon. Columns holding colour are treated as the
    icon's columns and skipped entirely.
    """
    tint = QColor(colour)
    img = pm.toImage().convertToFormat(QImage.Format.Format_ARGB32)
    icon_cols = _coloured_columns(img)
    for y in range(img.height()):
        for x in range(img.width()):
            if x in icon_cols:
                continue
            c = img.pixelColor(x, y)
            a = c.alpha()
            if a and _luma(c) < DARK_INK:
                img.setPixelColor(x, y, QColor(tint.red(), tint.green(), tint.blue(), a))
    return QPixmap.fromImage(img)


def tab_images(height: int = TAB_ART_H) -> dict[str, QPixmap]:
    """The four tab images, scaled to one art height and made readable as a set.

    The supplied art draws its word in near-black, which is invisible on this launcher's dark
    themes. `make_white_variants.py` bakes a `tab_<tab>_white.png` in which the word is already
    white and the icon is untouched; that copy is preferred whenever the chrome is dark (i.e. the
    palette's text colour is light), so nothing is repainted at run time. Without a baked copy the
    art as supplied is used and the ink-lift is the safety net. On a light theme the original
    black-word art is the correct one and is used as-is.
    """
    dark_chrome = _luma(QColor(TEXT)) > 0.5
    out: dict[str, QPixmap] = {}
    for tab in ("play", "memcard", "mods", "decomp", "settings"):
        baked = _maybe(f"tab_{tab}_white.png") if dark_chrome else None
        p = baked or _maybe(f"tab_{tab}.png")
        if not p:
            continue
        pm = QPixmap(str(p))
        if pm.isNull() or pm.height() == 0:
            continue
        width = max(1, round(pm.width() * height / pm.height()))
        scaled = pm.scaled(width, height, Qt.AspectRatioMode.IgnoreAspectRatio,
                           Qt.TransformationMode.SmoothTransformation)
        out[tab] = scaled if baked else _lift_dark_ink(scaled, TEXT)
    return out


def tab_image(tab: str, height: int = TAB_ART_H) -> QPixmap | None:
    """One tab's art, made readable exactly as the rest of its row is."""
    return tab_images(height).get(tab)


def app_icon() -> QIcon:
    for n in ("icon.ico", "icon.png"):
        p = _maybe(n)
        if p:
            i = QIcon(str(p))
            if not i.isNull():
                return i
    return QIcon(generated_icon())


def generated_icon() -> QPixmap:
    """Fallback icon so the taskbar is never a blank square."""
    pm = QPixmap(256, 256)
    pm.fill(Qt.GlobalColor.transparent)
    p = QPainter(pm)
    p.setRenderHint(QPainter.RenderHint.Antialiasing)
    g = QLinearGradient(0, 0, 256, 256)
    g.setColorAt(0.0, QColor(ACCENT))
    g.setColorAt(1.0, QColor(ACCENT_DIM))
    p.setBrush(g)
    p.setPen(Qt.PenStyle.NoPen)
    p.drawRoundedRect(8, 8, 240, 240, 40, 40)
    p.setPen(QColor(ON_ACCENT))
    f = QFont()
    f.setPointSize(96)
    f.setBold(True)
    p.setFont(f)
    p.drawText(pm.rect(), Qt.AlignmentFlag.AlignCenter, "DW3")
    p.end()
    return pm


def play_banner() -> QPixmap | None:
    p = _maybe("play_banner.png")
    if not p:
        return None
    pm = QPixmap(str(p))
    return pm if not pm.isNull() else None


def header_bar() -> QPixmap | None:
    p = _maybe("header_bar.png")
    if not p:
        return None
    pm = QPixmap(str(p))
    return pm if not pm.isNull() else None


def code_palette() -> dict[str, str]:
    """Colours for the decomp viewer, by the ROLE a token plays rather than by its spelling.

    An IDE's syntax colours are roles - keyword, type, function, string, number, comment - and this
    maps those roles onto the live palette, so the viewer reads like an editor instead of like the
    launcher's chrome while still following a theme swap. Read at call time: a palette change
    re-highlights rather than leaving stale colours behind.
    """
    return {
        "keyword": ACCENT,          # control flow and declaration keywords
        "type": OK,                 # s32, u8, VECTOR: the "type" colour
        "func": TEXT,               # a name followed by a call, and plain identifiers
        "string": ACCENT_DIM,       # warm but dimmer than a keyword
        "number": WARN,             # hex and decimal literals
        "comment": TEXT_DIM,        # dim, italic
        "preproc": ACCENT_HOVER,    # #include and friends
        "gutter_bg": PANEL,         # the gutter shares the editor's own surface (no inset box)
        "gutter_fg": TEXT_DIM,
        "gutter_active": TEXT,      # the current line's number
        "current_line": PANEL_HI,   # the current line's band across the editor
    }


class ArtFrame(QFrame):
    """A frame whose background is an asset, or a flat colour when the asset is absent.

    Keeping the fallback here (rather than in each caller) is what lets every asset slot be
    optional without any caller branching on it. With no explicit `fill` the frame tracks the
    live palette, so a theme swap recolours it instead of leaving a stale band.
    """

    def __init__(self, asset: QPixmap | None, fill: str | None = None, parent=None) -> None:
        super().__init__(parent)
        self._pm = asset
        self._fill_token = fill

    def set_art(self, asset: QPixmap | None) -> None:
        self._pm = asset
        self.update()

    def set_fill(self, fill: str | None) -> None:
        self._fill_token = fill
        self.update()

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        p = QPainter(self)
        r = self.rect()
        if self._pm is None or self._pm.isNull():
            p.fillRect(r, QColor(self._fill_token or BG_ALT))
            p.end()
            return
        scaled = self._pm.scaled(r.size(), Qt.AspectRatioMode.KeepAspectRatioByExpanding,
                                 Qt.TransformationMode.SmoothTransformation)
        p.drawPixmap(int((r.width() - scaled.width()) // 2),
                     int((r.height() - scaled.height()) // 2), scaled)
        p.end()


class ComboBox(QComboBox):
    """A dropdown that paints its own frame, value and caret.

    Qt draws a solid block as the arrow indicator for a stylesheet-styled QComboBox and will not let
    a stylesheet suppress it: `image: none` is ignored, and an `image:` is drawn *in addition to* the
    block (measured: a solid 8x5 bar sitting on top of the chevron, hiding all but its corners).
    Overriding paintEvent and not calling the base implementation is the only way to get one clean
    caret, so this draws the frame, the value and the caret from the same palette tokens the
    stylesheet uses for the other controls.
    """

    RADIUS, PAD_X, PAD_RIGHT = 6, 10, 26
    CARET_W, CARET_H, MARGIN, PEN_W = 9, 6, 13, 1.4

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        p = QPainter(self)
        p.setRenderHint(QPainter.RenderHint.Antialiasing, True)
        rect = self.rect()
        border = ACCENT if self.hasFocus() else BORDER
        p.setPen(QPen(QColor(border), 1.0))
        p.setBrush(QColor(PANEL_HI))
        p.drawRoundedRect(QRectF(rect).adjusted(0.5, 0.5, -0.5, -0.5), self.RADIUS, self.RADIUS)

        p.setPen(QColor(TEXT if self.isEnabled() else TEXT_DIM))
        p.drawText(rect.adjusted(self.PAD_X, 0, -self.PAD_RIGHT, 0),
                   int(Qt.AlignmentFlag.AlignVCenter | Qt.AlignmentFlag.AlignLeft),
                   self.currentText())

        pen = QPen(QColor(TEXT_DIM), self.PEN_W)
        pen.setCapStyle(Qt.PenCapStyle.RoundCap)
        pen.setJoinStyle(Qt.PenJoinStyle.RoundJoin)
        p.setPen(pen)
        cx = rect.right() - self.MARGIN
        cy = rect.center().y() + 1
        half_w, half_h = self.CARET_W / 2, self.CARET_H / 2
        p.drawPolyline(QPolygonF([QPointF(cx - half_w, cy - half_h),
                                  QPointF(cx, cy + half_h),
                                  QPointF(cx + half_w, cy - half_h)]))
        p.end()


class Banner(QWidget):
    """Optional hero art. Takes no space at all when the asset is missing, so the layout is
    unchanged for players who never drop one in."""

    def __init__(self, asset: QPixmap | None, max_height: int = 220, parent=None) -> None:
        super().__init__(parent)
        self._pm = asset
        self._max_h = max_height
        self.setVisible(asset is not None and not asset.isNull())
        if self.isVisible():
            self.setMinimumHeight(min(max_height, asset.height()))
            self.setMaximumHeight(max_height)
            self.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)

    def paintEvent(self, event) -> None:  # noqa: N802 (Qt naming)
        if self._pm is None or self._pm.isNull():
            return
        p = QPainter(self)
        r = self.rect()
        scaled = self._pm.scaled(r.size(), Qt.AspectRatioMode.KeepAspectRatio,
                                 Qt.TransformationMode.SmoothTransformation)
        p.drawPixmap(int((r.width() - scaled.width()) // 2),
                     int((r.height() - scaled.height()) // 2), scaled)
        p.end()


_EDITOR_ACTIVE = False


def set_editor_active(flag: bool) -> None:
    """Record whether the embedded save editor is driving the app-wide theme."""
    global _EDITOR_ACTIVE
    _EDITOR_ACTIVE = bool(flag)


def editor_theme_active() -> bool:
    return _EDITOR_ACTIVE


def tabbar_stylesheet() -> str:
    """Only the selectors safe to put on an ancestor of the embedded editor.

    When the editor drives the app-wide theme the launcher's full sheet cannot sit on the window:
    Qt merges rule sets and matches by specificity, so rules the editor's own sheet does not fully
    specify leak into the editor and move its widgets (its QGroupBox sets no `padding`, its QWidget
    rule deliberately paints no background). The tab bar is the one piece of chrome that cannot be
    styled per-page, and the editor contains no QTabWidget/QTabBar, so these rules are safe there.
    """
    return f"""
    QTabWidget::pane {{ border: none; background: transparent; top: -1px; }}
    QTabBar::tab {{ background: transparent; color: {TEXT_DIM}; padding: 10px 0;
                    border: none; border-bottom: 2px solid transparent;
                    margin-right: 22px; }}
    QTabBar::tab:selected {{ color: {TEXT}; border-bottom: 2px solid {ACCENT}; }}
    QTabBar::tab:hover {{ color: {TEXT}; }}
    """


def stylesheet() -> str:
    # One surface tone for everything that holds text. The page is BG; every container that sits
    # on it - a card, a group box, a list, a tree, a text area, an input's inset - is the SAME
    # token, PANEL. Earlier the insets used BG_ALT (a darker blue-grey) and the base QWidget rule
    # painted BG behind every label, so a panel full of text read as a box full of smaller, darker
    # boxes. The enclosing surface is what its children now match, so the panel looks seamless;
    # each control keeps its 1px BORDER, which is where the remaining structure lives.
    return tabbar_stylesheet() + f"""
    QWidget {{ background: transparent; color: {TEXT};
               font-family: 'Segoe UI', 'Noto Sans', sans-serif; font-size: 13px; }}
    QPushButton {{ background: {PANEL_HI}; color: {TEXT}; border: 1px solid {BORDER};
                   border-radius: 8px; padding: 7px 14px; }}
    QPushButton:hover {{ border-color: {ACCENT}; }}
    QPushButton:disabled {{ color: {TEXT_DIM}; border-color: {BORDER}; }}
    QPushButton#primary {{ background: {ACCENT}; color: {ON_ACCENT}; border: none;
                           font-weight: 600; padding: 12px 28px; font-size: 15px; }}
    QPushButton#primary:hover {{ background: {ACCENT_HOVER}; }}
    QPushButton#danger {{ background: {ERR}; color: {ON_ACCENT}; border: none;
                          font-weight: 600; }}
    QListWidget, QTableWidget, QPlainTextEdit, QTextEdit {{
        background: {PANEL}; border: 1px solid {BORDER}; border-radius: 8px; padding: 4px; }}
    QListWidget::item {{ padding: 6px 8px; border-radius: 6px; }}
    QListWidget::item:selected {{ background: {ACCENT_DIM}; color: {TEXT}; }}
    QGroupBox {{ background: {PANEL}; border: 1px solid {BORDER}; border-radius: 10px;
                 margin-top: 12px; }}
    QGroupBox::title {{ subcontrol-origin: margin; left: 12px; padding: 0 6px;
                        color: {TEXT_DIM}; font-weight: 600; }}
    QCheckBox, QRadioButton {{ spacing: 8px; }}
    QCheckBox::indicator {{ width: 15px; height: 15px; border: 1px solid {BORDER};
                            border-radius: 4px; background: {PANEL_HI}; }}
    QCheckBox::indicator:hover {{ border-color: {ACCENT}; }}
    QCheckBox::indicator:checked {{ background: {ACCENT}; border-color: {ACCENT}; }}
    QSpinBox, QLineEdit {{ background: {PANEL_HI}; border: 1px solid {BORDER};
                           border-radius: 6px; padding: 6px 10px; min-height: 18px; }}
    QLabel#h1 {{ font-size: 22px; font-weight: 700; }}
    QLabel#pageTitle {{ font-size: 16px; font-weight: 600; }}
    QLabel#section {{ color: {TEXT_DIM}; font-size: 11px; font-weight: 600; }}
    QLabel#hint {{ color: {TEXT_DIM}; font-size: 12px; }}
    QLabel#dim {{ color: {TEXT_DIM}; }}
    QLabel#warn {{ color: {WARN}; }}
    QLabel#err {{ color: {ERR}; }}
    QLabel#ok {{ color: {OK}; }}
    QScrollArea {{ border: none; }}
    /* Scoped by object name to the Decompilation tab's own widgets, so the embedded save editor can
       never be touched by them even if this sheet reaches an ancestor of it. */
    QFrame#card {{ background: {PANEL}; border: 1px solid {BORDER}; border-radius: 10px; }}
    QLabel#bigPct {{ font-size: 24px; font-weight: 700; }}
    QPushButton#segment {{ background: {PANEL_HI}; border: 1px solid {BORDER}; padding: 6px 16px; }}
    QPushButton#segment:checked {{ background: {ACCENT_DIM}; border-color: {ACCENT}; }}
    QTreeWidget#fileTree {{ background: {PANEL}; border: 1px solid {BORDER}; border-radius: 8px;
                            outline: none; }}
    QTreeWidget#fileTree::item {{ padding: 4px 6px; }}
    QTreeWidget#fileTree::item:selected {{ background: {ACCENT_DIM}; color: {TEXT}; }}
    QTreeWidget#fileTree QHeaderView::section {{ background: {PANEL}; color: {TEXT_DIM};
                                                 border: none; border-bottom: 1px solid {BORDER};
                                                 padding: 5px 6px; font-size: 11px;
                                                 font-weight: 600; }}
    QPlainTextEdit#codeView {{ background: {PANEL}; border: 1px solid {BORDER}; border-radius: 8px;
                               padding: 6px 8px; font-family: 'Cascadia Mono', Consolas, monospace;
                               font-size: 13px; selection-background-color: {ACCENT_DIM};
                               selection-color: {TEXT}; }}
    QListWidget#searchResults {{ background: {PANEL}; border: 1px solid {BORDER};
                                 border-radius: 8px; }}
    /* The editor pane's chrome: a file tab strip, a breadcrumb, and a status line, the three
       pieces an IDE's editor area is made of. */
    QTabBar#fileTabs {{ background: transparent; qproperty-drawBase: 0; }}
    QTabBar#fileTabs::tab {{ background: {PANEL}; color: {TEXT_DIM}; border: 1px solid {BORDER};
                             border-bottom: none; border-top-left-radius: 6px;
                             border-top-right-radius: 6px; padding: 4px 26px 4px 10px;
                             margin-right: 2px; }}
    QTabBar#fileTabs::tab:selected {{ background: {PANEL_HI}; color: {TEXT};
                                     border-color: {ACCENT}; }}
    QTabBar#fileTabs::tab:hover {{ color: {TEXT}; }}
    QLabel#breadcrumb {{ color: {TEXT_DIM}; font-size: 12px; }}
    QLabel#editorStatus {{ color: {TEXT_DIM}; font-size: 11px; }}
    QLabel#emptyState {{ color: {TEXT_DIM}; font-size: 13px; }}
    """
