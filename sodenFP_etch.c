//
// COMP-GENG 421 - Koby Soden
//
// Etch A Sketch Emulator module
// code for Final Project
//

#include "main.h"
#include "adc.h"
#include "etch.h"
#include "lcd.h"
#include "rgb.h"
#include "nav.h"
#include "accel.h"
#include "eprintf.h"


#define ETCH_ST_INIT		0		// state to initialize the system
#define ETCH_ST_CURSOR_1X1	1		// state to move the point without drawing 1 pixel (cursor)
#define ETCH_ST_STYLUS_1X1	2		// state to move the point and draw 1 pixel (stylus)
#define ETCH_ST_STYLUS_2X2	3		// state to move the point and draw 2x2 pixels (stylus)
#define ETCH_ST_CURSOR_2X2	4		// state to move the point without drawing 2x2 pixels (cursor)

#define NO_PRESS			0		//macros for nav switch readings
#define LT_PRESS 			1
#define RT_PRESS			2
#define UP_PRESS			3
#define DN_PRESS			4
#define SELECT_PRESS		5

#define HOR_DIV			((ADC_RDG_MAX + 1) / LCD_PIXEL_WIDTH)	// PosX ADC reading divisor
#define VER_DIV			((ADC_RDG_MAX + 1) / LCD_PIXEL_HEIGHT)	// PosY ADC reading divisor


typedef int				Bool;


int GetNavReading(void);


void EtchClock(void)
{
	static int EtchState = ETCH_ST_INIT;
	static int PrevPosX;
	static int PrevPosY;
	int NavStatus = NO_PRESS;

	int CurPosX = -1;	// ensure not equal to initial position
	int CurPosY = -1;	// ensure not equal to initial position

	// Get the current X,Y position, invert Y so clockwise rotation of right knob => up
	CurPosX = AdcLeftReadingGet() / HOR_DIV;
	CurPosY = (LCD_PIXEL_HEIGHT - 1 ) - (AdcRightReadingGet() / VER_DIV);

	// If there is a shake clear the screen and reinitialize it
	if (AccelIsShaken())
	{
		EtchState = ETCH_ST_INIT;
		AccelClearIsShaken();
		LcdClear();
	}
	// get nav switch readings if shake not detected
	else
	{
		NavStatus = GetNavReading();

		LcdReverse((Bool)NavStatus == SELECT_PRESS);		// reverse screen if nav switch select is pressed

	}

	switch (EtchState)
	{
	case ETCH_ST_INIT:
		LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render the cursor in its initial position
		EtchState = ETCH_ST_STYLUS_1X1;
		break;

	case ETCH_ST_CURSOR_1X1:
		LcdRenderObject(LCD_OBJ_POINT_1X1, PrevPosX, PrevPosY, LCD_MODE_CURSOR);		// erase the previous cursor

		// turn on green Led turn off red Led
		RGB_GRN_DIS();
		RGB_RED_ENA();

		// cursor to stylus mode
		if (NavStatus == DN_PRESS)
		{
			EtchState = ETCH_ST_STYLUS_1X1;
		}

		// increase cursor size
		if (NavStatus == RT_PRESS)
		{
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render new 2x2 cursor on transition
			EtchState = ETCH_ST_CURSOR_2X2;
		}

		if (NavStatus == NO_PRESS)
		{
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render new cursor
		}
		break;

	case ETCH_ST_CURSOR_2X2:
		LcdRenderObject(LCD_OBJ_POINT_2X2, PrevPosX, PrevPosY, LCD_MODE_CURSOR);

		// turn on green Led turn off red Led
		RGB_GRN_DIS();
		RGB_RED_ENA();

		// cursor to stylus mode
		if (NavStatus == DN_PRESS)
		{
			EtchState = ETCH_ST_STYLUS_2X2;
		}

		// decrease cursor size
		if (NavStatus == LT_PRESS)
		{
			LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render new 1x1 cursor on transition
			EtchState = ETCH_ST_CURSOR_1X1;
		}

		if (NavStatus == NO_PRESS)
		{
			LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_CURSOR);		// render new cursor
		}
			break;

	case ETCH_ST_STYLUS_1X1:
		LcdRenderObject(LCD_OBJ_POINT_1X1, CurPosX, CurPosY, LCD_MODE_STYLUS);			// render stylus

		// turn on red Led turn off green Led
		RGB_RED_DIS();
		RGB_GRN_ENA();

		// stylus to cursor mode
		if (NavStatus == UP_PRESS)
		{
			EtchState = ETCH_ST_CURSOR_1X1;
		}

		// increase stylus size
		if (NavStatus == RT_PRESS)
		{
			EtchState = ETCH_ST_STYLUS_2X2;
		}
		break;

	case ETCH_ST_STYLUS_2X2:
		LcdRenderObject(LCD_OBJ_POINT_2X2, CurPosX, CurPosY, LCD_MODE_STYLUS);		//  render stylus to lcd

		// turn on grn Led turn off red Led
		RGB_RED_DIS();
		RGB_GRN_ENA();

		// stylus to cursor mode
		if (NavStatus == UP_PRESS)
		{
			EtchState = ETCH_ST_CURSOR_2X2;
		}

		// decrease stylus size
		if (NavStatus == LT_PRESS)
		{
			EtchState = ETCH_ST_STYLUS_1X1;
		}
		break;
	}

	// save position for next tick
	PrevPosX = CurPosX;
	PrevPosY = CurPosY;
}


int GetNavReading(void)		// returns Nav reading as a macro
{
	if(NavDownIsPressed())
	{
		NavClearDownIsPressed();
		return DN_PRESS;
	}
	if(NavUpIsPressed())
	{
		NavClearUpIsPressed();
		return UP_PRESS;
	}
	if(NavLeftIsPressed())
	{
		NavClearLeftIsPressed();
		return LT_PRESS;
	}
	if(NavRightIsPressed())
	{
		NavClearRightIsPressed();
		return RT_PRESS;
	}
	if(NavSelectIsPressed())
	{
		// no clear so LCD is only inverted when the select button is held down
		return SELECT_PRESS;
	}
	return NO_PRESS;
}

