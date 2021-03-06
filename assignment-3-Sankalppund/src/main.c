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

int a=0;
uint16_t check_1 = 0;   // flag variable




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


  while (1)
  {

	  if(check_1==1)     // Flag set by interrupt for temperature measurement.
  {
	   i2c_enable();     // LPM ON

	   timerWaitUs(80);  // delay for 80ms

	   write_data();

	   timerWaitUs(8);   // delay for 8ms

	   read_data();

	   Temp_Sense_Value();

	   i2c_disable();   // LPM OFF
  }




//	  delayApproxOneSecond();
//	  gpioLed0SetOff();
//
//	  delayApproxOneSecond();
//	  gpioLed1SetOff();
//
//	  delayApproxOneSecond();
//	  gpioLed1SetOn();
	  //gpioLed0SetOn();

	  SLEEP_Sleep();          // commenting the SLEEP_Sleep() for EM0 mode.
  }
}






