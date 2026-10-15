/* Creates the screen fade, idle until started, on SCREEN_LAYER at
   the including overlay's FADER_DEPTH */
ScreenFade *OVL_NAME(createFader)(void) {
    ScreenFade *task = createTask(OVL_NAME(updateFader), sizeof(ScreenFade), 0);

    task->start = OVL_NAME(startFader);
    task->layerId = SCREEN_LAYER;
    task->depth = FADER_DEPTH;
    return task;
}
