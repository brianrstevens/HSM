#ifndef TURNSTILE_H_INCLUDED
#define TURNSTILE_H_INCLUDED

#include "hsm.h"

typedef struct HSM_Turnstile HSM_Turnstile;
typedef struct HSM_Turnstile_state HSM_Turnstile_state;
typedef struct HSM_Turnstile_event HSM_Turnstile_event;

enum HSM_Turnstile_sigs {
    HSM_TS_ON = HSM_SIGNALS,
    HSM_TS_OFF,
    HSM_TS_PAY,
    HSM_TS_ENTER,
    HSM_TS_OTHER
    };
struct HSM_Turnstile_event {
    HSM_event base;
    };

HSM_Turnstile * HSM_Turnstile_creator(void);
void HSM_Turnstile_dispatch(HSM_Turnstile *me, HSM_Turnstile_event event);

#endif // TURNSTILE_H_INCLUDED
