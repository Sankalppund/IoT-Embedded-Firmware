/*
 * main.h
 *
 *  Created on: Sep 25, 2019
 *      Author: sankalp
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#define time_delay (80)
#define time_delay_1 (8)

CORE_DECLARE_IRQ_STATE;

int Val_of_RSSI = 0;

extern uint32_t SetupLoadPowerManagement_flag;

extern uint32_t StartI2CWriteTransfer_flag;

extern uint32_t StartI2CReadTransfer_flag;

extern uint32_t SetupTempTimer_flag;

extern uint32_t TemperatureCalculation_flag;

int connection_flag = 0;

uint8_t connection_value = 0;




void state_machine();



uint16_t check_1 = 0;   // flag variable

#endif /* SRC_MAIN_H_ */
