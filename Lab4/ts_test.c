#include "TouchScreen.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "ts_lcd.h"

void test ()
{
if(get_ts_lcd())
{
  getpoint();
  char buffer[30];
    
    adc_init();
    //initialize screen
    tft_init_hw();
    tft_begin();
    tft_setRotation(3); 
    tft_fillScreen(ILI9340_BLACK);  
    
    while(1){
        //tft_fillScreen(ILI9341_BLACK);
        tft_setCursor(20, 200);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);

        //erase old text
        tft_setTextColor(ILI9340_BLACK);
        tft_writeString(buffer);
        
        struct TSPoint p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
        getPoint(&p);
        tft_setCursor(20, 200);
        tft_setTextColor(ILI9340_WHITE);
        sprintf(buffer,"x: %d, y: %d", p.x, p.y);
        tft_writeString(buffer);
    
        sleep_ms(100);
    }



}



}
