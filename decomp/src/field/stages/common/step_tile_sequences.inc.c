/* Plays a sequence of StageTileFrame animations on tile */
void stepTileSequences(StageTile *tile, StageTileFrame **seqs, StageTileCursor *cur, s32 depth) {
    s32 dt;

    if (depth == 0) {
        dt = GFX.funcs.getFrameTime();
        if (dt > 4) {
            dt = 4;
        }
        cur->timer -= dt;
    }
    if (cur->timer <= 0) {
        if (seqs[cur->seq][cur->index].last) {
            cur->seq++;
            cur->index = 0;
            if (seqs[cur->seq] == NULL) {
                cur->seq = 0;
            }
        } else {
            cur->index++;
        }
        cur->timer += seqs[cur->seq][cur->index].duration;
        stepTileSequences(tile, seqs, cur, depth + 1);
    }
    if (depth == 0) {
        tile->frame = seqs[cur->seq][cur->index].frame;
        tile->clutRow = seqs[cur->seq][cur->index].clutRow;
    }
}
