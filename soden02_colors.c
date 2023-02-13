//
// COMP-GENG 421 - Koby Soden
//
// RGB LED color effects module
//
#include "stdio.h"
#include "main.h"
#include "colors.h"
#include "rgb.h"
#include "gpio.h"
#include "dbg.h"

#define RED_UP		0     //Red Led fading up state
#define RED_DOWN	1	 //Red Led fading down state
#define GRN_UP      2
#define GRN_DOWN	3
#define BLU_UP		4
#define BLU_DOWN	5
#define YEL_UP		6
#define YEL_DOWN	7
#define CYN_UP		8
#define CYN_DOWN	9
#define PUR_UP		10
#define PUR_DOWN	11
#define ALL_UP		12
#define ALL_DOWN	13

#define DELAY_MS	100

static int RedLedPwm = 0;    //variables to track current PWM setting for each LED
static int GrnLedPwm = 0;
static int BluLedPwm = 0;

void ColorsClock(void)
{
	static int LedState = RED_UP;     // initial case
	static int DelayCnt = DELAY_MS;

	if (--DelayCnt > 0)              //only check switch statement every 100 ms
	{
		return;
	}
	DelayCnt = DELAY_MS;

	switch(LedState)
	{
		case RED_UP:
			RedUp();
			if(RedLedPwm >= RGB_PWM_MAX)	//check for end of LED Fading up
			{
				LedState = RED_DOWN;
			}
			break;

		case RED_DOWN:
			RedDn();
			if(RedLedPwm <= RGB_PWM_OFF)	//check for end of LED Fading down
			{
				LedState = GRN_UP;
			}
			break;

		case GRN_UP:
			GrnUp();
			if(GrnLedPwm >= RGB_PWM_MAX)
			{
				LedState = GRN_DOWN;
			}
			break;

		case GRN_DOWN:
			GrnDn();
			if(GrnLedPwm <= RGB_PWM_OFF)
			{
				LedState = BLU_UP;
			}
			break;

		case BLU_UP:
			BluUp();
			if(BluLedPwm >= RGB_PWM_MAX)
			{
				LedState = BLU_DOWN;
			}
			break;

		case BLU_DOWN:
			BluDn();
			if(BluLedPwm <= RGB_PWM_OFF)
			{
				LedState = YEL_UP;
			}
			break;

		case YEL_UP:
			RedUp();
			GrnUp();
			if(GrnLedPwm >= RGB_PWM_MAX)
			{
				LedState = YEL_DOWN;
			}
			break;

		case YEL_DOWN:
			RedDn();
			GrnDn();
			if(GrnLedPwm <= RGB_PWM_OFF)
			{
				LedState = CYN_UP;
			}
			break;

		case CYN_UP:
			BluUp();
			GrnUp();
			if(BluLedPwm >= RGB_PWM_MAX)
			{
				LedState = CYN_DOWN;
			}
			break;

		case CYN_DOWN:
			BluDn();
			GrnDn();
			if(BluLedPwm <= RGB_PWM_OFF)
			{
				LedState = PUR_UP;
			}
			break;

		case PUR_UP:
			RedUp();
			BluUp();
			if(BluLedPwm >= RGB_PWM_MAX)
			{
				LedState = PUR_DOWN;
			}
			break;

		case PUR_DOWN:
			RedDn();
			BluDn();
			if(BluLedPwm <= RGB_PWM_OFF)
			{
				LedState = ALL_UP;
			}
			break;

		case ALL_UP:
			RedUp();
			GrnUp();
			BluUp();
			if(BluLedPwm >= RGB_PWM_MAX)
			{
				LedState = ALL_DOWN;
			}
			break;

		case ALL_DOWN:
			RedDn();
			GrnDn();
			BluDn();
			if(BluLedPwm <= RGB_PWM_OFF)
			{
				LedState = RED_UP;
			}
			break;
	}
}

void RedUp(void)
{
	RedLedPwm +=1;
	RgbRedPwmSet(RedLedPwm);
}

void RedDn(void)
{
	RedLedPwm -=1;
	RgbRedPwmSet(RedLedPwm);
}

void GrnUp(void)
{
	GrnLedPwm += 1;
	RgbGrnPwmSet(GrnLedPwm);
}
void GrnDn(void)
{
	GrnLedPwm -=1;
	RgbGrnPwmSet(GrnLedPwm);
}

void BluUp(void)
{
	BluLedPwm +=1;
	RgbBluPwmSet(BluLedPwm);
}

void BluDn(void)
{
	BluLedPwm -=1;
	RgbBluPwmSet(BluLedPwm);
}
