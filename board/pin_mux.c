/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.0
processor: MKE02Z64xxx4
package_id: MKE02Z64VLD4
mcu_data: ksdk2_0
processor_version: 3.0.0
pin_labels:
- {pin_num: '1', pin_signal: PTD1/KBI1_P1/FTM2_CH3/SPI1_MOSI, label: Zero_X_Comp, identifier: Zero_X_Comp}
- {pin_num: '2', pin_signal: PTD0/KBI1_P0/FTM2_CH2/SPI1_SCK, label: U_Z_Cross, identifier: U_Z_Cross}
- {pin_num: '3', pin_signal: PTE7/FTM2_CLK/FTM1_CH1, label: U_Led_Debug_2, identifier: U_Led_Debug_2}
- {pin_num: '4', pin_signal: PTH2/BUSOUT/FTM1_CH0, label: U_Buzzer, identifier: U_Buzzer}
- {pin_num: '9', pin_signal: PTB7/I2C0_SCL/EXTAL, label: U_Water_Sensor, identifier: U_Water_Sensor}
- {pin_num: '10', pin_signal: PTB6/I2C0_SDA/XTAL, label: U_Heater, identifier: U_Heater}
- {pin_num: '12', pin_signal: PTB5/FTM2_CH5/SPI0_PCS0/ACMP1_OUT, label: U_Voltage_Config, identifier: U_Voltage_Config}
- {pin_num: '13', pin_signal: PTB4/FTM2_CH4/SPI0_MISO/NMI/ACMP1_IN2, label: U_Pump, identifier: U_Pump}
- {pin_num: '14', pin_signal: PTC3/FTM2_CH3/ADC0_SE11, label: U_MS_BU_Present, identifier: U_MS_BU_Present}
- {pin_num: '15', pin_signal: PTC2/FTM2_CH2/ADC0_SE10, label: U_BU_Home_Work, identifier: U_BU_Home_Work}
- {pin_num: '16', pin_signal: PTD7/KBI1_P7/UART2_TX, label: U_Led_Debug, identifier: U_Led_Debug}
- {pin_num: '17', pin_signal: PTD6/KBI1_P6/UART2_RX, label: U_BU_Fault, identifier: U_BU_Fault}
- {pin_num: '18', pin_signal: PTD5/KBI1_P5, label: U_Flowmeter_Pulse, identifier: U_Flowmeter_Pulse}
- {pin_num: '19', pin_signal: PTC1/FTM2_CH1/ADC0_SE9, label: U_BU_Rev, identifier: U_BU_Rev}
- {pin_num: '20', pin_signal: PTC0/FTM2_CH0/ADC0_SE8, label: U_BU_Fwd, identifier: U_BU_Fwd}
- {pin_num: '21', pin_signal: PTB3/KBI0_P7/SPI0_MOSI/FTM0_CH1/ADC0_SE7, label: Tp1, identifier: Tp1}
- {pin_num: '22', pin_signal: PTB2/KBI0_P6/SPI0_SCK/FTM0_CH0/ADC0_SE6, label: U_Valve_1, identifier: U_Valve_1}
- {pin_num: '23', pin_signal: PTB1/KBI0_P5/UART0_TX/ADC0_SE5, label: U_BU_Current, identifier: U_BU_Current}
- {pin_num: '24', pin_signal: PTB0/KBI0_P4/UART0_RX/ADC0_SE4, label: U_NTC_Boiler, identifier: U_NTC_Boiler}
- {pin_num: '25', pin_signal: PTA7/FTM2_FLT2/ACMP1_IN1/ADC0_SE3, label: U_Power_Cut, identifier: U_Power_Cut}
- {pin_num: '26', pin_signal: PTA6/FTM2_FLT1/ACMP1_IN0/ADC0_SE2, label: U_BU_Dreg_Door, identifier: U_BU_Dreg_Door}
- {pin_num: '29', pin_signal: PTD4/KBI1_P4, label: TP2, identifier: TP2}
- {pin_num: '30', pin_signal: PTD3/KBI1_P3/SPI1_PCS0, label: TP3, identifier: TP3}
- {pin_num: '31', pin_signal: PTD2/KBI1_P2/SPI1_MISO, label: U_Config_1, identifier: U_Config_1}
- {pin_num: '32', pin_signal: PTA3/KBI0_P3/UART0_TX/I2C0_SCL, label: TX_Uart_Lin, identifier: TX_Uart_Lin}
- {pin_num: '33', pin_signal: PTA2/KBI0_P2/UART0_RX/I2C0_SDA, label: RX_Uart_Lin, identifier: RX_Uart_Lin}
- {pin_num: '34', pin_signal: PTA1/KBI0_P1/FTM0_CH1/ACMP0_IN1/ADC0_SE1, label: U_Grinder_Sensor, identifier: U_Grinder_Sensor}
- {pin_num: '35', pin_signal: PTA0/KBI0_P0/FTM0_CH0/ACMP0_IN0/ADC0_SE0, label: U_Grinder_Analog, identifier: U_Grinder_Analog}
- {pin_num: '36', pin_signal: PTC7/UART1_TX, label: TX_Uart_Aux, identifier: TX_Uart_Aux}
- {pin_num: '37', pin_signal: PTC6/UART1_RX, label: RX_Uart_Aux, identifier: RX_Uart_Aux}
- {pin_num: '38', pin_signal: PTE2/SPI0_MISO, label: UI_Busy, identifier: UI_Busy}
- {pin_num: '39', pin_signal: PTE1/SPI0_MOSI, label: Tp4, identifier: Tp4}
- {pin_num: '40', pin_signal: PTE0/SPI0_SCK/FTM1_CLK, label: U_Grinder, identifier: U_Grinder}
- {pin_num: '41', pin_signal: PTC5/FTM1_CH1/RTCO, label: Tp5, identifier: Tp5}
- {pin_num: '43', pin_signal: PTA5/IRQ/FTM0_CLK/RESET, label: Mcu_Ext_Reset, identifier: Mcu_Ext_Reset}
- {pin_num: '42', pin_signal: PTC4/RTCO/FTM1_CH0/ACMP0_IN2/SWD_CLK, label: SWD_CLK, identifier: SWD_CLK}
- {pin_num: '44', pin_signal: PTA4/ACMP0_OUT/SWD_DIO, label: SWD_DIO, identifier: SWD_DIO}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: GPIOA, signal: 'GPIO, 25', pin_signal: PTD1/KBI1_P1/FTM2_CH3/SPI1_MOSI, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '2', peripheral: FTM2, signal: 'CH, 2', pin_signal: PTD0/KBI1_P0/FTM2_CH2/SPI1_SCK, direction: INPUT, filter_selection: no_init}
  - {pin_num: '3', peripheral: GPIOB, signal: 'GPIO, 7', pin_signal: PTE7/FTM2_CLK/FTM1_CH1, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '9', peripheral: GPIOA, signal: 'GPIO, 15', pin_signal: PTB7/I2C0_SCL/EXTAL, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '10', peripheral: GPIOA, signal: 'GPIO, 14', pin_signal: PTB6/I2C0_SDA/XTAL, direction: OUTPUT, pullup_enable: disable, filter_selection: no_init}
  - {pin_num: '12', peripheral: GPIOA, signal: 'GPIO, 13', pin_signal: PTB5/FTM2_CH5/SPI0_PCS0/ACMP1_OUT, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '13', peripheral: GPIOA, signal: 'GPIO, 12', pin_signal: PTB4/FTM2_CH4/SPI0_MISO/NMI/ACMP1_IN2, direction: OUTPUT, high_drive_enable: enable, filter_selection: no_init}
  - {pin_num: '14', peripheral: GPIOA, signal: 'GPIO, 19', pin_signal: PTC3/FTM2_CH3/ADC0_SE11, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '15', peripheral: GPIOA, signal: 'GPIO, 18', pin_signal: PTC2/FTM2_CH2/ADC0_SE10, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '16', peripheral: GPIOA, signal: 'GPIO, 31', pin_signal: PTD7/KBI1_P7/UART2_TX, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '17', peripheral: GPIOA, signal: 'GPIO, 30', pin_signal: PTD6/KBI1_P6/UART2_RX, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '18', peripheral: KBI1, signal: 'P, 5', pin_signal: PTD5/KBI1_P5, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '19', peripheral: GPIOA, signal: 'GPIO, 17', pin_signal: PTC1/FTM2_CH1/ADC0_SE9, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '20', peripheral: GPIOA, signal: 'GPIO, 16', pin_signal: PTC0/FTM2_CH0/ADC0_SE8, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '21', peripheral: GPIOA, signal: 'GPIO, 11', pin_signal: PTB3/KBI0_P7/SPI0_MOSI/FTM0_CH1/ADC0_SE7, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '22', peripheral: FTM0, signal: 'CH, 0', pin_signal: PTB2/KBI0_P6/SPI0_SCK/FTM0_CH0/ADC0_SE6, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '23', peripheral: ADC, signal: 'SE, 5', pin_signal: PTB1/KBI0_P5/UART0_TX/ADC0_SE5, filter_selection: no_init}
  - {pin_num: '24', peripheral: ADC, signal: 'SE, 4', pin_signal: PTB0/KBI0_P4/UART0_RX/ADC0_SE4, filter_selection: no_init}
  - {pin_num: '25', peripheral: GPIOA, signal: 'GPIO, 7', pin_signal: PTA7/FTM2_FLT2/ACMP1_IN1/ADC0_SE3, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '26', peripheral: GPIOA, signal: 'GPIO, 6', pin_signal: PTA6/FTM2_FLT1/ACMP1_IN0/ADC0_SE2, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '29', peripheral: GPIOA, signal: 'GPIO, 28', pin_signal: PTD4/KBI1_P4, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '30', peripheral: GPIOA, signal: 'GPIO, 27', pin_signal: PTD3/KBI1_P3/SPI1_PCS0, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '31', peripheral: GPIOA, signal: 'GPIO, 26', pin_signal: PTD2/KBI1_P2/SPI1_MISO, direction: INPUT, filter_selection: fltdiv3}
  - {pin_num: '32', peripheral: UART0, signal: TX, pin_signal: PTA3/KBI0_P3/UART0_TX/I2C0_SCL, filter_selection: no_init}
  - {pin_num: '33', peripheral: UART0, signal: RX, pin_signal: PTA2/KBI0_P2/UART0_RX/I2C0_SDA, filter_selection: no_init}
  - {pin_num: '36', peripheral: UART1, signal: TX, pin_signal: PTC7/UART1_TX, filter_selection: no_init}
  - {pin_num: '37', peripheral: UART1, signal: RX, pin_signal: PTC6/UART1_RX, filter_selection: no_init}
  - {pin_num: '38', peripheral: GPIOB, signal: 'GPIO, 2', pin_signal: PTE2/SPI0_MISO, direction: INPUT, filter_selection: no_init}
  - {pin_num: '39', peripheral: GPIOB, signal: 'GPIO, 1', pin_signal: PTE1/SPI0_MOSI, direction: OUTPUT, high_drive_enable: enable, filter_selection: no_init}
  - {pin_num: '40', peripheral: GPIOB, signal: 'GPIO, 0', pin_signal: PTE0/SPI0_SCK/FTM1_CLK, direction: OUTPUT, high_drive_enable: enable, filter_selection: no_init}
  - {pin_num: '41', peripheral: GPIOA, signal: 'GPIO, 21', pin_signal: PTC5/FTM1_CH1/RTCO, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '43', peripheral: SystemControl, signal: RESET, pin_signal: PTA5/IRQ/FTM0_CLK/RESET, direction: INPUT, pullup_enable: enable, filter_selection: fltdiv3}
  - {pin_num: '4', peripheral: FTM1, signal: 'CH, 0', pin_signal: PTH2/BUSOUT/FTM1_CH0, direction: OUTPUT, filter_selection: no_init}
  - {pin_num: '42', peripheral: GPIOA, signal: 'GPIO, 20', pin_signal: PTC4/RTCO/FTM1_CH0/ACMP0_IN2/SWD_CLK, filter_selection: no_init}
  - {pin_num: '44', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: PTA4/ACMP0_OUT/SWD_DIO, identifier: '', filter_selection: no_init}
  - {pin_num: '35', peripheral: ADC, signal: 'SE, 0', pin_signal: PTA0/KBI0_P0/FTM0_CH0/ACMP0_IN0/ADC0_SE0, filter_selection: no_init}
  - {pin_num: '34', peripheral: KBI0, signal: 'P, 1', pin_signal: PTA1/KBI0_P1/FTM0_CH1/ACMP0_IN1/ADC0_SE1, direction: INPUT, filter_selection: fltdiv2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
//void BOARD_InitPins(void)
//{
//    /* High Current Drive Capability of PTB4: 0x01u */
//    PORT_SetHighDriveEnable(PORT, kPORT_HighDrive_PTB4, 1);
//    /* High Current Drive Capability of PTE0: 0x01u */
//    PORT_SetHighDriveEnable(PORT, kPORT_HighDrive_PTE0, 1);
//    /* High Current Drive Capability of PTE1: 0x01u */
//    PORT_SetHighDriveEnable(PORT, kPORT_HighDrive_PTE1, 1);
//    /* Filter Selection for Input from PTA: 0x03u */
//    PORT_SetFilterSelect(PORT, kPORT_FilterPTA, kPORT_FILTERDIV3);
//    /* Filter Selection for Input from PTB: 0x03u */
//    PORT_SetFilterSelect(PORT, kPORT_FilterPTB, kPORT_FILTERDIV3);
//    /* Filter Selection for Input from PTC: 0x03u */
//    PORT_SetFilterSelect(PORT, kPORT_FilterPTC, kPORT_FILTERDIV3);
//    /* Filter Selection for Input from PTD: 0x03u */
//    PORT_SetFilterSelect(PORT, kPORT_FilterPTD, kPORT_FILTERDIV3);
//    /* Filter Selection for Input from RST: 0x03u */
//    PORT_SetFilterSelect(PORT, kPORT_FilterRST, kPORT_FILTERDIV3);
//    /* Filter Selection for Input from KBI0: 0x02u */
//    PORT_SetFilterSelect(PORT, kPORT_FilterKBI0, kPORT_FILTERDIV2);
//    /* Filter Selection for Input from KBI1: 0x03u */
//    PORT_SetFilterSelect(PORT, kPORT_FilterKBI1, kPORT_FILTERDIV3);
//    /* Pull Enable for Port A Bit 5: 0x01u */
//    PORT_SetPinPullUpEnable(PORT, kPORT_PTA, kPORT_PinIdx5, 1);
//    /* Pull Enable for Port B Bit 6: 0x00u */
//    PORT_SetPinPullUpEnable(PORT, kPORT_PTB, kPORT_PinIdx6, 0);
//    /* pin 33,32 is configured as UART0_RX, UART0_TX */
//    PORT_SetPinSelect(kPORT_UART0, kPORT_UART0_RXPTA2_TXPTA3);
//    /* pin 22 is configured as FTM0_CH0 */
//    PORT_SetPinSelect(kPORT_FTM0CH0, kPORT_FTM0_CH0_PTB2);
//    /* pin 4 is configured as FTM1_CH0 */
//    PORT_SetPinSelect(kPORT_FTM1CH0, kPORT_FTM1_CH0_PTH2);
//    /* pin 2 is configured as FTM2_CH2 */
//    PORT_SetPinSelect(kPORT_FTM2CH2, kPORT_FTM2_CH2_PTD0);
//    /* pin 13 is configured as PTB4 */
//    PORT_SetPinSelect(kPORT_NMI, kPORT_NMI_OTHERS);
//
//    SIM->SOPT = ((SIM->SOPT &
//                  /* Mask bits to zero which are setting */
//                  (~(SIM_SOPT_CLKOE_MASK)))
//
//                 /* Bus Clock Output Enable: 0x00u. */
//                 | SIM_SOPT_CLKOE(0x00u));
//}


void BOARD_InitPins(void) {
    PORT_SetPinSelect(kPORT_UART0, kPORT_UART0_RXPTB0_TXPTB1);      /* pin 34,33 is configured as UART0_RX, UART0_TX */
    PORT_SetPinSelect(kPORT_FTM0CH0, kPORT_FTM0_CH0_PTB2);          /* pin 32  PTB2 is configured as FTM0_CH0 */
    PORT_SetPinSelect(kPORT_FTM1CH1, kPORT_FTM1_CH1_PTC5);          /* pin 61  PTC5 is configured as FTM1_CH1 */

    PORT_SetFilterDIV1WidthThreshold(PORT, 3);                      // BUSCLK/16  (1 usec)
    PORT_SetFilterDIV2WidthThreshold(PORT, 4);                      // BUSCLK/512(32 usec)
    PORT_SetFilterDIV3WidthThreshold(PORT, 5);                      // LPOCLK/32 (From 25,6 msec to 47,7 msec, 32 msec typ)
    PORT_SetFilterSelect(PORT, kPORT_FilterKBI0, kPORT_BUSCLK_OR_NOFILTER); /* Filter Selection for Input from KBI0: 0x02u */
    PORT_SetPinPullUpEnable(PORT, kPORT_PTB, kPORT_PinIdx3, 1);     /* Pull Enable for Port A Bit 0: 0x01u */

    //SIM->SOPT |= 0x00080000;  /* pin PTH02 Busclout */            
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
