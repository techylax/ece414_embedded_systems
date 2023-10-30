#include "fsm.h"
#include "hardware/gpio.h"
#include "read_input.h"
#include "TFTMaster.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "ts_lcd.h"
#include "getButtonPressed.h"
#include "arrayClear.h"

int result;
bool pressed;

uint16_t px;
uint16_t py;
int number1 = 0;
int number2 = 0;
char num_1[30];
int number_add;
char num_2[30];
char result1[30];
char initial_buf[30];

void operation()
{
    static int operator;

    static enum FSM_STATES {
        INITIAL,
        DIGIT1,
        DIGIT2,
        RESULT
    } FSM_STATE;

    static int input_sense;

    switch (FSM_STATE)

    {

    case INITIAL:
        printf("Initial \n");

        if (get_ts_lcd(&px, &py))
        {
            // printf("True \n");
            FSM_STATE = DIGIT1;
            input_sense = read_input(px, py);
            printf(" input sense = %d \n", input_sense);
        }

        else
        {
            FSM_STATE = INITIAL;
        }

        break;

    case DIGIT1:

        printf("DIGIT1 \n");

        pressed = get_ts_lcd(&px, &py);

        if (pressed)
        {
            input_sense = read_input(px, py);
            // If an operator is pressed, it goes to digit 2
            if (input_sense >= 10 && input_sense <= 13) // 10 t0 13 are values for operator
            {
                FSM_STATE = DIGIT2;
                operator= input_sense;
                printf("%d", operator);
                printf("%d", operator);
            }
            else if (input_sense == 14)
            {
                FSM_STATE = RESULT;
            }
            else if (input_sense >= 0 && input_sense <= 9)
            {
                FSM_STATE = DIGIT1;
            }
            else if (input_sense == 15)
            {
                result = 0;
                number1 = 0;
                number2 = 0;
                FSM_STATE = INITIAL;
            }
            else
            {
                FSM_STATE = DIGIT1;
            }
        }
        else{
            FSM_STATE = DIGIT1;
        }

        break;

    case DIGIT2:

        printf("Digit2 Entered \n");
        pressed = get_ts_lcd(&px, &py);
        if (pressed)
        {
            input_sense = read_input(px, py);
            // If an operator is pressed, it goes to digit 2
            if (input_sense >= 10 && input_sense <= 13)
            {
                FSM_STATE = RESULT;
            }
            else if (input_sense == 14)
            {
                FSM_STATE = RESULT;
            }
            else if (input_sense == 15)
            {
                result = 0;
                number1 = 0;
                number2 = 0;
                FSM_STATE = INITIAL;
            }
            else
            {
                FSM_STATE = DIGIT2;
            }
        }
        else{
            FSM_STATE = DIGIT2;
        }

        break;

    case RESULT:

        printf("Result State Entered \n");
        if (get_ts_lcd(&px, &py))
        {
            input_sense = read_input(px, py);
            printf("input_sense=%d\n", input_sense);

            if (input_sense == 15)
            {
                result = 0;
                number1 = 0;
                number2 = 0;
                //sprintf(num_1, "%d", 0);
                //sprintf(num_2, "%d", 0);
                //sprintf(result1, "%d", 0);
                FSM_STATE = INITIAL;
            }
            else if (input_sense >= 10 && input_sense <= 13)
            {
                FSM_STATE = DIGIT2;
                number1 = result;
                number2 = 0;
                operator = input_sense;
            }
            else
            {
                FSM_STATE = RESULT;
            }
        }

        break;
    }

    switch (FSM_STATE)
    {

    case INITIAL:


        // erase old result
        tft_setTextColor(ILI9340_BLACK);
        tft_setCursor(20, 24);
        tft_setTextSize(2);
        tft_writeString(result1);
        tft_setCursor(20, 24);
        tft_setTextSize(2);
        tft_writeString(num_1);
        tft_setCursor(20, 24);
        tft_setTextSize(2);
        tft_writeString(num_2);
        // reset all arrays

        arrayClear(num_1);
        arrayClear(num_2);
        arrayClear(result1);
        //sprintf(num_1, "%d", 0);
        //sprintf(num_2, "%d", 0);
        //sprintf(result1, "%d", 0);
        // display 0
        tft_setTextColor(ILI9340_WHITE);
        tft_setCursor(20, 24);
        tft_setTextSize(2);
        sprintf(initial_buf, "%d", 0);
        tft_writeString(initial_buf);
        printf("Test");
        break;

    case DIGIT1:

        if(!pressed){
            printf("no digit pressed");
        }
        else if ((pressed) && (input_sense >= 0 && input_sense <= 9))
        {
            // Erase old stuff
            tft_setTextColor(ILI9340_BLACK);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            // sprintf(num_1);
            // Sense the inputs
            tft_writeString(initial_buf);
            printf("Erased");
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

    case DIGIT2:

        if (!pressed){

            printf("no digit pressed");

        }
        else if (pressed && (input_sense >= 0 && input_sense <= 9))
        {
            // Erase old stuff
            tft_setTextColor(ILI9340_BLACK);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            // sprintf(num_1);
            // Sense the inputs
            tft_writeString(result1);
            tft_setCursor(20, 24);
            tft_writeString(num_1);
            tft_setCursor(20, 24);
            tft_writeString(num_2);
            tft_setCursor(20, 24);
            tft_writeString(result1);
            printf("Erased");
            number_add = input_sense;
            // Display the number
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24);
            tft_setTextSize(2);

            number2 = ((number2)*10 + number_add);
            sprintf(num_2, "%d", number2);
            tft_writeString(num_2);
            result = number2;
        }

        break;

    case RESULT:

        printf("Actual Result State");
        printf("%d \n", operator);

        if (operator== 10)
        {
            printf("addition result");
            tft_setTextColor(ILI9340_BLACK);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(num_2);
            result = number1 + number2;
            printf("%d", result);
            // display result
            sprintf(result1, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(result1);
        }

        else if (operator== 11)
        {
            tft_setTextColor(ILI9340_BLACK);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(num_2);
            result = number1 - number2;
            // display result
            sprintf(result1, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(result1);
        }

        else if (operator== 12)
        {
            tft_setTextColor(ILI9340_BLACK);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(num_2);
            result = number1 * number2;
            // display result
            sprintf(result1, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(result1);
        }

        else if (operator== 13)
        {
            tft_setTextColor(ILI9340_BLACK);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(num_2);
            result = number1 / number2;
            // display result
            sprintf(result1, "%d", result);
            tft_setTextColor(ILI9340_WHITE);
            tft_setCursor(20, 24);
            tft_setTextSize(2);
            tft_writeString(result1);
        }
        else
        {
            printf("else");
        }

        break;
    }
}
