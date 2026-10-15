/* Makes the card grid (CARD_GRID) take its cards off one by one
   (grid->hide, OVL_NAME(updateHiding)) */
void OVL_NAME(hideCards)(CARD_GRID *grid) {
    grid->setSubstate(grid, 1);
}
