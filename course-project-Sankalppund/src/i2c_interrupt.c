/*
 * File Name: i2c_interrupt.c
 *
 * Auther: Sankalp Pund
 *
 */

/************************************************
 * Header files
 *************************************************/


#include "i2cspm.h"
#include "i2c.h"
#include "em_i2c.h"
#include "gpio.h"
#include "em_core.h"
#include "log.h"
#include "i2c_interrupt.h"
#include "gatt_db.h"
#include "infrastructure.h"
#include "native_gecko.h"
#include "display.h"
#include "state_machine.h"


#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1


/*********************************************************
 * Declaration of Global Variables used in i2c_interrupt.c
 *
 *********************************************************/

bool write_status = 0;
bool read_status = 0;
bool write_status_p = 0;
uint32_t SetupLuxTimer_flag = 0;
uint32_t LuxCalculation_flag = 0;
uint32_t SetupProxTimer_flag = 0;
uint16_t final_data=0;
uint16_t prox_val;
/*
 * Function Name - Interrupt_I2C_INIT()
 * Description - This function initialize the I2C0
 * Inputs - none
 * Return Value - none.
 */

void Interrupt_I2C_INIT()
{
	I2CSPM_Init_TypeDef config_i2c = I2CSPM_INIT_DEFAULT;      // Initializing the driver instances.


	config_i2c.sclPort         = gpioPortC;                 // port c for SCL pin
	config_i2c.sdaPort         = gpioPortC;                 // port c for SDA pin
	config_i2c.sclPin          = (10);                      // select pin 10 for SCL pin
	config_i2c.sdaPin          = (11);                      // select pin 11 for SCL pin
	config_i2c.portLocationScl = (14);                      // Port location is 14 for SCL pin
	config_i2c.portLocationSda = (16);                      // Port location is 16 for SCL pin


	I2CSPM_Init(&config_i2c);                               // Enabling the clocks for i2c

	NVIC_EnableIRQ(I2C0_IRQn);                                           // Enable interrupts in NVIC

	I2C_IntEnable(I2C0,I2C_IEN_RXDATAV | I2C_IEN_RXFULL | I2C_IEN_TXC ); // Enable I2C Interrupts

}

/*
 * Function Name - I2C_WRITE()
 * Description - This function used to write data to through I2C0 with repeat start condition
 * Inputs - uint8_t device_address, uint8_t command to be send, uint8_t data to be written
 * Return Value - none.
 */

void I2C_WRITE(uint8_t device_address, uint8_t command, uint8_t data)
{
	I_write_data.addr = device_address<<1;

	I_write_data.flags = I2C_FLAG_WRITE_WRITE;

	I_write_data.buf[0].data = &command;

	I_write_data.buf[1].data = &data;

	I_write_data.buf[0].len = 1;

	I_write_data.buf[1].len = 1;

	write_status=1;

	I2C_TransferReturn_TypeDef transfer_status = I2C_TransferInit( I2C0, &I_write_data);

	if (transfer_status != i2cTransferInProgress)
	{
		LOG_INFO("Failed");
	}
	else
		LOG_INFO("Transferring write command");


}

/*
 * Function Name - I2C_WRITE_READ()
 * Description - This function used to write and read data through I2C0
 * Inputs - uint8_t device_address, uint8_t command against which data is needed.
 * Return Value - none.
 */

void I2C_WRITE_READ(uint8_t device_address, uint8_t command)
{
	I_read_data.addr = device_address<<1;

	I_read_data.flags = I2C_FLAG_WRITE_READ;

	I_read_data.buf[0].data	=	&command;

	I_read_data.buf[0].len	=	1;

	I_read_data.buf[1].data	=	data_rx_1;

	I_read_data.buf[1].len	=	2;

	read_status=1;

	I2C_TransferReturn_TypeDef transfer_status = I2C_TransferInit( I2C0, &I_read_data);

	if (transfer_status != i2cTransferInProgress)
	{
		LOG_INFO("Failed");
	}
	if(transfer_status=i2cTransferDone)
	{
		LOG_INFO("I2C_read_word done");
	}
	else
		LOG_INFO("",data_rx_1[0]);
	    LOG_INFO("",data_rx_1[1]);


}

/*
 * Function Name - I2C_WRITE_WRITE()
 * Description - This function used to write data to proximity sensor with repeat start condition.
 * Inputs - uint8_t command, uint8_t command_1,uint8_t length.
 * Return Value - none.
 */

void I2C_WRITE_WRITE(uint8_t command, uint8_t command_1,uint8_t length)
{
	I2C_TransferSeq_TypeDef write_data;

	write_data.addr        = 0x4C<<1 ;

	write_data.flags       = I2C_FLAG_WRITE_WRITE;

	write_data.buf[0].data = &command;

	write_data.buf[1].data = &command_1;

	write_data.buf[0].len  = 1;

	if(length==1)
	{
		write_data.buf[1].len  = 1;
	}
	if(length==2)
	{
		write_data.buf[1].len  = 2;
	}
	I2C_TransferReturn_TypeDef transfer_status = I2C_TransferInit( I2C0, &write_data);

	if (transfer_status != i2cTransferInProgress)
	{
		LOG_INFO("Failed");
	}
	else
		LOG_INFO("Transferring write command");
}

/*
 * Function Name - I2C_WRITE_WRITE()
 * Description - This function used to write data to proximity sensor with repeat start condition.
 * Inputs - uint8_t command, uint8_t command_1,uint8_t length.
 * Return Value - none.
 */
void I2C_WRITE_P()
{
	I2C_TransferSeq_TypeDef write_data;

	uint8_t command[3]={0x14,0x00,0x06};

	write_data.addr        = 0x4C<<1 ;  // (0x40 << 1)

	write_data.flags       = I2C_FLAG_WRITE;

	write_data.buf[0].data = command;

	write_data.buf[0].len  = 3;

	write_status_p=1;

	I2C_TransferReturn_TypeDef transfer_status = I2C_TransferInit( I2C0, &write_data);

	if (transfer_status != i2cTransferInProgress)
	{
		LOG_INFO("Failed");
	}
	else
		LOG_INFO("Transferring write command");
}

/*
 * Function Name - I2C_READ_P()
 * Description - This function used to read data from proximity sensor.
 * Inputs - uint8_t command
 * Return Value - uint8_t.
 */

uint8_t I2C_READ_P(uint8_t command)
{
	uint16_t val;

	I_read_data.addr = 0x4C<<1;

	I_read_data.flags = I2C_FLAG_WRITE_READ;

	I_read_data.buf[0].data	=	&command;

	I_read_data.buf[0].len	=	1;

	I_read_data.buf[1].data	=	data_rx_p;

	I_read_data.buf[1].len	=	2;


	I2C_TransferReturn_TypeDef transfer_status = I2C_TransferInit( I2C0, &I_read_data);


	val|=data_rx_p[1]<<8;
	val|=data_rx_p[0];

	LOG_INFO("Proximity Value is %d",val);

	if (transfer_status != i2cTransferInProgress)
	{
		LOG_INFO("Failed");
	}
	if(transfer_status=i2cTransferDone)
	{
		LOG_INFO("I2C_read_word done");
	}
	else
		LOG_INFO("Buffer 0=%d",data_rx_p[0]);
	    LOG_INFO("Buffer 1=%d",data_rx_p[1]);

	prox_val=data_rx_p[1];

	return prox_val;
}
/*
 * Function Name - I2C0_IRQHandler(void)
 * Description - This function is an IRQ handler for I2C0 Interrupts.
 * Inputs - none.
 * Return Value - none.
 */

void I2C0_IRQHandler(void)

{
	CORE_ATOMIC_IRQ_DISABLE();

	I2C_TransferReturn_TypeDef Tran_status = I2C_Transfer(I2C0);

	if(Tran_status == i2cTransferDone)
	{
		LOG_INFO("I2C transfer done in ISR now");

		if(write_status == 1)
		{
			//event_status = SetupLuxTimer;

			SetupLuxTimer_flag |= 0x04;

			gecko_external_signal(SetupLuxTimer_flag);

			write_status = 0;                                        // resetting the write flag.
		}
		else if(read_status == 1)
		{
			//event_status = LuxCalculation;

			LuxCalculation_flag |= 0x10;

			gecko_external_signal(LuxCalculation_flag);

			read_status = 0;                                         // resetting the read flag.
		}
		if(write_status_p == 1)
		{
			//event_status = SetupLuxTimer;

			SetupProxTimer_flag |= 0x15;

			gecko_external_signal(SetupProxTimer_flag);

			write_status_p = 0;                                        // resetting the write flag.
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

/*
 * Function Name - light_state(void)
 * Description - This function is to calculate the light intensity from LUX sensor.
 * Inputs - none.
 * Return Value - uint16_t lux value
 */

uint16_t light_state(void)
{
	/* 	Storing received data in a 16-bit variable */
	lux_sen_val = 0;

	lux_sen_val	=	data_rx_1[1];

	lux_sen_val	= lux_sen_val << 8;

	lux_sen_val	|=	data_rx_1[0];

	final_data = (uint16_t)(lux_sen_val);

	LOG_INFO("Raw_Value = %d", final_data);

	if(final_data>LUX_THRESHOLD)
	{
		displayPrintf(DISPLAY_ROW_BTADDR2,"W/H LIGHTS ON");
	}
	if(final_data<LUX_THRESHOLD)
	{
		displayPrintf(DISPLAY_ROW_BTADDR2,"W/H LIGHTS OFF");
	}

	return final_data;
}























































































































































