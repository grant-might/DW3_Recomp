/* Creates the name entry's windows: the title, the name, the keyboard's three
   tabs, the button labels and the message */
void OVL_NAME(createNameWindows)(NameEntry *task, NameEntryWindows *windows) {
    s32 i;

    windows->title = createTextWindow(task->layer, 1, 0x20, 0x1A);
    windows->title->setPalette(windows->title, PALETTE_GREEN);
    windows->name = createTextWindow(task->layer, 1, 0x4B, 0x40);
    windows->name->setSpacing(windows->name, 0x13, 0);
    windows->name->style = &OVL_NAME(nameStyle);
    for (i = 0; i < 3; i++) {
        windows->tabs[i] = createTextWindow(task->layer, 1, 0x2F + i * 0x4E, 0x5B);
        windows->tabs[i]->setDepth(windows->tabs[i], task->depth - 1);
        windows->tabs[i]->setLines(windows->tabs[i], 7);
        windows->tabs[i]->setSpacing(windows->tabs[i], 0xE, 0x12);
        windows->tabs[i]->style = &OVL_NAME(nameStyle);
    }
    windows->leftLabel = createTextWindow(task->layer, 1, 0xCE, 0xC6);
    windows->rightLabel = createTextWindow(task->layer, 1, 0xE1, 0xC6);
    windows->l1Label = createTextWindow(task->layer, 1, 0x13, 0x62);
    windows->l1Label->setDepth(windows->l1Label, task->depth - 1);
    windows->r1Label = createTextWindow(task->layer, 1, 0x123, 0x62);
    windows->r1Label->setDepth(windows->r1Label, task->depth - 1);
    windows->message = createTextWindow(task->layer, 1, 0x3E, 0x72);
}
