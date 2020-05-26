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
#include "main.h"
#include "display.h"
#include "stdio.h"


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

	displayInit();

	gpioEnableDisplay();


	// Initialize BLE stack.
	// This is disabled for assignments #2, 3 and 4 as it will prevent sleep modes below EM2.
	gecko_init(&config);

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

	current_state=PowerOff;

	event_status = event_null;

	SetupLoadPowerManagement_flag = 0;

	StartI2CWriteTransfer_flag = 0;

	StartI2CReadTransfer_flag = 0;




	while (1)
	{
		state_machine();
		/***********************************
		 * Ref: soc-smartphone SDK example.
		 ***********************************/
		struct gecko_cmd_packet* evt;
		// Check for stack event.
		evt = gecko_wait_event();
		// Run application and event handler.
		ble_event_handler(evt);
	}
}

/************************************
 *
 * Function name: ble_event_handler()
 *
 * return: none.
 *
 * reference:soc-smartphone SDK example.
 ***************************************/

void ble_event_handler(struct gecko_cmd_packet*evt)
{
	gecko_update(evt);

	switch(BGLIB_MSG_ID(evt->header))
	{
	case gecko_evt_system_boot_id:

		gecko_cmd_le_gap_set_advertise_timing(0,400,400,0,0);

		gecko_cmd_le_gap_start_advertising(0,le_gap_general_discoverable, le_gap_connectable_scannable);

		displayPrintf(DISPLAY_ROW_NAME, "Sankalp Gecko Server");

		displayPrintf(DISPLAY_ROW_BTADDR, "Advertising!!!");

		ble_address();


		break;

	case gecko_evt_le_connection_closed_id:

		gecko_cmd_system_halt(1);
		gecko_cmd_system_set_tx_power(0);
		gecko_cmd_system_halt(0);

		gecko_cmd_le_gap_start_advertising(1,le_gap_general_discoverable, le_gap_connectable_scannable);

		connection_flag = 0;

		displayPrintf(DISPLAY_ROW_CONNECTION, "Disconnect");
		displayPrintf(DISPLAY_ROW_TEMPVALUE,"");

		break;

	case gecko_evt_le_connection_rssi_id:

		Val_of_RSSI = evt->data.evt_le_connection_rssi.rssi;

		if(Val_of_RSSI > -35)
		{
			gecko_cmd_system_set_tx_power(-260);
		}
		else if( -45 < Val_of_RSSI && Val_of_RSSI <= -35)
		{
			gecko_cmd_system_set_tx_power(-200);
		}
		else if( -55 < Val_of_RSSI && Val_of_RSSI <= -45)
		{
			gecko_cmd_system_set_tx_power(-150);
		}
		else if( -65 < Val_of_RSSI && Val_of_RSSI <= -55)
		{
			gecko_cmd_system_set_tx_power(-50);
		}
		else if( -75 < Val_of_RSSI && Val_of_RSSI <= -65)
		{
			gecko_cmd_system_set_tx_power(0);
		}
		else if( -85 < Val_of_RSSI && Val_of_RSSI <= -75)
		{
			gecko_cmd_system_set_tx_power(50);
		}
		else if(Val_of_RSSI <= -85)
		{
			gecko_cmd_system_set_tx_power(80);
		}

		break;

	case gecko_evt_le_connection_opened_id:

		displayPrintf(DISPLAY_ROW_CONNECTION, "Connected");

		connection_value = evt->data.evt_le_connection_opened.connection;

		gecko_cmd_le_connection_set_timing_parameters(connection_value,60,60,3,900,0,0);

		connection_flag = 1;





		break;

	case gecko_evt_system_external_signal_id:

		if((evt->data.evt_system_external_signal.extsignals && SetupLoadPowerManagement_flag==0x01)!=0)
		{
			event_status = SetupLoadPowerManagement;

			SetupLoadPowerManagement_flag = 0;

		}
		if((evt->data.evt_system_external_signal.extsignals && StartI2CWriteTransfer_flag==0x02)!=0)
		{
			event_status = StartI2CWriteTransfer;

			StartI2CWriteTransfer_flag = 0;

		}
		if((evt->data.evt_system_external_signal.extsignals && SetupTempTimer_flag==0x04)!=0)
		{

			event_status = SetupTempTimer;

			SetupTempTimer_flag = 0;

		}
		if((evt->data.evt_system_external_signal.extsignals && StartI2CReadTransfer_flag==0x08)!=0)
		{

			event_status = StartI2CReadTransfer;

			StartI2CReadTransfer_flag = 0;
		}
		if((evt->data.evt_system_external_signal.extsignals && TemperatureCalculation_flag==0x10)!=0)
		{
			event_status = TemperatureCalculation;

			TemperatureCalculation_flag = 0;



		}


		break;

	case gecko_evt_gatt_server_characteristic_status_id:

		if(evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation)
		{
			gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_server_characteristic_status.connection);
		}
		break;

	case gecko_evt_hardware_soft_timer_id:

		displayUpdate();

	}
}

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

				//Temp_cal_BLE();

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






