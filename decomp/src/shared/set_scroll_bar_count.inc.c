/* Sets how many items the list has, and how many it shows at a time */
void OVL_NAME(setScrollBarCount)(ScrollBar *bar, s32 pageSize, s32 count) {
    bar->pageSize = pageSize;
    bar->count = count;
    bar->hasCount = 1;
}
