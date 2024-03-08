/* Program to make LCD screen go red when LEFT
   Pushbutton (SW3) is pressed, and go TEAL
   when the RIGHT Pushbutton (SW2) is pressed.
   It will also count from 0-5 every 2 seconds
   using Ticker.
*/

// INCLUDE SECTION
#include "mbed.h"                               // include mbed.h
#include "Grove_LCD_RGB_Backlight.h"            // include Grove_LCD_RGB_Backlight.h

// Hardware Definition Section
Grove_LCD_RGB_Backlight LCD(D14,D15);           // associated LCD with pins (D14,D15) I2C

// GLOBAL VARIABLE Section
volatile short l_counter=0, r_counter=0;        // l_counter and r_counter to 0
volatile short i;                               // variable i will be your counter
char buff[20];                                  // buff is used for sprintf

// Interrupt Definition Section
InterruptIn pb_left(SW3);                       // pb_left is Asynch Interrupt associated with SW3
InterruptIn pb_right(SW2);                      // pb_right is   "       "        "         "  SW2

Ticker tock;

void abc(void)                                  // Interrupt Service Routine for pb_left
{                                               // When left pushbutton is pressed
    l_counter=1;                                // change global variable l_counter to 1    
}
void def(void)                                  // Interrupt Service Routine for pb_right
{                                               // When right pushbutton is pressed
    r_counter=1;                                // change global variable r_counter to 1
}
void c_up(void)                                 // Interrupt Service Routine for tock
{
    i++;                                        // add 1 to counter
}

int main(void)
{
    tock.attach(&c_up,2.0);
    pb_left.rise(&abc);                         // IRQ Service Routine set to address &abc 
    pb_right.fall(&def);                        // IRQ Service Routine set to address &def
    LCD.setRGB(0xff,0xff,0xff);                 // Make LCD screen WHITE
    for(;;)
    {
        if(i > 5)                               // if i is greater than 5 reset to 0
        {
            i=0;
        }
        sprintf(buff,"%4d",i);                  // set i in field of 4 inside buffer
        LCD.locate(5,0);                        // set cursor to column 5 row 0
        LCD.print(buff);                        // print buff to LCD
       
      {
        if(l_counter==1)                        // has pb_left been pressed and released
        {
            LCD.setRGB(0xff,0x00,0x00);         // if so then turn LCD RED
            l_counter=0;                        // reset GLOBAL var l_counter to 0
        }                                       // so it doesn't trigger it again

        if(r_counter==1)                        // has pb_right been pressed
        {                                       
            LCD.setRGB(0x00,0x80,0x80);         // if so then set LCD to TEAL
            r_counter=0;                        // reset GLOBAL var r_counter to 0
        }                                       // so it doesn't trigger again
      }                                       
    }

}