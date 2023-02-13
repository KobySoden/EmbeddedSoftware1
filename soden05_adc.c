//
// COMP-GENG 421 - Koby Soden
//
// Analog-to-Digital Converter (ADC) module
//

#include "main.h"
#include "adc.h"
#include "eprintf.h"


#define TICKS_200MSEC		200		// ticks in 200 msec

#define DEGREES_MAX			270
#define ACCURACY_MULT		10
#define NUM_QUANTA			4095

#define RIGHT_GREATER		1
#define LEFT_GREATER		0


static UInt AdcLeftReadingGet(void);
static UInt AdcRightReadingGet(void);


void AdcInit(void)
{
	// Enable the GPIOA port clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// Configure PA0 as ADC_IN0 (left knob), PA1 as ADC_IN1 (right knob)
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);

	// Enable the clock for ADC1
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

	// Set the conversion trigger source to software start
	LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);

	// Configure for single conversion mode of a single channel
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
	LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_UNITARY_CONV);
}


void AdcClock(void)
{
	static int TickCount = TICKS_200MSEC;
	static int AdcStates = LEFT_GREATER;			//initial case

	// Update the ADC readings every 200 msec
	if (--TickCount <= 0)
	{
		//calculate rotation in degrees.
		int LeftRotation = (AdcLeftReadingGet() * DEGREES_MAX * ACCURACY_MULT) / NUM_QUANTA;
		int RightRotation = (AdcRightReadingGet() * DEGREES_MAX * ACCURACY_MULT) / NUM_QUANTA;

		switch(AdcStates)							//ADC state machine
		{
			case LEFT_GREATER:
				if (LeftRotation + 20 < RightRotation)		// +20 for historesis
				{
					AdcStates = RIGHT_GREATER;
					break;
				}   // only break on a transition or after both conditions have been checked

			case RIGHT_GREATER:
				if (RightRotation + 20 < LeftRotation)
				{
					AdcStates = LEFT_GREATER;
					break;
				}
			break;
		}

		if(AdcStates)
		{
			eprintf("Right: %4d  Left: %4d\n", RightRotation/10, LeftRotation/10);
		}

		else
		{
			eprintf("Left: %4d  Right: %4d\n", LeftRotation/10 , RightRotation/10);
		}

		TickCount = TICKS_200MSEC;
	}
}


static UInt AdcLeftReadingGet(void)
{
	// Set the ADC channel
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);

	// Enable the ADC, wait for it to be ready
	LL_ADC_Enable(ADC1);
	while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
		;

	// Start the conversion, wait for it to complete
	LL_ADC_REG_StartConversionSWStart(ADC1);
	while (!LL_ADC_IsActiveFlag_EOCS(ADC1))
		;

	// Disable the ADC
	LL_ADC_Disable(ADC1);

	return LL_ADC_REG_ReadConversionData12(ADC1);
}


static UInt AdcRightReadingGet(void)
{
	// Set the ADC channel
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);

	// Enable the ADC, wait for it to be ready
	LL_ADC_Enable(ADC1);
	while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
		;

	// Start the conversion, wait for it to complete
	LL_ADC_REG_StartConversionSWStart(ADC1);
	while (!LL_ADC_IsActiveFlag_EOCS(ADC1))
		;

	// Disable the ADC
	LL_ADC_Disable(ADC1);

	return LL_ADC_REG_ReadConversionData12(ADC1);
}
