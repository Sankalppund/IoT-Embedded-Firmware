/*
 * Letimer0.h
 *
 *  Created on: Sep 11, 2019
 *      Author: sankalp
 */

#ifndef SRC_LETIMER0_H_
#define SRC_LETIMER0_H_


// Enter Values below to select Total time period, LED On period and Energy Modes.

#define Time_Period_1 (3000)   // Enter Total time period value in milliseconds.
#define LED_On_Time_2 (0)   // Enter LED_ON time period value in milliseconds.
#define Energy_Mode (3)        // Enter Value 0,1,2,3 for Energy Mode EM0,EM1, EM2 & EM3 respectively.

void LETIMER0_IRQHandler(void); // Function Prototype
void LETIMER_setup(void); // Function Prototype

#endif /* SRC_LETIMER0_H_ */
