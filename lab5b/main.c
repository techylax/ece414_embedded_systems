#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "TouchScreen.h"
#include "TFTMaster.h"
#include "calculator.h"
#include "fsm.h"


int main(){
calculator();
stdio_init_all();
while(1)
{


//printf("TEST FILE");
operation();
sleep_ms(500);
}
}