/* 
 * File:   i2c_master_int.h
 * Author: Giorgos1992
 *
 * Created on April 12, 2015, 2:53 PM
 */

#ifndef I2C_MASTER_INT__H__
#define I2C_MASTER_INT__H__

// buffer pointer type.  The buffer is shared by an ISR and mainline code.
// the pointer to the buffer is also shared by an ISR and mainline code.
// Hence the double volatile qualification
typedef volatile unsigned char * volatile buffer_t;


void i2c_master_setup(); //sets up i2c2 as a master using an interrupt

// initiate an i2c write read operation at the given address. You can optionally only read or only write by passing zero lengths for the reading or writing
// this will not return until the transaction is complete.  returns false on error
int i2c_write_read(unsigned int addr, const buffer_t write, unsigned int wlen, const buffer_t read, unsigned int rlen );

#endif
