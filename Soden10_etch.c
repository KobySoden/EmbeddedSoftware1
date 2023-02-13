//
// COMP-GENG 421 - Koby Soden
//
// Etch A Sketch Emulator module
//

#include "main.h"
#include "accel.h"
#include "eprintf.h"
#include "etch.h"
#include "lcd.h"

#define LCD_DRAW 		0
#define LCD_CLEAR  		1

#define DLY_2000MSEC	2000

void EtchClock(void)
{
	static int ScreenState = LCD_DRAW;
	static int DelayCount = 0;

	// Delay 2000 msec between clocks of the state machine if lCD was shaken
	if (--DelayCount > 0)
	{
		//Reset delay if a shake is detected during the delay
		if (AccelIsShaken())
		{
			DelayCount = DLY_2000MSEC;
			AccelClearIsShaken();
		}
		return;
	}
	//check for shake when delay is not active
	if (AccelIsShaken())
	{
		ScreenState = LCD_CLEAR;
		DelayCount = DLY_2000MSEC;
		AccelClearIsShaken();
	}

	switch(ScreenState)
	{
		case LCD_DRAW:
			LcdPrintf(0,0,"Professor Lupfer");
			LcdPrintf(1,0,"This is my");
			LcdPrintf(2,0,"Favorite");
			LcdPrintf(3,0,"Course");
			break;

		case LCD_CLEAR:
			ClrScreen();
			ScreenState = LCD_DRAW;
			break;
	}

}

void ClrScreen(void)
{
	LcdPrintf(0,0,"                     ");
	LcdPrintf(1,0,"                     ");
	LcdPrintf(2,0,"                     ");
	LcdPrintf(3,0,"                     ");
}
