/*
 * main.h
 *
 *  Created on: Sep 25, 2019
 *      Author: sankalp
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_


CORE_DECLARE_IRQ_STATE;


extern uint32_t SetupLoadPowerManagement_flag;

extern uint32_t StartI2CWriteTransfer_flag;

extern uint32_t StartI2CReadTransfer_flag;

extern uint32_t SetupTempTimer_flag;

extern uint32_t TemperatureCalculation_flag;

void state_machine();


#endif /* SRC_MAIN_H_ */
