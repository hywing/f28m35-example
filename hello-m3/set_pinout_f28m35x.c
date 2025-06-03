//###########################################################################
// FILE:   set_pinout_f28m35x.c
// TITLE:  GPIO setup for the F28M35x controlCARD.
//###########################################################################
// $TI Release: F28M35x Support Library v210 $
// $Release Date: Wed Feb 15 16:25:13 CST 2017 $
// $Copyright: Copyright (C) 2011-2017 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/epi.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "board_drivers/set_pinout_f28m35x.h"

//*****************************************************************************
//
//! \addtogroup set_pinout_api
//! @{
//
//*****************************************************************************



//*****************************************************************************
//
// Set the GPIO port control registers appropriately for the hardware.
//
// This function determines the correct port control settings to enable the
// basic peripheral signals for the development board on their respective pins
// and also ensures that all required EPI signals are correctly routed.  The
// EPI signal configuration is determined from the daughter board information
// structure passed via the \e psInfo parameter.
//
//*****************************************************************************
static void
PortControlSet(void)
{
   
    //
    // To begin with, we set the port control values for all the non-EPI
    // peripherals.
    //


    //UART Setup
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PE4_U0RX);
    GPIOPinConfigure(GPIO_PE5_U0TX);
    
    //SSI SD Card Setup
    GPIOPinTypeSSI(SDC_GPIO_PORT_BASE, SDC_SSI_TX | SDC_SSI_RX | SDC_SSI_CLK);
    GPIOPinTypeGPIOOutput(SDCARD_CS_BASE, SDCARD_CS_PIN);
    GPIOPadConfigSet(SDC_GPIO_PORT_BASE, SDC_SSI_PINS,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(SDCARD_CS_BASE, SDCARD_CS_PIN,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPinConfigure(GPIO_PD0_SSI0TX);
    GPIOPinConfigure(GPIO_PD1_SSI0RX);
    GPIOPinConfigure(GPIO_PD2_SSI0CLK);
    GPIOPinWrite(SDCARD_CS_BASE, SDCARD_CS_PIN, SDCARD_CS_PIN);


    // USB Pins
    GPIOPinConfigure(GPIO_PC5_USB0EPEN);
    GPIOPinConfigure(GPIO_PJ1_USB0PFLT);
    GPIOPinTypeUSBDigital(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinTypeUSBDigital(GPIO_PORTJ_BASE, GPIO_PIN_1);
    GPIOPinTypeUSBAnalog(GPIO_PORTF_BASE, GPIO_PIN_6);
    GPIOPinTypeUSBAnalog(GPIO_PORTG_BASE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);


    //Ethernet pin setup
    GPIODirModeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PC4_MIITXD3);

    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_6, GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_6, GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PE6_MIIMDIO);

    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PF5_MIIRXD3);

    GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_7|GPIO_PIN_3|GPIO_PIN_1|GPIO_PIN_0,
                   GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_7|GPIO_PIN_3|GPIO_PIN_1|
                     GPIO_PIN_0,
                     GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PG0_MIIRXD2);
    GPIOPinConfigure(GPIO_PG1_MIIRXD1);
    GPIOPinConfigure(GPIO_PG3_MIIRXDV);
    GPIOPinConfigure(GPIO_PG7_MIITXER);

    GPIODirModeSet(
        GPIO_PORTH_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|
        GPIO_PIN_3|
        GPIO_PIN_1, GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(
        GPIO_PORTH_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|
        GPIO_PIN_3|
        GPIO_PIN_1, GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PH1_MIIRXD0);
    GPIOPinConfigure(GPIO_PH3_MIITXD2);
    GPIOPinConfigure(GPIO_PH4_MIITXD1);
    GPIOPinConfigure(GPIO_PH5_MIITXD0);
    GPIOPinConfigure(GPIO_PH6_MIITXEN);
    GPIOPinConfigure(GPIO_PH7_MIITXCK);

    GPIODirModeSet(
        GPIO_PORTJ_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|
        GPIO_PIN_3|
        GPIO_PIN_2|GPIO_PIN_0, GPIO_DIR_MODE_HW);
    GPIOPadConfigSet(
        GPIO_PORTJ_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|
        GPIO_PIN_3|
        GPIO_PIN_2|GPIO_PIN_0, GPIO_PIN_TYPE_STD);
    GPIOPinConfigure(GPIO_PJ0_MIIRXER);
    GPIOPinConfigure(GPIO_PJ2_MIIRXCK);
    GPIOPinConfigure(GPIO_PJ3_MIIMDC);
    GPIOPinConfigure(GPIO_PJ4_MIICOL);
    GPIOPinConfigure(GPIO_PJ5_MIICRS);
    GPIOPinConfigure(GPIO_PJ6_MIIPHYINTRn);
    GPIOPinConfigure(GPIO_PJ7_MIIPHYRSTn);


}

//*****************************************************************************
//
//! Configures the device pinout for the development board.
//!
//! This function configures each pin of the device to route the! appropriate
//! peripheral signal as required by the design of the development board.
//!
//! \note This module can be built in two ways.  If the label SIMPLE_PINOUT_SET
//! is not defined, the PinoutSet() function will attempt to read an I2C EEPROM
//! to determine which daughter board is attached to the development kit board
//! and use information from that EEPROM to dynamically configure the EPI
//! appropriately.  In this case, if no EEPROM is found, the EPI configuration
//! will default to that required to use the SDRAM daughter board which is
//! included with the base development kit.
//!
//! If SIMPLE_PINOUT_SET is defined, however, all the dynamic configuration
//! code is replaced with a very simple function which merely sets the pinout
//! and EPI configuration statically.  This is a better representation of how a
//! real-world application would likely initialize the pinout and EPI timing
//! and takes significantly less code space than the dynamic, daughter-board
//! detecting version.  The example offered here sets the pinout and EPI
//! configuration appropriately for the Flash/SRAM/LCD daughter board.
//!
//! \return None.
//
//*****************************************************************************
void
PinoutSet(void)
{
    
    //
    // Enable all GPIO banks.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

    //
    // Determine the port control settings required to enable the EPI pins
    // and other peripheral signals for this daughter board and set all the
    // GPIO port control registers.
    //
    PortControlSet();

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
