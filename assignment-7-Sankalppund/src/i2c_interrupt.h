

/*
 * i2c_interrupt.h
 *
 *  Created on: Sep 25, 2019
 *      Author: sanka
 */

#include "em_i2c.h"

#ifndef SRC_I2C_INTERRUPT_H_
#define SRC_I2C_INTERRUPT_H_


I2C_TransferSeq_TypeDef I_write_data;

I2C_TransferSeq_TypeDef I_read_data;

void Temp_cal_BLE(void);



#endif /* SRC_I2C_INTERRUPT_H_ */




