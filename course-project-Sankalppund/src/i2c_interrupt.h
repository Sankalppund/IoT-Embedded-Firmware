

/*
 * i2c_interrupt.h
 *
 *  Created on: Sep 25, 2019
 *      Author: sankalp pund
 */

#ifndef SRC_I2C_INTERRUPT_H_
#define SRC_I2C_INTERRUPT_H_

/* Header file*/

#include "em_i2c.h"

/*Global Variables and  Instances*/

I2C_TransferSeq_TypeDef I_write_data;

I2C_TransferSeq_TypeDef I_read_data;

I2C_TransferSeq_TypeDef I_read_write_data;

uint8_t buffer_tx[2];

uint8_t lux_val;

uint8_t lux_val_buff[2];

uint16_t lux_sen_val;

uint8_t data_rx_1[2];

uint8_t data_rx_p[2];

#define LUX_THRESHOLD (300)

/* Function Prototype*/

void Interrupt_I2C_INIT();

void I2C_WRITE(uint8_t,uint8_t,uint8_t);

void I2C_WRITE_READ(uint8_t,uint8_t);

void I2C_WRITE_WRITE(uint8_t,uint8_t,uint8_t);

void I2C_WRITE_P();

uint8_t I2C_READ_P(uint8_t);

uint16_t light_state(void);

void I2C0_IRQHandler(void);

#endif /* SRC_I2C_INTERRUPT_H_ */




