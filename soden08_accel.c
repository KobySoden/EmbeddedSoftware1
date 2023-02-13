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
#include "lcd.h"
#include "rgb.h"


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

#define ARCCOS_LUT_SIZE		(2 * RDG_1G + 1)

#define REFRESH_TICKS		250

#define XYZ_BUF_SIZE		3

#define DEG_TILT			30


#define UP_THRESHOLD		90 - DEG_TILT
#define DN_THRESHOLD		90 + DEG_TILT
#define RT_THRESHOLD		90 + DEG_TILT
#define LT_THRESHOLD 		90 - DEG_TILT

#define LCD_CENTER			10
#define DISP_CHAR_WIDTH		21


// Arccosine array goes here
const static Int ArcCosineArray[45] =
{
180,
162,
155,
149,
144,
140,
136,
132,
129,
126,
123,
120,
117,
114,
111,
108,
105,
103,
100,
 97,
 95,
 92,
 90,
 87,
 84,
 82,
 79,
 76,
 74,
 71,
 68,
 65,
 62,
 60,
 56,
 53,
 50,
 47,
 43,
 39,
 35,
 30,
 24,
 17,
  0,
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


void AccelGenLut(void)
{
	int SignedReading;

	eprintf("\n");	// get to a new line after stray first character
	eprintf("const static Int ArcCosineArray[%d] = \n", ARCCOS_LUT_SIZE);
	eprintf("{\n");
	for (SignedReading = -RDG_1G; SignedReading <= RDG_1G; SignedReading++)
	{
		eprintf("%3d,\n", (UInt) (180 * acos((float) SignedReading / RDG_1G) / PI));
	}
	eprintf("};\n");
}


void AccelClock(void)
{
	static int TickCount = REFRESH_TICKS;
	static int xDeg;
	static int yDeg;

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

			xDeg = Rdg2Deg(RdgBuf[RDG_IDX_XOUT]);
			yDeg = Rdg2Deg(RdgBuf[RDG_IDX_YOUT]);

	//following 4 conditionals check for readings that are
	//not up not down not right and not left and clear the screen/LED accordingly
	if (xDeg >= UP_THRESHOLD)
	{
		LcdPrintf(1,0,"                     ");
		RGB_RED_DIS();
	}
	if(xDeg <= DN_THRESHOLD)
	{
		LcdPrintf(3,0,"                     ");
		RGB_GRN_DIS();
	}
	if (yDeg <= RT_THRESHOLD)
	{
		LcdPrintf(2,0,"                     ");
		RGB_BLU_DIS();
	}
	if(xDeg >= LT_THRESHOLD)
	{
		LcdPrintf(2,0,"                     ");
	}

	//following 4 conditionals check for up down left and right
	if (xDeg < UP_THRESHOLD)
	{
		LcdPrintf(1, LCD_CENTER -1, "UP"); //-1 centers the word "UP"
		RGB_RED_ENA();
	}
	if (xDeg > DN_THRESHOLD){
		LcdPrintf(3, LCD_CENTER -2, "DOWN");
		RGB_GRN_ENA();
	}
	if (yDeg > RT_THRESHOLD){
		LcdPrintf(2, DISP_CHAR_WIDTH -5, "RIGHT");   //-5 right justifies the word "RIGHT" from the end of the screen
		RGB_BLU_ENA();
	}
	if (yDeg < LT_THRESHOLD){
		LcdPrintf(2,0, "LEFT");
	}
}

static int Rdg2Deg(UInt8 Reading)
{
	int SignedReading;

	SignedReading = (int) ((Reading << RDG_SHIFT) >> RDG_SHIFT);
	SignedReading = SignedReading < -RDG_1G ? -RDG_1G : SignedReading;
	SignedReading = SignedReading >  RDG_1G ?  RDG_1G : SignedReading;
	return ArcCosineArray[SignedReading + RDG_1G];
}
