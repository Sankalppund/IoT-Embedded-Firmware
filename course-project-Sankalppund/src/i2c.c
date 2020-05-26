/* file : i2c.c
 * Course: IoT Embedded Firmware
 * Date:   09/18/2019
 * Auther: Sankalp Pund
 *
 * Fall 2019
 *
 */

/************************************************
 * Header files
 *************************************************/


#include "i2cspm.h"
#include "i2c.h"
#include "em_i2c.h"
#include "gpio.h"
#include"em_letimer.h"
#include"i2c_interrupt.h"

#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1

#include"log.h"
#include"display.h"

/***************************************************
 * Declaration of Global Variables used in i2c.c
 *
 ****************************************************/

uint32_t CNT_TIME=0;
uint32_t CNT_TIME_DELAY = 0;
uint32_t COMP1_VAR = 0;
uint32_t COMP0_Max = 0;

/*************************************************************************************
 *
 *Function Name: timerWaitUs()
 *
 *This function is a delay function which blocks at least "us_wait" microseconds,
 *using LETIMER0 tick counts as a reference, and which supports waits as long as those
 *needed by the Load Power Management and I2C.
 *
 ***************************************************************************************/

void timerWaitUs(uint32_t us_wait)
{
	CNT_TIME = LETIMER_CounterGet(LETIMER0);


	if(CNT_TIME >= us_wait)
	{
		COMP1_VAR = (CNT_TIME - us_wait);
	}
	else
	{
		COMP0_Max = LETIMER_CompareGet(LETIMER0,0);

		COMP1_VAR = (COMP0_Max - us_wait+ CNT_TIME );
	}

	LETIMER_CompareSet(LETIMER0,1,COMP1_VAR);
	LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP1);

}



