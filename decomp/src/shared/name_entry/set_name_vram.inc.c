/* Sets where in VRAM the keyboard's images go */
void OVL_NAME(setNameVram)(NameEntry *task, s32 x, s32 y) {
    task->vramX = x;
    task->vramY = y;
}
