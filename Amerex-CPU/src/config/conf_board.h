/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define MAIN_EN             (PIO_PD14_IDX)
//#define ERR_LED             (PIO_PD31_IDX)
#define SYS_RST_LED         (PIO_PB11_IDX)
#define Alarm_silence       (PIO_PB14_IDX)
#define SEC_PWR_LED         (PIO_PB10_IDX)
#define sys_normal          (PIO_PD4_IDX)
#define Trouble_LED         (PIO_PB13_IDX)
#define ACT_FIRE            (PIO_PA26_IDX)
#define Amerex_R            (PIO_PA31_IDX)
#define Amerex_G            (PIO_PA29_IDX)
#define Amerex_B            (PIO_PA30_IDX)
//#define NIMH                (PIO_PD12_IDX)
#define CAPCHG_EN           (PIO_PD16_IDX)
#define HD_EN               (PIO_PD7_IDX)
//#define SEC_EN              (PIO_PA6_IDX)
#define MEAS_EN             (PIO_PD28_IDX)	//BATT_EN
#define HM_EN               (PIO_PD6_IDX)
//#define MEAS_EN             (PIO_PD13_IDX)
#define RTC_Flash_EN        (PIO_PD17_IDX)
#define SYS_RESET_SW_PIN    (PIO_PA1_IDX)
#define TEST_SW_PIN         (PIO_PA0_IDX)
#define ALARM_SILEN_PIN     (PIO_PA2_IDX)
#define CAN_ACT             (PIO_PD24_IDX)
//#define MUX_A1              (PIO_PD25_IDX)
#define DIMMER_IN           (PIO_PD26_IDX)
#define ACTA_P              (PIO_PD3_IDX)
#define ACTB_P              (PIO_PD0_IDX)
#define ACTA_N              (PIO_PD2_IDX)
#define ACTB_N              (PIO_PD1_IDX)
#define Fire_input          (PIO_PA5_IDX)
#define CYLPRS_SW_Pin       (PIO_PD27_IDX)
#define ACT_FIRE_ON         (PIO_PA26_IDX)
#define RELAY_A             (PIO_PA24_IDX)
#define RELAY_B             (PIO_PA23_IDX)
#define CAN_Vio            	(PIO_PD19_IDX)
#define WakeUp_input       	(PIO_PB5_IDX)



/** CAN0 PIN RX. */
#define PIN_CAN0_RX_IDX           PIO_PB3_IDX
#define PIN_CAN0_RX_FLAGS         IOPORT_MODE_MUX_A

/** CAN0 PIN TX. */
#define PIN_CAN0_TX_IDX           PIO_PB2_IDX
#define PIN_CAN0_TX_FLAGS         IOPORT_MODE_MUX_A

/** CAN0 transceiver PIN EN. */
#define PIN_CAN0_TR_EN_IDX        PIO_PD30_IDX
#define PIN_CAN0_TR_EN_FLAGS      IOPORT_DIR_OUTPUT


#define PWMH0               PIO_PD20_IDX
#define PIN_PWMH0_FLAGS     (IOPORT_MODE_MUX_A)
#define PWMH0_Amerex_LOGO   PWM_CHANNEL_0


#define PWMH1               PIO_PD21_IDX
#define PIN_PWMH1_FLAGS     (IOPORT_MODE_MUX_A)
#define PWMH1_LEDS          PWM_CHANNEL_1

#define PWMH2               PIO_PD22_IDX
#define PIN_PWMH2_FLAGS     (IOPORT_MODE_MUX_A)
#define PWMH2_Fire_LED      PWM_CHANNEL_2

#define PWMH3               PIO_PD23_IDX
#define PIN_PWMH3_FLAGS    (IOPORT_MODE_MUX_A)
#define PWMH3_Buzzer        PWM_CHANNEL_3






/**
 * Push button #2 definition. Attributes = pull-up + debounce + interrupt on
 * both edges.
 */
// #define PUSHBUTTON_3_NAME        "FIRE INPUT SWITCH"
// #define PUSHBUTTON_3_WKUP_LINE   (1)
// #define PUSHBUTTON_3_WKUP_FSTT   (PMC_FSMR_FSTT1)
// #define GPIO_PUSH_BUTTON_3         (PIO_PA5_IDX)
// #define GPIO_PUSH_BUTTON_3_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
// #define GPIO_PUSH_BUTTON_3_SENSE (IOPORT_SENSE_LEVEL_LOW)
//
// #define PIN_PUSHBUTTON_3       {PIO_PA5, PIOA, ID_PIOA, PIO_INPUT,
// 		PIO_PULLUP | PIO_DEBOUNCE | IOPORT_SENSE_LEVEL_LOW}
// #define PIN_PUSHBUTTON_3_MASK  PIO_PA5
// #define PIN_PUSHBUTTON_3_PIO   PIOA
// #define PIN_PUSHBUTTON_3_ID    ID_PIOA
// #define PIN_PUSHBUTTON_3_TYPE  PIO_INPUT
// #define PIN_PUSHBUTTON_3_ATTR  (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_LOW_LEVEL)
// #define PIN_PUSHBUTTON_3_IRQn  PIOA_IRQn
/**
 * Push button #3 definition. Attributes = pull-up + debounce + interrupt on
 * rising edge.
 */
  /** TWI0 pins definition */
  #define TWI0_DATA_GPIO   PIO_PA3_IDX
  #define TWI0_DATA_FLAGS  (IOPORT_MODE_MUX_A)
  #define TWI0_CLK_GPIO    PIO_PA4_IDX
  #define TWI0_CLK_FLAGS   (IOPORT_MODE_MUX_A)


  #define EDBG_I2C_MODULE  TWI0
  #define EDBG_I2C_MODULE_ID  ID_TWI0

  /** SPI MISO pin definition. */
  #define SPI_MISO_GPIO         (PIO_PA12_IDX)
  #define SPI_MISO_FLAGS        (IOPORT_MODE_MUX_A)
  /** SPI MOSI pin definition. */
  #define SPI_MOSI_GPIO         (PIO_PA13_IDX)
  #define SPI_MOSI_FLAGS        (IOPORT_MODE_MUX_A)
  /** SPI SPCK pin definition. */
  #define SPI_SPCK_GPIO         (PIO_PA14_IDX)
  #define SPI_SPCK_FLAGS        (IOPORT_MODE_MUX_A)

  /** SPI chip select 0 pin definition. (Only one configuration is possible) */
  #define SPI_NPCS0_GPIO        (PIO_PA11_IDX)
  #define SPI_NPCS0_FLAGS       (IOPORT_MODE_MUX_A)




#define BOARD_FREQ_SLCK_XTAL        (32768U)
#define BOARD_FREQ_SLCK_BYPASS      (32768U)
#define BOARD_FREQ_MAINCK_XTAL      (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS    (12000000U)
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US        15625
#endif // CONF_BOARD_H
