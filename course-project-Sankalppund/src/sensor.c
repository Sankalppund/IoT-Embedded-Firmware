/*
 * sensor.c
 *
 *  Created on: Dec 3, 2019
 *      Author: sankalp pund
 */

/*Header files*/

#include <src/sensor.h>
#include "display.h"
#include "log.h"
#include "i2c_interrupt.h"

#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1



/*
 * Function Name - powerup_proximity(void)
 * Description - This function is used to power on the proximity sensor
 * Inputs - none
 * Return Value - none
 */
void powerup_proximity(void)
{
	I2C_WRITE_WRITE(CONFIGURE_I2C,0x65,1);

	I2C_WRITE_P();
}

/*
 * Function Name - measure_distance(void)
 * Description - This function is used to measure distance from proximity sensor
 * Inputs - none
 * Return Value - uint8_t
 */
uint8_t measure_distance(void)
{
	I2C_WRITE_WRITE(COMMAND,0x81,1);

	reg_val = I2C_READ_P(CONF_REG);

	if(reg_val>PROX_THRESHOLD)
	{
		displayPrintf(DISPLAY_ROW_CLIENTADDR,"EM DOOR OPEN");
	}
	if(reg_val<PROX_THRESHOLD)
	{
		displayPrintf(DISPLAY_ROW_CLIENTADDR,"EM DOOR CLOSED");
	}
	return reg_val;
}


