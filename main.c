#include <stdio.h>
#include <stdlib.h>

#include "hsm.h"
#include "turnstile.h"

int main() {
    HSM_Turnstile * machine = HSM_Turnstile_creator();
    if (machine == NULL) {
        printf("could not allocate memory for machine");
        return 1;
        }
    HSM_Turnstile_event event;
    char input;

    for(;;) {

        input = getchar();

        if (input == '\n') {
            continue;
            }
        else if (input == 'N' || input == 'n') {
            event.base.signal = HSM_TS_ON;
            }
        else if (input == 'F' || input == 'f') {
            event.base.signal = HSM_TS_OFF;
            }
        else if (input == 'P' || input == 'p') {
            event.base.signal = HSM_TS_PAY;
            }
        else if (input == 'E' || input == 'e') {
            event.base.signal = HSM_TS_ENTER;
            }
        else if (input == 'Q' || input == 'q') {
            printf("\nHope you enjoyed yourself. Bye-bye!\n\n");
            break;
            }
        else {
            event.base.signal = HSM_TS_OTHER;
            }
        HSM_Turnstile_dispatch(machine, event);
        }
    return 0;
    }
