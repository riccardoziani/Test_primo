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

#ifndef _FSL_ACMP_H_
#define _FSL_ACMP_H_

#include "fsl_common.h"

/*!
 * @addtogroup acmp
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief ACMP driver version 2.0.0. */
#define FSL_ACMP_DRIVER_VERSION (MAKE_VERSION(2U, 0U, 0U))
/*@}*/

/*!
* @brief Analog Comparator Hysterisis Selection.
*/
typedef enum _acmp_hysterisis_mode
{
    kACMP_HysterisisLevel1 = 0U, /*!< ACMP hysterisis is 20mv. >*/
    kACMP_HysterisisLevel2 = 1U, /*!< ACMP hysterisis is 30mv. >*/
} acmp_hysterisis_mode_t;

/*!
* @brief DAC Voltage Reference source.
*/
typedef enum _acmp_reference_voltage_source
{
    kACMP_VrefSourceVin1 = 0U, /*!< The DAC selects Bandgap as the reference. */
    kACMP_VrefSourceVin2 = 1U, /*!< The DAC selects VDDA as the reference. */
} acmp_reference_voltage_source_t;

/*!
* @brief The sensitivity modes of the interrupt trigger.
*/
typedef enum _acmp_interrupt_mode
{
    kACMP_OutputFallingInterruptMode = 0U, /*!< ACMP interrupt on output falling edge. >*/
    kACMP_OutputRisingInterruptMode = 1U,  /*!< ACMP interrupt on output rising edge. >*/
    //    kACMP_OutputFallingInterruptMode = 2U,  /*!< ACMP interrupt on output falling edge. >*/
    kACMP_OutputBothEdgeInterruptMode = 3U, /*!< ACMP interrupt on output falling or rising edge. >*/
} acmp_interrupt_mode_t;

/*!
* @brief The ACMP input channel selection.
*/
typedef enum _acmp_input_channel_selection
{
    kACMP_ExternalReference0 = 0U, /*!< External reference 0 is selected to as input channel. >*/
    kACMP_ExternalReference1 = 1U, /*!< External reference 1 is selected to as input channel. >*/
    kACMP_ExternalReference2 = 2U, /*!< External reference 2 is selected to as input channel. >*/
    kACMP_InternalDACOutput = 3U,  /*!< Internal DAC putput is selected to as input channel. >*/
} acmp_input_channel_selection_t;

enum _acmp_status_flags
{
    kACMP_InterruptFlag = ACMP_CS_ACF_MASK, /*!< ACMP interrupt on output valid edge. >*/
    kACMP_OutputFlag = ACMP_CS_ACO_MASK,    /*!< The current value of the analog comparator output. >*/
};

/*!
* @brief Configuration for ACMP.
*/
typedef struct _acmp_config
{
    bool enablePinOut;                     /*!< The comparator output is available on the associated pin. */
    acmp_hysterisis_mode_t hysteresisMode; /*!< Hysteresis mode. */
} acmp_config_t;

/*!
* @brief Configuration for Internal DAC.
*/
typedef struct _acmp_dac_config
{
    uint8_t DACValue; /*!< Value for DAC Output Voltage. Available range is 0-63. */
    acmp_reference_voltage_source_t referenceVoltageSource; /*!< Supply voltage reference source. */
} acmp_dac_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
* @brief Initialize the ACMP.
*
* The default configuration can be got by calling ACMP_GetDefaultConfig().
*
* @param base ACMP peripheral base address.
* @param config Pointer to ACMP configuration structure.
*/
void ACMP_Init(ACMP_Type *base, const acmp_config_t *config);

/*!
* @brief De-Initialize the ACMP.
*
* @param base ACMP peripheral basic address.
*/
void ACMP_Deinit(ACMP_Type *base);

/*!
* @brief Gets the default configuration for ACMP.
*
* This function initializes the user configuration structure to default value. The default value are:
* Example:
*   @code
*   config->enablePinOut = false;
*   config->hysteresisMode = kACMP_HysterisisLevel1;
*   @endcode
*
* @param config Pointer to ACMP configuration structure.
*/
void ACMP_GetDefaultConfig(acmp_config_t *base);

/*!
* @brief Enable/Disable the ACMP module.
*
* @param base ACMP peripheral base address.
@ @param enable Switcher to enable/disable ACMP module.
*/
static inline void ACMP_Enable(ACMP_Type *base, bool enable)
{
    if (enable)
    {
        base->CS |= ACMP_CS_ACE_MASK;
    }
    else
    {
        base->CS &= ~ACMP_CS_ACE_MASK;
    }
}

/*!
* @brief Enable the ACMP interrupt and determines the sensitivity modes of the interrupt trigger.
*
*
* @param base ACMP peripheral base address.
* @param mode Select one interrupt mode to generate interrupt.
*/
void ACMP_EnableInterrupt(ACMP_Type *base, acmp_interrupt_mode_t mode);

/*!
* @breif Disable the ACMP interrupt.
*
* @param base ACMP peripheral base address.
*/
static inline void ACMP_DisableInterrupt(ACMP_Type *base)
{
    base->CS &= ~ACMP_CS_ACIE_MASK;
}

/*!
* @brief Configure the ACMP positive and negative input channel.
*
* @param base ACMP peripheral base address.
* @param PositiveInput ACMP Positive Input Select. Refer to "acmp_input_channel_selection_t".
* @param negativeInout ACMP Negative Input Select. Refer to "acmp_input_channel_selection_t".
*/
void ACMP_SetChannelConfig(ACMP_Type *base,
                           acmp_input_channel_selection_t PositiveInput,
                           acmp_input_channel_selection_t negativeInout);

/*
* @brief Configure the internal DAC.
*
* @param base ACMP peripheral base address.
* @param config Pointer to DAC configuration structure. "NULL" is for disabling the feature.
*/
void ACMP_SetDACConfig(ACMP_Type *base, const acmp_dac_config_t *config);

/*!
* @brief Enable/Disable ACMP input pin.
*        The API controls if the corresponding ACMP external pin can be driven by an analog input
*
* @param base ACMP peripheral base address.
* @param mask The mask of the pin associated with channel ADx. Valid range is AD0:0x1U ~ AD3:0x4U.
*             For example: If enable AD0, AD1 and AD2 pins, mask should be set to 0x7U(0x1 | 0x2 | 0x4).
*/
void ACMP_EnableInputPin(ACMP_Type *base, uint32_t mask, bool enable);

/*!
* @brief Get ACMP status flags.
*
* @param base ACMP peripheral base address.
* @return Flags' mask if indicated flags are asserted. See "_acmp_status_flags".
*/
static inline uint8_t ACMP_GetStatusFlags(ACMP_Type *base)
{
    return (base->CS) & (ACMP_CS_ACF_MASK | ACMP_CS_ACO_MASK);
}

/*!
* @brief Clear interrupts status flag.
*
* @param base ACMP peripheral base address.
*/
static inline void ACMP_ClearInterruptFlags(ACMP_Type *base)
{
    base->CS &= ~ACMP_CS_ACF_MASK;
}
#endif /* _FSL_ACMP_H_ */
