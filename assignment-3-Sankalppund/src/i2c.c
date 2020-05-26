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

#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1

#include"log.h"

/***************************************************
 * Declaration of Global Variables used in i2c.c
 *
 ****************************************************/

uint8_t Si7021_add[1] = {0xF3};
uint16_t Size_of_Buffer = sizeof(Si7021_add[0]);
uint16_t temp_Sen_Value=0;
uint16_t Value = 0;
uint16_t read_buffer[2];
uint16_t Status_flag=0;
uint32_t CNT_TIME=0;
uint32_t CNT_TIME_DELAY = 0;

/*******************************************************************************************************
 * Function Name = i2c_init()
 *
 * this function configures the i2c peripherals.
 *
 * Reference : https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/structI2CSPM__Init__TypeDef.html
 *******************************************************************************************************/

void i2c_init()
{
	I2CSPM_Init_TypeDef config_i2c = I2CSPM_INIT_DEFAULT;  // Initializing the driver instances.

	config_i2c.sclPort         = gpioPortC;                // port c for SCL pin
	config_i2c.sdaPort         = gpioPortC;                // port c for SDA pin
	config_i2c.sclPin          = (10);                     // select pin 10 for SCL pin
	config_i2c.sdaPin          = (11);                     // select pin 11 for SCL pin
	config_i2c.portLocationScl = (14);                     // Port location is 14 for SCL pin
	config_i2c.portLocationSda = (16);                     // Port location is 16 for SCL pin


	I2CSPM_Init(&config_i2c);                              // Enabling the clocks for i2c

}

/***************************************************************************************************
 * Function Name: write_data()
 *
 * This function transfers the writes command from master(i2c) to slave i.e temperature sensor.
 *
 * Refernce: https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/structI2C__TransferSeq__TypeDef.html
 *
 ****************************************************************************************************/

void write_data()
{
	I2C_TransferSeq_TypeDef write_data;

	write_data.addr        = 0x80 ;  // (0x40 << 1)
	write_data.flags       = I2C_FLAG_WRITE;
	write_data.buf[0].data = Si7021_add ;
	write_data.buf[0].len  = Size_of_Buffer;


	I2CSPM_Transfer(I2C0,&write_data);
}

/****************************************************************************************************
 *
 * Function Name: read_data()
 *
 * This function reads the the temperature from temperature sensor.
 *
 * Refernce: https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/structI2C__TransferSeq__TypeDef.html
 *
 ****************************************************************************************************/

void read_data()
{
	I2C_TransferSeq_TypeDef read_data;

	read_data.addr        = 0x80 ;  // (0x40 << 1)
	read_data.flags       = I2C_FLAG_READ;
	read_data.buf[0].data = read_buffer;
	read_data.buf[0].len  = 1;
	read_data.buf[1].data = read_buffer;
	read_data.buf[1].len  = 1;



   I2CSPM_Transfer(I2C0,&read_data);

   Status_flag=I2CSPM_Transfer(I2C0,&read_data);

}

/****************************************************************************************************
 *
 *Function Name:  Temp_Sense_Value()
 *
 * This function stores the temperature value in a variable temp_Sen_Value and then formulates the
 * values of temperature in degree celsius.
 *
 * Reference:https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
 *
 ****************************************************************************************************/

void Temp_Sense_Value()
{
	temp_Sen_Value|=((uint16_t)(read_buffer[0]) << 8);

	temp_Sen_Value|= read_buffer[1];


/* Expression for converting temperature in degree Celsius
 * Reference: https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
 */


	Value = ((175.72*(temp_Sen_Value)/65536) - 46.85);

	   if(Status_flag==1)
	   {
	      	 LOG_INFO("\n transferring data");
	   }

	   if(Status_flag==0)
	   {
		   LOG_INFO("\n Data transfer is successful");
	   }
	   if(Status_flag==(-1))
	   {
	   	 LOG_INFO("\n receiving the NACK");
	   }

	   LOG_INFO( "Temperature_in_degree_Celsius = %d" ,Value);


}

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

	CNT_TIME_DELAY = (CNT_TIME - us_wait);

	while((LETIMER_CounterGet(LETIMER0)) != (CNT_TIME_DELAY));

}










