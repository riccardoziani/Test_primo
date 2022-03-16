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

#include "fsl_adc.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get instance number for ADC module.
 *
 * @param base ADC peripheral base address
 */
static uint32_t ADC_GetInstance(ADC_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to ADC bases for each instance. */
static ADC_Type *const s_adcBases[] = ADC_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to ADC clocks for each instance. */
static const clock_ip_name_t s_adcClocks[] = ADC_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t ADC_GetInstance(ADC_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_adcBases); instance++)
    {
        if (s_adcBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_adcBases));

    return instance;
}

void ADC_Init(ADC_Type *base, const adc_config_t *config)
{
    assert(NULL != config);

    uint32_t tmp32;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the clock. */
    CLOCK_EnableClock(s_adcClocks[ADC_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Configure SC2 register. */
    tmp32 = (base->SC2) & (~ADC_SC2_REFSEL_MASK);
    tmp32 |= ADC_SC2_REFSEL(config->referenceVoltageSource);
    base->SC2 = tmp32;

    /* Configure SC3 register. */
    tmp32 =
        ADC_SC3_ADICLK(config->clockSource) | ADC_SC3_MODE(config->ResolutionMode) | ADC_SC3_ADIV(config->clockDivider);
    if (config->enableLowPower)
    {
        tmp32 |= ADC_SC3_ADLPC_MASK;
    }
    if (config->enableLongSampleTime)
    {
        tmp32 |= ADC_SC3_ADLSMP_MASK;
    }
    base->SC3 = tmp32;
}

void ADC_Deinit(ADC_Type *base)
{
    /* Disable ADC module. */
    base->SC1 |= ADC_SC1_ADCH_MASK;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable the clock. */
    CLOCK_DisableClock(s_adcClocks[ADC_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void ADC_GetDefaultConfig(adc_config_t *config)
{
    config->referenceVoltageSource = kADC_ReferenceVoltageSourceAlt0;
    config->enableLowPower = false;
    config->enableLongSampleTime = false;
    config->clockDivider = kADC_ClockDivider1;
    config->ResolutionMode = kADC_Resolution8BitMode;
    config->clockSource = kADC_ClockSourceAlt0;
}

void ADC_SetHardwareCompare(ADC_Type *base, const adc_hardware_compare_config_t *config)
{
    assert(NULL != config);

    uint32_t tmp32;

    /* Configure SC2 register. */
    tmp32 = (base->SC2) & ~(ADC_SC2_ACFGT_MASK | ADC_SC2_ACFE_MASK);
    tmp32 |= ((config->compareMode) << ADC_SC2_ACFGT_SHIFT);
    base->SC2 = tmp32;

    /* Configure CV register. */
    base->CV = ADC_CV_CV(config->compareValue);
}

void ADC_SetFifoConfig(ADC_Type *base, const adc_fifo_config_t *config)
{
    assert(NULL != config);

    uint32_t tmp32;

    /* Configure SC4 register. */
    tmp32 = ADC_SC4_AFDEP(config->FifoDepth);
    if (config->enableFifoScanMode)
    {
        tmp32 |= ADC_SC4_ASCANE_MASK;
    }
    if (config->enableCompareAndMode)
    {
        tmp32 |= ADC_SC4_ACFSEL_MASK;
    }
#if defined(FSL_FEATURE_ADC_HAS_SC4_HTRGME) && FSL_FEATURE_ADC_HAS_SC4_HTRGME
    if (config->enableHWTriggerMultConv)
    {
        tmp32 |= ADC_SC4_HTRGME_MASK;
    }
#endif /* FSL_FEATURE_ADC_HAS_SC4_HTRGME */
    base->SC4 = tmp32;
}

void ADC_SetChannelConfig(ADC_Type *base, const adc_channel_config_t *config)
{
    assert(NULL != config);

    uint32_t tmp32;

    /* Configure SC1 register. */
    tmp32 = ADC_SC1_ADCH(config->channelNumber);
    if (config->enableContinuousConversion)
    {
        tmp32 |= ADC_SC1_ADCO_MASK;
    }
    if (config->enableInterruptOnConversionCompleted)
    {
        tmp32 |= ADC_SC1_AIEN_MASK;
    }
    base->SC1 = tmp32;
}

bool ADC_GetChannelStatusFlags(ADC_Type *base)
{
    if (ADC_SC1_COCO_MASK == (ADC_SC1_COCO_MASK & (base->SC1)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint32_t ADC_GetStatusFlags(ADC_Type *base)
{
    uint32_t tmp32;

    tmp32 = (base->SC2) & (ADC_SC2_ADACT_MASK | ADC_SC2_FEMPTY_MASK | ADC_SC2_FFULL_MASK);

    return tmp32;
}
