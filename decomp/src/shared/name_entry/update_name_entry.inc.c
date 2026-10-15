/* The name entry's task: loads the keyboard's images and picks its pages,
   then runs and draws the keyboard */
void OVL_NAME(updateNameEntry)(NameEntry *task, NameEntryWindows *windows) {
    TimLoader loader;

    switch (task->state) {
    case TASK_INIT:
    default:
        task->nextState(task);
        initTimLoader(&loader);
        loader.setImagePos(task->vramX, task->vramY);
        loader.loadArchive(FILE_CACHE.getEntry(NAME_ENTRY_FILE_KEYBOARD << 16));
        if (NAME_ENTRY_JAPANESE) {
            OVL_NAME(keyboard).pageCount = 3;
            OVL_NAME(keyboard).tabTexts = OVL_NAME(keyPagesJp);
            OVL_NAME(keyboard).pages = OVL_NAME(keyCharsJp);
        } else {
            OVL_NAME(keyboard).pageCount = 1;
            OVL_NAME(keyboard).tabTexts = OVL_NAME(keyPages);
            OVL_NAME(keyboard).pages = OVL_NAME(keyChars);
        }
        task->unusedBC.duration = 10;
        task->messageScale.duration = 10;
        task->keyboardScale.duration = 10;
        OVL_NAME(createNameWindows)(task, windows);
        break;
    case TASK_RUN:
        OVL_NAME(updateKeyboard)(task, windows);
        OVL_NAME(drawKeyboard)(task);
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}
