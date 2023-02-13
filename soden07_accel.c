//
// COMP-GENG 421 - Koby Soden
//
// Accelerometer module
//

#include <math.h>

#include "main.h"
#include "accel.h"
#include "eprintf.h"
#include "i2c.h"


#define ALERT_BITMASK		0x40	// Alert bit in XOUT, YOUT, ZOUT regs

#define PI					3.14159

#define I2C_ADDR_ACCEL		0x98

#define RDG_BITS			6
#define RDG_SHIFT			(32 - RDG_BITS)

#define RDG_1G				22

#define RDG_IDX_XOUT		0
#define RDG_IDX_YOUT		1
#define RDG_IDX_ZOUT		2

#define REG_ADDR_XOUT		0x00
#define REG_ADDR_YOUT		0x01
#define REG_ADDR_ZOUT		0x02
#define REG_ADDR_INTSU		0x06
#define REG_ADDR_MODE		0x07
#define REG_ADDR_SR			0x08
#define REG_ADDR_PDET		0x09
#define REG_ADDR_PD			0x0A

#define RAD_2_DEG(RAD)		((180.0 * RAD) / PI)

#define ARCCOS_LUT_SIZE		(2 * RDG_1G + 1)

#define REFRESH_TICKS		250

#define XYZ_BUF_SIZE		3

#define ARCCOS_MULT			10000

// Arccosine lookup table goes here
const static UInt ArccosArray[45] =
{
 180,  162,  155,  149,  144,
 140,  136,  132,  129,  126,
 123,  120,  117,  114,  111,
 108,  105,  103,  100,   97,
  95,   92,   90,   87,   84,
  82,   79,   76,   74,   71,
  68,   65,   62,   60,   56,
  53,   50,   47,   43,   39,
  35,   30,   24,   17,    0,
};



static int Rdg2Deg(UInt8 Reading);


void AccelInit(void)
{
	UInt8 XmtBuf[1];

	// Clear all of the control registers to their default values
	XmtBuf[0] = 0x00;
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_MODE,  1, XmtBuf);	// clear TON bit to get out of test mode
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_INTSU, 1, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_SR,    1, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_PDET,  1, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_PD,    1, XmtBuf);

	// Enable the active mode
	XmtBuf[0] = 0x01;
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_MODE,  1, XmtBuf);
}


// Function to generate the arccosine lookup table
void AccelGenLut(void)
{
		float Read;
		UInt Incr;

		eprintf("\n");	// get to a new line after stray first character
		eprintf("const static UInt ArccosArray[%d] = \n", ARCCOS_LUT_SIZE);
		eprintf("{\n");
		for (Read = - RDG_1G; Read < RDG_1G; )	// read is incremented in the inner loop
		{
			// Output five values per line
			eprintf("\t");
			for (Incr = 0; Incr < 5; Incr++)
			{
				eprintf("%4d, ", (UInt) (RAD_2_DEG(acos(Read++/(float)RDG_1G))));
			}
			eprintf("\n");
		}
		eprintf("};\n");
}


void AccelClock(void)
{
	static int TickCount = REFRESH_TICKS;

	UInt8 RdgBuf[XYZ_BUF_SIZE];

	// Read the accelerometer and display the results periodically
	if (--TickCount > 0)
	{
		return;
	}
	TickCount = REFRESH_TICKS;

	// Read the X, Y, Z values until there are no alert bits set
	do
	{
		I2cRead(I2C_ADDR_ACCEL, REG_ADDR_XOUT, XYZ_BUF_SIZE, RdgBuf);
	}
	while ((RdgBuf[RDG_IDX_XOUT] | RdgBuf[RDG_IDX_YOUT] | RdgBuf[RDG_IDX_ZOUT]) & ALERT_BITMASK);

	eprintf("X: %3d  Y: %3d  Z: %3d\n",
			Rdg2Deg(RdgBuf[RDG_IDX_XOUT]),
			Rdg2Deg(RdgBuf[RDG_IDX_YOUT]),
			Rdg2Deg(RdgBuf[RDG_IDX_ZOUT]));
}


static int Rdg2Deg(UInt8 Reading)
{
	int deg = (int) ((Reading << RDG_SHIFT) >> RDG_SHIFT);
	if(deg > 22) //check for readings above 22 and adjust them
		{
		deg = 22;
		}
	if(deg < -22)
		{
		deg = -22;
		}
	deg = ArccosArray[deg+22];
	return deg;
}
