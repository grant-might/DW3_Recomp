/* Shows the name entry's windows with their texts, or hides them */
void OVL_NAME(showNameWindows)(NameEntry *task, NameEntryWindows *windows, s32 show) {
    s32 i;

    if (show != 0) {
        windows->title->setString(windows->title, FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)), 1);
        windows->name->setText(windows->name, task->name);
        windows->name->setPalette(windows->name, PALETTE_BLUE);
        for (i = 0; i < 3; i++) {
            windows->tabs[i]->setString(windows->tabs[i], FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)),
                                        OVL_NAME(keyboard).tabTexts[task->page].texts[i]);
            windows->tabs[i]->setPalette(windows->tabs[i], PALETTE_BLUE);
        }
        windows->leftLabel->setString(windows->leftLabel, FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)), 0xD);
        windows->leftLabel->setPalette(windows->leftLabel, PALETTE_BLUE);
        windows->rightLabel->setString(windows->rightLabel, FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)), 0xE);
        windows->rightLabel->setPalette(windows->rightLabel, PALETTE_BLUE);
        if (OVL_NAME(keyboard).pageCount >= 2) {
            windows->l1Label->setString(windows->l1Label, FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)), 0x10);
            windows->l1Label->setPalette(windows->l1Label, PALETTE_BLUE);
            windows->r1Label->setString(windows->r1Label, FILE_CACHE.load(TEXT_FILE(TEXT_NAME_ENTRY)), 0x11);
            windows->r1Label->setPalette(windows->r1Label, PALETTE_BLUE);
        }
    } else {
        windows->title->setVisible(windows->title, 0);
        windows->name->setVisible(windows->name, 0);
        for (i = 0; i < 3; i++) {
            windows->tabs[i]->setVisible(windows->tabs[i], 0);
        }
        windows->leftLabel->setVisible(windows->leftLabel, 0);
        windows->rightLabel->setVisible(windows->rightLabel, 0);
        windows->l1Label->setVisible(windows->l1Label, 0);
        windows->r1Label->setVisible(windows->r1Label, 0);
    }
}
