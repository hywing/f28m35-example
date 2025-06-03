//###########################################################################
// FILE:   blinky_c28.c
// TITLE:  Blinky Example for F28M35x.
//
//! \addtogroup control_example_list
//! <h1> Blinky (blinky)</h1>
//!
//! This example blinks LED2.
//!
//
//###########################################################################
// $TI Release: F28M35x Support Library v210 $
// $Release Date: Wed Feb 15 16:25:13 CST 2017 $
// $Copyright: Copyright (C) 2011-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include <string.h>

#define C28_FREQ    150         //CPU frequency in MHz
#define LED_0_DIR_REG GpioG2CtrlRegs.GPEDIR.bit.GPIO135
#define LED_0_DAT_REG GpioG2DataRegs.GPEDAT.bit.GPIO135


void main(void)
{
   unsigned long delay;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F28M35x_SysCtrl.c file.
    InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the F28M35x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
    InitGpio(); // Skipped for this example
    EALLOW;
    LED_0_DIR_REG = 1;
    EDIS;
    LED_0_DAT_REG = 1;// turn off LED
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
    DINT;

#ifdef _FLASH
// Copy time critical code and Flash setup code to RAM
// This includes the following functions:  InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
// symbols are created by the linker. Refer to the device .cmd file.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
    InitFlash();
#endif

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F28M35x_PieCtrl.c file.
    InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F28M35x_DefaultIsr.c.
// This function is found in F28M35x_PieVect.c.
    InitPieVectTable();

// Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM


// Step 6. IDLE loop. Just sit and loop forever (optional):
    for(;;)
    {
        //
        // Turn on LED
        //
        LED_0_DAT_REG = 0;
        //
        // Delay for a bit.
        //
        for(delay = 0; delay < 200000; delay++)
        {
        }

        //
        // Turn off LED
        //
        LED_0_DAT_REG = 1;
        //
        // Delay for a bit.
        //
        for(delay = 0; delay < 200000; delay++)
        {
        }
    }
}





