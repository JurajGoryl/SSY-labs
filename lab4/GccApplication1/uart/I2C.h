/*
 * I2C.h
 *
 * Created: 3/11/2025 12:28:46
 *  Author: Student
 */ 


#ifndef I2C_H_
#define I2C_H_

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t byte);
uint8_t i2c_readACK(void);
uint8_t i2c_readNACK(void);
uint8_t i2c_getStatus(void);
