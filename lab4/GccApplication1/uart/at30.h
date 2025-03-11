/*
 * at30.h
 *
 * Created: 3/11/2025 12:27:53
 *  Author: Student
 */ 


#ifndef AT30_H_
#define AT30_H_
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include "../makra.h"
#include "I2C.h"
#define TemperatureSensorADDRR 0b10010111
#define SerialEEPROMADDRR 0b10100111
#define TemperatureSensorADDRW 0b10010110
#define SerialEEPROMADDRW 0b10100110
#define OS 15
#define R1 14
#define R0 13
#define FT1 12
#define FT0 11
#define POL 10
#define CMPINT 9
#define SD 9
#define NVRBSY 0
#define Tempperature_temp_Register 0x00
#define Tempperature_configuration_Register 0x01
#define Tempperature_Low_Limit_Register 0x02
#define Tempperature_High_Limit_Register 0x03

uint8_t at30_set_precision(uint8_t precision);
float at30_read_temperature(void);