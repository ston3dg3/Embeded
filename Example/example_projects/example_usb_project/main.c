/*
 * Copyright (C) 2014 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

/**
 * @file
 * @date 18 Feb, 2015
 * @version 1.0.0
 *
 * @brief USB demo example
 *
 * The example implements a communication through VCOM with a Host (Computer).
 *
 * History <br>
 *
 * Version 1.0.0 Initial <br>
 *
 */

#include "VirtualSerial.h"

/* Clock configuration */
XMC_SCU_CLOCK_CONFIG_t clock_config =
{
    .syspll_config.p_div = 2,
    .syspll_config.n_div = 80,
    .syspll_config.k_div = 4,
    .syspll_config.mode = XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL,
    .syspll_config.clksrc = XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP,
    .enable_oschp = true,
    .calibration_mode = XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_FACTORY,
    .fsys_clksrc = XMC_SCU_CLOCK_SYSCLKSRC_PLL,
    .fsys_clkdiv = 1,
    .fcpu_clkdiv = 1,
    .fccu_clkdiv = 1,
    .fperipheral_clkdiv = 1
};


void SystemCoreClockSetup(void)
{
    /* Setup settings for USB clock */
    XMC_SCU_CLOCK_Init(&clock_config);

    XMC_SCU_CLOCK_EnableUsbPll();
    XMC_SCU_CLOCK_StartUsbPll(2, 64);
    XMC_SCU_CLOCK_SetUsbClockDivider(4);
    XMC_SCU_CLOCK_SetUsbClockSource(XMC_SCU_CLOCK_USBCLKSRC_USBPLL);
    XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_USB);

    SystemCoreClockUpdate();
}

/**
 * Main program entry point. This routine configures the hardware required by
 * the application, then enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    uint16_t Bytes = 0;

    USB_Init();

    while (1)
    {
        /* Check if data received */
        Bytes = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);

        while(Bytes > 0)
        {
            /* Send data back to the host */
            CDC_Device_SendByte(&VirtualSerial_CDC_Interface, CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface));
            --Bytes;
        }

        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    }
}

