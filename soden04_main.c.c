//
// COMP-GENG 421 - Tom Lupfer
//
// Session 7 - Compact embedded eprintf() implementation and test
//

#include <stdio.h>

#include "main.h"
#include "clock.h"
#include "dbg.h"
#include "eprintf.h"
#include "gpio.h"
#include "uart.h"


#define TESTb
#define TESTc_
#define TESTd_
#define TESTs_
#define TESTx_
#define TESTX_
#define TESTpercentage_


int main(void)
{
	// Initialize the hardware
	ClockInit();
	GpioInit();
	UartInit();

	eprintf("ABC\n");
	eprintf("Hello, world! It is 2020!\n");
	eprintf("Hello, world! It is %d!\n", 2020);

#ifdef TESTb
	eprintf("%%b test: ABC%bDEF\n", 0x1);

	eprintf("%%b test: ABC%bDEF\n", 0x0);

	eprintf("%%b test: ABC%bDEF\n", 0x80000000);  //1 followed by all zeros

	eprintf("%%b test: ABC%bDEF\n", 0xFFFFFFFF);  //all 1's

	eprintf("%%b test: ABC%6bDEF\n", 1);		//tests for various eprintf functionalities

	eprintf("%%b test: ABC%06bDEF\n", 2);

	eprintf("%%b test: ABC%-bDEF\n", 3);

	eprintf("%%b test: ABC%-6bDEF\n", 4);

	eprintf("%%b test: ABC%-06bDEF\n", 5);

	eprintf("%%b test: ABC%bDEF\n", -1);     //signed int test

	eprintf("%%b test: ABC%bDEF\n", -6);     //additional signed int test
#endif	// TESTb


#ifdef TESTc
	eprintf("%%c test: ABC%cDEF\n", 'x');
	 printf("%%c test: ABC%cDEF\n", 'x');

	eprintf("%%c test: ABC%6cDEF\n", 'x');
	 printf("%%c test: ABC%6cDEF\n", 'x');

	eprintf("%%c test: ABC%06cDEF\n", 'x');
	 printf("%%c test: ABC%06cDEF\n", 'x');

	eprintf("%%c test: ABC%-cDEF\n", 'x');
	 printf("%%c test: ABC%-cDEF\n", 'x');

	eprintf("%%c test: ABC%-6cDEF\n", 'x');
	 printf("%%c test: ABC%-6cDEF\n", 'x');

	eprintf("%%c test: ABC%-06cDEF\n", 'x');
	 printf("%%c test: ABC%-06cDEF\n", 'x');
#endif	// TESTc


#ifdef TESTd

	eprintf("%%d test: ABC%dDEF\n", 0);
	 printf("%%d test: ABC%dDEF\n", 0);

	eprintf("%%d test: ABC%6dDEF\n", 0);
	 printf("%%d test: ABC%6dDEF\n", 0);

	eprintf("%%d test: ABC%06dDEF\n", 0);
	 printf("%%d test: ABC%06dDEF\n", 0);

	eprintf("%%d test: ABC%-dDEF\n", 0);
	 printf("%%d test: ABC%-dDEF\n", 0);

	eprintf("%%d test: ABC%-6dDEF\n", 0);
	 printf("%%d test: ABC%-6dDEF\n", 0);

	eprintf("%%d test: ABC%-06dDEF\n", 0);
	 printf("%%d test: ABC%-06dDEF\n", 0);


	eprintf("%%d test: ABC%dDEF\n", 2021);
	 printf("%%d test: ABC%dDEF\n", 2021);

	eprintf("%%d test: ABC%6dDEF\n", 2021);
	 printf("%%d test: ABC%6dDEF\n", 2021);

	eprintf("%%d test: ABC%06dDEF\n", 2021);
	 printf("%%d test: ABC%06dDEF\n", 2021);

	eprintf("%%d test: ABC%-dDEF\n", 2021);
	 printf("%%d test: ABC%-dDEF\n", 2021);

	eprintf("%%d test: ABC%-6dDEF\n", 2021);
	 printf("%%d test: ABC%-6dDEF\n", 2021);

	eprintf("%%d test: ABC%-06dDEF\n", 2021);
	 printf("%%d test: ABC%-06dDEF\n", 2021);


	eprintf("%%d test: ABC%dDEF\n", 123456789);
	 printf("%%d test: ABC%dDEF\n", 123456789);

	eprintf("%%d test: ABC%6dDEF\n", 123456789);
	 printf("%%d test: ABC%6dDEF\n", 123456789);

	eprintf("%%d test: ABC%06dDEF\n", 123456789);
	 printf("%%d test: ABC%06dDEF\n", 123456789);

	eprintf("%%d test: ABC%-dDEF\n", 123456789);
	 printf("%%d test: ABC%-dDEF\n", 123456789);

	eprintf("%%d test: ABC%-6dDEF\n", 123456789);
	 printf("%%d test: ABC%-6dDEF\n", 123456789);

	eprintf("%%d test: ABC%-06dDEF\n", 123456789);
	 printf("%%d test: ABC%-06dDEF\n", 123456789);


	eprintf("%%d test: ABC%dDEF\n", -2021);
	 printf("%%d test: ABC%dDEF\n", -2021);

	eprintf("%%d test: ABC%6dDEF\n", -2021);
	 printf("%%d test: ABC%6dDEF\n", -2021);

	eprintf("%%d test: ABC%06dDEF\n", -2021);
	 printf("%%d test: ABC%06dDEF\n", -2021);

	eprintf("%%d test: ABC%-dDEF\n", -2021);
	 printf("%%d test: ABC%-dDEF\n", -2021);

	eprintf("%%d test: ABC%-6dDEF\n", -2021);
	 printf("%%d test: ABC%-6dDEF\n", -2021);

	eprintf("%%d test: ABC%-06dDEF\n", -2021);
	 printf("%%d test: ABC%-06dDEF\n", -2021);


	eprintf("%%d test: ABC%dDEF\n", -123456789);
	 printf("%%d test: ABC%dDEF\n", -123456789);

	eprintf("%%d test: ABC%6dDEF\n", -123456789);
	 printf("%%d test: ABC%6dDEF\n", -123456789);

	eprintf("%%d test: ABC%06dDEF\n", -123456789);
	 printf("%%d test: ABC%06dDEF\n", -123456789);

	eprintf("%%d test: ABC%-dDEF\n", -123456789);
	 printf("%%d test: ABC%-dDEF\n", -123456789);

	eprintf("%%d test: ABC%-6dDEF\n", -123456789);
	 printf("%%d test: ABC%-6dDEF\n", -123456789);

	eprintf("%%d test: ABC%-06dDEF\n", -123456789);
	 printf("%%d test: ABC%-06dDEF\n", -123456789);

#endif	// TESTd


#ifdef TESTs
	eprintf("%%s test: ABC%sDEF\n", "str");
	 printf("%%s test: ABC%sDEF\n", "str");

	eprintf("%%s test: ABC%6sDEF\n", "str");
	 printf("%%s test: ABC%6sDEF\n", "str");

	eprintf("%%s test: ABC%06sDEF\n", "str");
	 printf("%%s test: ABC%06sDEF\n", "str");

	eprintf("%%s test: ABC%-sDEF\n", "str");
	 printf("%%s test: ABC%-sDEF\n", "str");

	eprintf("%%s test: ABC%-6sDEF\n", "str");
	 printf("%%s test: ABC%-6sDEF\n", "str");

	eprintf("%%s test: ABC%-06sDEF\n", "str");
	 printf("%%s test: ABC%-06sDEF\n", "str");

	eprintf("%%s test: ABC%sDEF\n", "longstring");
	 printf("%%s test: ABC%sDEF\n", "longstring");

	eprintf("%%s test: ABC%6sDEF\n", "longstring");
	 printf("%%s test: ABC%6sDEF\n", "longstring");

	eprintf("%%s test: ABC%06sDEF\n", "longstring");
	 printf("%%s test: ABC%06sDEF\n", "longstring");

	eprintf("%%s test: ABC%-sDEF\n", "longstring");
	 printf("%%s test: ABC%-sDEF\n", "longstring");

	eprintf("%%s test: ABC%-6sDEF\n", "longstring");
	 printf("%%s test: ABC%-6sDEF\n", "longstring");

	eprintf("%%s test: ABC%-06sDEF\n", "longstring");
	 printf("%%s test: ABC%-06sDEF\n", "longstring");
#endif	// TESTs


#ifdef TESTx
	eprintf("%%x test: ABC%xDEF\n", 0);
	 printf("%%x test: ABC%xDEF\n", 0);

	eprintf("%%x test: ABC%6xDEF\n", 0);
	 printf("%%x test: ABC%6xDEF\n", 0);

	eprintf("%%x test: ABC%06xDEF\n", 0);
	 printf("%%x test: ABC%06xDEF\n", 0);

	eprintf("%%x test: ABC%-xDEF\n", 0);
	 printf("%%x test: ABC%-xDEF\n", 0);

	eprintf("%%x test: ABC%-6xDEF\n", 0);
	 printf("%%x test: ABC%-6xDEF\n", 0);

	eprintf("%%x test: ABC%-06xDEF\n", 0);
	 printf("%%x test: ABC%-06xDEF\n", 0);


	eprintf("%%x test: ABC%xDEF\n", 0x37AF);
	 printf("%%x test: ABC%xDEF\n", 0x37AF);

	eprintf("%%x test: ABC%6xDEF\n", 0x37AF);
	 printf("%%x test: ABC%6xDEF\n", 0x37AF);

	eprintf("%%x test: ABC%06xDEF\n", 0x37AF);
	 printf("%%x test: ABC%06xDEF\n", 0x37AF);

	eprintf("%%x test: ABC%-xDEF\n", 0x37AF);
	 printf("%%x test: ABC%-xDEF\n", 0x37AF);

	eprintf("%%x test: ABC%-6xDEF\n", 0x37AF);
	 printf("%%x test: ABC%-6xDEF\n", 0x37AF);

	eprintf("%%x test: ABC%-06xDEF\n", 0x37AF);
	 printf("%%x test: ABC%-06xDEF\n", 0x37AF);
#endif	// TESTx


#ifdef TESTX
	eprintf("%%X test: ABC%XDEF\n", 0x37AF);
	 printf("%%X test: ABC%XDEF\n", 0x37AF);

	eprintf("%%X test: ABC%6XDEF\n", 0x37AF);
	 printf("%%X test: ABC%6XDEF\n", 0x37AF);

	eprintf("%%X test: ABC%06XDEF\n", 0x37AF);
	 printf("%%X test: ABC%06XDEF\n", 0x37AF);

	eprintf("%%X test: ABC%-XDEF\n", 0x37AF);
	 printf("%%X test: ABC%-XDEF\n", 0x37AF);

	eprintf("%%X test: ABC%-6XDEF\n", 0x37AF);
	 printf("%%X test: ABC%-6XDEF\n", 0x37AF);

	eprintf("%%X test: ABC%-06XDEF\n", 0x37AF);
	 printf("%%X test: ABC%-06XDEF\n", 0x37AF);


	eprintf("%%X test: ABC%XDEF\n", 0xFFFFFFFF);
	 printf("%%X test: ABC%XDEF\n", 0xFFFFFFFF);

	eprintf("%%X test: ABC%6XDEF\n", 0xFFFFFFFF);
	 printf("%%X test: ABC%6XDEF\n", 0xFFFFFFFF);

	eprintf("%%X test: ABC%06XDEF\n", 0xFFFFFFFF);
	 printf("%%X test: ABC%06XDEF\n", 0xFFFFFFFF);

	eprintf("%%X test: ABC%-XDEF\n", 0xFFFFFFFF);
	 printf("%%X test: ABC%-XDEF\n", 0xFFFFFFFF);

	eprintf("%%X test: ABC%-6XDEF\n", 0xFFFFFFFF);
	 printf("%%X test: ABC%-6XDEF\n", 0xFFFFFFFF);

	eprintf("%%X test: ABC%-06XDEF\n", 0xFFFFFFFF);
	 printf("%%X test: ABC%-06XDEF\n", 0xFFFFFFFF);
#endif	// TESTX


#ifdef TESTpercentage
	eprintf("%%%% test: ABC%%DEF\n");
	 printf("%%%% test: ABC%%DEF\n");

	eprintf("%%%% test: ABC%6%DEF\n");
	 printf("%%%% test: ABC%6%DEF\n");

	eprintf("%%%% test: ABC%06%DEF\n");
	 printf("%%%% test: ABC%06%DEF\n");

	eprintf("%%%% test: ABC%-%DEF\n");
	 printf("%%%% test: ABC%-%DEF\n");

	eprintf("%%%% test: ABC%-6%DEF\n");
	 printf("%%%% test: ABC%-6%DEF\n");

	eprintf("%%%% test: ABC%-06%DEF\n");
	 printf("%%%% test: ABC%-06%DEF\n");
#endif	// TESTpercentage


	// Main loop
	while(TRUE)
	{
		// Wait for tick timer
		ClockWaitForTick();

		// Echo chars received on the serial port
		DBG_SET();
		if (UartCharAvail())
		{
			UartCharWrite(UartCharRead());
		}
		DBG_CLR();

	}	// End - main loop
}
