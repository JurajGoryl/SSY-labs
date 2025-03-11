/*
 * I2C.c
 *
 * Created: 3/11/2025 12:28:28
 *  Author: Student
 */ 


#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include "../makra.h"

void i2c_init(void)
{
	TWSR = 0x00;
	TWBR = 0x02;
	
	TWCR = (1<<TWEN);
}

void i2c_start ( void )
{
	TWCR = (1<<TWINT) |(1<<TWSTA) |(1<<TWEN) ;
	while ( (TWCR & (1<<TWINT) ) == 0 ) ;
}

void i2c_stop ( void )
{
	TWCR = (1<<TWINT) |(1<<TWSTO) |(1<<TWEN) ;
}

void i2c_write ( uint8_t u8data )
{
	TWDR = u8data ;
	TWCR = (1<<TWINT) |(1<<TWEN) ;
	while ( (TWCR & (1<<TWINT) ) == 0 ) ;
}

uint8_t i2c_readACK ( void )
{
	TWCR = (1<<TWINT) |(1<<TWEN) |(1<<TWEA) ;
	while ( (TWCR & (1<<TWINT) ) == 0 ) ;
	return TWDR;
}

uint8_t i2c_readNACK ( void )
{
	TWCR = (1<<TWINT) |(1<<TWEN) ;
	while ( (TWCR & (1<<TWINT) ) == 0 ) ;
	return TWDR;
}

uint8_t i2c_get_status ( void )
{
	uint8_t status;
	status = TWSR & 0xF8 ;
	return status;
}