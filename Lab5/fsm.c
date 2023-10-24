#include "fsm.h"
#include "hardware/gpio.h"
#include "read_input.h"
#include "TFTMaster.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "ts_lcd.h"


void operation()
{

int x;
int y;
struct TSPoint p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
int operator;

//sense inputs during transition:;

int input_sense = read_input(x, y);

char initial_buf[30];

static enum FSM_STATES {
    INITIAL,
    SENSESIGN1,
    DIGIT1,
    SENSESIGN2,
    DIGIT2,
    RESULT
} FSM_STATE;

int input = read_input();

    switch (FSM_STATE)

    {

    case INITIAL:
        printf("Initial");

        if(get_ts_lcd()){
            FSM_STATE = DIGIT1;
            getPoint(&p);
            x = p.x;
            y = p.y; 
        }
        break;

    case DIGIT1:

        printf("Sense");
        
        if (get_ts_lcd()){
        getPoint(&p);
        x = p.x;
        y = p.y;
        input_sense = read_input(x,y);
        // If an operator is pressed, it goes to digit 2
        if (input_sense >=10 && input_sense <= 13)   // 10 t0 13 are values for operator
        {
            FSM_STATE = SENSESIGN2;
            operator = input_sense;
        }
        else if (input_sense == 14){
            FSM_STATE = RESULT;
        }
        else{
            FSM_STATE = DIGIT1;
        }
        }
        

        break;


    case SENSESIGN2:
        printf("Second Digit Sign");

        if (get_ts_lcd()){
        getPoint(&p);
        x = p.x;
        y = p.y;
        input_sense = read_input(x,y);
        // If an operator is pressed, it goes to digit 2
        if (input_sense >=10 && input_sense <= 13)
        {
            FSM_STATE = SENSESIGN2;
        }
        else if(input_sense == 15){
            FSM_STATE = INITIAL;
        }
        else{
            FSM_STATE = DIGIT2;
        }
        }
        break;

    case DIGIT2:



        if (get_ts_lcd()){
        getPoint(&p);
        x = p.x;
        y = p.y;
        input_sense = read_input(x,y);
        // If an operator is pressed, it goes to digit 2
        if (input_sense >=10 && input_sense <= 13)
        {
            FSM_STATE = RESULT;
        }
        else if (input_sense == 14){
            FSM_STATE = RESULT;
        }
        else if (input_sense == 15){
            FSM_STATE = INITIAL;
        }
        else{
            FSM_STATE = DIGIT2;
        }
        }

        break;

    case RESULT:


        if (input_sense == 15)
        {
            FSM_STATE = INITIAL;
            result = 0;
        }
        else{
            FSM_STATE = RESULT;
        }

        break;
    }


int sign1;
int sign2;
int number1 = 0;
int number2 = 0;
char num_1[30];
int number_add;
char num_2[30];
char result[30];


switch (FSM_STATE){

case INITIAL:
    //erase old result
    tft_setTextColor(ILI9340_BLACK);
    tft_setCursor(20, 24); 
    tft_setTextSize(2);
    tft_writeString(result);
    // display 0
    tft_setTextColor(ILI9340_WHITE);
    tft_setCursor(20, 24);
    tft_setTextSize(2);
    sprintf(initial_buf, "%d", 0);
    tft_writeString(initial_buf);
    break;

case DIGIT1:

    tft_setTextColor(ILI9340_BLACK);
    tft_setCursor(20, 24);
    tft_writeString(initial_buf);

    if (input_sense >= 0 && input_sense<= 9)
    {

        // Erase old stuff
        tft_setTextColor(ILI9340_BLACK);
        tft_setCursor(20, 24); 
        tft_setTextSize(2);
        //sprintf(num_1);

        //Sense the inputs
        sign1 = 1;
        number_add = input_sense;
        // Display the number
        tft_setTextColor(ILI9340_WHITE);
        tft_setCursor(20, 24); 
        tft_setTextSize(2);
       
        number1 = ((number1)*10 + number_add);
        sprintf(num_1, "%d", number1);
        tft_writeString(num_1);
        result = number1;

    }
    break;
    
    case SENSESIGN2:

    if (input_sense == 11){
        //set and print the negative sign
        sign2 = -1;
        sprintf(num_2, "-");
        tft_setCursor(20, 24); 
        
    }
    else if (input_sense>=0 && input_sense<=9) {
        
         // Erase old stuff
        tft_setTextColor(ILI9340_BLACK);
        tft_setCursor(20, 24); 
        tft_setTextSize(2);
       // sprintf(num_2);

        //Sense the inputs
        sign2 = 1;
        number_add = input_sense;
        // Display the number
        tft_setTextColor(ILI9340_WHITE);
        tft_setCursor(20, 24); 
        tft_setTextSize(2);
       
        number2 = ((number2)*10 + number_add);
        sprintf(num_2, "%d", number2);
        tft_writeString(num_2);
    }


    case DIGIT2:
         if (input_sense >= 0 && input_sense<= 9){
    
        // Erase old stuff
        tft_setTextColor(ILI9340_BLACK);
        tft_setCursor(20, 24); 
        tft_setTextSize(2);
        //sprintf(num_2);

        //Sense the inputs

        number_add = input_sense;
        // Display the number
        tft_setTextColor(ILI9340_WHITE);
        tft_setCursor(20, 24); 
        tft_setTextSize(2);
       
        number2 = sign2*((number2)*10 + number_add);
        sprintf(num_2, "%d", number2);
        tft_writeString(num_2);

    }

    break;

    case RESULT:

        if (operator== 10)
        {
            result = number1 + number2;
            // display result
            sprintf(result, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24); 
            tft_setTextSize(2);
            tft_writeString(result);
        }

        else if (operator== 11)
        {
            result = number1 - number2;
            // display result
            sprintf(result, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24); 
            tft_setTextSize(2);
            tft_writeString(result);
        }

        else if (operator== 12)
        {
            result = number1 * number2;
            // display result
            sprintf(result, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24); 
            tft_setTextSize(2);
            tft_writeString(result);
        }

        else if (operator== 13)
        {
            result = number1 / number2;
            // display result
            sprintf(result, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24); 
            tft_setTextSize(2);
            tft_writeString(result);
        }

        break;
    }
}

