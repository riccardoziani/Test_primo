/*******************************************************************************
Filename: hal.c
Author: Riccardo Ziani
Created: 24/02/2015
Description: 

*******************************************************************************/
// include files
#include "hal.h"
//#include "sig.h"
//#include "SAECO.h"


// private definitions *********************************************************

// private typedefs ************************************************************

// global data *****************************************************************

//Ac*       ac          = AC;
//Adc*      adc         = ADC;
//Port*     port        = PORT;
//Rtc*      rtc         = RTC;
//Sercom*   uscp_sercom = SERCOM1;
//Sercom*   lin_sercom  = SERCOM4;
//Sysctrl*  sysctrl     = SYSCTRL;
//Nvmctrl*  nvm_module  = NVMCTRL;
//Wdt*      wdt         = WDT;

kbi_config_t kbiConfig;
ftm_config_t ftmInfo;
pit_config_t pitConfig;
tpm_config_t tpm0_Info;
tpm_config_t tpm1_Info;
ftm_chnl_pwm_signal_param_t ftm0_Param;
ftm_chnl_pwm_signal_param_t ftm1_Param;
ftm_chnl_pwm_signal_param_t ftm2_Param;
ftm_chnl_pwm_signal_param_t ftm3_Param;
ftm_chnl_pwm_signal_param_t ftm4_Param;
ftm_chnl_pwm_signal_param_t ftm5_Param;
ftm_chnl_pwm_signal_param_t ftm6_Param;


tpm_chnl_pwm_signal_param_t tpm0_Param;
tpm_chnl_pwm_signal_param_t tpm1_Param;

volatile    uint16_t    main_freq_tmp;
volatile    uint32_t    testptr;
volatile    uint32_t    led_msec_delay;

// private data ****************************************************************

bool_t  pcb_voltage230V; /* set true if the board is 230V */

// private functions ***********************************************************

void_t  HAL_U_VOLTAGE_CONFIG(void_t);

// Public functions ***********************************************************




/**
 * \brief
 * Configure zero-crossing interrupt routine on FTM2.
 * Enable Input capture event on PTD0 pin(zero-crossing).
 * Channel 2 -> Used for zero-crossing input capture
 * Channel 3 -> Used for Grinder control partialization
 * Channel 4 -> Used for Pump control partialization
 * Channel 5 -> Used for output compare, sync when to turn-off loads, or turn-on AC loads @ full power
 *
 * \author Riccardo Ziani
 * \date   23/01/2018
 *
 * @return void_t
 */
void_t Hal_FTM2_Init (void_t)
{
    // ************  FTM2 Init  ***********

    /* Define the init structure for the default input pin */
    gpio_pin_config_t pin_config_DigitalInput = {
    kGPIO_DigitalInput, 0
    };


//    /* Define the init structure for the default output pin LOW*/
//    gpio_pin_config_t pin_config_DigitalOutput_Low = {
//    kGPIO_DigitalOutput, 0
//    };


    ftm0_Param.chnlNumber = kFTM_Chnl_0;
    ftm0_Param.dutyCyclePercent = 30;
    ftm0_Param.level = kFTM_LowTrue;

    ftm2_Param.chnlNumber = kFTM_Chnl_1;
    ftm2_Param.dutyCyclePercent = 30;
    ftm2_Param.level = kFTM_LowTrue;


    FTM_GetDefaultConfig(&ftmInfo);
    ftmInfo.prescale = kFTM_Prescale_Divide_16;
    /* Update deadTimePrescale for fast clock*/
    ftmInfo.deadTimePrescale = kFTM_Deadtime_Prescale_16;
    /* Need a deadtime value of about 650nsec */
    ftmInfo.deadTimeValue = 63;

    GPIO_PinInit(kGPIO_PORTC, 0, &pin_config_DigitalInput);
    GPIO_PinInit(kGPIO_PORTC, 1, &pin_config_DigitalInput);
    GPIO_PinInit(kGPIO_PORTC, 2, &pin_config_DigitalInput);
    GPIO_PinInit(kGPIO_PORTC, 3, &pin_config_DigitalInput);
    //GPIO_PinInit(kGPIO_PORTC, 0, &pin_config_DigitalOutput_Low);
    //GPIO_PortSet(kGPIO_PORTC, 1u << 0);

    /* Initialize FTM module */
    FTM_Init(FTM2, &ftmInfo);

    /* Setup output of a combined PWM signal */
    FTM_SetupPwm(FTM2, &ftm0_Param, 1U, kFTM_CombinedPwm, 1U, CLOCK_GetFreq(kCLOCK_LpoClk));
    FTM_SetupPwm(FTM2, &ftm2_Param, 1U, kFTM_CombinedPwm, 1U, CLOCK_GetFreq(kCLOCK_LpoClk));
//  FTM_SetupPwm(FTM2, &ftm0_Param, 1U, kFTM_CombinedPwm, 1U, kFTM_FixedClock);

    /* Enable complementary output on the channel pair */
    FTM_SetComplementaryEnable(FTM2, kFTM_Chnl_0, true);

    /* Enable complementary output on the channel pair */
    FTM_SetComplementaryEnable(FTM2, kFTM_Chnl_1, true);

    /* Enable Deadtime insertion on the channel pair */
    FTM_SetDeadTimeEnable(FTM2, kFTM_Chnl_0, true);

    /* Enable Deadtime insertion on the channel pair */
    FTM_SetDeadTimeEnable(FTM2, kFTM_Chnl_2, true);


    FTM2->CNTIN = 2000;
    FTM2->MOD = 6000;
    FTM2->CONTROLS[0].CnV = 2500;
    FTM2->CONTROLS[1].CnV = 3000;

    FTM2->CONTROLS[2].CnV = 4600;
    FTM2->CONTROLS[3].CnV = 5000;
    FTM_StartTimer(FTM2, kFTM_FixedClock);








    //FTM2->MODE = FTM_MODE_FTMEN(0); // Update registers immediately, no buffer update...

    ////* Setup edge capture on a FTM channel 2 */
    //FTM_SetupInputCapture(FTM2, kFTM_Chnl_2, kFTM_RisingEdge, 15);


//  /* Enable channel interrupt when the edge is detected */
//  FTM_EnableInterrupts(FTM2, kFTM_Chnl2InterruptEnable);
//  NVIC_SetPriority (FTM2_IRQn, 2); // Set Priority level 2.
//
//  /* Enable at the NVIC */
//  EnableIRQ(FTM2_IRQn);


    return;
} // end of function Hal_FTM2_Init


/**
 * \brief
 * Deinit the FTM2 timer module
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_FTM2_Zx_Deinit (void_t)
{
  FTM_Deinit(FTM2);
    return;
} // end of function Hal_FTM2_Zx_Deinit



/**
 * \brief
 * Init the TPM0 peripheral to drive EV in PWM
 * PWM have a period of 45,45 usec -> 22 KHz
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_Ev_Init (void_t)
{
    // ************  TPM0 Init  ***********

    /* Configure tpm params with frequency 22kHZ @ 70% */
    tpm0_Param.chnlNumber = kTPM_Chnl_0;
    tpm0_Param.level = kTPM_HighTrue;
    tpm0_Param.dutyCyclePercent = 70;

    TPM_GetDefaultConfig(&tpm0_Info);

    /* Initialize TPM module */
    TPM_Init(TPM0, &tpm0_Info);

    TPM_SetupPwm(TPM0, &tpm0_Param, 1U, kTPM_CenterAlignedPwm, 22000U, CLOCK_GetFreq(kCLOCK_BusClk));

    TPM_StartTimer(TPM0, kTPM_SystemClock);

    return;
} // end of function Hal_Ev_Init


/**
 * \brief
 * Deinit the TPM0 peripheral used to drive EV in PWM
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_Ev_Deinit (void_t)
{
    TPM_Deinit(TPM0);
    return;
} // end of function Hal_Ev_Deinit


/**
 * \brief
 * Init the Pit module channel 0 and channel 1.
 * Channel 0: Periodic interrupt timer every 1 msecond 
 * Channel 1: No interrupt free running, module 32 bit @bus clock(268,435 seconds @ 16 MHz)).
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_Pit_Init (void_t)
{
    // ************  PIT Init  ***********

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);
    PIT->MCR |= PIT_MCR_FRZ(1); // PITs are stopped in debug mode...

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000U, CLOCK_GetFreq(kCLOCK_BusClk))-1);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, 0xffffffff);

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    NVIC_SetPriority (PIT_CH0_IRQn, 3); // Set lowest Iterrupt Priority.

    /* Enable at the NVIC */
    EnableIRQ(PIT_CH0_IRQn);

    /* Start channel 0 */
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    /* Start channel 1 */
    PIT_StartTimer(PIT, kPIT_Chnl_1);

    return;
} // end of function Hal_Pit_Init


/**
 * \brief
 * Deinit the PIT peripheral
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_Pit_Deinit (void_t)
{
    PIT_Deinit(PIT);
    return;
} // end of function Hal_Pit_Deinit












/**
 * \brief
 * Init the TPM1 peripheral to drive Buzzer in PWM
 * PWM have a period of 212,76 usec -> 4,7kHZ @ 50%
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 *                                           
 * \return None 
 */
void_t Hal_Buzzer_Init (void_t)
{
    // ************  TPM1 Init  ***********

    /* Init output for buzzer PWM. */

     // Remember! set gpio state before seriin pwm in order to hava a correct state in idle mode...
    //GPIO_PinInit(kGPIO_PORTC, 5, &pin_config_DigitalOutput_Low);
    //GPIO_PortSet(kGPIO_PORTC, 1u << 5);

    /* Configure tpm params with frequency 4,7kHZ @ 50% */
    tpm1_Param.chnlNumber = kTPM_Chnl_1;
    tpm1_Param.level = kTPM_HighTrue;
    tpm1_Param.dutyCyclePercent = 50;

    TPM_GetDefaultConfig(&tpm1_Info);

    /* Initialize TPM module */
    TPM_Init(TPM1, &tpm1_Info);

    TPM_SetupPwm(TPM1, &tpm1_Param, 1U, kTPM_EdgeAlignedPwm, 4700U, CLOCK_GetFreq(kCLOCK_BusClk));

    TPM_StartTimer(TPM1, kTPM_SystemClock);

    return;
} // end of function Hal_Buzzer_Init


/**
 * \brief
 * Deinit the TPM1 peripheral used to drive Buzzer in PWM
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_Buzzer_Deinit (void_t)
{
    TPM_Deinit(TPM1);
    return;
} // end of function Hal_Buzzer_Deinit



/**
 * \brief
 * Init the KBI0 peripheral to set external interrupt for grinder sensor.
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_KBI0_Init (void_t)
{
    // ************  KBI0 Init  ***********

    kbiConfig.mode = kKBI_EdgesDetect;
    kbiConfig.pinsEnabled = GR_SENSOR_PIN;
    kbiConfig.pinsEdge = GR_SENSOR_PIN; /* Raising edge.*/
    KBI_Init(KBI0, &kbiConfig);
    NVIC_SetPriority (KBI0_IRQn, 3); // Set Priority level 3.
    PORT_SetFilterSelect(PORT, kPORT_FilterKBI0, kPORT_FILTERDIV2); /* Filter Selection for Input from KBI0: 0x02u */
    return;
} // end of function Hal_KBI0_Init


/**
 * \brief
 * Deinit the KBI0 peripheral.
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t Hal_KBI0_Deinit (void_t)
{
    KBI_Deinit(KBI0);
    return;
} // end of function Hal_KBI0_Deinit

/**
 * \brief
 * Init the KBI0 peripheral to set external interrupt falling bedge.
 * 
 * \author Riccardo Ziani
 * \date   23/01/2018
 * 
 * \return None 
 */
void_t HAL_Resync_Gr_Sensor_Pulses (void_t)
{
    kbiConfig.mode = kKBI_EdgesDetect;
    kbiConfig.pinsEnabled = GR_SENSOR_PIN;
    kbiConfig.pinsEdge = 0;                  /* Falling edge.*/
    KBI_Init(KBI0, &kbiConfig);
    return;
} // end of function HAL_Resync_Gr_Sensor_Pulses



////******************************************************************************
//// Function: Hal_SYSCTRL_Stby_Mode
//// Created: 24/03/2015
//// Parameters:   None
//// Returns:      None
//// Description:  < place here any information concerning the function purpose >
////
////******************************************************************************
//void_t Hal_SYSCTRL_Stby_Mode (void_t)
//{
//#if (CONF_CLOCK_DPLL_ENABLE == true)
//
//        sysctrl->DPLLCTRLA.reg &= SYSCTRL_DPLLCTRLA_RESETVALUE;
//#endif
//
//#if (CONF_CLOCK_DFLL_ENABLE == true)
//        while (!sysctrl->PCLKSR.bit.DFLLRDY);   // Wait for DFLL sync
//        sysctrl->DFLLCTRL.reg &= ~SYSCTRL_DFLLCTRL_ENABLE;
//#endif
//
//        while (!sysctrl->PCLKSR.bit.BOD33RDY);
//        sysctrl->BOD33.reg &= ~SYSCTRL_BOD33_ENABLE;
//
//        sysctrl->VREF.reg &= ~SYSCTRL_VREF_MASK;
//
//#if (CONF_CLOCK_OSC32K_ENABLE == true)
//        sysctrl->OSC32K.reg = SYSCTRL_OSC32K_RESETVALUE;
//#endif
//        return;
//} // end of function Hal_SYSCTRL_Stby_Mode






/**
 * \brief
 * Init timer for PWM control on BU direction pins
 *
 * \author Emanuel Urgese
 * \date   09/03/2016
 * \param  None
 * \return None
 */
void_t HAL_Bu_Init (void_t)
{
//  /** Configure the generic clock for the module tc7 --> GLCK3 (DFLL 48MHz/2) */
//      gclk->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(GCLK_CLKCTRL_GEN_GCLK3_Val) | GCLK_CLKCTRL_ID(TC7_GCLK_ID);
//
//      while(tc7->COUNT8.STATUS.bit.SYNCBUSY);
//      tc7->COUNT8.CTRLA.bit.SWRST = true;
//      while(tc7->COUNT8.CTRLA.bit.SWRST || tc7->COUNT8.STATUS.bit.SYNCBUSY);
//
//      /**
//       * Set CTRLA register for TC7
//       *
//       * bit [15:14]  -> Reserved
//       * bit [13:12]  -> PRESCSYNC: Prescaler and counter synchronization
//       * bit 11       -> RUNSTDBY: Run in stand-by
//       * bit [10:8]   -> PRESCALER: Prescaler
//       * bit 7        -> Reserved
//       * bit [6:5]    -> WAVEGEN: Waveform generation operation
//       * bit 4        -> Reserved
//       * bit [3:2]    -> MODE: TC mode
//       * bit 1        -> ENABLE: Enable
//       * bit 0        -> SWRST: Software reset
//       * Write protected, Enable protected, Write synchronized
//       */
//      tc7->COUNT8.CTRLA.reg =         TC_CTRLA_PRESCSYNC(TC_CTRLA_PRESCSYNC_RESYNC_Val)       |       /** Reload counter on next clock */
//                                                              TC_CTRLA_PRESCALER(TC_CTRLA_PRESCALER_DIV4_Val)         |       /**  6 MHz clock module after prescaler...(24MHz/4) */
//                                                              TC_CTRLA_WAVEGEN(TC_CTRLA_WAVEGEN_NPWM_Val)                     |       /**  Normal PWM (Top value is PER), Clear when reach */
//                                                              TC_CTRLA_MODE(TC_CTRLA_MODE_COUNT8_Val)                         |       /**  Counter 8bit */
//                                                              TC_CTRLA_ENABLE;                                                                                /**  Enable */
//
//      while(tc7->COUNT8.STATUS.bit.SYNCBUSY);
//
//      /**
//       * Set Period value register --> 255 = max period in 8bit mode
//       * -> 42.3 us (23,64Khz)
//       * Write synchronized, Read synchronized
//       */
//      tc7->COUNT8.PER.reg = 255;
//      while(tc7->COUNT8.STATUS.bit.SYNCBUSY);
//      return;
}



//******************************************************************************
// Function: Hal_Zero_Cross_Stby_Mode
// Created: 25/03/2015
// Parameters:   None  
// Returns:      None
// Description:  < place here any information concerning the function purpose >
// 
//******************************************************************************
void_t Hal_Zero_Cross_Stby_Mode (void_t)
{
//      while(tcc0->SYNCBUSY.reg & TCC_SYNCBUSY_ENABLE);
//      tcc0->CTRLA.reg &= ~TCC_CTRLA_ENABLE;
//
//      tcc0->CTRLA.bit.SWRST = true;
//  while(tcc0->CTRLA.bit.SWRST);
//
//      system_interrupt_disable((enum system_interrupt_vector)TCC0_IRQn);	// Disable NVIC
//      NVIC_SetPriority (TCC0_IRQn, SYSTEM_INTERRUPT_PRIORITY_LEVEL_0);
//      return;
} // end of function Hal_Zero_Cross_Stby_Mode


//******************************************************************************
// Function: Hal_WDT_Stby_Mode
// Created: 25/03/2015
// Parameters:   None  
// Returns:      None
// Description:  < place here any information concerning the function purpose >
// 
//******************************************************************************
void_t Hal_WDT_Stby_Mode (void_t)
{
//      HAL_WDT_Disable();
//      system_interrupt_disable(SYSTEM_INTERRUPT_MODULE_WDT);
//      NVIC_SetPriority (WDT_IRQn, SYSTEM_INTERRUPT_PRIORITY_LEVEL_0);
//      wdt->INTENCLR.reg = WDT_INTENCLR_MASK;	// Disable any interrupt requests.
//      wdt->INTFLAG.reg = WDT_INTFLAG_MASK;	// Resets any pendings flags.
//      return;
} // end of function Hal_WDT_Stby_Mode


////******************************************************************************
//// Function: Hal_EIC_Stby_Mode
//// Created: 25/03/2015
//// Parameters:   None
//// Returns:      None
//// Description:  < place here any information concerning the function purpose >
////
////******************************************************************************
//void_t Hal_EIC_Stby_Mode (void_t)
//{
//        /* Configure the generic clock 0 for the module(8MHz) and enable it */
//        gclk->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(GCLK_CLKCTRL_GEN_GCLK0_Val) | GCLK_CLKCTRL_ID(EIC_GCLK_ID);
//        system_interrupt_disable(SYSTEM_INTERRUPT_MODULE_EIC);
//        eic->INTFLAG.reg = EIC_INTFLAG_MASK;    // Clear any interrupt flags on the module
//        return;
//} // end of function Hal_EIC_Stby_Mode
//  //

////******************************************************************************
//// Function: Hal_AHB_APBx_Stby_Mode
//// Created: 25/03/2015
//// Parameters:   None
//// Returns:      None
//// Description:  < place here any information concerning the function purpose >
////
////******************************************************************************
//void_t Hal_AHB_APBx_Stby_Mode (void_t)
//{
//        /* disable APB clock Bridge C for All peripherals not used in stby*/
//        pm->APBCMASK.reg = PM_APBCMASK_RESETVALUE & ~PM_APBCMASK_ADC;
//
//        /* disable APB clock Bridge B for All peripherals not used in stby*/
//        pm->APBBMASK.reg =      ~PM_APBBMASK_USB & ~PM_APBBMASK_DMAC &
//                                                ~PM_APBBMASK_PAC1;
//
//        /* disable APB clock Bridge A for All peripherals not used in stby*/
//        pm->APBAMASK.reg =      ~PM_APBAMASK_WDT & ~PM_APBAMASK_PAC0;
//
//        /* disable AHB clock  All peripherals not used in stby*/
//        pm->AHBMASK.reg =       ~PM_AHBMASK_USB  & ~PM_AHBMASK_DMAC;
//        return;
//
//} // end of function Hal_AHB_APBx_Stby_Mode


////******************************************************************************
//// Function: Hal_AHB_APBx_Normal_Operation
//// Created: 03/02/2016
//// Parameters:   None
//// Returns:      None
//// Description:  Enable bus clock for all the peripherals inside the uC
////
////******************************************************************************
//void_t Hal_AHB_APBx_Normal_Operation (void_t)
//{
//        /* Enable AHB clock fora all peripherals
//        Bit6 USB
//        bit5 DMAC
//        bit4 NVM Control
//        bit3 DSU
//        bit2 HPB2
//        bit1 HPB1
//        bit0 HPB0
//        */
//        pm->AHBMASK.reg = PM_AHBMASK_MASK;
//
//        /* Enable APB clock Bridge A for all peripherals
//        Bit6 EIC
//        bit5 RTC
//        bit4 WDT
//        bit3 GCLK
//        bit2 SYSCTRL
//        bit1 PM
//        bit0 PAC0
//        */
//        pm->APBAMASK.reg = PM_APBAMASK_MASK;
//
//        /* Enable APB clock Bridge B for all peripherals
//        bit5 USB
//        bit4 DMAC
//        bit3 PORT
//        bit2 NVM CTRL
//        bit1 DSU
//        bit0 PAC1
//        */
//        pm->APBBMASK.reg =      PM_APBBMASK_MASK;
//
//        /* Enable APB clock Bridge C for all peripherals
//        bit21 AC1       bit14 TC6       bit7 SERCOM5    bit0 PAC2
//        bit20 I2S       bit13 TC5       bit6 SERCOM4
//        bit19 PTC       bit12 TC4       bit5 SERCOM3
//        bit18 DAC       bit11 TC3       bit4 SERCOM2
//        bit17 AC        bit10 TCC2      bit3 SERCOM1
//        bit16 ADC       bit9 TCC1       bit2 SERCOM0
//        bit15 TC7       bit8 TCC0       bit1 EVSYS
//        */
//        pm->APBCMASK.reg = PM_APBCMASK_MASK;
//        return;
//} // end of function Hal_AHB_APBx_Normal_Operation


///**
// * \brief
// * Initialize ADC uC peripheral in order to acquire the following signal
// * - U_BU_CURRENT on AIN7
// * - U_NTC_BOILER on AIN6
// * Peripheral is clocked @ 2MHz (clock source is GCLK4 prescaled by 4)
// * Sampling time is set to 5us every sample.
// * - Channel NTC:
// *   + Reference VANA/2 -> Gain set to 1/2x
// *   + Accumulation of 128 samples (automatic average to get 16 bit result)
// *     after shifted by 6 position in order to have 10 bit result
// * - Channel BU:
// *   + Reference 1V internal reference
// *   + Accumulation of 128 samples (automatic average to get 16 bit result)
// *     after shifted by 4 position in order to have 12 bit result
// * The function set the register to start the conversion for BU channel first.
// * Interrupt Priority -> LEVEL 3
// * Must be called when initialize hardware before to start the main loop.
// *
// * \author Emanuel Urgese
// * \date 20/01/2016
// *
// * \return None
// */
//void_t HAL_ADC_Init (void_t)
//{
//        uint16_t dummy_value;
//    /**
//     * Configure VREF register;
//     * bit [31:27]  -> Reserved
//     * bit [26:16]      -> CALIB: Bandgap voltage generator calibration
//     * bit [15:3]       -> Reserved
//     * bit 2            -> BGOUTEN: Bandgap output enable
//     * bit 1            -> TSEN: Temperature sensor enable
//     * bit 0            -> Reserved
//     * Write protected
//     * NB: Calibration value is loaded from internal NVM at reset
//     */
//        sysctrl->VREF.reg |= SYSCTRL_VREF_BGOUTEN;      /** Bandgap Output Enable */
//        //----------------------------------------------------------------------
//
//        /** Configure the generic clock for the module ADC(OSC8) and enable it */
//        gclk->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(GCLK_CLKCTRL_GEN_GCLK4_Val) | GCLK_CLKCTRL_ID(ADC_GCLK_ID);
//
//        adc->CTRLA.reg |= ADC_CTRLA_ENABLE;         /** Enable ADC module*/
//        while(adc->STATUS.bit.SYNCBUSY);
//
//
//    /**
//     * Configure CALIB register;
//     * bit [15:11]  -> Reserved
//     * bit [10:8]       -> BIAS_CAL: Bias calibration value
//     * bit [7:0]        -> LINEARITY_CAL: Linearity calibration value
//     * Write protected
//     * Load in the fixed device ADC calibration constants
//     */
//        adc->CALIB.reg =            ADC_CALIB_BIAS_CAL(*(uint32_t*)ADC_FUSES_BIASCAL_ADDR >> ADC_FUSES_BIASCAL_Pos) |
//                                    ADC_CALIB_LINEARITY_CAL(*(uint64_t*)ADC_FUSES_LINEARITY_0_ADDR >> ADC_FUSES_LINEARITY_0_Pos);
//
//    /**
//     * Configure SAMPCTRL register;
//     * bit [7:6]        -> Reserved
//     * bit [5:0]        -> SAMPLEN: Sampling time lenght
//     * Write protected
//     */
//        adc->SAMPCTRL.reg =     ADC_SAMPCTRL_SAMPLEN(9);        /** Sampling Time  = (SAMPLEN+1)*CLKADC/2 -> 10 us */
//
//    /**
//     * Configure CTRLB register;
//     * bit [15:11]      -> Reserved
//     * bit [10:8]               -> PRESCALER: Prescaler configuration
//     * bit [7:6]                -> Reserved
//     * bit [5:4]                -> RESSEL: Conversion result resolution
//     * bit 3                    -> CORREN: Digital correction logic enabled
//     * bit 2                    -> FREERUN: Free running mode
//     * bit 1                    -> LEFTADJ: Left adjust result
//     * bit 0                    -> DIFFMODE: Differential mode
//     * Write protected, write syncronized
//     */
//        adc->CTRLB.reg =                ADC_CTRLB_PRESCALER(ADC_CTRLB_PRESCALER_DIV4_Val) |             /** Peripheral clock divided by 4 (2MHz)    */
//                                        ADC_CTRLB_RESSEL(ADC_CTRLB_RESSEL_16BIT_Val);                   /** 16 For averaging mode output            */
//
//        while(adc->STATUS.bit.SYNCBUSY);
//
////	-------------------------- Prepare for ADC BU current conversion -------------------------------------------
//
//    /**
//     * Configure AVGCTRL register;
//     * bit 7            -> Reserved
//     * bit [6:4]        -> ADJRES: Adjusting result
//     * bit [3:0]        -> SAMPLENUM: Number of Samples to be collected
//     * Write protected
//     */
//        adc->AVGCTRL.reg =          ADC_AVGCTRL_ADJRES(4)|                                      /** Division factor 16 result 12 bit    */
//                                    ADC_AVGCTRL_SAMPLENUM(ADC_AVGCTRL_SAMPLENUM_128_Val);       /** 128 sample                          */
//
//    /**
//     * Configure REFCTRL register;
//     * bit 7            -> REFCOMP: Reference buffer offset compensation
//     * bit [6:4]                -> Reserved
//     * bit [3:0]                -> REFSEL: Reference selection
//     * Write protected
//     */
//        adc->REFCTRL.reg =              ADC_REFCTRL_REFCOMP |                                           /** enabled               */
//                                        ADC_REFCTRL_REFSEL(ADC_REFCTRL_REFSEL_INT1V_Val);               /** Internal 1V reference */
//
//    /**
//     * Configure INPUTCTRL register;
//     * bit [31:28]      -> Reserved
//     * bit [27:24]      -> GAIN: Gain factor selection
//     * bit [23:20]      -> INPUTOFFSET: Positive mux setting offset
//     * bit [19:16]      -> INPUTSCAN: Number of input channel
//     * included in scan
//     * bit [15:13]              -> Reserved
//     * bit [12:8]               -> MUXNEG: Negative mux input selection
//     * bit [7:5]                -> Reserved
//     * bit [4:0]                -> Positive mux input selection
//     * Write protected, write syncronized
//     */
//        adc->INPUTCTRL.reg =    ADC_INPUTCTRL_GAIN(ADC_INPUTCTRL_GAIN_1X_Val)           |   /** Gain x1                 */
//                                ADC_INPUTCTRL_INPUTOFFSET(0)                            |   /** No offset               */
//                                ADC_INPUTCTRL_INPUTSCAN(0)                              |   /** No scan                 */
//                                ADC_INPUTCTRL_MUXNEG(ADC_INPUTCTRL_MUXNEG_IOGND_Val)    |   /** Internal Ground         */
//                                ADC_INPUTCTRL_MUXPOS(ADC_INPUTCTRL_MUXPOS_PIN7_Val);        /** ADC AIN7 Bu current     */
//
//        while(adc->STATUS.bit.SYNCBUSY);                                    /** Synchronization Busy... */
//        adc->SWTRIG.bit.START = true;                                       /** Start conversion        */
//        while(!adc->INTFLAG.bit.RESRDY);                                    /** wait result...          */
//        dummy_value = adc->RESULT.reg;
//        (void_t)(dummy_value);
//
////  ---------------------------End BU Current ------------------------------------------------
//
//
//        adc->INTENCLR.reg =     ADC_INTENCLR_MASK;  /** Disable all adc interrupt       */
//        adc->INTFLAG.reg =      ADC_INTFLAG_MASK;       /** Clear all adc interrupt flags   */
//
//        NVIC_SetPriority (ADC_IRQn, SYSTEM_INTERRUPT_PRIORITY_LEVEL_3);
//        system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_ADC);
//        return;
//} // end of function ADC_Init


//// AUX IN
//bool_t is_HAL_U_AUX_IN_HIGH(void_t)
//{
//    bool_t tmp = false;
//    if((PORTA.IN.reg & PORT_PA18))
//    {
//        tmp = true;
//    }
//    else
//    {
//        tmp = false;
//    }
//    return (tmp);
//}
//
//// AUX OUT
//void_t HAL_SET_AUX_OUT(bool_t level)
//{
//    if(level)
//    {
//        (PORTA.OUTSET.reg = PORT_PA16);
//    }
//    else
//    {
//        (PORTA.OUTCLR.reg = PORT_PA16);
//    }
//    return;
//}

/**
 * \brief
 * Reads the the voltage configuration 
 * variable set at GPIO init
 * 
 * \author Nicolò Bosio
 * \date   24/10/2017
 *
 * \return bool_t
 */
bool_t Is_PowerSet_230V(void_t)
{
    return(pcb_voltage230V);
}

/**
 * \brief
 * Reads the input pin just once to determine
 * whether the machine is 230V or 120V.
 *
 * \author Nicolò Bosio
 * \date   24/10/2017
 *
 * \return void_t
 */
void_t HAL_U_VOLTAGE_CONFIG()
{
    pcb_voltage230V = true;
    return;
}
