//
// COMP/GENG 421 - Tom Lupfer
//
// Session 3 - State machine concepts
//

#include "stm32l1xx.h"
#include "stm32l1xx_nucleo.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_gpio.h"


#define FALSE			0
#define TRUE			1

#define DBG_CLR()		(LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_4))
#define DBG_SET()		(LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_4))

#define LED_GRN_DIS()	(LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7))
#define LED_GRN_ENA()	(LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7))

#define LED_TICKS_OFF	750
#define LED_TICKS_ON	250

#define LED_ST_OFF		0
#define LED_ST_ON		1

#define TICK_DLY_CNT	200


void GpioInit(void);
void LedClock(void);


int main(void)
{
	volatile int	TickCnt;

	// Initialize the hardware
	GpioInit();

	// Main loop
	while(TRUE)
	{
		// Delay for a 1 msec state machine tick
		for (TickCnt = 0; TickCnt < TICK_DLY_CNT; TickCnt++)
			;

		// Clock the LED state machine, measure its duration
		DBG_SET();
		LedClock();
		DBG_CLR();

	}	// End - main loop
}


void GpioInit(void)
{
	// Enable the clock to GPIO port C
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

	// GPIO PC4 -> Debug pin
	// Initialize PC4 to an output, low
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_4);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);

	// GPIO PC7 -> Green LED (active low)
	// Initialize PC7 to an output, high (green LED off)
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
}


void LedClock(void)
{
	static int	LedBlinkCnt = LED_TICKS_OFF;
	static int	LedState = LED_ST_OFF;

	// Clock the LED state machine
	switch(LedState)
	{
	case LED_ST_OFF:
		if (--LedBlinkCnt <= 0)
		{
			LED_GRN_ENA();
			LedBlinkCnt = LED_TICKS_ON;
			LedState = LED_ST_ON;
		}
		break;

	case LED_ST_ON:
		if (--LedBlinkCnt <= 0)
		{
			LED_GRN_DIS();
			LedBlinkCnt = LED_TICKS_OFF;
			LedState = LED_ST_OFF;
		}
		break;
	}
}
