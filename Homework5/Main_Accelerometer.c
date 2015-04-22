/*
 * File:   HW5_Accel_main.c
 * Author: Giorgos1992
 *
 * Created on April 15, 2015, 11:31 AM
 */

#include<xc.h> // processor SFR definitions
#include<sys/attribs.h> // __ISR macro
#include <stdlib.h>
#include <stdio.h>
#include <string.h> //for itoa operation

//Include the 2 libraries for the LED
#include "i2c_display.h"
#include "i2c_master_int.h"

//Include library for the accelerometer
#include "accel.h"

#define MAX 1000

//Wiring choices
//A0 for AN0, B7 for LED1, B13 for USER, and B15 for OC1/LED2
////CS - RB3 (digital pin), SDO - RB5 (SDI1), SDA - RB2 (SDO1)

// <editor-fold defaultstate="collapsed" desc="DEVCFGs here">
/* ***********************DEVCFGs here ********************** */
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
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ASCII table">
// lookup table for all of the ascii characters
static const char ASCII[96][5] = {
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  (space)
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ?
,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f ?
}; // end char ASCII[96][5]
         // </editor-fold>


int readADC(void);
void writeletter(int letter, int x, int y);
void writemessage(char *message, int xo, int yo);

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
        RPB15Rbits.RPB15R = 0b0101; // set B15 to OC1 //Chapter11 of Datasheet

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

        acc_setup();
//        acc_write_register(, );

        __builtin_enable_interrupts();

        short accels[3]; // accelerations for the 3 axes
        short mags[3]; // magnetometer readings for the 3 axes
        short temp;

        // read the accelerometer from all three axes
        // the accelerometer and the pic32 are both little endian by default (the lowest address has the LSB)
        // the accelerations are 16-bit twos compliment numbers, the same as a short
        //acc_read_register(OUT_X_L_A, (unsigned char *) accels, 6);
        // need to read all 6 bytes in one transaction to get an update.
        //acc_read_register(OUT_X_L_M, (unsigned char *) mags, 6);
        // read the temperature data. Its a right justified 12 bit two's compliment number
        //acc_read_register(TEMP_OUT_L, (unsigned char *) &temp, 2);


           display_init();
           char Message[MAX];

//           int Number;
//           Number = 1992;
//           snprintf(Message, MAX, "Giorgos %d %d!", Number, temp);
//           //writes message with starting location x=28, y=32;
//           writemessage(Message,28,32);
           int q=0;
           int p;
           while (q<1000000000) //for 50 seconds
           {
            acc_read_register(OUT_X_L_A, (unsigned char *) accels, 6);
//            snprintf(Message, MAX, "Data:%d %d %d", accels[0]*64/16000, accels[1]*32/16000, accels[2]/16000);
//            writemessage(Message,32,64);
            if (accels[0]>=0){
                for (p=32;p<=32+accels[0]*32/16000;p=p++){
                    display_pixel_set(p,64,1);
                    display_pixel_set(p,65,1);
                    display_pixel_set(p,66,1);
                }
            }
            else
                {
                 for (p=32;p>=32+accels[0]*32/16000;p=p--){
                    display_pixel_set(p,64,1);
                    display_pixel_set(p,65,1);
                    display_pixel_set(p,66,1);
                }
            }
//            for (p=32;p=32+accels[0]*32/16000;p=p+(accels[1]>0)?+1:-1){
//                display_pixel_set(p,64,1);
//                display_pixel_set(p,65,1);
//                display_pixel_set(p,66,1);
//            }
            if (accels[1]>=0){
                for (p=64;p<=64+accels[1]*64/16000;p=p++/* p+(accels[1]>0)?+1:-1*/){
                    display_pixel_set(31,p,1);
                    display_pixel_set(32,p,1);
                    display_pixel_set(33,p,1);
                    }
            }
            else
                {
                  for (p=64;p>=64+accels[1]*64/16000;p=p--/* p+(accels[1]>0)?+1:-1*/){
                    display_pixel_set(31,p,1);
                    display_pixel_set(32,p,1);
                    display_pixel_set(33,p,1);
                    }
            }
          
            display_draw();
            display_clear();
//            for (p=32;p=32+accels[0]*32/16000;p=p+(accels[0]>0)?+1:-1){
//                display_pixel_set(p,64,0);
//                display_pixel_set(p,65,0);
//                display_pixel_set(p,66,0);
//            }
            q++;
           }

           // <editor-fold defaultstate="collapsed" desc="while loop for blinking lights">
           while (1) {
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
           // </editor-fold>
}




void writeletter(int letter, int x, int y){

        int k,l;
        int BIT;
        int number;
        for (l=0;l<=4;l++){
            for (k=0;k<=7;k++){
            number = ASCII[letter][l];
            BIT =(number & (1<<k))>>k;
            display_pixel_set(x+k,y+l,BIT);
//            display_draw();
        };
//        display_draw();
        }
//        display_draw();
}

void writemessage(char *message, int xo, int yo){
    int i=0;
    while (message[i]){
        writeletter((int)message[i]-0x20,xo,yo+i*6);
        i++;
    }

};


// <editor-fold defaultstate="collapsed" desc="readADC function">
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
// </editor-fold>