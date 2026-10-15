/* The mode's entry point (MODE_ENTRY_POINTS): starts its root task
   (update_scene.inc.c) */
Task *OVL_NAME(start)(void) {
    return createTask(OVL_NAME(updateScene), sizeof(SCENE_TASK), sizeof(SCENE_CHILD *));
}
