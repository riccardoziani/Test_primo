
#include "hal.h"
#include "crc.h"
#include "nvm.h"
#include "board.h"
#include "timer.h"
#include "flash.h"
#include "clock_config.h"

//#include "fsl_debug_console.h"

// Versione 01

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* Define the init structure for the default output pin HIGH*/
gpio_pin_config_t pin_config_DigitalOutput_High = {
    kGPIO_DigitalOutput, 1
};

/* Define the init structure for the default output pin LOW*/
gpio_pin_config_t pin_config_DigitalOutput_Low = {
    kGPIO_DigitalOutput, 0
};

/* Define the init structure for the default input pin */
gpio_pin_config_t pin_config_DigitalInput = {
    kGPIO_DigitalInput, 0
};


/* Define the init structure for the output LED pin*/
gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput, 0,
};




/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint16_t pin_C0;
volatile uint16_t pin_C1;
volatile uint16_t pin_C2;
volatile uint16_t pin_C3;
volatile uint32_t dbg1_cntr;
volatile uint32_t main_pass;
volatile uint32_t primask;
volatile uint16_t counter_ftm2;


volatile FTM_Type * zzzzFTM2 = FTM2;
volatile uint8_t testsizeof;


/*******************************************************************************
 * Code
 ******************************************************************************/


/*!
 * @brief Main function
 */
int main(void)
    {
    testsizeof = sizeof(bool_t);
    testsizeof = sizeof(char_t);
    testsizeof = sizeof(int8_t);
    testsizeof = sizeof(uchar_t);
    testsizeof = sizeof(int16_t);
    testsizeof = sizeof(int32_t);
    testsizeof = sizeof(uint32_t);
    testsizeof = sizeof(int64_t);
    testsizeof = sizeof(uint64_t);
    testsizeof = sizeof(float32_t);
    testsizeof = sizeof(float64_t);

    //nvm.fw_ver = 0x0011;   // Just for dbg...
    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_InitBootClocks();
    //BOARD_InitDebugConsole();

    SysTick_Config(SystemCoreClock/1000);   // 32000 1 msec...
    Hal_Pit_Init();     //  Init the Pit module channel 0 and channel 1
    CRC_Init();
    nvm_init();             
    Hal_Ev_Init();      //  Init the TPM0 peripheral to drive EV in PWM
    Hal_Buzzer_Init();  //  Init the TPM1 peripheral to drive Buzzer in PWM
    Hal_KBI0_Init();    //  Init the KBI0 peripheral to set external interrupt for grinder sensor.
    Hal_FTM2_Init();  //  Configure zero-crossing interrupt routine on FTM2.

    CRC_Test();

    /* Init output test GPIO. */
    GPIO_PinInit(kGPIO_PORTB, 3, &pin_config_DigitalInput);    // grinder sensor

    PORT_SetPinPullUpEnable(PORT, kPORT_PTH, kPORT_PinIdx6, true);     /* Pull Enable for Port H Bit 6: 0x01u */
    GPIO_PinInit(kGPIO_PORTH, 6, &pin_config_DigitalInput);    // test
    GPIOB->PIDR = 0xBFFFFFFF;

    GPIO_PinInit(kGPIO_PORTD, 5, &pin_config_DigitalOutput_Low);
    GPIO_PinInit(kGPIO_PORTB, 0, &pin_config_DigitalOutput_Low);
    GPIO_PinInit(kGPIO_PORTA, 0, &pin_config_DigitalOutput_Low);


    /* Print a note to terminal. */
    //PRINTF("\r\n GPIO Driver example\r\n");
    //PRINTF("\r\n The LED is blinking.\r\n");

    /* Init output LED GPIO. */
    GPIO_PinInit(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, &led_config);
    GPIO_PortSet(BOARD_LED_RED_GPIO_PORT, 1u << BOARD_LED_RED_GPIO_PIN);

    /* Init output LED GPIO. */
    GPIO_PinInit(kGPIO_PORTH, 2, &led_config);
    GPIO_PortSet(kGPIO_PORTH, 1u << 2);

    /* Init output LED GPIO. */
    GPIO_PinInit(kGPIO_PORTE, 7, &led_config);
    GPIO_PortSet(kGPIO_PORTE, 1u << 7);

    while (1)
        {
        primask = DisableGlobalIRQ();
        WDOG8_Refresh(WDOG);
        EnableGlobalIRQ(primask);
        TimeBase_Handler();

        main_freq_tmp++;    // calcolo la freq. del main program
        main_pass++;

        counter_ftm2 = zzzzFTM2->CNT;
		uint16_t ledMsecDelay = led_msec_delay;
		if (ledMsecDelay == 0)
            {
            GPIO_PortClear(BOARD_LED_RED_GPIO_PORT, 1u << BOARD_LED_RED_GPIO_PIN);
            }
        else if (led_msec_delay == 200)
            {
            GPIO_PortSet(BOARD_LED_RED_GPIO_PORT, 1u << BOARD_LED_RED_GPIO_PIN);
            GPIO_PortClear(kGPIO_PORTH, 1u << 2);

            }
        else if (led_msec_delay == 400)
            {
            GPIO_PortSet(kGPIO_PORTH, 1u << 2);
            GPIO_PortClear(kGPIO_PORTE, 1u << 7);

            }
        else if (led_msec_delay == 600)
            {
            GPIO_PortSet(kGPIO_PORTE, 1u << 7);
            GPIO_PortClear(BOARD_LED_RED_GPIO_PORT, 1u << BOARD_LED_RED_GPIO_PIN);
            }
        else if (led_msec_delay == 800)
            {
            led_msec_delay = 0;
            }

        if (GPIO_PinRead(kGPIO_PORTC, 0))
            {
            pin_C0 = 1000;
            }
        else
            {
            pin_C0 = 2000;
            }

        if (GPIO_PinRead(kGPIO_PORTC, 1))
            {
            pin_C1 = 2100;
            }
        else
            {
            pin_C1 = 3100;
            }
        if (GPIO_PinRead(kGPIO_PORTC, 2))
            {
            pin_C2 = 3500;
            }
        else
            {
            pin_C2 = 4500;
            }

        if (GPIO_PinRead(kGPIO_PORTC, 3))
            {
            pin_C3 = 4800;
            }
        else
            {
            pin_C3 = 5500;
            }
        }
    }
