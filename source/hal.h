/******************************************************************************
Filename: Hal.h
Author: Urgese Emanuel
Created: 02/02/2016

Description:
File header for hal.c
Hardware abstraction level

Usage:
< place here info for module users >
******************************************************************************/

#ifndef hal_H
#define hal_H
    
//*****************************************************************************

// Include files **************************************************************
#include "pin_mux.h"
#include "fsl_pit.h"
#include "fsl_tpm.h"
#include "fsl_ftm.h"
#include "fsl_kbi.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_wdog8.h"
#include "philips_types.h"


// Public typedefs  ********************************************************************************

typedef struct _Version_Data_t
{
    const uint32_t ver[1];          //  0x00
    const uint32_t build[1];        //  0x00+4
    const char_t date[12];          //  0x00+8
    const char_t time[9];           //  0x00+12
} Version_Data_t;





// Public general defines *************************************************************************
//#define CHECK_ERROR1
//
///* with time quantum on valves of 666ns period has to be 83.33 to have 18KHz frequency -> 1.2 %/bit*/
//#define VALVES_PERIOD 83


// Input define ---------------------------------------------------------

#define GR_SENSOR_PIN (1<<7)

#define HAL_U_WATER_SENSOR1_OUT1()  (!GPIO_PinRead(kGPIO_PORTB, BOARD_INITPINS_U_Water_Sensor_PIN))  // True on Low signal...
#define HAL_U_BEAN_DOOR()           (true)  //(PORTB.IN.reg & PORT_PB15)
#define HAL_U_BU_POSITION()         (true)  //(!(PORTB.IN.reg & PORT_PB30))
#define HAL_U_BU_NPRESENCE()        (true)  //(PORTB.IN.reg & PORT_PB31)
#define HAL_U_DREG()                (true)  //(PORTA.IN.reg & PORT_PA27)
#define HAL_U_DREG_DOOR()           (true)  //(PORTB.IN.reg & PORT_PB23)

// Read without debounce                    
#define HAL_U_BRIDGE_CONFIG()       (true)  //(PORTB.IN.reg & PORT_PB13)
#define HAL_U_MASTER_SLAVE()        (true)  //(PORTA.IN.reg & PORT_PA11)

// Analog input --------------------------------------------------------
/**
 * \brief
 * The system start the ADC conversion for all analog signal.
 * Interrupt on ADC channel are enabled:
 * - U_BU_CURRENT
 * - U_NTC_BOILER
 * The conversion end when ADC_complete flag is set to true.
 * Must be called by TIMER_Handler every 10ms in order to start 
 * conversion.
 */
#define HAL_ADC_Start() {__asm("nop");}
//{
//    adc->INTFLAG.reg =  ADC_INTFLAG_MASK;     /** Clear all adc interrupt flags   */
//    adc->INTENSET.reg = ADC_INTENSET_RESRDY;  /** Set result ready interrupt flag */
//    adc->SWTRIG.bit.START = true;             /** Start next conversion           */
//}

/**
 * \brief                                                                                     \
 * Reset the ADC module @ factory default.
 * Disable the interrupt request on ADC module.
 * Must be called before to go in power-off mode. 
 */
#define HAL_ADC_Stby_Mode() {__asm("nop");}                                                                     
//{
//    adc->CTRLA.reg = AC_CTRLA_SWRST;                                /** ADC software reset */
//    while(adc->STATUS.bit.SYNCBUSY);                                /** Sync */
//    system_interrupt_disable(SYSTEM_INTERRUPT_MODULE_ADC);
//    NVIC_SetPriority (ADC_IRQn, SYSTEM_INTERRUPT_PRIORITY_LEVEL_0);
//}

//-Watchdog macro -------------------------------------------------------
/**
 * \brief
 * Reset the Watchdog counter 
 */
#define HAL_WDT_Reset()  {__asm("nop");}                         
//{
//    while(wdt->STATUS.bit.SYNCBUSY);
//    wdt->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
//}

/**
 * \brief
 * Enable Watchdog as configured at init 
 */
#define HAL_WDT_Enable() {__asm("nop");}                        
//{
//    while(wdt->STATUS.bit.SYNCBUSY);
//    wdt->CTRL.reg |= WDT_CTRL_ENABLE;
//}

/**
 * \brief
 * Disable watchdog 
 */
#define HAL_WDT_Disable() {__asm("nop");}                       
//{
//    while(wdt->STATUS.bit.SYNCBUSY);
//    wdt->CTRL.reg &= ~(WDT_CTRL_ENABLE);
//}                                                                                   
//-----------------------------------------------------------------------             

// PIN_FLOWMETER
#define HAL_Is_PIN_FLOWMETER_LOW()   (true)// (!(PORTA.IN.reg & PORT_PA14))
#define HAL_Is_PIN_FLOWMETER_HIGH()  (true)// (PORTA.IN.reg & PORT_PA14)

// Power 5v cut
#define HAL_ENABLE_5V_CUT()      	 // (PORTB.OUTSET.reg = PORT_PB02)
#define HAL_DISABLE_5V_CUT()     	 // (PORTB.OUTCLR.reg = PORT_PB02)

// LED1_CPU_DEBUG
#define HAL_LED1_CPU_DEBUG_ON()    	 // (PORTA.OUTCLR.reg = PORT_PA15)
#define HAL_LED1_CPU_DEBUG_OFF()   	 // (PORTA.OUTSET.reg = PORT_PA15)
#define HAL_LED1_CPU_DEBUG_TOGGLE()	 // (PORTA.OUTTGL.reg = PORT_PA15)

// AUX OUT
//#define HAL_SET_AUX_OUT(value)          (value? (PORTA.OUTSET.reg = PORT_PA16) : (PORTA.OUTCLR.reg = PORT_PA16))


// DEBUG_MODE
#define HAL_Debugger_Present() 			(true)

// RESET CAUSE                          
#define HAL_Reset_Cause() 			    //(*(uint8_t*)0x200000B0)
// RESET CAUSE Power Reset: Resets caused by an electrical issue.
#define HAL_Reset_Cause_BOD33() 	  //  ((*(uint8_t*)0x200000B0 & PM_RCAUSE_BOD33) == true)
#define HAL_Reset_Cause_BOD12() 	  //  ((*(uint8_t*)0x200000B0 & PM_RCAUSE_BOD12) == true)
#define HAL_Reset_Cause_POR() 		  //  ((*(uint8_t*)0x200000B0 & PM_RCAUSE_POR) == true)

// RESET CAUSE User Reset: Resets caused by the application.
#define HAL_Reset_Cause_SYST() 		 //   ((*(uint8_t*)0x200000B0 & PM_RCAUSE_SYST) == true)
#define HAL_Reset_Cause_WTD() 		 //   ((*(uint8_t*)0x200000B0 & PM_RCAUSE_WDT) == true)
#define HAL_Reset_Cause_EXT() 		 //   ((*(uint8_t*)0x200000B0 & PM_RCAUSE_EXT) == true)

// -------------------------------------------------------------------------------------------------------------

#define HAL_Get_RTC_Count()            //     (rtc->MODE0.COUNT.reg)
//------------------------------------------------- AC-LOADS ---------------------------------------------------
// TRIAC GRINDER
#define HAL_TRIAC_GRINDER_ON()       // (PORTA.OUTSET.reg = PORT_PA00)
#define HAL_TRIAC_GRINDER_OFF()      // (PORTA.OUTCLR.reg = PORT_PA00)

// TRIAC HEATER                      //
#define HAL_TRIAC_HEATER_ON()        // (PORTA.OUTSET.reg = PORT_PA01)
#define HAL_TRIAC_HEATER_OFF()       // (PORTA.OUTCLR.reg = PORT_PA01)
#define HAL_IS_HEATER_ON()           // (PORTA.IN.reg   &	PORT_PA01)

// TRIAC PUMP                        //
#define HAL_TRIAC_PUMP_ON()        	 // (PORTA.OUTSET.reg = PORT_PA03)
#define HAL_TRIAC_PUMP_OFF()         // (PORTA.OUTCLR.reg = PORT_PA03)
                                     //

//------------------------------------------------- H-BRIDGE MACRO ---------------------------------------------
#define HAL_SET_HBRIDGE_FWD_PWM(value)     //   {while(tc7->COUNT8.STATUS.bit.SYNCBUSY);tc7->COUNT8.CC[0].reg = value;}
#define HAL_SET_HBRIDGE_REV_PWM(value)     //   {while(tc7->COUNT8.STATUS.bit.SYNCBUSY);tc7->COUNT8.CC[1].reg = value;}
                                           //
#define HAL_BU_IS_FAULT()  			       //   (PORTA.IN.reg & PORT_PA10)

//------------------------------------------------- EV MACRO ---------------------------------------------------
#define Hal_PWM_Valve_Ev1(value)   		      //  {while(tc5->COUNT8.STATUS.bit.SYNCBUSY);tc5->COUNT8.CC[0].reg = value;}

#define HAL_U_EV12_FAULT()                   // (PORTB.IN.reg & PORT_PB03)

#define HAL_EV_ENABLE()                      // (PORTB.OUTSET.reg = PORT_PB12)
#define HAL_EV_DISABLE()                     // (PORTB.OUTCLR.reg = PORT_PB12)
                                             //
//--------------------------------------------- GRINDER MACRO --------------------------------------------------


/**
 * \brief 
 * Config interrupt on pin "Grinder sensor" to detect rise edge of the signal 
 */
#define HAL_Gr_Sensor_Pulses_Config_RiseDetection() {__asm("nop");}                                                             
//{
//    eic->CONFIG[1].reg &= ~EIC_CONFIG_SENSE2_Msk;                           /** Clear config sense  */
//    eic->CONFIG[1].reg |= EIC_CONFIG_SENSE2(EIC_CONFIG_SENSE2_RISE_Val);    /** Enable rise-edge interrupt */
//}

/**
 * \brief 
 * Config interrupt on pin "Grinder sensor" to detect fall edge of the signal 
 */
#define HAL_Gr_Sensor_Pulses_Config_FallDetection() {__asm("nop");}                                                             
//{
//    eic->CONFIG[1].reg &= ~EIC_CONFIG_SENSE2_Msk;                           /** Clear config sense */
//    eic->CONFIG[1].reg |= EIC_CONFIG_SENSE2(EIC_CONFIG_SENSE2_FALL_Val);    /** Enable fall-edge interrupt */
//}

/**
 * \brief 
 * Enable interrupt on Grinder sensor pin 
 */
#define HAL_Enable_Gr_Sensor_Pulses() {__asm("nop");}                                                                           
//{
//    eic->INTFLAG.reg = EIC_INTFLAG_EXTINT10;                        /** Clear flag interrupt on pin      */
//    eic->INTENSET.reg = EIC_INTENSET_EXTINT10;                      /** Enable external interrupt on pin */
//}

/** 
 * \brief 
 * Disable interrupt on grinder sensor pin
 */
#define HAL_Disable_Gr_Sensor_Pulses()  {KBI_DisableInterrupts(KBI0);} /*Disable external interrupt on pin grinder sensor */                                                                \

   

///**                                                       
// * \brief                                                 
// * Return TC3 counter register status
// */
//#define HAL_Get_TC3_Count()    (tc3->COUNT16.COUNT.reg)

///**
// * \brief
// * Enable filter activity on Grinder sensor pin
// * Reload timer every 20us in order to check pin status
// */
//#define HAL_Enable_Filter_Activity()                                                                            \
//{                                                                                                               \
//    uint32_t tc3_cntr_local = HAL_Get_TC3_Count();                                                             \
//    while(tc3->COUNT16.STATUS.bit.SYNCBUSY);                                                                    \
//    tc3->COUNT16.CC[0].reg = tc3_cntr_local + 160;  /** 20 u seconds... */                                      \
//    tc3->COUNT16.INTFLAG.reg    = TC_INTFLAG_MC0;   /** clear flag cc0 after channel rearm. */                  \
//    tc3->COUNT16.INTENSET.reg   = TC_INTENSET_MC0;  /** Set interrupt request on cc0 match...   */              \
//}

///**
// * \brief
// * Disable timer for filter activity on Grinder sensor pin
// */
//#define HAL_Disable_Filter_Activity()                                                                           \
//{                                                                                                               \
//    tc3->COUNT16.INTFLAG.reg    = TC_INTFLAG_MC0;   /** clear flag cc0 after channel rearm. */                  \
//    tc3->COUNT16.INTENCLR.reg   = TC_INTENCLR_MC0;  /** Disable ISR request on cc0 match... */                  \
//}

/** 
 * \brief 
 * Return the configuration of interrupt on grinder sensor pin 
 * true if detect fall edge of signal 
 */
#define HAL_GR_Sensor_FallEdge_Detect()    // (eic->CONFIG[1].reg & EIC_CONFIG_SENSE2(EIC_CONFIG_SENSE2_FALL_Val))

/** 
 * \brief 
 * Return the configuration of interrupt on grinder sensor pin 
 * true if detect rise edge of signal 
 */
#define HAL_GR_Sensor_RiseEdge_Detect()   //  (eic->CONFIG[1].reg & EIC_CONFIG_SENSE2(EIC_CONFIG_SENSE2_RISE_Val))

/** 
 * \brief 
 * true if IRQ on Grinder sensor pin as occurred
 */
#define HAL_IRQ_GR_sensor()                // (eic->INTFLAG.bit.EXTINT10)

/** 
 * \brief 
 * Clear interrupt flag on grinder sensor pin
 */
#define HAL_Clear_IRQ_flag_GR_Sensor()     // (eic->INTFLAG.reg = EIC_INTFLAG_EXTINT10)

/** 
 * \brief 
 * true if status of Grinder sensor pin is High
 */
#define HAL_PIN_GRINDER_SENSOR()  (GPIO_PinRead(kGPIO_PORTB, 3))
//#define HAL_PIN_IRQ_HIGH()                  (PORTB.IN.reg & PORT_PB10)

/** 
 * \brief 
 * true if status of Grinder sensor pin is Low
 */
//#define HAL_PIN_IRQ_LOW()                   (!(PORTB.IN.reg & PORT_PB10))
//--------------------------------------------- ZERO CROSSING MACRO --------------------------------------------------

//
//#define HAL_Enable_ZC_EXTINT()              (eic->EVCTRL.reg |= EIC_EVCTRL_EXTINTEO13)
//
//#define HAL_Get_CC0_value()                 (tcc0->CC[0].reg)

//---------------------------------------------------------------------------------------------------------------
// Public data *************************************************************************************
extern const Version_Data_t version_data;
extern const char_t *const firmware_version;


//extern    Ac* 		ac;
//extern    Adc*		adc;
//extern    Dmac*		dmac;
//extern    Pm* 		pm;
//extern    Port*		port;
//extern    Rtc*		rtc;
//extern    Sercom*		uscp_sercom;
//extern    Sercom*		lin_sercom;
//extern    Sysctrl*	sysctrl;
//extern    Eic*		eic;
//extern    Gclk*		gclk;
//extern    Nvmctrl* 	nvm_module;
//extern    Evsys*		evsys;
//extern    Tcc*		tcc0;
//extern    Tcc*		tcc1;
//extern    Tcc*		tcc2;
//extern    Tc* 		tc3;
//extern    Tc* 		tc4;
//extern    Tc* 		tc5;
//extern    Tc* 		tc6;
//extern    Tc* 		tc7;
//extern    Wdt*		wdt;


extern  volatile    uint16_t    main_freq_tmp;
extern  volatile    uint32_t    led_msec_delay;

// Public functions ***********************************************************
void    	LIN_Uart_Init           		(void_t);
void_t      Hal_LIN_Uart_Stby               (void_t);
void_t 		LIN_BREAK_DetectEnable 			(void_t);
void_t 		LIN_BREAK_DetectDisable			(void_t);

void_t		HAL_GCLK_Init 		  	 	    (void_t);
void_t      HAL_CLKSOURCE_Init              (void_t);
void_t		Hal_Set_DFLL48M 		  	 	(void_t);
void_t		Hal_Set_CPU_Clock_Stby_Mode	  	(void_t);
void_t		Hal_Tc3_Grinder_Filter_Init	 	(void_t);
void_t		Hal_OSC32K_OSCULP32K_Calib	 	(void_t);
void_t      HAL_Disable_ZC_EXTINT           (void_t);
void_t		Hal_Zero_Cross_Init_TCC         (void_t);
void_t		Hal_GPIO_Init 		      	 	(void_t);
void_t		Hal_GPIO_Stby_Mode	      	 	(void_t);
void_t      Hal_GPIO_PowerOff_Mode          (void_t);
void_t      Hal_UART_AUX_To_GPIO            (void_t);
void_t      Hal_Enable_UartAuxInFromUI      (void_t);
void_t		Hal_RTC_Init 		      	 	(void_t);

void_t		Hal_Pit_Init 		      	 	(void_t);
void_t		Hal_Pit_Deinit 		            (void_t);
void_t		Hal_Ev_Init 		      	 	(void_t);
void_t		Hal_Ev_Deinit 		            (void_t);
void_t		Hal_FTM2_Init 		      	(void_t);
void_t		Hal_FTM2_Zx_Deinit              (void_t);
void_t		Hal_Buzzer_Init 		        (void_t);
void_t		Hal_Buzzer_Deinit 		        (void_t);
void_t		Hal_KBI0_Init 		  		    (void_t);
void_t		Hal_KBI0_Deinit 		  		(void_t);
void_t		HAL_Resync_Gr_Sensor_Pulses 	(void_t);

void_t      delay_usec                      (uint32_t usec);
void_t      delay_msec                      (uint32_t msec);

void_t      HAL_Bu_Init                     (void_t);
void_t		Hal_WDT_Init 		      	 	(void_t);
void_t		Hal_RTC_Stby_Mode 		      	(uint32_t ISR_address);
void_t		Hal_SYSCTRL_Stby_Mode 		    (void_t);
void_t		Hal_Zero_Cross_Stby_Mode 		(void_t);
void_t		Hal_WDT_Stby_Mode 		        (void_t);
void_t		Hal_EIC_Stby_Mode 		        (void_t);
void_t		Hal_AHB_APBx_Normal_Operation   (void_t);
void_t		Hal_AHB_APBx_Stby_Mode 		    (void_t);
void_t      HAL_ADC_Init                    (void_t);
bool_t      is_HAL_U_AUX_IN_HIGH            (void_t);
void_t      HAL_SET_AUX_OUT                 (bool_t);
bool_t      Is_PowerSet_230V                (void_t);


#endif
