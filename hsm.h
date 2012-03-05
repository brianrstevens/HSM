#ifndef HSM_H_INCLUDED
#define HSM_H_INCLUDED

#define HSM_MAX_LEVEL 5

typedef struct HSM HSM;
typedef struct HSM_state HSM_state;
typedef struct HSM_event HSM_event;
typedef char HSM_signal;
typedef int (*HSM_handler)(HSM *me, HSM_event *e);

enum HSM_RESERVED_SIGS {
    HSM_ENTRY,
    HSM_EXIT,
    HSM_SIGNALS
    };

void HSM_dispatch(HSM *me, HSM_event *e);
void HSM_tran(HSM *me, HSM_state *target, HSM_event *e);

struct HSM {
    struct HSM *pointer;
    struct HSM_state *state;
    };
struct HSM_state {
    struct HSM_state *pointer;
    HSM_handler handler;
    struct HSM_state *map[HSM_MAX_LEVEL];
    int level;
    };
struct HSM_event {
    HSM_signal signal;
    };

#endif // HSM_H_INCLUDED
