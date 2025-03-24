/*
 * File:   main.c
 * Author: Sebastian Moosbauer
 *
 * Created on March 24, 2025, 4:57 PM
 */


#include <xc.h>
//#include "Libraries/LCD/GLCD_library.h"
//
//#include "GLCD_config.h"
//#include "utils.h"

// function prototype. Used to publish the functions name. It can then
// be implement elsewhere. In case of this function it is implemented 
// below main(), to increase readability.
void __init(void);
void update_display(unsigned int time);

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
            for (unsigned int i = 0; i < 2604; ++i) {
                Nop();
            }

            time += 1;
        }
        
        update_display(time);

    }

    return;
}

// Ignore below code for now. We'll come to initialization soon.

void __init(void) {
    // Set Oscillator Frequency to 4MHz (you will use this setting a lot!)
    OSCCON = 0x00;
    // Enable LCD
//    GLCD_Init();

    ANSELB = 0; // which mode are the pins configured for?
    TRISB = 0b11000011; // how are pins configured here?
    LATB = 0b00111100; // are the LEDs on or off?    

    T0CONbits.TMR0ON = 0;
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b110;
}

void update_display(unsigned int time) {
    static unsigned int previous_time = 0;

    if (time != previous_time) {
        // print new time to Display
    }

    if (T0CONbits.TMR0ON) {
        // update "Laufbalken"
        static unsigned int delay = 0;
        if (++delay < 100000) { // for F_osc = 4MHz ~ 100ms
            return;
        }

        // update Laufbalken

    }

}
