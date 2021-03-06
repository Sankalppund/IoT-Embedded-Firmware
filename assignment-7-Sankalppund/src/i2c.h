/*
 * i2c.h
 *
 * Course: IoT Embedded Firmware
 * Date:   09/18/2019
 * Auther: Sankalp Pund
 *
 * Fall 2019
 *
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_


/**************************
 * function prototype
 ***************************/
void i2c_init();
void write_data();
void read_data();
void Temp_Sense_Value();
void timerWaitUs();




#endif /* SRC_I2C_H_ */
