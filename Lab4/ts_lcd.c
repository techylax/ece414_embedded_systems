#include "ts_lcd.h"
#include "TouchScreen.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"

bool get_ts_lcd(uint16_t *px, uint16_t *py){

struct TSPoint tpoint;

getpoint(&tpoint);

*px = 0.1*(tpoint.x-400);
*py = 0.0886*(tpoint.y - 790);

if((tpoint.z<400)){
    return true;
}
else{
    return false;
}
}



void ts_lcd_init()
{
//leave blank
}