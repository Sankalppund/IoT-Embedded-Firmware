/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "em_device.h"
#include "em_chip.h"
#include "gpio.h"
#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

#include"Letimer0.h"
#include"Sleep.h"

//Logging headers
#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1
#include"log.h"
#include "em_core.h"
#include "i2c_interrupt.h"

uint16_t check_1 = 0;   // flag variable

CORE_DECLARE_IRQ_STATE;

#define time_delay (80)
#define time_delay_1 (8)





uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
		.config_flags = 0,
		.sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
		.bluetooth.max_connections = MAX_CONNECTIONS,
		.bluetooth.heap = bluetooth_stack_heap,
		.bluetooth.heap_size = sizeof(bluetooth_stack_heap),
		.bluetooth.sleep_clock_accuracy = 100, // ppm
		.gattdb = &bg_gattdb_data,
		.ota.flags = 0,
		.ota.device_name_len = 3,
		.ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
		.pa.config_enable = 1, // Enable high power PA
		.pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};


int main(void)
{
	// Initialize device
	initMcu();
	// Initialize board
	initBoard();
	// Initialize application
	initApp();
	// Initialize logging module
	logInit();

	gpioInit();


	// Initialize BLE stack.
	// This is disabled for assignments #2, 3 and 4 as it will prevent sleep modes below EM2.
	// gecko_init(&config);

	/* Infinite loop */

	LETIMER_setup();



	if(Energy_Mode==1||Energy_Mode==2)
	{
		SLEEP_SleepBlockBegin(Energy_Mode+1);
	}
	if(Energy_Mode==3)
	{
		EMU_EnterEM3(true);
	}

	//Initialize i2c

	i2c_init();

	//Initialize i2c_interrupt

	Interrupt_i2c_init();

	current_state = PowerOff;


	while (1)
	{

		//		if(check_1==1)                         // Flag set by interrupt for temperature measurement(Polling).
		//		{
		//
		//			CORE_ENTER_CRITICAL();
		//
		//			i2c_enable();                      // LPM ON
		//
		//			timerWaitUs(time_delay);           // delay for 80ms
		//
		//			//write_data();
		//
		//			Interrupt_write_data();
		//
		//			timerWaitUs(time_delay_1);         // delay for 8ms
		//
		//			//read_data();
		//
		//			Interrupt_read_data();
		//
		//			//Temp_Sense_Value();
		//
		//			Temp_cal();
		//
		//			check_1 = 0;                     // Clearing the event flag.
		//
		//			i2c_disable();                   // LPM OFF
		//
		//			CORE_EXIT_CRITICAL();
		//		}




		// State Machine Implementation.//

		CORE_irqState_t IRQ_State;

		switch(current_state)
		{
		// State 0:
		case PowerOff:

			IRQ_State = CORE_EnterCritical();

			if(event_status == SetupLoadPowerManagement)                  // event set by LETIMER UF Interrupt.
			{

				LOG_INFO("\n\nState 0");

				event_status = event_null;

				i2c_enable();                                             // LPM ON

				timerWaitUs(time_delay);                                  // delay for 80ms

				next_state = WaitForPowerUpSequenceComplete;

				current_state = next_state;



			}

			CORE_ExitCritical(IRQ_State);

			break;

			// State 1:
		case WaitForPowerUpSequenceComplete:

			IRQ_State = CORE_EnterCritical();

			if(event_status == StartI2CWriteTransfer)                     // event set by LETIMER COMP1 interrupt.
			{

				LOG_INFO("\n\nState 1");

				event_status = event_null;

				SLEEP_SleepBlockBegin(sleepEM2);                         // Blocking the energy mode EM2 and above.
				//Working in EM-1 for I2C.

				Interrupt_write_data();                                  // writing command to temperature sensor.

				next_state = WaitI2CWriteToComplete;

				current_state = next_state;



			}

			CORE_ExitCritical(IRQ_State);

			break;

			// State 2:
		case WaitI2CWriteToComplete:

			IRQ_State = CORE_EnterCritical();

			if(event_status == SetupTempTimer)                          // Event set by I2C Interrupt.
			{
				LOG_INFO("\n\nState 2");

				event_status = event_null;

				timerWaitUs(time_delay_1);                             // Delay for 8ms for temperature value ready.


				next_state = WaitforTemperatureValueReady;

				current_state = next_state;



			}

			CORE_ExitCritical(IRQ_State);

			break;

			// State 3:
		case WaitforTemperatureValueReady:

			IRQ_State = CORE_EnterCritical();

			if(event_status == StartI2CReadTransfer)                      // event set by LETIMER COMP1 interrupt.
			{

				LOG_INFO("\n\nState 3");

				event_status = event_null;

				Interrupt_read_data();                                   // Reading Temperature value from temperature sensor.


				next_state = WaitforI2CAddressReadComplete;

				current_state = next_state;


			}

			CORE_ExitCritical(IRQ_State);


			break;

			// State 4:
		case WaitforI2CAddressReadComplete:

			IRQ_State = CORE_EnterCritical();

			if(event_status == TemperatureCalculation)                   // Event set by I2C Interrupt.
			{
				LOG_INFO("\n\nState 4");

				event_status = event_null;

				Temp_cal();                                             // Temperature calculation in degree celsius.

				i2c_disable();                                          // LPM OFF.

				SLEEP_SleepBlockEnd(sleepEM2);                          // All energy modes are allowed now.

				next_state = PowerOff;

				current_state = next_state;
			}

			CORE_ExitCritical(IRQ_State);

			break;


		}

		if(event_status == event_null)                             // sleeping when there is no events.
		{
			SLEEP_Sleep();                                         // commenting the SLEEP_Sleep() for EM0 mode.
		}


		//	  delayApproxOneSecond();
		//	  gpioLed0SetOff();
		//
		//	  delayApproxOneSecond();
		//	  gpioLed1SetOff();
		//
		//	  delayApproxOneSecond();
		//	  gpioLed1SetOn();
		//    gpioLed0SetOn();


	}
}






