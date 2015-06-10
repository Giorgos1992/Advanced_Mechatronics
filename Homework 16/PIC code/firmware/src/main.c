#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes


#include <xc.h> // processor SFR definitions
#include <sys/attribs.h> // __ISR macro
#include <stdio.h>
#include <string.h> //for itoa operation

int main ( void )
{

       // startup
        __builtin_disable_interrupts();
        __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
        BMXCONbits.BMXWSDRM = 0x0;
        INTCONbits.MVEC = 0x1;
        DDPCONbits.JTAGEN = 0;
    // set up USER pin as digital input; these are also set in a different c.file
        TRISBbits.TRISB13 = 1; //1 for input, 0 for output
        ANSELBbits.ANSB13 = 0; // 0 for digital, 1 for analog

       // <editor-fold defaultstate="collapsed" desc="PWM: pin configuration">


        //PWM Cycle for 1st/right Motor: B14 for Phase / B15 for IN

        // set up LED2 as OC1 using Timer2 at 1kHz
        ANSELBbits.ANSB15 = 0; // 0 for digital, 1 for analog
        RPB15Rbits.RPB15R = 0b0101; // set B15 to OC1 //Chapter11 of Datasheet

         //Set-up Timer3 and OC1
        T3CONbits.TCKPS = 0;     // Timer2 prescaler N=4 (1:8)
        PR3 = 19999;              // period = (PR2+1) * N * 25 ns -> 2 kHz
        TMR3 = 0;                // initial TMR2 count is 0
        OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
        OC1RS = 9999;             // duty cycle = OC1RS/(PR2+1) = 50%
        OC1R = 9999;              // initialize before turning OC1 on; afterward it is read-only
        T3CONbits.ON = 1;        // turn on Timer2
        OC1CONbits.ON = 1;       // turn on OC1

//        // DIRECTION OUTPUT PIN :B14
	TRISBbits.TRISB14 = 0; //Set B7 to output
	LATBbits.LATB14 = 0;    // Setting it to 0 initially
        //ANSEL =0

       // PWM CYCLE for 2nd Motor:: B5 is IN and B7 is PHASE//

        // Pins B7-B5 for right motor's PHASE (IN1) and EN (IN2)
        // Timer2 and OC3
        //T2 - Timer2 8 8 IFS0<8> IEC0<8> IPC2<4:2> IPC2<1:0>
//      ANSELBbits.ANSB5 = 0; // 0 for digital, 1 for analog //unnecessary
        RPB5Rbits.RPB5R = 0b0101; // set B15 to OC1 //Chapter11 of Datasheet

	T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1 (1:1)
	PR2 = 19999;              // period = (PR2+1) * N * 25 ns = 100 us, 20 kHz
	TMR2 = 0;                // initial TMR2 count is 0
	OC2CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
	OC2R = 6999;              // initialize before turning OC3 on; afterward it is read-only
	OC2RS = 6999;             // duty cycle = OC3RS/(PR2+1) = 75% // Not necessary
	OC2CONbits.OCTSEL = 1;   //Use timer 3 for comparison
	T2CONbits.ON = 1;        // turn on Timer3
	OC2CONbits.ON = 1;       // turn on OC3

	// DIRECTION OUTPUT PIN :B7
	TRISBbits.TRISB7 = 0; //Set B7 to output
	LATBbits.LATB7 = 0;    // Setting it to 0 initially

        // <editor-fold defaultstate="collapsed" desc="Test Phone Movement">

        //Forward Motion
        OC2RS = 15999;
        OC1RS = 15999;

        //Spin CW
         _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < 2 * 20000000)
        {
            ;
        }
        OC2RS = 0;
        OC1RS = 19999;

        //Spin CCW
          _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < 2 * 20000000)
        {
            ;
        }
        OC1RS = 0;
        OC2RS = 19999;

        //Stop
          _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < 2 * 20000000)
        {
            ;
        }
        OC1RS = 0;
        OC2RS = 0;
         // </editor-fold>

        // </editor-fold>

        __builtin_enable_interrupts();

    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


