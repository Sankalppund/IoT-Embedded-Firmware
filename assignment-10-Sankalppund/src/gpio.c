/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include "gpio.h"
#include "em_gpio.h"
#include <string.h>
#include "core_cm4.h"
#include "log.h"
#include "em_core.h"

uint32_t Button_flag = 0;

uint8_t PB0 = 0;

/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define	LED0_port   (gpioPortF)
#define LED0_pin	(4)
#define LED1_port   (gpioPortF)
#define LED1_pin    (5)





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

	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	/* Clear Pending Interrupt */
	GPIO_IntClear(GPIO_IntGet());
	/* Configure GPIO interrupt.*/

	GPIOINT_CallbackRegister(PB0_PIN, gpio_handler);

}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}
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
void gpio_handler()
{
	CORE_ATOMIC_IRQ_DISABLE();

	PB0 ^= 1;

	LOG_INFO("GPIO INTERRUPT HANDLER: %d\n", PB0);

	Button_flag |= 0x11;

	gecko_external_signal(Button_flag);

	CORE_ATOMIC_IRQ_ENABLE();
}



