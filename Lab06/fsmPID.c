#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "timer.h"
#include "ic.h"


    static char cmdbuff[20];
    static int cmdindex = 0;
    static char c;

static enum FSM_States{
    INIT,
    CTRL
} FSM_STATE;


void FSM_Tick(){



switch(FSM_STATE){

    case INIT:
    printf("This is initial state");
    FSM_STATE = WAIT4CHAR;
    break;


    case CTRL:
    printf ("This is wait4chat state");
    
    if(!uart_is_readable(uart0)){
        FSM_STATE = WAIT4CHAR;
    }
    else{
        c = uart_getc(uart0);
        cmdbuff[cmdindex ++] = c;

    }

    break;

}

switch(FSM_STATE){

    case INIT:
    break;


    case WAIT4CHAR:
    break;
}