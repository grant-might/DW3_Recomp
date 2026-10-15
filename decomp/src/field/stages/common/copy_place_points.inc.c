/* Copies the points of PLACE (with PLACEARG) in LIST to SLOTS, one each */
void copyPlacePoints(StageSlot *slots, StagePoints **list, s32 place, s32 placeArg) {
    StagePoints *entry;
    StagePoint *point;

    for (;;) {
        entry = *list;
        if (entry == NULL) {
            return;
        }
        if (entry->place == place && entry->placeArg == placeArg) {
            break;
        }
        list++;
    }
    point = entry->points;
    slots->arg = point->arg;
    slots->x = point->x;
    slots->y = point->y;
    slots->dir = point->dir;
    slots->place = point->place;
    slots->placeArg = point->placeArg;
    while (point->next != NULL) {
        point = point->next;
        slots++;
        slots->arg = point->arg;
        slots->x = point->x;
        slots->y = point->y;
        slots->dir = point->dir;
        slots->place = point->place;
        slots->placeArg = point->placeArg;
    }
}
