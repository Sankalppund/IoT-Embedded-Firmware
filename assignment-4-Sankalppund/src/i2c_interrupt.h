

/*
 * i2c_interrupt.h
 *
 *  Created on: Sep 25, 2019
 *      Author: sanka
 */

#include "em_i2c.h"

#ifndef SRC_I2C_INTERRUPT_H_
#define SRC_I2C_INTERRUPT_H_


I2C_TransferSeq_TypeDef I_write_data;

I2C_TransferSeq_TypeDef I_read_data;



/******************************
 * State Enumeration variables.
 *******************************/

typedef enum{

	PowerOff,
	WaitForPowerUpSequenceComplete,
	WaitI2CWriteToComplete,
	WaitforTemperatureValueReady,
	WaitforI2CAddressReadComplete,


}state;


/******************************
 * Event Enumeration variables.
 *******************************/

typedef enum{

	event_null,
	SetupLoadPowerManagement,
	StartI2CWriteTransfer,
	SetupTempTimer,
	StartI2CReadTransfer,
	TemperatureCalculation,

}event;

/*Enum Variables*/

state current_state;
state previous_state;
state next_state;
event event_status;

#endif /* SRC_I2C_INTERRUPT_H_ */




