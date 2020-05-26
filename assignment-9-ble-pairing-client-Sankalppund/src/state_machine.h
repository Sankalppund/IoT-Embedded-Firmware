/*
 * state_machine.h
 *
 *  Created on: Oct 17, 2019
 *      Author: sanka
 */

#ifndef SRC_STATE_MACHINE_H_
#define SRC_STATE_MACHINE_H_


/******************************
 * State Enumeration variables.
 *******************************/

typedef enum{
	//nullState,
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

#endif /* SRC_STATE_MACHINE_H_ */
