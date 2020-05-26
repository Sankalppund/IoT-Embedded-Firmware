

/************************************************
 * Header files
 *************************************************/


#include "i2cspm.h"
#include "i2c.h"
//#include "em_i2c.c"
#include "em_i2c.h"
#include "gpio.h"
#include "em_core.h"
//#include "core_cm4.h"

#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1

#include"log.h"
#include "i2c_interrupt.h"

/*********************************************************
 * Declaration of Global Variables used in i2c_interrupt.c
 *
 *********************************************************/

uint8_t Si7021_add_I[1] = {0xE3};
uint16_t Size_of_Buffer_I = sizeof(Si7021_add_I[0]);
uint16_t temp_Sen_Value_I=0;
float Value_1 = 0;
uint8_t read_buffer_I[2];
bool write_status = 0;
bool read_status = 0;



void Interrupt_i2c_init()
{


	//	I2C_Init_TypeDef config_i2c_ITR = I2C_INIT_DEFAULT;                  // Initializing the driver instances."Interrupt"
	//
	//	config_i2c_ITR.enable = true;                                        // Enable when initialization done.
	//
	//	config_i2c_ITR.master = true;                                        // Set to master mode.
	//
	//	I2C_Init(I2C0,&config_i2c_ITR);                                      // Initialize I2C


	NVIC_EnableIRQ(I2C0_IRQn);                                           // Enable interrupts in NVIC

	I2C_IntEnable(I2C0,I2C_IEN_RXDATAV | I2C_IEN_RXFULL | I2C_IEN_TXC ); // Enable I2C Interrupts

}

/***************************************************************************************************
 * Function Name: Interrupt_write_data()
 *
 * This function transfers the writes command from master(i2c) to slave i.e temperature sensor.
 *
 * Reference: https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/structI2C__TransferSeq__TypeDef.html
 *
 ****************************************************************************************************/

void Interrupt_write_data()
{


	I_write_data.addr        = 0x80 ;  // (0x40 << 1)
	I_write_data.flags       = I2C_FLAG_WRITE;
	I_write_data.buf[0].data = Si7021_add_I ;
	I_write_data.buf[0].len  = Size_of_Buffer_I;


	I2C_TransferInit(I2C0,&I_write_data);                        // Start I2C transfer sequence.


	write_status = 1;                                            // Setting the write flag.

}


/****************************************************************************************************
 *
 * Function Name: Interrupt_read_data()
 *
 * This function reads the the temperature from temperature sensor.
 *
 * Reference: https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/structI2C__TransferSeq__TypeDef.html
 *
 ****************************************************************************************************/

void Interrupt_read_data()
{

	//I2C_TransferSeq_TypeDef I_read_data;

	I_read_data.addr        = 0x80 ;  // (0x40 << 1)
	I_read_data.flags       = I2C_FLAG_READ;
	I_read_data.buf[0].data = read_buffer_I;
	I_read_data.buf[0].len  = 2;

	I2C_TransferInit(I2C0,&I_read_data);                                // Start I2C transfer sequence.

	read_status = 1;                                                    // Setting the read flag.
}


/****************************************************************************************************
 *
 * Function Name:  Temp_cal()
 *
 * This function stores the temperature value in a variable temp_Sen_Value_I and then formulates the
 * values of temperature in degree celsius.
 *
 * Reference:https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
 *
 ****************************************************************************************************/

void Temp_cal()
{

	temp_Sen_Value_I|=((uint16_t)(read_buffer_I[0]) << 8);

	temp_Sen_Value_I|= read_buffer_I[1];


	/* Expression for converting temperature in degree Celsius
	 * Reference: https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf
	 */

	Value_1 = ((175.72*(temp_Sen_Value_I)/65536) - 46.85);

	LOG_INFO("Timestamp = %d",loggerGetTimestamp());
	LOG_INFO("Temperature_in_degree_Celsius =  %f ",Value_1);


}

/***************************************************************************************************
 * Function Name: I2C0_IRQHandler()
 *
 * This is I2C Interrupt handler. Checks the status of transfer.
 * Returns successful transfer on complete transfer and return error if failed.
 *
 * Reference: https://www.silabs.com/community/mcu/32-bit/forum.topic.html/interrupt-driveni2c-FM0S
 *
 ****************************************************************************************************/


void I2C0_IRQHandler(void)

{
	CORE_ATOMIC_IRQ_DISABLE();

	I2C_TransferReturn_TypeDef Tran_status = I2C_Transfer(I2C0);

	if(Tran_status == i2cTransferDone)
	{
		if(write_status == 1)
		{
			event_status = SetupTempTimer;

			write_status = 0;                                        // resetting the write flag.
		}
		else if(read_status == 1)
		{
			event_status = TemperatureCalculation;

			read_status = 0;                                         // resetting the read flag.
		}

	}

	if(Tran_status == i2cTransferInProgress)
	{
		LOG_INFO("Successful Transfer in progress & Error Code is = %d",Tran_status);
	}
	else if (Tran_status == i2cTransferNack)
	{
		LOG_INFO("NACK received during transfer & Error Code is = %d",Tran_status);
	}
	else if (Tran_status == i2cTransferBusErr)
	{
		LOG_INFO("Bus error during transfer (misplaced START/STOP) & Error Code is = %d",Tran_status);
	}
	else if (Tran_status == i2cTransferArbLost)
	{
		LOG_INFO("Arbitration lost during transfer & Error Code is = %d",Tran_status);
	}
	else if (Tran_status == i2cTransferUsageFault)
	{
		LOG_INFO("Usage fault & Error Code is = %d",Tran_status);
	}
	else if (Tran_status == i2cTransferSwFault)
	{
		LOG_INFO("SW fault & Error Code is = %d",Tran_status);
	}

	CORE_ATOMIC_IRQ_ENABLE();
}











