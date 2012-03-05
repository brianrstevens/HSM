#include "hsm.h"

void HSM_dispatch(HSM *me, HSM_event *e) {
    int nothandled = 1;
    int depth = me->state->level;
    do {
        nothandled = (me->state->map[depth]->handler)(me, e);
        depth = depth - 1;
        } while (nothandled);
    }
void HSM_tran(HSM *me, HSM_state *target, HSM_event *e) {
    if (me->state == target) {
        return;
        }
    int depth = 0;
    int sharedparent = 0;
    while (me->state->map[depth] == target->map[depth]) {
        sharedparent = depth;
        depth = depth + 1;
        }
    e->signal = HSM_EXIT;
    depth = me->state->level;
    while (depth > sharedparent) {
        (me->state->map[depth]->handler)(me, e);
        depth = depth -1;
        }
    e->signal = HSM_ENTRY;
    depth = sharedparent + 1;
    while (depth <= target->level) {
        (target->map[depth]->handler)(me, e);
        depth = depth + 1;
        }
    me->state = target;
    }
