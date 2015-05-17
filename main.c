//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void main(void)
{
	/* Stop Watchdog  */
	MAP_WDT_A_holdTimer();

}
