/* Sets the name to start from, converted to full-width characters and padded
   with spaces */
void OVL_NAME(setName)(NameEntry *task, char *name) {
    TextTools conv;
    s32 i;

    initTextTools(&conv);
    conv.convert(task->name, name, 0);
    for (i = strlen((char *)task->name) >> 1; i < task->maxLength; i++) {
        task->name[i] = SJIS_SPACE;
    }
}
