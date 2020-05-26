/*
 * state_machine.c
 *
 *  Created on: Oct 17, 2019
 *      Author: sankalp pund
 */

#include"Letimer0.h"
#include"Sleep.h"

//Logging headers
#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1
#include"log.h"
#include "em_core.h"
#include "i2c_interrupt.h"
#include "main.h"
#include "display.h"
#include "stdio.h"
#include "state_machine.h"
#include "main.h"



//current_state = PowerOff;
//
//event_status = event_null;

extern connection_flag;


/*********************************************
 *
 * Function Name: state_machine()
 *
 * Returns: none
 *
 *********************************************/


void state_machine()
{
	if(connection_flag == 1)
	{

		CORE_irqState_t IRQ_State;

		switch(current_state)
		{
		//State 0:
		case PowerOff:

			if(event_status == SetupLoadPowerManagement)                  // event set by LETIMER UF Interrupt.
			{

				LOG_INFO("\n\nState 0");

				LOG_INFO("current state is %d", current_state);

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				i2c_enable();                                             // LPM ON

				current_state = WaitForPowerUpSequenceComplete;

				timerWaitUs(time_delay);                                  // delay for 80ms

			}



			break;

			// State 1:
		case WaitForPowerUpSequenceComplete:

			if(event_status == StartI2CWriteTransfer)                     // event set by LETIMER COMP1 interrupt.
			{

				LOG_INFO("\n\nState 1");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				SLEEP_SleepBlockBegin(sleepEM2);                         // Blocking the energy mode EM2 and above.
				//Working in EM-1 for I2C.

				Interrupt_write_data();                                  // writing command to temperature sensor.

				current_state = WaitI2CWriteToComplete;
			}



			break;

			// State 2:
		case WaitI2CWriteToComplete:

			if(event_status == SetupTempTimer)                          // Event set by I2C Interrupt.
			{
				LOG_INFO("\n\nState 2");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				current_state = WaitforTemperatureValueReady;

				timerWaitUs(time_delay_1);                             // Delay for 8ms for temperature value ready.

			}

			break;

			// State 3:
		case WaitforTemperatureValueReady:

			if(event_status == StartI2CReadTransfer)                      // event set by LETIMER COMP1 interrupt.
			{

				LOG_INFO("\n\nState 3");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				Interrupt_read_data();                                   // Reading Temperature value from temperature sensor.

				current_state = WaitforI2CAddressReadComplete;

			}

			break;

			// State 4:
		case WaitforI2CAddressReadComplete:

			if(event_status == TemperatureCalculation)                   // Event set by I2C Interrupt.
			{
				LOG_INFO("\n\nState 4");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);


				Temp_cal();                                             // Temperature calculation in degree celsius.

				Temp_cal_BLE();

				//i2c_disable();                                          // LPM OFF.

				SLEEP_SleepBlockEnd(sleepEM2);                          // All energy modes are allowed now.

				current_state = PowerOff;
			}



			break;


		}
	}
}

