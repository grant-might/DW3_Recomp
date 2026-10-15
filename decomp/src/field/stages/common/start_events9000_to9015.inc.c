/* Starts events 9000 to 9015 (updateEvent9015), with StagePartyChildren */
void *startEvents9000To9015(void) {
    return createTask(updateEvent9015, sizeof(StageTask), sizeof(StagePartyChildren));
}
