/*
 * state_machine.c
 *
 *  Created on: Oct 17, 2019
 *      Author: sankalp pund
 */

/*Header Files*/

#include"Letimer0.h"
#include"Sleep.h"
#include"log.h"
#include "em_core.h"
#include "i2c_interrupt.h"
#include "main.h"
#include "display.h"
#include "stdio.h"
#include "state_machine.h"
#include "main.h"
#include "gpio.h"

//Logging headers
#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1

/*Global Variables*/

uint8_t lux_flag=0;
uint16_t Ch0, Ch1;
uint8_t SM_State=0;


/*
 * Function Name - state_machine(void)
 * Description - This function a scheduler for LUX sensor
 * Inputs - none
 * Return Value - none.
 */
void state_machine()
{

	if(SM_State==0)
	{
		CORE_irqState_t IRQ_State;

		switch(current_state)
		{
		//State 0:
		case PowerOff:

			if(event_status == SetupLoadPowerManagement)                  // event set by LETIMER UF Interrupt.
			{

				LOG_INFO("\n\nLUX State Machine 1 State 0");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				i2c_enable();                                             // LPM ON

				Interrupt_I2C_INIT();

				current_state = WaitForPowerUpSequenceComplete;

			}

			break;

			// State 1:
		case WaitForPowerUpSequenceComplete:

			if(event_status == StartI2CWriteTransfer)                     // event set by LETIMER COMP1 interrupt.
			{


				LOG_INFO("\n\nLUX State Machine 1 State 1");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				I2C_WRITE(LX_ADDR,LX_CMD_CTR,LX_POWER_ON);

				current_state = WaitI2CWriteToComplete;

			}

			break;

			// State 2:
		case WaitI2CWriteToComplete:

			if(event_status == SetupLuxTimer)                          // Event set by I2C Interrupt.
			{
				LOG_INFO("\n\nLUX State Machine 1 State 2");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				current_state = WaitforLuxValueReady;

			}

			break;

			// State 3:
		case WaitforLuxValueReady:

			if(event_status == StartI2CReadTransfer)                      // event set by LETIMER COMP1 interrupt.
			{

				LOG_INFO("\n\nLUX State Machine 1 State 3");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				if(lux_flag==0)
				{
					I2C_WRITE_READ(LX_ADDR,DATA0);
					lux_flag=1;
				}
				if(lux_flag==1)
				{
					I2C_WRITE_READ(LX_ADDR,DATA1);
					lux_flag=0;
				}

				current_state = WaitforI2CAddressReadComplete;

			}

			break;

			// State 4:
		case WaitforI2CAddressReadComplete:

			if(event_status == LuxCalculation)                   // Event set by I2C Interrupt.
			{
				LOG_INFO("\n\nLUX State Machine 1 State 4");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				light_state();

				current_state = PowerOff;

				SM_State=1;
			}

			break;
		}


	}
}

/*
 * Function Name - state_machine_2(void)
 * Description - This function a scheduler for proximity sensor
 * Inputs - none
 * Return Value - none.
 */

void state_machine_2()
{
	if(SM_State==1)
	{
		CORE_irqState_t IRQ_State;

		switch(current_state)
		{
		//State 0:
		case PowerOff:

			if(event_status == SetupLoadPowerManagement)                  // event set by LETIMER UF Interrupt.
			{

				LOG_INFO("\n\nProximity State Machine 2 State 0");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				i2c_enable();                                             // LPM ON

				Interrupt_I2C_INIT();

				current_state = WaitForPowerUpSequenceComplete;

			}

			break;

			// State 1:
		case WaitForPowerUpSequenceComplete:

			if(event_status == StartI2CWriteTransfer)                     // event set by LETIMER COMP1 interrupt.
			{


				LOG_INFO("\n\Proximity State Machine 2 State 1");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				powerup_proximity();

				current_state = WaitI2CWriteToComplete;

			}

			break;

			// State 2:
		case WaitI2CWriteToComplete:

			if(event_status == SetupLuxTimer)                          // Event set by I2C Interrupt.
			{
				LOG_INFO("\n\nProximity State Machine 2 State 2");

				IRQ_State = CORE_EnterCritical();

				event_status = event_null;

				CORE_ExitCritical(IRQ_State);

				measure_distance();

				current_state = PowerOff;

				SM_State=0;
			}

			break;
		}


	}
}

