/*
 * sensor.h
 *
 *  Created on: Dec 3, 2019
 *      Author: sankalp pund
 */

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

#include <stdbool.h>
//#include "native_gecko.h"
#include "em_i2c.h"
#include "em_cmu.h"

/*
 * Macros for Register Locations
 */

#define COMMAND (0x04)
#define RESULT  (0x08)
#define MCPU_TO_HOST_MAILBOX (0x12)
#define CONFIGURE_PMU (0x14)
#define CONFIGURE_I2C (0x1C)
#define PROX_THRESHOLD (90)
#define CONF_REG (0x0A)

uint8_t temp1;
uint16_t temp;
uint16_t tempshift;
uint16_t finaltemp;

uint8_t errorCode;
uint8_t ecode;
uint8_t reg_val;


#define SDAPORT 			gpioPortC
#define SDAPIN				(11)
#define SCLPORT 			gpioPortC
#define SCLPIN				(10)
#define SENSORPORT 			gpioPortD
#define SENSORPIN			(15)


void powerup_proximity(void);
uint8_t measure_distance(void);


#endif /* SRC_SENSOR_H_ */
