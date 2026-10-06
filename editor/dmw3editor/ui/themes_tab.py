"""Theme picker page, switch the whole app's palette live.

Each theme is a selectable card: name, a one-line description, and three
swatches (main background, surface card, accent). Clicking a card applies the
theme to the running QApplication immediately (theme.apply) and remembers it
via QSettings so the next launch opens in the same theme.
"""

from __future__ import annotations

from PySide6.QtCore import QSettings, Qt
from PySide6.QtGui import QColor, QPainter, QPixmap
from PySide6.QtWidgets import (
    QApplication,
    QFrame,
    QHBoxLayout,
    QLabel,
    QVBoxLayout,
    QWidget,
)

from dmw3editor.ui import theme


def _swatch(color: str, size: int = 26, radius: int = 6) -> QPixmap:
    pm = QPixmap(size, size)
    pm.fill(Qt.transparent)
    c = QColor(color) if not color.startswith("rgba") else QColor(
        *[int(x) for x in color[5:-1].split(",")[:3]]
    )
    p = QPainter(pm)
    p.setRenderHint(QPainter.Antialiasing)
    p.setBrush(c)
    p.setPen(Qt.NoPen)
    p.drawRoundedRect(0, 0, size, size, radius, radius)
    p.end()
    return pm


class ThemeCard(QFrame):
    """One clickable theme preview card."""

    def __init__(self, key: str, on_pick) -> None:
        super().__init__()
        self.key = key
        self.setProperty("role", "theme")
        self.setCursor(Qt.PointingHandCursor)

        pal = theme.PALETTES[key]
        row = QHBoxLayout(self)
        row.setContentsMargins(14, 12, 14, 12)
        row.setSpacing(12)

        # three swatches: main bg, surface, accent
        sw = QHBoxLayout()
        sw.setSpacing(5)
        for color, tip in (
            (pal["BG"], "Main background"),
            (pal["SURFACE"], "Card surface"),
            (pal["ACCENT"], "Accent"),
        ):
            lbl = QLabel()
            lbl.setPixmap(_swatch(color))
            lbl.setToolTip(tip)
            sw.addWidget(lbl)
        sw.addStretch(1)
        row.addLayout(sw)

        col = QVBoxLayout()
        col.setSpacing(2)
        name = QLabel(pal["label"])
        name.setProperty("role", "title")
        col.addWidget(name)
        tag = QLabel(pal["tag"])
        tag.setProperty("role", "hint")
        col.addWidget(tag)
        row.addLayout(col, 1)

        self.active_lbl = QLabel("")
        self.active_lbl.setProperty("role", "hint")
        row.addWidget(self.active_lbl)

        self._on_pick = on_pick

    def mouseReleaseEvent(self, ev) -> None:  # noqa: N802
        if ev.button() == Qt.LeftButton:
            self._on_pick(self.key)
        super().mouseReleaseEvent(ev)

    def set_active(self, is_active: bool) -> None:
        self.setProperty("selected", "true" if is_active else "false")
        self.active_lbl.setText("✓ In use" if is_active else "Click to use")
        # force style re-evaluation of the dynamic property
        self.style().unpolish(self)
        self.style().polish(self)
        self.update()


class ThemesTab(QWidget):
    """Sidebar page listing every available palette."""

    def __init__(self) -> None:
        super().__init__()
        self._cards: dict[str, ThemeCard] = {}

        root = QVBoxLayout(self)
        root.setContentsMargins(20, 16, 20, 16)
        root.setSpacing(12)

        note = QLabel(
            "Pick a color theme, it applies across the whole editor "
            "immediately and is remembered for next launch."
        )
        note.setWordWrap(True)
        note.setProperty("role", "hint")
        root.addWidget(note)

        # grid of theme cards, two per row
        from PySide6.QtWidgets import QGridLayout

        grid = QGridLayout()
        grid.setSpacing(10)
        for i, key in enumerate(theme.theme_names()):
            card = ThemeCard(key, self._pick)
            self._cards[key] = card
            grid.addWidget(card, i // 2, i % 2)
        root.addLayout(grid)

        self._status = QLabel("")
        self._status.setProperty("role", "hint")
        root.addWidget(self._status)
        root.addStretch(1)

    # -- public -----------------------------------------------------------
    def bind(self, save) -> None:
        """No save needed for theming; kept for the uniform page API."""
        return

    def refresh(self) -> None:
        current = theme.active_theme()
        for key, card in self._cards.items():
            card.set_active(key == current)

    # -- internal ---------------------------------------------------------
    def _pick(self, key: str) -> None:
        app = QApplication.instance()
        if app is None:
            return
        theme.apply(app, key)
        self.refresh()
        self._status.setText(f"Theme set to {theme.theme_label(key)}")
        # remember across launches
        settings = QSettings("DMW3SaveEditor", "DMW3SaveEditor")
        settings.setValue("theme", key)


def saved_theme() -> str:
    """Theme key stored by the picker, or the default."""
    settings = QSettings("DMW3SaveEditor", "DMW3SaveEditor")
    saved = settings.value("theme", theme.DEFAULT_THEME)
    if saved not in theme.PALETTES:
        return theme.DEFAULT_THEME
    return str(saved)
