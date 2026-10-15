/* The card grid's (CARD_GRID) hiding, once grid->hide starts it: takes a
   card off every 2 frames, then kills the grid when none are left */
void OVL_NAME(updateHiding)(CARD_GRID *grid) {
    switch (grid->substate) {
    case 0:
        break;
    case 1:
        if (grid->shown != 0) {
            grid->shown--;
            grid->nextSubstate(grid);
            grid->counter = GFX.funcs.getTime();
        } else {
            grid->state = TASK_KILL;
        }
        break;
    case 2:
        if (GFX.funcs.getTime() - grid->counter >= 2) {
            grid->substate = 1;
        }
        break;
    }
}
