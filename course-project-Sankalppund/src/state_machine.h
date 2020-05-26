/*
 * state_machine.h
 *
 *  Created on: Oct 17, 2019
 *      Author: sankalp pund
 */

#ifndef SRC_STATE_MACHINE_H_
#define SRC_STATE_MACHINE_H_


/******************************
 * State Enumeration variables.
 *******************************/

typedef enum{

	PowerOff,
	WaitForPowerUpSequenceComplete,
	WaitI2CWriteToComplete,
	WaitforLuxValueReady,
	WaitforI2CAddressReadComplete,

}state;

/******************************
 * Event Enumeration variables.
 *******************************/

typedef enum{

	event_null,
	SetupLoadPowerManagement,
	StartI2CWriteTransfer,
	SetupLuxTimer,
	StartI2CReadTransfer,
	LuxCalculation,

}event;

/*Macros for LUX reg address*/

#define LX_ADDR (0x39)
#define LX_CMD_CTR (0x80|0x00)
#define LX_POWER_ON (0x03)
#define DATA0 (0x80|0x0C)
#define DATA1 (0x80|0x0E)



/*Enum Variables*/

state current_state;
state previous_state;
state next_state;
event event_status;

#endif /* SRC_STATE_MACHINE_H_ */
