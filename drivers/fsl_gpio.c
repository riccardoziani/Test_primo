/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PORT_NUMBERS_EACH_GPIO   (4U)
#define PIN_NUMBERS_EACH_PORT (8U)
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(FSL_FEATURE_SOC_GPIO_COUNT) && FSL_FEATURE_SOC_GPIO_COUNT
static GPIO_Type *const s_gpioBases[] = GPIO_BASE_PTRS;
#endif

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT
static FGPIO_Type *const s_fgpioBases[] = FGPIO_BASE_PTRS;
#if defined(FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL) && FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map FGPIO instance number to clock name. */
static const clock_ip_name_t s_fgpioClockName[] = FGPIO_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#endif /* FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL */

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void GPIO_PinInit(gpio_port_num_t port, uint8_t pin, const gpio_pin_config_t *config)
{
    assert(config);
    
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    GPIO_Type *base = s_gpioBases[instance];

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(1U << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT)));
        base->PIDR &= ~(1U << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT)));   // Riccardo
    }
    else
    {
        GPIO_PinWrite(port, pin, config->outputLogic);
        base->PDDR |= (1U << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT)));
    }
}

void GPIO_PinWrite(gpio_port_num_t port, uint8_t pin, uint8_t output)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    GPIO_Type *base = s_gpioBases[instance];

    if (output == 0U)
    {
        base->PCOR = 1U << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT));
    }
    else
    {
        base->PSOR = 1U << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT));
    }
}

void GPIO_PortSet(gpio_port_num_t port, uint8_t mask)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    GPIO_Type *base = s_gpioBases[instance];

    base->PSOR = ((uint32_t)mask << (shift * PIN_NUMBERS_EACH_PORT));
}

void GPIO_PortClear(gpio_port_num_t port, uint8_t mask)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    GPIO_Type *base = s_gpioBases[instance];

    base->PCOR = ((uint32_t)mask << (shift * PIN_NUMBERS_EACH_PORT));
}

void GPIO_PortToggle(gpio_port_num_t port, uint8_t mask)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    GPIO_Type *base = s_gpioBases[instance];

    base->PTOR = ((uint32_t)mask << (shift * PIN_NUMBERS_EACH_PORT));
}

uint32_t GPIO_PinRead(gpio_port_num_t port, uint8_t pin)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    GPIO_Type *base = s_gpioBases[instance];

    return (((base->PDIR) >> ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT))) & 0x01U);
}

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL) && FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL
void FGPIO_PortInit(FGPIO_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Ungate FGPIO periphral clock */
    CLOCK_EnableClock(s_fgpioClockName[FGPIO_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}
#endif /* FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL */

void FGPIO_PinInit(gpio_port_num_t port, uint8_t pin, const gpio_pin_config_t *config)
{
    assert(config);

    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    FGPIO_Type *base = s_fgpioBases[instance];

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(1U << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT)));
    }
    else
    {
        FGPIO_PinWrite(port, pin, config->outputLogic);
        base->PDDR |= (1U << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT)));
    }
}

void FGPIO_PinWrite(gpio_port_num_t port, uint8_t pin, uint8_t output)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    FGPIO_Type *base = s_fgpioBases[instance];

    if (output == 0U)
    {
        base->PCOR = 1 << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT));
    }
    else
    {
        base->PSOR = 1 << ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT));
    }
}

void FGPIO_PortSet(gpio_port_num_t port, uint8_t mask)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    FGPIO_Type *base = s_fgpioBases[instance];

    base->PSOR = ((uint32_t)mask << (shift * PIN_NUMBERS_EACH_PORT));
}

void FGPIO_PortClear(gpio_port_num_t port, uint8_t mask)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    FGPIO_Type *base = s_fgpioBases[instance];

    base->PCOR = ((uint32_t)mask << (shift * PIN_NUMBERS_EACH_PORT));
}

void FGPIO_PortToggle(gpio_port_num_t port, uint8_t mask)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    FGPIO_Type *base = s_fgpioBases[instance];

    base->PTOR = ((uint32_t)mask << (shift * PIN_NUMBERS_EACH_PORT));
}

uint32_t FGPIO_PinRead(gpio_port_num_t port, uint8_t pin)
{
    uint8_t instance = (uint8_t)port / PORT_NUMBERS_EACH_GPIO;
    uint8_t shift = (uint8_t)port % PORT_NUMBERS_EACH_GPIO;
    FGPIO_Type *base = s_fgpioBases[instance];

    return (((base->PDIR) >> ((uint32_t)pin + (shift * PIN_NUMBERS_EACH_PORT))) & 0x01U);
}

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */
