/* Writes the name typed, without its leading and trailing spaces, back in
   half-width characters */
void OVL_NAME(getName)(NameEntry *task, char *out) {
    TextTools conv;
    s32 i;

    for (i = 0; i < task->maxLength * 2; i++) {
        out[i] = 0;
    }
    for (i = task->maxLength - 1; i >= 0 && task->name[i] == SJIS_SPACE; i--) {
        task->name[i] = 0;
    }
    for (i = 0; i < task->maxLength && task->name[i] == SJIS_SPACE; i++) {
    }
    initTextTools(&conv);
    conv.convert(out, &task->name[i], 1);
}
