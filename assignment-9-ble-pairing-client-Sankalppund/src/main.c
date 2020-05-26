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
#include "state_machine.h"
#include "ble_device_type.h"




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

	//event_status = event_null;

	while (1)
	{

		/***********************************
		 * Ref: soc-smartphone SDK example.
		 ***********************************/
		struct gecko_cmd_packet* evt;
		// Check for stack event.
		evt = gecko_wait_event();
		// Run application and event handler.

		// SERVER
#if (BUILD_INCLUDES_BLE_SERVER==1)

		state_machine();

		ble_event_handler(evt);

#endif

		// Client
#if (BUILD_INCLUDES_BLE_CLIENT==1)

		ble_client(evt);
#endif

	}
}






