//
// COMP-GENG 422 - Koby Soden
//
// I2C module
//


#include "main.h"
#include "i2c.h"
#include "eprintf.h"


#define I2C_REQUEST_READ	0x01
#define I2C_REQUEST_WRITE	0x00


void I2cInit(void)
{
	// Enable the clock for I2C1
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

	// Configure I2C1 for 400KHz
	I2C1->CR2 = 0x0020;		// APB1 freq: 32MHz
	I2C1->CCR = 0x801B;		// Fm mode, Tlow/Thigh: 2, CCR: 27
	I2C1->TRISE = 0x0009;	// Fm mode Trise(Max) = 300nS; 300/31.25 = 9.6, use integer part
	I2C1->CR1 = 0x0001;		// PE: 1 (enable I2C1)

	// Enable the clock for GPIO Port B
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	// Configure PB9, PB8 for I2C1 SDA, SCL
	GPIOB->AFR[1] = 0x00000044;		// PB9: AF4 (I2C1_SDA), PB8: AF4 (I2C1_SCL)
	GPIOB->MODER |= 0x000A0000;		// PB9, PB8: alt fcn mode
}


void I2cWriteBuf(UInt8 DevAddr, UInt8 RegAddr, int NumBytes, UInt8 * DataBuf)
{
	// Generate start
	I2C1->CR1 |= 0x0100;

	// Wait for SB to be set
	while (!(I2C1->SR1 & 0x0001))
		;

	//Send device address in write mode across line
	I2C1->DR = DevAddr | I2C_REQUEST_WRITE;

	// Wait for ADDR to be set
	while (!(I2C1->SR1 & 0x0002))
		;

	I2C1->SR2;				// reading SR2 clears ADDR

	if (NumBytes == 1)
	{
		I2C1->DR = RegAddr;
	}
	else
	{
		I2C1->DR = RegAddr | 0x80;		//auto increment per accelerometer datasheet
	}
	// Wait for TxE to be set
	while (!(I2C1->SR1 & 0x0080))
		;

	//data write loop
	while(NumBytes > 0)
	{

		//add current data to databuffer
		I2C1->DR = *DataBuf;
		DataBuf++;

		// Wait for TxE to be set
		while (!(I2C1->SR1 & 0x0080))
			;

		NumBytes--;
	}
	// Generate stop
	I2C1->CR1 |= 0x0200;
}


void I2cReadBuf(UInt8 DevAddr, UInt8 RegAddr, int NumBytes, UInt8 * DataBuf)
{
	// Generate start
	I2C1->CR1 |= 0x0100;

	//might need to wait here
	// Wait for SB to be set
	while (!(I2C1->SR1 & 0x0001))
		;

	//Send device address in write mode across line
	I2C1->DR = DevAddr | I2C_REQUEST_WRITE;

	// Wait for ADDR to be set
	while (!(I2C1->SR1 & 0x0002))
		;

	I2C1->SR2;							// reading SR2 clears ADDR
	if (NumBytes > 1)
	{
		RegAddr |= 0x80;				//auto increment per accelerometer datasheet
	}
	I2C1->DR = RegAddr;

	// Wait for TxE to be set
	while (!(I2C1->SR1 & 0x0080))
		;

	// Generate repeated start
	I2C1->CR1 |= 0x0100;

	// Wait for SB to be set
	while (!(I2C1->SR1 & 0x0001))
		;

	//send device address in read mode across line
	I2C1->DR = DevAddr | I2C_REQUEST_READ;

	// Wait for ADDR to be set
	while (!(I2C1->SR1 & 0x0002))
		;
	I2C1->SR2;				// reading SR2 clears ADDR
	I2C1->CR1 |= 0x0400;	//ack

	//loop through data being read
	while(NumBytes > 0)
	{
		// Wait for RxNE to be set
		while (!(I2C1->SR1 & 0x0040))
			;
		*DataBuf = I2C1->DR;
		DataBuf++;

		if (NumBytes > 1)
		{
			//ack
			I2C1->CR1 |= 0x0400;
		}
		else
		{
			//nack
			I2C1->CR1 &= ~0x0400;
		}
		NumBytes--;
	}
		// Generate stop
		I2C1->CR1 |= 0x0200;
}
