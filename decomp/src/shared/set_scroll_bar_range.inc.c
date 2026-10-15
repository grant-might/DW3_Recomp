/* Sets the y range the scroll bar's thumb moves in */
void OVL_NAME(setScrollBarRange)(ScrollBar *bar, s32 top, s32 bottom) {
    bar->top = top;
    bar->bottom = bottom;
    bar->hasRange = 1;
}
