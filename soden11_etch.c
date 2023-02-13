//
// COMP-GENG 421 - Koby Soden
//
// Etch A Sketch Emulator module
//

#include "main.h"
#include "etch.h"
#include "lcd.h"
#include "eprintf.h"


#define DELAY_TICKS		10
#define ADC_READING_MAX	4095



void EtchClock(void)
{
	static int TickCount = DELAY_TICKS;
	static int PosX = 0, PosY = 0;

	if (--TickCount > 0)
	{
		return;
	}
	TickCount = DELAY_TICKS;

	PosX = AdcLeftReadingGet()*LCD_PIXEL_WIDTH/ADC_READING_MAX;
	PosY = LCD_PIXEL_HEIGHT - (AdcRightReadingGet()*LCD_PIXEL_HEIGHT/ADC_READING_MAX);

	LcdRenderObject(LCD_OBJ_CURSOR_2X2, PosX, PosY);

}
