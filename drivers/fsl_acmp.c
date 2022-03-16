/*
 * The Clear BSD License
 * Copyright 2017 NXP
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

#include "fsl_acmp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get the ACMP instance from the peripheral base address.
 *
 * @param base ACMP peripheral base address.
 * @return ACMP instance.
 */
static uint32_t ACMP_GetInstance(ACMP_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Array of ACMP peripheral base address. */
static ACMP_Type *const s_acmpBases[] = ACMP_BASE_PTRS;
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Clock name of ACMP. */
static const clock_ip_name_t s_acmpClock[] = ACMP_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Codes
 ******************************************************************************/
static uint32_t ACMP_GetInstance(ACMP_Type *base)
{
    uint32_t instance = 0U;
    uint32_t acmpArrayCount = (sizeof(s_acmpBases) / sizeof(s_acmpBases[0]));

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < acmpArrayCount; instance++)
    {
        if (s_acmpBases[instance] == base)
        {
            break;
        }
    }

    return instance;
}

void ACMP_Init(ACMP_Type *base, const acmp_config_t *config)
{
    assert(NULL != config);

    uint8_t tmp8;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Open clock gate. */
    CLOCK_EnableClock(s_acmpClock[ACMP_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Disable the module before configuring it. */
    ACMP_Enable(base, false);

    /* ACMP_CS register. */
    tmp8 = base->CS & (~(ACMP_CS_HYST_MASK | ACMP_CS_ACOPE_MASK));

    tmp8 |= ACMP_CS_HYST(config->hysteresisMode);
    if (config->enablePinOut)
    {
        tmp8 |= ACMP_CS_ACOPE_MASK;
    }
    base->CS = tmp8;
}

void ACMP_Deinit(ACMP_Type *base)
{
    /* Disable the ACMP module. */
    ACMP_Enable(base, false);
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Close clock gate. */
    CLOCK_DisableClock(s_acmpClock[ACMP_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void ACMP_GetDefaultConfig(acmp_config_t *config)
{
    assert(NULL != config);

    config->enablePinOut = false;
    config->hysteresisMode = kACMP_HysterisisLevel1;
}

void ACMP_EnableInterrupt(ACMP_Type *base, acmp_interrupt_mode_t mode)
{
    uint8_t tmp8;

    /* ACMP_CS register. */
    tmp8 = base->CS & (~ACMP_CS_ACMOD_MASK);

    tmp8 |= ACMP_CS_ACIE_MASK | ACMP_CS_ACMOD(mode);
    base->CS = tmp8;
}

void ACMP_SetChannelConfig(ACMP_Type *base,
                           acmp_input_channel_selection_t PositiveInput,
                           acmp_input_channel_selection_t negativeInout)
{
    uint8_t tmp8;

    /* ACMP_C0 register. */
    tmp8 = ACMP_C0_ACPSEL(PositiveInput) | ACMP_C0_ACNSEL(negativeInout);
    base->C0 = tmp8;
}

void ACMP_SetDACConfig(ACMP_Type *base, const acmp_dac_config_t *config)
{
    uint8_t tmp8;

    /* "NULL" is for disabling the feature. */
    if (NULL == config)
    {
        tmp8 = 0U;
    }
    else
    {
        /* ACMP_C1 register. */
        tmp8 = ACMP_C1_DACEN_MASK | ACMP_C1_DACREF(config->referenceVoltageSource) | ACMP_C1_DACVAL(config->DACValue);
    }
    base->C1 = tmp8;
}

void ACMP_EnableInputPin(ACMP_Type *base, uint32_t mask, bool enable)
{
    assert(mask <= ACMP_C2_ACIPE_MASK);

    if (enable)
    {
        base->C2 |= ACMP_C2_ACIPE(mask);
    }
    else
    {
        base->C2 &= ~ACMP_C2_ACIPE(mask);
    }
}
