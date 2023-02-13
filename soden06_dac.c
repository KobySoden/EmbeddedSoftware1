//
// COMP-GENG 421 - Koby Soden
//
// DAC (Digital-to-Analog Converter) module
//

#include "main.h"
#include "dac.h"

#define RAMP_OUTPUTx
#define SIN_OUTPUT

#define DAC_VAL_MIN		0
#define DAC_VAL_MAX		4095
#define DAC_VAL_RANGE	(DAC_VAL_MAX - DAC_VAL_MIN + 1)

#define PI				3.14159
#define DEG_2_RAD(DEG)	((PI * DEG) / 180.0)

#define SIN_MULT			10000

static void DacValueSet(UInt Val);

UInt DegToSineToDac(UInt degrees);

void DacInit(void)
{
	// Enable the GPIOA port clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// Configure PA4 as DAC_OUT1
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);

	// Enable the clock for DAC1
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);

	// Enable the output buffer for DAC1 channel 1
	LL_DAC_SetOutputBuffer(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_BUFFER_ENABLE);

	// Enable DAC1 channel 1
	LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
}


void DacClock(void)
{
#ifdef RAMP_OUTPUT
	static UInt Incr = 0;

	DacValueSet(Incr++);
#endif	// RAMP_OUTPUT

#ifdef SIN_OUTPUT

	static UInt Degrees = 0;

	DacValueSet(DegToSineToDac(Degrees++));

#endif	// SIN_OUTPUT
}


static void DacValueSet(UInt Val)
{
	LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, Val);
}

UInt DegToSineToDac(UInt degrees)
{
	//Lookup table for the values of sine
	const static UInt SineArray[90] =
	{
	   0,  174,  348,  523,  697,
	 871, 1045, 1218, 1391, 1564,
	1736, 1908, 2079, 2249, 2419,
	2588, 2756, 2923, 3090, 3255,
	3420, 3583, 3746, 3907, 4067,
	4226, 4383, 4539, 4694, 4848,
	4999, 5150, 5299, 5446, 5591,
	5735, 5877, 6018, 6156, 6293,
	6427, 6560, 6691, 6819, 6946,
	7071, 7193, 7313, 7431, 7547,
	7660, 7771, 7880, 7986, 8090,
	8191, 8290, 8386, 8480, 8571,
	8660, 8746, 8829, 8910, 8987,
	9063, 9135, 9205, 9271, 9335,
	9396, 9455, 9510, 9563, 9612,
	9659, 9702, 9743, 9781, 9816,
	9848, 9876, 9902, 9925, 9945,
	9961, 9975, 9986, 9993, 9998,
	};

	UInt val;

	if (degrees > 360)				//adjusts degree value for angles above 360
	{
		degrees = degrees % 360;
	}

	if(degrees < 90)
	{
		val = SineArray[degrees];	 //get sine value from LUT
		val *= DAC_VAL_RANGE;
		val = val/(2*SIN_MULT);
		val += DAC_VAL_RANGE/2;				// center at the middle of Dac range
	}
	else if (degrees > 90 && degrees < 180)		//same as 0-90 but backwards
	{
		degrees = degrees % 90;
		val = SineArray[90-degrees];
		val *= DAC_VAL_RANGE;
		val = val/(2*SIN_MULT);
		val += DAC_VAL_RANGE/2;
	}

	else if (degrees > 180 && degrees < 270)	//same as 0-90 but negative i.e. below 2047
	{
		degrees = degrees % 90;
		val = SineArray[degrees];
		val *= DAC_VAL_RANGE;
		val = val/(2*SIN_MULT);
		val = (DAC_VAL_RANGE/2) - val;
	}

	else if (degrees > 270)						// same as 90-180 but negative
	{
		degrees = degrees % 90;
		val = SineArray[90-degrees];
		val *= DAC_VAL_RANGE;
		val = val/(2*SIN_MULT);
		val = (DAC_VAL_RANGE/2) - val;
	}

	return val;
}
