
/* Letimer0.c
 *
 * created on: Sep 11, 2019
 * Auther: Sankalp Pund
 */


/*Header Files*/

#include "em_letimer.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_core.h"
#include "Letimer0.h"
#include <sleep.h>
#include <em_emu.h>
#include <stdbool.h>
#include "log.h"
#include "native_gecko.h"
#include "state_machine.h"


/*Global Variables*/

uint32_t overflow_count =0;
uint32_t current_time = 0;
uint32_t count_of_milliseconds = 0;
extern state current_state;

uint32_t SetupLoadPowerManagement_flag;

uint32_t StartI2CWriteTransfer_flag;

uint32_t StartI2CReadTransfer_flag;

uint32_t SetupLoadPowerManagementProx_flag;

uint32_t StartI2CWriteTransfer_flag_Prox;

/*
 * Function Name - LETIMER_setup(void)
 * Description - This initializes LETIMER, Clocks, interrupts and energy modes.
 * Inputs - none
 * Return Value - none.
 */

void LETIMER_setup(void)
{

#if(Energy_Mode==0|Energy_Mode==1|Energy_Mode==2)

	CMU_OscillatorEnable(cmuOsc_LFXO,true,true);     // Enable LFXO Oscillator for EM0,EM1 and EM2
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);// Clock source is LFXO
	CMU_ClockDivSet(cmuClock_LETIMER0, 4);           // prescaling the frequency

	// since LETIMER0 is 16 bit counter with 32768Hz frequency. No. of ticks are 65536.
	// Prescaling the frequency of clock by 4 which is equal to 8192.
	// so COMP0=frequency*Time_Period & COMP1=frequency*Time_Period

#define Time_Period ((Time_Period_1*8192)/1000)   // #define to take Time_Period_1 values from Letimer.h file
#define LED_On_Time ((LED_On_Time_2*8192)/1000)   // #define to take LED_On_Time_2 values from Letimer.h file

#endif

#if(Energy_Mode==3)

	CMU_OscillatorEnable(cmuOsc_ULFRCO,true,true);     // Enable ULFRCO Oscillator for EM3
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);// Clock source is ULFRCO

	// since LETIMER0 is 16 bit counter . No. of ticks are 65536.
	// Frequency of clock = 1000Hz.
	// so COMP0=frequency*Time_Period & COMP1=frequency*Time_Period

#define Time_Period ((Time_Period_1*1000)/1000)    // #define to take Time_Period_1 values from Letimer.h file
#define LED_On_Time ((LED_On_Time_2*1000)/1000)    // #define to take LED_On_Time_2 values from Letimer.h file

#endif

	CMU_ClockEnable(cmuClock_LFA, true);          // enable the LFA parent clock
	CMU_ClockEnable(cmuClock_LETIMER0, true);     // enable the LETIMER0 clock


	//Link for reference used for below code is(https://www.silabs.com/community/wireless/bluetooth/knowledge-base.entry.html/2017/08/17/using_low_energytim-9RYB).
	// Link for APIs used below is (https://siliconlabs.github.io/Gecko_SDK_Doc/efm32wg/html/group__emlib.html)
	// Set configurations for LETIMER 0

	const LETIMER_Init_TypeDef config = {
			.enable = true,               /* Start counting when init completed. */
			.debugRun = false,            /* Counter shall not keep running during debug halt. */
			.comp0Top = true,             /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
			.bufTop = false,              /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
			.out0Pol = 0,                 /* Idle value for output 0. */
			.out1Pol = 0,                 /* Idle value for output 1. */
			.ufoa0 = letimerUFOAPwm,      /* PWM output on output 0 */
			.ufoa1 = letimerUFOAPwm,      /* PWM output on output 1*/
			.repMode = letimerRepeatFree  /* Count until stopped */
	};


	LETIMER_Init(LETIMER0, &config);  // Initialize LETIMER

	LETIMER_CompareSet(LETIMER0, 0, Time_Period);  // Taking values from #define Time_Period for COMP0
	LETIMER_CompareSet(LETIMER0, 1, LED_On_Time); // Taking values from #define LED_On_Time for COMP1



	NVIC_EnableIRQ( LETIMER0_IRQn );    // Enabling the Nested Interrupt.

	LETIMER_IntEnable(LETIMER0,LETIMER_IFS_UF|LETIMER_IFS_COMP1 );   //enabling LETIMER0 interrupt.
	LETIMER_Enable(LETIMER0,true);
}


/*
 * Function Name - LETIMER0_IRQHandler(void)
 * Description - This function is an ISR for LETIMER0
 * Inputs - none
 * Return Value - none.
 */
void LETIMER0_IRQHandler(void)  // IRQ Handler
{
	CORE_ATOMIC_IRQ_DISABLE();

	uint32_t store_int = LETIMER_IntGetEnabled( LETIMER0 ); // Storing inturrupt register value of LETIMER in temp variable store_int
	LETIMER_IntClear( LETIMER0, store_int );  // clear LETIMER inturrupt.



	if( store_int & LETIMER_IFS_COMP1)  // Condition check for COM1
	{

		if (current_state == WaitForPowerUpSequenceComplete)
		{

			//event_status = StartI2CWriteTransfer;

			StartI2CWriteTransfer_flag |= 0x02;

			gecko_external_signal(StartI2CWriteTransfer_flag);

			StartI2CWriteTransfer_flag_Prox |= 0x14;

			gecko_external_signal(StartI2CWriteTransfer_flag_Prox);

		}
		else if (current_state == WaitforLuxValueReady)
		{
			//event_status = StartI2CReadTransfer;

			StartI2CReadTransfer_flag |= 0x08;

			gecko_external_signal(StartI2CReadTransfer_flag);
		}

	}


	if( store_int & LETIMER_IFS_UF)   // Condition check for Under_flow
	{

		overflow_count++;

		current_time = LETIMER_CounterGet(LETIMER0);

		count_of_milliseconds = ((overflow_count * 3000)+ current_time);  // Time-stamp calculation.

		//event_status = SetupLoadPowerManagement;

		SetupLoadPowerManagement_flag |= 0x01;

		gecko_external_signal(SetupLoadPowerManagement_flag);

		SetupLoadPowerManagementProx_flag |= 0x13;

		gecko_external_signal(SetupLoadPowerManagementProx_flag);
	}

	CORE_ATOMIC_IRQ_ENABLE();
}











