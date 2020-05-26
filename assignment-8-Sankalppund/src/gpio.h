/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>

#define Sensor_port (gpioPortD)
#define Sensor_pin  (15)
#define LCD_ENABLE_PORT (gpioPortD)
#define LCD_ENABLE_PIN (15)
#define LCD_PORT (gpioPortD)
#define LCD_PIN (13)
#define PB0_PORT (gpioPortF)
#define PB0_PIN (6)


#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	1
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		1

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioEnableDisplay();
void gpioSetDisplayExtcomin(bool high);


#endif /* SRC_GPIO_H_ */
