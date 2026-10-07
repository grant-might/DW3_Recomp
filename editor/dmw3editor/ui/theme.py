"""ShadCN-inspired multi-theme system for the DMW3 save editor.

Each theme is a named palette. ``apply(app, name)`` installs Fusion, the
matching QPalette, and a stylesheet built from that palette. Widgets opt into
variants via Qt dynamic properties (``role="primary"``, ``role="nav"`` ...),
styled by the selectors below — identical rules for every theme, colors swap.

Layers: background < surface < elevated; text: foreground / muted / faint.
Borders are hairline; radius is 6-10px; focus is an accent ring.
"""

from __future__ import annotations

import pathlib
import sys

from PySide6.QtGui import QColor, QPalette
from PySide6.QtWidgets import QApplication

from dmw3editor.paths import writable_dir

# --------------------------------------------------------------------------
# Theme palettes
#
# Every theme defines the same semantic tokens so the stylesheet stays one
# template. "dark" only drives the QPalette highlight/disabled choices that
# differ between light and dark surfaces.
# --------------------------------------------------------------------------

PALETTES: dict[str, dict] = {
    # --- default: Kumamon's hat blue main + Agumon yellow accent -----------
    "digimon-blue": {
        "label": "Kumamon",
        "tag": "Kumamon blue + Agumon yellow",
        "dark": True,
        # base surfaces
        "BG": "#0f2040",
        "SIDEBAR": "#12264e",
        "SURFACE": "#172e5e",
        "SURFACE_2": "#203a73",
        "BORDER": "#2a4a90",
        "BORDER_STRONG": "#3560b3",
        # text
        "TEXT": "#eaf1ff",
        "TEXT_MUTED": "#a9bde7",
        "TEXT_FAINT": "#7287b8",
        # accent (Agumon yellow)
        "ACCENT": "#ffc533",
        "ACCENT_HOVER": "#ffd968",
        "ACCENT_PRESSED": "#dfa51a",
        "ACCENT_DIM": "#a97d0d",
        "ON_ACCENT": "#241a00",
        # selection
        "SELECT_BG": "rgba(255, 197, 51, 0.16)",
        "SELECT_TX": "#ffdf8a",
        # semantic
        "WARN": "#e6b450",
        "DANGER": "#ef5350",
        "OK": "#4ade80",
        # component extras
        "HOVER_BG": "#244183",
        "HOVER_BORDER": "#4066bd",
        "INPUT_HOVER_BORDER": "#4a70c6",
        "ALT_BG": "#142a55",
        "ROW_HOVER": "rgba(255, 255, 255, 0.05)",
        "HEADER_BG": "#0d1c3a",
        "NAV_HOVER": "#1b3566",
        "TRACK_BG": "#27457f",
        "TRACK_HOVER": "#31538f",
        "TOOLTIP_BG": "#182f5e",
    },
    # --- black main + Guilmon red accent -----------------------------------
    "guilmon-black": {
        "label": "Guilmon",
        "tag": "Black + Guilmon red",
        "dark": True,
        "BG": "#0d0d10",
        "SIDEBAR": "#121218",
        "SURFACE": "#17171d",
        "SURFACE_2": "#1f1f27",
        "BORDER": "#27272f",
        "BORDER_STRONG": "#363640",
        "TEXT": "#f0f0f3",
        "TEXT_MUTED": "#9b9ba6",
        "TEXT_FAINT": "#65656f",
        "ACCENT": "#e0382e",
        "ACCENT_HOVER": "#f04e42",
        "ACCENT_PRESSED": "#bb2b21",
        "ACCENT_DIM": "#8a1f18",
        "ON_ACCENT": "#ffffff",
        "SELECT_BG": "rgba(224, 56, 46, 0.18)",
        "SELECT_TX": "#ff9a91",
        "WARN": "#e6b450",
        "DANGER": "#ef5350",
        "OK": "#4ade80",
        "HOVER_BG": "#26262e",
        "HOVER_BORDER": "#41414d",
        "INPUT_HOVER_BORDER": "#484854",
        "ALT_BG": "#14141a",
        "ROW_HOVER": "rgba(255, 255, 255, 0.05)",
        "HEADER_BG": "#0a0a0d",
        "NAV_HOVER": "#1e1e26",
        "TRACK_BG": "#2b2b35",
        "TRACK_HOVER": "#383844",
        "TOOLTIP_BG": "#1c1c23",
    },
    # --- Terriermon green: pale shell main + green accent ------------------
    # The key stays `eggshell-green`: a saved theme that no longer matches a key falls back to the
    # default, so renaming the key would quietly forget the player's choice.
    "eggshell-green": {
        "label": "Terriermon",
        "tag": "Terriermon green",
        "dark": False,
        "BG": "#f4f0e6",
        "SIDEBAR": "#eae4d4",
        "SURFACE": "#fbf8f0",
        "SURFACE_2": "#ede7d6",
        "BORDER": "#d8d0ba",
        "BORDER_STRONG": "#bdb394",
        "TEXT": "#26241c",
        "TEXT_MUTED": "#5f5947",
        "TEXT_FAINT": "#93896f",
        "ACCENT": "#74a93f",
        "ACCENT_HOVER": "#87bd54",
        "ACCENT_PRESSED": "#5c8c30",
        "ACCENT_DIM": "#466f24",
        "ON_ACCENT": "#ffffff",
        "SELECT_BG": "rgba(116, 169, 63, 0.16)",
        "SELECT_TX": "#4a7428",
        "WARN": "#a87c1e",
        "DANGER": "#c0392b",
        "OK": "#2e8b57",
        "HOVER_BG": "#e6dfca",
        "HOVER_BORDER": "#c4b995",
        "INPUT_HOVER_BORDER": "#c4b995",
        "ALT_BG": "#efeadd",
        "ROW_HOVER": "rgba(0, 0, 0, 0.04)",
        "HEADER_BG": "#e2dbc6",
        "NAV_HOVER": "#e9e2cf",
        "TRACK_BG": "#ddd5bd",
        "TRACK_HOVER": "#cec4a6",
        "TOOLTIP_BG": "#fbf8f0",
    },
    # --- Beelzemon indigo main + pearl white accent ------------------------
    "beelzemon-purple": {
        "label": "Beelzemon",
        "tag": "Indigo + pearl accent",
        "dark": True,
        "BG": "#201742",
        "SIDEBAR": "#261b52",
        "SURFACE": "#2e2160",
        "SURFACE_2": "#3b2c79",
        "BORDER": "#4b3898",
        "BORDER_STRONG": "#5d48b8",
        "TEXT": "#f0ecfc",
        "TEXT_MUTED": "#b6a9df",
        "TEXT_FAINT": "#8373b2",
        "ACCENT": "#f5f0ff",
        "ACCENT_HOVER": "#ffffff",
        "ACCENT_PRESSED": "#ddd2f3",
        "ACCENT_DIM": "#a99cd6",
        "ON_ACCENT": "#241a45",
        "SELECT_BG": "rgba(245, 240, 255, 0.14)",
        "SELECT_TX": "#ffffff",
        "WARN": "#e6b450",
        "DANGER": "#ef5350",
        "OK": "#4ade80",
        "HOVER_BG": "#3a2a75",
        "HOVER_BORDER": "#6a52c6",
        "INPUT_HOVER_BORDER": "#6a52c6",
        "ALT_BG": "#251b4a",
        "ROW_HOVER": "rgba(255, 255, 255, 0.06)",
        "HEADER_BG": "#1a1138",
        "NAV_HOVER": "#2f2361",
        "TRACK_BG": "#4a3a8f",
        "TRACK_HOVER": "#5a47a6",
        "TOOLTIP_BG": "#2e2160",
    },
    # --- Gargomon green main + semi-transparent gold accent ----------------
    "gargomon-green": {
        "label": "Gargomon",
        "tag": "Green + gold accent",
        "dark": True,
        "BG": "#17371f",
        "SIDEBAR": "#1b4026",
        "SURFACE": "#224a2e",
        "SURFACE_2": "#2c5c3a",
        "BORDER": "#366e47",
        "BORDER_STRONG": "#46865a",
        "TEXT": "#edf6ef",
        "TEXT_MUTED": "#a6caaf",
        "TEXT_FAINT": "#76a083",
        "ACCENT": "#e2bd55",
        "ACCENT_HOVER": "#eccb6e",
        "ACCENT_PRESSED": "#c9a53f",
        "ACCENT_DIM": "#9c7f2c",
        "ON_ACCENT": "#241d00",
        "SELECT_BG": "rgba(226, 189, 85, 0.18)",
        "SELECT_TX": "#f3dd9a",
        "WARN": "#e6b450",
        "DANGER": "#ef5350",
        "OK": "#4ade80",
        "HOVER_BG": "#2a5636",
        "HOVER_BORDER": "#57976b",
        "INPUT_HOVER_BORDER": "#57976b",
        "ALT_BG": "#1a3d23",
        "ROW_HOVER": "rgba(255, 255, 255, 0.05)",
        "HEADER_BG": "#122c18",
        "NAV_HOVER": "#1f4729",
        "TRACK_BG": "#366344",
        "TRACK_HOVER": "#427754",
        "TOOLTIP_BG": "#244d30",
    },
}

DEFAULT_THEME = "digimon-blue"
_ACTIVE = DEFAULT_THEME


def theme_names() -> list[str]:
    """Palette keys in display order."""
    return list(PALETTES.keys())


def theme_label(key: str) -> str:
    return PALETTES.get(key, {}).get("label", key)


def active_theme() -> str:
    return _ACTIVE


def _css(p: dict) -> str:
    return f"""
* {{
    font-family: "Segoe UI", "Inter", "Noto Sans", sans-serif;
    font-size: 13px;
}}
/* Base text color for every widget. We intentionally do NOT paint a
   background here: painting BG on all widgets would draw darker boxes behind
   every label/container sitting on a card/header surface. Page background
   comes from QPalette.Window (BG); cards/headers/inputs paint their own
   surfaces below; plain labels/containers stay transparent and inherit. */
QWidget {{
    color: {p["TEXT"]};
}}
/* Leaf text widgets must NOT paint a background either: a QLabel/QCheckBox
   stretched across a grid/form cell would otherwise draw a box behind the
   text. Role rules below (warning/danger) still override via specificity. */
QLabel, QCheckBox, QRadioButton, QToolButton {{
    background: transparent;
}}

/* ---------- surfaces ---------- */
QFrame[role="header"] {{
    background: {p["SIDEBAR"]};
    border-bottom: 1px solid {p["BORDER"]};
}}
QFrame[role="card"] {{
    background: {p["SURFACE"]};
    border: 1px solid {p["BORDER"]};
    border-radius: 10px;
}}
QFrame[role="panel"] {{
    background: {p["SURFACE"]};
    border: 1px solid {p["BORDER"]};
    border-radius: 10px;
}}
QLabel[role="title"] {{
    font-size: 17px;
    font-weight: 700;
    letter-spacing: 0.2px;
    color: {p["TEXT"]};
}}
QLabel[role="subtitle"] {{
    color: {p["TEXT_MUTED"]};
    font-size: 12px;
}}
QLabel[role="caption"] {{
    color: {p["TEXT_FAINT"]};
    font-size: 11px;
}}
QLabel[role="hint"] {{
    color: {p["TEXT_MUTED"]};
    font-size: 12px;
}}
QLabel[role="section"] {{
    color: {p["TEXT_FAINT"]};
    font-size: 11px;
    font-weight: 600;
    letter-spacing: 0.9px;
    text-transform: uppercase;
}}
QLabel[role="warning"] {{
    color: {p["WARN"]};
    background: rgba(230, 180, 80, 0.08);
    border: 1px solid rgba(230, 180, 80, 0.28);
    border-radius: 8px;
    padding: 9px 12px;
}}
QLabel[role="danger"] {{
    color: {p["DANGER"]};
    background: rgba(239, 83, 80, 0.08);
    border: 1px solid rgba(239, 83, 80, 0.28);
    border-radius: 8px;
    padding: 9px 12px;
}}

/* ---------- buttons ---------- */
QPushButton {{
    background: {p["SURFACE_2"]};
    border: 1px solid {p["BORDER_STRONG"]};
    border-radius: 7px;
    padding: 0 16px;
    min-height: 32px;
    color: {p["TEXT"]};
}}
QPushButton:hover {{ background: {p["HOVER_BG"]}; border-color: {p["HOVER_BORDER"]}; }}
QPushButton:pressed {{ background: {p["BG"]}; }}
QPushButton:disabled {{ color: {p["TEXT_FAINT"]}; border-color: {p["BORDER"]}; background: {p["SURFACE"]}; }}

QPushButton[role="primary"] {{
    background: {p["ACCENT"]};
    border: 1px solid {p["ACCENT"]};
    color: {p["ON_ACCENT"]};
    font-weight: 600;
}}
QPushButton[role="primary"]:hover {{ background: {p["ACCENT_HOVER"]}; border-color: {p["ACCENT_HOVER"]}; }}
QPushButton[role="primary"]:pressed {{ background: {p["ACCENT_PRESSED"]}; }}
QPushButton[role="primary"]:disabled {{
    background: {p["SURFACE_2"]};
    border-color: {p["BORDER"]};
    color: {p["TEXT_FAINT"]};
}}

QPushButton[role="ghost"] {{
    background: transparent;
    border: 1px solid transparent;
    color: {p["TEXT_MUTED"]};
    padding: 0 10px;
    min-height: 28px;
}}
QPushButton[role="ghost"]:hover {{ color: {p["TEXT"]}; background: {p["SURFACE_2"]}; border-color: {p["BORDER"]}; }}

QPushButton[role="danger"] {{
    background: transparent;
    border: 1px solid rgba(239, 83, 80, 0.45);
    color: {p["DANGER"]};
}}
QPushButton[role="danger"]:hover {{ background: rgba(239, 83, 80, 0.12); border-color: {p["DANGER"]}; }}

QPushButton[role="success"] {{
    background: transparent;
    border: 1px solid rgba(74, 222, 128, 0.4);
    color: {p["OK"]};
}}
QPushButton[role="success"]:hover {{ background: rgba(74, 222, 128, 0.1); border-color: {p["OK"]}; }}

/* sidebar nav items */
QPushButton[role="nav"] {{
    background: transparent;
    border: none;
    border-radius: 6px;
    text-align: left;
    padding: 0 10px;
    min-height: 30px;
    color: {p["TEXT_MUTED"]};
    font-size: 13px;
}}
QPushButton[role="nav"]:hover {{ background: {p["NAV_HOVER"]}; color: {p["TEXT"]}; }}
QPushButton[role="nav"]:checked {{
    background: {p["SELECT_BG"]};
    color: {p["SELECT_TX"]};
    font-weight: 600;
}}
QPushButton[role="nav"]:disabled {{ color: {p["TEXT_FAINT"]}; }}

/* sprite picker tiles (8 rookie partners) */
QToolButton[role="sprite"] {{
    background: transparent;
    border: 1px solid transparent;
    border-radius: 8px;
    color: {p["TEXT_MUTED"]};
    font-size: 12px;
    padding: 4px 2px 3px 2px;
    min-width: 78px;
}}
QToolButton[role="sprite"]:hover {{
    background: {p["SURFACE_2"]};
    border-color: {p["BORDER"]};
    color: {p["TEXT"]};
}}
QToolButton[role="sprite"]:checked {{
    background: {p["SELECT_BG"]};
    border-color: {p["ACCENT_DIM"]};
    color: {p["SELECT_TX"]};
    font-weight: 600;
}}
QToolButton[role="sprite"]:disabled {{ color: {p["TEXT_FAINT"]}; }}

/* theme preview card (Themes page) */
QFrame[role="theme"] {{
    background: {p["SURFACE"]};
    border: 1px solid {p["BORDER"]};
    border-radius: 10px;
}}
QFrame[role="theme"]:hover {{ border-color: {p["HOVER_BORDER"]}; }}
QFrame[role="theme"][selected="true"] {{
    border: 2px solid {p["ACCENT"]};
}}

/* ---------- inputs ---------- */
QComboBox, QSpinBox, QLineEdit, QDoubleSpinBox {{
    background: {p["SURFACE_2"]};
    border: 1px solid {p["BORDER_STRONG"]};
    border-radius: 6px;
    padding: 0 10px;
    selection-background-color: {p["ACCENT_DIM"]};
    min-height: 32px;
    max-height: 32px;
}}
QComboBox:hover, QSpinBox:hover, QLineEdit:hover {{
    border-color: {p["INPUT_HOVER_BORDER"]};
}}
QComboBox:focus, QSpinBox:focus, QLineEdit:focus {{
    border-color: {p["ACCENT"]};
}}
QComboBox::drop-down {{ border: none; width: 22px; }}
QComboBox::down-arrow {{
    image: none;
    border-left: 4px solid transparent;
    border-right: 4px solid transparent;
    border-top: 5px solid {p["TEXT_MUTED"]};
    margin-right: 6px;
}}
QComboBox QAbstractItemView {{
    background: {p["SURFACE_2"]};
    border: 1px solid {p["BORDER_STRONG"]};
    border-radius: 6px;
    selection-background-color: {p["ACCENT_DIM"]};
    selection-color: white;
    outline: none;
    padding: 3px;
}}
QSpinBox::up-button, QSpinBox::down-button {{ width: 0; border: none; }}

/* ---------- tables ---------- */
QTableWidget, QTableView {{
    background: {p["SURFACE"]};
    alternate-background-color: {p["ALT_BG"]};
    border: 1px solid {p["BORDER"]};
    border-radius: 8px;
    gridline-color: transparent;
    selection-background-color: {p["SELECT_BG"]};
    selection-color: {p["TEXT"]};
}}
QTableWidget::item, QTableView::item {{
    padding: 4px 8px;
    border: none;
}}
QTableWidget::item:hover, QTableView::item:hover {{
    background: {p["ROW_HOVER"]};
}}
QHeaderView::section {{
    background: {p["HEADER_BG"]};
    color: {p["TEXT_MUTED"]};
    border: none;
    border-bottom: 1px solid {p["BORDER_STRONG"]};
    padding: 8px 10px;
    font-size: 11px;
    font-weight: 600;
}}
QTableCornerButton::section {{ background: {p["HEADER_BG"]}; border: none; }}

/* ---------- groups / cards ---------- */
QGroupBox {{
    background: {p["SURFACE"]};
    border: 1px solid {p["BORDER"]};
    border-radius: 10px;
    margin-top: 12px;
    font-weight: 600;
}}
QGroupBox::title {{
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 12px;
    padding: 1px 6px;
    color: {p["TEXT_MUTED"]};
    font-weight: 600;
    font-size: 12px;
}}

/* ---------- text areas ---------- */
QPlainTextEdit {{
    background: {p["SURFACE"]};
    border: 1px solid {p["BORDER"]};
    border-radius: 8px;
    padding: 8px;
    selection-background-color: {p["ACCENT_DIM"]};
    color: {p["TEXT"]};
}}
QPlainTextEdit:focus {{ border-color: {p["ACCENT"]}; }}

/* ---------- switch (developer mode etc.) ---------- */
QCheckBox {{ color: {p["TEXT_MUTED"]}; spacing: 8px; }}
QCheckBox:hover {{ color: {p["TEXT"]}; }}
QCheckBox::indicator {{
    width: 34px; height: 18px; border-radius: 9px;
    background: {p["TRACK_BG"]}; border: 1px solid {p["BORDER_STRONG"]};
}}
QCheckBox::indicator:hover {{ background: {p["TRACK_HOVER"]}; }}
QCheckBox::indicator:checked {{
    background: {p["ACCENT_DIM"]}; border-color: {p["ACCENT_DIM"]};
}}
QCheckBox::indicator:checked:hover {{ background: {p["ACCENT"]}; }}

/* compact tick checkboxes (table cells: Earned column etc.) — transparent
   background so no "placeholder box" shows behind the indicator */
QCheckBox[role="tick"] {{
    background: transparent;
    spacing: 4px;
}}
QCheckBox[role="tick"]::indicator {{
    width: 18px;
    height: 18px;
    border-radius: 5px;
    background: {p["SURFACE_2"]};
    border: 1px solid {p["BORDER_STRONG"]};
}}
QCheckBox[role="tick"]:hover {{ background: transparent; }}
QCheckBox[role="tick"]::indicator:hover {{
    background: {p["TRACK_HOVER"]};
    border-color: {p["HOVER_BORDER"]};
}}
QCheckBox[role="tick"]::indicator:checked {{
    background: {p["ACCENT"]};
    border-color: {p["ACCENT"]};
    image: url(__TICK_URL__);
}}
QCheckBox[role="tick"]::indicator:checked:hover {{
    background: {p["ACCENT_HOVER"]};
    border-color: {p["ACCENT_HOVER"]};
    image: url(__TICK_URL__);
}}
QCheckBox[role="tick"]::indicator:disabled {{
    background: {p["SURFACE"]};
    border-color: {p["BORDER"]};
}}
QCheckBox[role="tick"]::indicator:disabled:checked {{
    background: {p["ACCENT_DIM"]};
    border-color: {p["ACCENT_DIM"]};
    image: url(__TICK_URL__);
}}

/* ---------- menus / status ---------- */
QMenuBar {{ background: {p["SIDEBAR"]}; color: {p["TEXT"]}; }}
QMenuBar::item {{ background: transparent; padding: 4px 10px; }}
QMenuBar::item:selected {{ background: {p["SURFACE_2"]}; color: {p["TEXT"]}; }}
QMenu {{
    background: {p["SURFACE_2"]};
    color: {p["TEXT"]};
    border: 1px solid {p["BORDER_STRONG"]};
    border-radius: 8px;
    padding: 5px;
}}
QMenu::item {{ padding: 5px 24px 5px 12px; border-radius: 5px; }}
QMenu::item:selected {{ background: {p["SELECT_BG"]}; color: {p["SELECT_TX"]}; }}
QMenu::separator {{ height: 1px; background: {p["BORDER_STRONG"]}; margin: 4px 8px; }}

QStatusBar {{
    background: {p["SIDEBAR"]};
    color: {p["TEXT_MUTED"]};
    border-top: 1px solid {p["BORDER"]};
}}
QStatusBar::item {{ border: none; }}

/* ---------- scrollbars ---------- */
QScrollBar:vertical {{ background: transparent; width: 10px; margin: 0; }}
QScrollBar::handle:vertical {{
    background: {p["BORDER_STRONG"]};
    border-radius: 5px;
    min-height: 28px;
}}
QScrollBar::handle:vertical:hover {{ background: {p["ACCENT_DIM"]}; }}
QScrollBar::add-line, QScrollBar::sub-line {{ height: 0; width: 0; }}
QScrollBar:horizontal {{ background: transparent; height: 10px; }}
QScrollBar::handle:horizontal {{
    background: {p["BORDER_STRONG"]};
    border-radius: 5px;
    min-width: 28px;
}}
QScrollBar::handle:horizontal:hover {{ background: {p["ACCENT_DIM"]}; }}

QToolTip {{
    background: {p["TOOLTIP_BG"]};
    color: {p["TEXT"]};
    border: 1px solid {p["BORDER_STRONG"]};
    padding: 5px 8px;
    border-radius: 5px;
}}
"""


def _check_icon_path(color_hex: str) -> str:
    """Cache a small tick PNG (ON_ACCENT-colored) under assets/misc/ and
    return its absolute path for a stylesheet url(). Qt stylesheets will not
    draw a native check glyph once ::indicator is styled, so we supply our own.
    """
    if getattr(sys, "frozen", False):
        assets_dir = writable_dir("assets/misc")
    else:
        assets_dir = pathlib.Path(__file__).resolve().parent.parent / "assets" / "misc"
    assets_dir.mkdir(parents=True, exist_ok=True)
    safe = color_hex.lstrip("#").lower()
    path = assets_dir / f"tick_{safe}.png"
    if path.exists():
        return str(path).replace("\\", "/")

    from PySide6.QtCore import QPointF, Qt
    from PySide6.QtGui import QColor, QPainter, QPen, QPixmap

    pm = QPixmap(18, 18)
    pm.fill(Qt.transparent)
    p = QPainter(pm)
    p.setRenderHint(QPainter.Antialiasing)
    pen = QPen(QColor(color_hex))
    pen.setWidthF(2.6)
    pen.setCapStyle(Qt.RoundCap)
    pen.setJoinStyle(Qt.RoundJoin)
    p.setPen(pen)
    p.drawPolyline(
        [
            QPointF(3.5, 9.5),
            QPointF(7.2, 13.0),
            QPointF(14.5, 5.0),
        ]
    )
    p.end()
    pm.save(str(path))
    return str(path).replace("\\", "/")


def apply(app: QApplication, name: str | None = None) -> None:
    """Apply a named theme (default: the current active theme)."""
    global _ACTIVE
    if name is None:
        name = _ACTIVE
    if name not in PALETTES:
        name = DEFAULT_THEME
    _ACTIVE = name
    p = PALETTES[name]
    dark = p["dark"]

    app.setStyle("Fusion")

    pal = QPalette()
    pal.setColor(QPalette.Window, QColor(p["BG"]))
    pal.setColor(QPalette.WindowText, QColor(p["TEXT"]))
    pal.setColor(QPalette.Base, QColor(p["SURFACE_2"]))
    pal.setColor(QPalette.AlternateBase, QColor(p["ALT_BG"]))
    pal.setColor(QPalette.Text, QColor(p["TEXT"]))
    pal.setColor(QPalette.Button, QColor(p["SURFACE_2"]))
    pal.setColor(QPalette.ButtonText, QColor(p["TEXT"]))
    pal.setColor(QPalette.Highlight, QColor(p["ACCENT_DIM"]))
    # Pearl-accent theme has a light lavender ACCENT_DIM -> dark highlighted text.
    hl_text = "#ffffff"
    if name == "beelzemon-purple":
        hl_text = p["ON_ACCENT"]
    pal.setColor(QPalette.HighlightedText, QColor(hl_text))
    pal.setColor(QPalette.ToolTipBase, QColor(p["TOOLTIP_BG"]))
    pal.setColor(QPalette.ToolTipText, QColor(p["TEXT"]))
    pal.setColor(QPalette.PlaceholderText, QColor(p["TEXT_FAINT"]))
    pal.setColor(QPalette.Disabled, QPalette.Text, QColor(p["TEXT_FAINT"]))
    pal.setColor(QPalette.Disabled, QPalette.ButtonText, QColor(p["TEXT_FAINT"]))
    pal.setColor(QPalette.Link, QColor(p["ACCENT"]))
    app.setPalette(pal)

    css = _css(p).replace("__TICK_URL__", _check_icon_path(p["ON_ACCENT"]))
    app.setStyleSheet(css)
