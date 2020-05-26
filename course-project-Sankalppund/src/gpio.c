/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

/*Header files*/

#include "gpio.h"
#include "em_gpio.h"
#include <string.h>
#include "core_cm4.h"
#include "log.h"
#include "em_core.h"

/*Global Variables*/

uint32_t Button_flag = 0;

uint32_t Button_flag_1 = 0;

uint8_t PB0 = 0;

uint8_t PB1 = 0;



/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define	LED0_port   (gpioPortF)
#define LED0_pin	(4)
#define LED1_port   (gpioPortF)
#define LED1_pin    (5)


/*
 * Function Name - gpioInit(void)
 * Description - This function initializes gpio ports, pins and interrupts.
 * Inputs - none
 * Return Value - none
 */

void gpioInit()
{
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);
	GPIO_PinModeSet(Sensor_port,Sensor_pin, gpioModePushPull, false);
	GPIO_PinModeSet(PB0_PORT,PB0_PIN,gpioModeInput, true);
	GPIO_IntConfig(PB0_PORT,PB0_PIN,true,true,true);
	GPIO_PinModeSet(PB1_PORT,PB1_PIN,gpioModeInput, true);
	GPIO_IntConfig(PB1_PORT,PB1_PIN,true,true,true);

	/* Set Mode for Proximity Sensor*/
	//GPIO_PinModeSet(PROXIMITY_PORT,PROXIMITY_PIN,gpioModeInput, true);

	/* Configure and Enable GPIO Interrupt For PROXIMITY SENSOR*/
	//GPIO_IntConfig(PROXIMITY_PORT,PROXIMITY_PIN,false,true,true);

	/*Enable Even and Odd NVIC Interrupts*/
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/* Clear Pending Interrupt */
	GPIO_IntClear(GPIO_IntGet());

	GPIO_IntGet();


	/* Configure GPIO Even and Odd interrupt.*/

	GPIOINT_CallbackRegister(PB0_PIN, gpio_handler_PB0);

	GPIOINT_CallbackRegister(PB1_PIN, gpio_handler_PB1);

}

/*Function to switch on LED0*/

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}

/*Function to switch off LED0*/

void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}

/*Function to switch on LED1*/

void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}

/*Function to switch off LED1*/

void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}

/*Function to Enable display*/

void gpioEnableDisplay()
{
	GPIO_PinOutSet(LCD_ENABLE_PORT,LCD_ENABLE_PIN);
}

void gpioSetDisplayExtcomin(bool high)
{
	if(high==true)
	{
		GPIO_PinOutSet(LCD_ENABLE_PORT,LCD_PIN);
	}
	if(high==false)
	{
		GPIO_PinOutClear(LCD_ENABLE_PORT,LCD_PIN);
	}

}

/******************************************
 * Function Name: i2c_enable()
 *
 * function enables load power management.
 *
 ******************************************/
void i2c_enable()
{
	GPIO_PinOutSet(Sensor_port,Sensor_pin);
}

/******************************************
 * Function Name: i2c_disable()
 *
 * function disables load power management.
 *
 ******************************************/



void i2c_disable()
{
	GPIO_PinOutClear(Sensor_port,Sensor_pin);
}




/***************************************************************************************************
 * Function Name: GPIO_EVEN_IRQHandler()
 *
 * This is GPIO Interrupt Handler.
 *
 * Handles interrupt of PB0.
 *
 * Reference: https:https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg1/html/gpiointerrupt_8c_source.html
 *
 ****************************************************************************************************/
void gpio_handler_PB0()
{
	CORE_ATOMIC_IRQ_DISABLE();

	PB0 ^= 1;

	LOG_INFO("GPIO INTERRUPT HANDLER: %d\n", PB0);

	Button_flag |= PB0_EVENT;

	gecko_external_signal(Button_flag);

	CORE_ATOMIC_IRQ_ENABLE();
}

/***************************************************************************************************
 * Function Name: GPIO_ODD_IRQHandler()
 *
 * This is GPIO Interrupt Handler.
 *
 * Handles interrupt of PB1.
 *
 * Reference: https:https://siliconlabs.github.io/Gecko_SDK_Doc/efr32bg1/html/gpiointerrupt_8c_source.html
 *
 ****************************************************************************************************/
void gpio_handler_PB1()
{
	CORE_ATOMIC_IRQ_DISABLE();

	PB1 ^= 1;

	LOG_INFO("GPIO INTERRUPT HANDLER: %d\n", PB1);

	Button_flag_1 |= PB1_EVENT;

	gecko_external_signal(Button_flag_1);

	CORE_ATOMIC_IRQ_ENABLE();
}
