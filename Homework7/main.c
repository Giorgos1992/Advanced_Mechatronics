/* 
 * File:   main.c
 * Author: Giorgos1992
 *
 * Created on April 1, 2015, 6:13 AM
 */

#include<xc.h> // processor SFR definitions
#include<sys/attribs.h> // __ISR macro

//Wiring choices
//A0 for AN0, B7 for LED1, B13 for USER, and B15 for OC1/LED2

// DEVCFGs here

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // not boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // free up secondary osc pins by turning sosc off
#pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // slowest wdt
#pragma config WINDIS = OFF // no wdt window
#pragma config FWDTEN = OFF // wdt off by default
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 40MHz
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz:8MHz/2
#pragma config FPLLMUL = MUL_20 // multiply clock after FPLLIDIV:4*20 = 80MHz
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 40MHz:80/2 = 40MHz
#pragma config UPLLIDIV = DIV_2 // divide 8MHz input clock, then multiply by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = ON // allow only one reconfiguration
#pragma config IOL1WAY = ON // allow only one reconfiguration
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // controlled by USB module

int readADC(void);

int main() {

    // startup
        __builtin_disable_interrupts();
        // set the CP0 CONFIG register to indicate that
        // kseg0 is cacheable (0x3) or uncacheable (0x2)
        // see Chapter 2 "CPU for Devices with M4K Core"
        // of the PIC32 reference manual
        __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
        // no cache on this chip!
        // 0 data RAM access wait states
        BMXCONbits.BMXWSDRM = 0x0;
        // enable multi vector interrupts
        INTCONbits.MVEC = 0x1;
        // disable JTAG to be able to use TDI, TDO, TCK, TMS as digital
        DDPCONbits.JTAGEN = 0;

    // set up USER pin as digital input
        TRISBbits.TRISB13 = 1; //1 for input, 0 for output
        ANSELBbits.ANSB13 = 0; // 0 for digital, 1 for analog

    // set up LED1 pin as a digital output
        TRISBbits.TRISB7 = 0; //Set B7 to output
        PORTBbits.RB7 = 0; //Set B7 to 0-LOW/1-HIGH to begin with

    // set up LED2 as OC1 using Timer2 at 1kHz
       //ANSELBbits.ANSB15 = 0; // 0 for digital, 1 for analog
        RPB15Rbits.RPB15R = 0b0101; // set B14 to OC1 //Chapter11 of Datasheet
              
    //Set-up Timer2 to call ISR at a fixed frequency of 1kHz.
        //T2 - Timer2 8 8 IFS0<8> IEC0<8> IPC2<4:2> IPC2<1:0>
        //Set up Timer 2 to roll over with frequency 1kHz
        T2CONbits.TCKPS = 2;     // Timer2 prescaler N=4 (1:8)
        PR2 = 9999;              // period = (PR2+1) * N * 25 ns = 1kHz
        TMR2 = 0;                // initial TMR2 count is 0
        OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
        OC1RS = 5000;             // duty cycle = OC1RS/(PR2+1) = 50%
        OC1R = 9999;              // initialize before turning OC1 on; afterward it is read-only
        T2CONbits.ON = 1;        // turn on Timer2
        OC1CONbits.ON = 1;       // turn on OC1

       // set up A0 as AN0
        ANSELAbits.ANSA0 = 1;
        AD1CON3bits.ADCS = 3;
        AD1CHSbits.CH0SA = 0;
        AD1CON1bits.ADON = 1;

        __builtin_enable_interrupts();
        while (1) {
       //Use the core timer to double check your CPU clock settings
            // set core timer to 0, remember it counts at half the CPU clock
            //That is, at 20MHz
            _CP0_SET_COUNT(0);
            PORTBbits.RB7 = !PORTBbits.RB7; //Inverts B7 -> LED1
            // wait for half a second, setting LED brightness to pot angle while waiting
             while (_CP0_GET_COUNT() < 10000000) {
                unsigned int val = readADC(); //maximum value is 1023
                OC1RS = val * 9999/1023;
                if (PORTBbits.RB13 == 1){} //High if not pressed
                // nothing

               else {
               PORTBbits.RB7 = !PORTBbits.RB7; //Inverts B7 -> LED1

                }
             }
       }
}

int readADC(void) {
    int elapsed = 0;
    int finishtime = 0;
    int sampletime = 20;
    int a = 0;

    AD1CON1bits.SAMP = 1;
    elapsed = _CP0_GET_COUNT();
    finishtime = elapsed + sampletime;
    while (_CP0_GET_COUNT() < finishtime) {
    }
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE) {
    }
    a = ADC1BUF0;
    return a;
}