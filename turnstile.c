#include <stdio.h>
#include <stdlib.h>

#include "hsm.h"
#include "turnstile.h"

struct HSM_Turnstile {
    HSM base;
    int passengercount;

    struct HSM_TurnstileState_off {
        HSM_state base;
        } off;

    struct HSM_TurnstileState_on {
        HSM_state base;

        struct HSM_TurnstileState_locked {
            HSM_state base;
            } locked;

        struct HSM_TurnstileSate_unlocked {
            HSM_state base;
            } unlocked;

        } on;
    };

void HSM_Turnstile_dispatch(HSM_Turnstile *me, HSM_Turnstile_event event) {
    HSM_Turnstile_event * e = &event;
    HSM_dispatch((HSM *) me, (HSM_event *) e);
    }

int HSM_Turnstile_off(HSM *me, HSM_event *e) {
    HSM_Turnstile * self = (HSM_Turnstile *) me;
    HSM_Turnstile_event * event = (HSM_Turnstile_event *) e;
    char signal = event->base.signal;
    switch (signal) {
        case HSM_ENTRY:
            printf("OFF\n");
            return 0;
        case HSM_EXIT:
            printf("TURNING ON\n");
            return 0;
        case HSM_TS_ON:
            HSM_tran(me, self->on.locked.base.pointer, e);
            return 0;
        default:
            printf("STILL OFF\n");
            return 0;
        }
    }
int HSM_Turnstile_on(HSM *me, HSM_event *e) {
    HSM_Turnstile * self = (HSM_Turnstile *) me;
    HSM_Turnstile_event * event = (HSM_Turnstile_event *) e;
    char signal = event->base.signal;
    switch (signal) {
        case HSM_ENTRY:
            printf("ON\n");
            return 0;
        case HSM_EXIT:
            printf("%d PASSENGER(S)\n", self->passengercount);
            printf("TURNING OFF\n");
            return 0;
        case HSM_TS_OFF:
            HSM_tran(me, self->off.base.pointer, e);
            return 0;
        case HSM_TS_ON:
            printf("STILL ON\n");
            return 0;
        default:
            printf("TYPE BETTER\n");
            return 0;
        }
    }
int HSM_Turnstile_locked(HSM *me, HSM_event *e) {
    HSM_Turnstile * self = (HSM_Turnstile *) me;
    HSM_Turnstile_event * event = (HSM_Turnstile_event *) e;
    char signal = event->base.signal;
    switch (signal) {
        case HSM_ENTRY:
            printf("LOCKED\n");
            return 0;
        case HSM_EXIT:
            return 0;
        case HSM_TS_PAY:
            printf("YOU PAID\n");
            HSM_tran(me, self->on.unlocked.base.pointer, e);
            return 0;
        case HSM_TS_ENTER:
            printf("YOU DIDN'T PAY\n");
            return 0;
        default:
            return 1;
        }
    }
int HSM_Turnstile_unlocked(HSM *me, HSM_event *e) {
    HSM_Turnstile * self = (HSM_Turnstile *) me;
    HSM_Turnstile_event * event = (HSM_Turnstile_event *) e;
    char signal = event->base.signal;
    switch (signal) {
        case HSM_ENTRY:
            printf("UNLOCKED\n");
            return 0;
        case HSM_EXIT:
            return 0;
        case HSM_TS_PAY:
            printf("YOU PAID AGAIN\n");
            return 0;
        case HSM_TS_ENTER:
            self->passengercount = self->passengercount + 1;
            printf("IN YOU GO\n");
            HSM_tran(me, self->on.locked.base.pointer, e);
            return 0;
        default:
            return 1;
        }
    }

HSM_Turnstile * HSM_Turnstile_creator(void) {
    HSM_Turnstile *instance;
    instance = (struct HSM_Turnstile *) malloc(sizeof(HSM_Turnstile));
    if (instance == NULL) {
        return NULL;
        }

    instance->base.pointer = &(instance->base);
    instance->base.state = &(instance->off.base);

    instance->passengercount = 0;

    instance->off.base.pointer = &(instance->off.base);
    instance->off.base.handler = &HSM_Turnstile_off;
    instance->off.base.map[0] = NULL;
    instance->off.base.map[1] = &(instance->off.base);
    instance->off.base.level = 1;

    instance->on.base.pointer = &(instance->on.base);
    instance->on.base.handler = &HSM_Turnstile_on;
    instance->on.base.map[0] = NULL;
    instance->on.base.map[1] = &(instance->on.base);
    instance->on.base.level = 1;

    instance->on.locked.base.pointer = &(instance->on.locked.base);
    instance->on.locked.base.handler = &HSM_Turnstile_locked;
    instance->on.locked.base.map[0] = NULL;
    instance->on.locked.base.map[1] = &(instance->on.base);
    instance->on.locked.base.map[2] = &(instance->on.locked.base);
    instance->on.locked.base.level = 2;

    instance->on.unlocked.base.pointer = &(instance->on.unlocked.base);
    instance->on.unlocked.base.handler = &HSM_Turnstile_unlocked;
    instance->on.unlocked.base.map[0] = NULL;
    instance->on.unlocked.base.map[1] = &(instance->on.base);
    instance->on.unlocked.base.map[2] = &(instance->on.unlocked.base);
    instance->on.unlocked.base.level = 2;

    return instance;
    }
/*    HSM_Turnstile instance = {
        { //base
        &(instance.base), //base.pointer
        &(instance.off.base), //base.state
        },
        0, //passengercount
        { //off state:
            { //off.base
            &(instance.off.base), //off.base.pointer
            &HSM_Turnstile_off, //off.base.handler
            {NULL, &(instance.off.base)}, //off.base.map
            1, //level
            } //end of off.base
        }, //end of off state
        { //on state:
            { //on.base
            &(instance.on.base), //on.base.pointer
            &HSM_Turnstile_on, //on.base.handler
            {NULL, &(instance.on.base)}, //on.base.map
            1, //level
            }, //end of on.base
            { //on.locked state:
                { //locked.base
                &(instance.on.locked.base), //locked.base.pointer
                &HSM_Turnstile_locked, //locked.base.handler
                {NULL, &(instance.on.base), &(instance.on.locked.base)}, //locked.base.map
                2, //level
                }, //end of locked.base
                0, //fare
                0, //paid
            }, //end of locked state
            { //on.unlocked state:
                { //unlocked.base
                &(instance.on.unlocked.base), //unlocked.base.pointer
                &HSM_Turnstile_unlocked, //unlocked.base.handler
                {NULL, &(instance.on.base), &(instance.on.unlocked.base)}, //unlocked.base.map
                2, //level
                }, //end of unlocked.base
            }, //end of unlocked state
        } //end of on state
    }; */
