/*
 * File:   main.c
 * Author: Sebastian Moosbauer
 *
 * Created on March 24, 2025, 4:57 PM
 */


#include <xc.h>
#include "LCD/GLCD_library.h"
//
#include "GLCD_config.h"

// function prototype. Used to publish the functions name. It can then
// be implement elsewhere. In case of this function it is implemented 
// below main(), to increase readability.
void __init(void);
void update_time(unsigned int time);
void update_bar(void);

// main code, i.e. the application itself

void main(void) {
    // Execute initialization of Microcontroller (will be explained soon!)
    __init();

    unsigned int time = 0; // up to 65535 seconds = 18.2 hours can be stored
    unsigned char led_counter = 0;

    // while(1) is an endless loop. It's a very common part of microcontroller
    // code, as it prevents the application from termination. Without it, the
    // application would terminate after one execution and the microcontroller
    // would restart. Not very useful...
    while (1) {

        // only use for loop to waste time, if Timer is not enabled
        if (T0CONbits.TMR0ON == 0) {
            // delay for ?? ms
            Nop();
            // __debug_break();
            for (unsigned int i = 0; i < 5208; ++i) {
                Nop();
            }
            Nop();
            // __debug_break();

            ++time;
        }
        
        if(T0CONbits.TMR0ON == 1 && INTCONbits.TMR0IF) {
            INTCONbits.TMR0IF = 0;
            ++time;
        }

        update_time(time);

        if (PORTBbits.RB1 == 0) {
            while (PORTBbits.RB1 == 0);
            update_bar();
        }

    }

    return;
}

// Ignore below code for now. We'll come to initialization soon.

void __init(void) {
    // Set Oscillator Frequency to 250KHz (you will use this setting a lot!)
    OSCCON = 0x20;
    // Enable LCD
    GLCD_Init();

    ANSELB = 0; // which mode are the pins configured for?
    TRISB = 0b11000011; // how are pins configured here?
    LATB = 0b00111100; // are the LEDs on or off?    

    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 1;
}

void update_time(unsigned int time) {
    static unsigned int previous_time = 0;
    time *= 5;
    if (time != previous_time) {
        previous_time = time;
        unsigned int minutes = (unsigned int) (time / 600);
        unsigned int no_mins = (unsigned int) (time - minutes * 600);
        unsigned int seconds = (unsigned int) (no_mins / 10);
        unsigned char ms = no_mins - seconds * 10;
        GLCD_Text2Out(0, 4, ":  . ");
        GLCD_Value2Out_00(0, 2, minutes, 2);
        GLCD_Value2Out_00(0, 5, seconds, 2);
        GLCD_Value2Out_00(0, 8, ms, 1);
    }
}

void update_bar(void) {
    static unsigned char bar[11] = {};
    static char num_chars_in_bar = 0;
    // update "Laufbalken"
    bar[num_chars_in_bar] = '#';
    bar[num_chars_in_bar + 1] = '\0';
    // update bar
    GLCD_Text2Out(2, 0, bar);

    ++num_chars_in_bar;
    if (num_chars_in_bar >= 11) {
        num_chars_in_bar = 0;
        bar[0] = '\0';
        GLCD_Clear2Row(2);
    }
}
