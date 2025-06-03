//###########################################################################
// FILE:   blinky.c
// TITLE:  blinky example.
//###########################################################################
// $TI Release: F28M35x Support Library v210 $
// $Release Date: Wed Feb 15 16:25:13 CST 2017 $
// $Copyright: Copyright (C) 2011-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "board_drivers/set_pinout_f28m35x.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include <string.h>

//*****************************************************************************
//
//! \addtogroup master_example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED3.
//
//*****************************************************************************

#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern unsigned long RamfuncsLoadStart;
extern unsigned long RamfuncsRunStart;
extern unsigned long RamfuncsLoadSize;
#endif

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}

#endif

//*****************************************************************************
//
// Blink LED3
//*****************************************************************************
int
main(void)
{

    volatile unsigned long ulLoop;

    // Disable Protection
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Sets up PLL, M3 running at 75MHz and C28 running at 150MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xF) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 |
                         SYSCTL_XCLKDIV_4);

#ifdef _FLASH
// Copy time critical code and Flash setup code to RAM
// This includes the following functions:  InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
// symbols are created by the linker. Refer to the device .cmd file.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
    FlashInit();
#endif

    // Enable clock supply for LED GPIOs
    SysCtlPeripheralEnable(LED_0_PERIPH);
    SysCtlPeripheralEnable(LED_1_PERIPH);

    // Disable clock supply for the watchdog modules
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG0);

    // Enable processor interrupts.
    IntMasterEnable();

    // Set up the Pin for LED3
    GPIOPinTypeGPIOOutput(LED_0_BASE, LED_0_PIN);
    GPIOPinWrite(LED_0_BASE, LED_0_PIN, ~0);

    GPIOPinTypeGPIOOutput(LED_1_BASE, LED_1_PIN);
    GPIOPinWrite(LED_1_BASE, LED_1_PIN, ~0);

    // Loop forever while the timers run.
    while(1)
    {
        //
        // Turn on the LED.
        //
        GPIOPinWrite(LED_0_BASE, LED_0_PIN, ~0);
        GPIOPinWrite(LED_1_BASE, LED_1_PIN, 0);
        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 2000000; ulLoop++)
        {
        }
        //
        // Turn off the LED.
        //
        GPIOPinWrite(LED_0_BASE, LED_0_PIN, 0);
        GPIOPinWrite(LED_1_BASE, LED_1_PIN, ~0);
        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 2000000; ulLoop++)
        {
        }

    }
}
