/*
 * File Name: main.c
 *
 * Auther: Sankalp Pund
 *
 */

/*Header Files*/

#include <stdbool.h>
#include "native_gecko.h"
#include "log.h"
#include "state_machine.h"
#include "em_core.h"



extern void gecko_main_init();
bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);
extern void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);

int main(void)
{

	// Initialize stack
	gecko_main_init();

	/*Enable Log*/
	logInit();

	/*Enable GPIO*/
	gpioInit();

	/*Enable Display*/
	displayInit();

	gpioEnableDisplay();

	/*Enable LETIMER*/
	LETIMER_setup();

	while(1)
	{
		/*LUX State Machine*/
		state_machine();

		/*Proximity State Machine*/
		state_machine_2();

		/*Mesh Stack handler*/
		struct gecko_cmd_packet *evt = gecko_wait_event();
		bool pass = mesh_bgapi_listener(evt);
		if (pass)
		{
			handle_gecko_event(BGLIB_MSG_ID(evt->header), evt);
		}

	}
}
