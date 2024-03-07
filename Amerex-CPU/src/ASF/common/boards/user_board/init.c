/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include "compiler.h"
#include "board.h"
#include "conf_board.h"
#include "ioport.h"
#include "main_header.h"

/**
 * \brief Set peripheral mode for IOPORT pins.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param port IOPORT port to configure
 * \param masks IOPORT pin masks to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_port_peripheral_mode(port, masks, mode) \
	do {\
		ioport_set_port_mode(port, masks, mode);\
		ioport_disable_port(port, masks);\
	} while (0)

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
	do {\
		ioport_set_pin_mode(pin, mode);\
		ioport_disable_pin(pin);\
	} while (0)

/**
 * \brief Set input mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 * \param sense Sense for interrupt detection (\ref ioport_sense)
 */
#define ioport_set_pin_input_mode(pin, mode, sense) \
	do {\
		ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);\
		ioport_set_pin_mode(pin, mode);\
		ioport_set_pin_sense_mode(pin, sense);\
	} while (0)


 /** UART0 pins (UTXD0 and URXD0) definitions, PA10,9. */
 
 #ifdef Debug_enable
 
		#define PINS_UART0        (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
		#define PINS_UART0_FLAGS  (IOPORT_MODE_MUX_A)

		#define PINS_UART0_PORT   IOPORT_PIOA
		#define PINS_UART0_MASK   (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
		#define PINS_UART0_PIO    PIOA
		#define PINS_UART0_ID     ID_PIOA
		#define PINS_UART0_TYPE   PIO_PERIPH_A
		#define PINS_UART0_ATTR   PIO_DEFAULT
#endif



void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 	 */
	/** PWM LED0 pin definitions. */

		/* Initialize IOPORTs */
		ioport_init();

 		ioport_set_pin_dir(MAIN_EN, IOPORT_DIR_OUTPUT);
		//ioport_set_pin_dir(ERR_LED, IOPORT_DIR_OUTPUT);

		ioport_set_pin_dir(Amerex_R, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(Amerex_G, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(Amerex_B, IOPORT_DIR_OUTPUT);

		ioport_set_pin_dir(SYS_RST_LED, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(Alarm_silence, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(SEC_PWR_LED, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(sys_normal, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(Trouble_LED, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(ACT_FIRE, IOPORT_DIR_OUTPUT);
		//ioport_set_pin_dir(NIMH, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(CAPCHG_EN, IOPORT_DIR_OUTPUT);

		ioport_set_pin_dir(TEST_SW_PIN, IOPORT_DIR_INPUT);      // switch pin set as input
		ioport_set_pin_dir(ALARM_SILEN_PIN, IOPORT_DIR_INPUT);      // switch pin set as input
		ioport_set_pin_dir(SYS_RESET_SW_PIN, IOPORT_DIR_INPUT);      // switch pin set as input

		ioport_set_pin_dir(HD_EN, IOPORT_DIR_OUTPUT);
		//ioport_set_pin_dir(SEC_EN, IOPORT_DIR_OUTPUT);
		//ioport_set_pin_dir(BATT_EN, IOPORT_DIR_OUTPUT);

		ioport_set_pin_dir(HM_EN, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(MEAS_EN, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(RTC_Flash_EN, IOPORT_DIR_OUTPUT);

		ioport_set_pin_dir(CAN_ACT, IOPORT_DIR_OUTPUT);
		//ioport_set_pin_dir(MUX_A1, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(DIMMER_IN, IOPORT_DIR_OUTPUT);

		ioport_set_pin_dir(ACTA_P, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(ACTB_P, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(ACTA_N, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(ACTB_N, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(ACT_FIRE_ON, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(RELAY_A, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(RELAY_B, IOPORT_DIR_OUTPUT);

		ioport_set_pin_dir(Fire_input, IOPORT_DIR_INPUT);
		ioport_set_pin_dir(CYLPRS_SW_Pin, IOPORT_DIR_INPUT);

 		ioport_set_pin_dir(CAN_Vio, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(PIN_CAN0_TR_EN_IDX, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir(WakeUp_input, IOPORT_DIR_INPUT);


// 		 /* Configure PWM LED0 pin */
 		 ioport_set_pin_peripheral_mode(PWMH0, IOPORT_MODE_MUX_A);
 		 ioport_set_pin_peripheral_mode(PWMH1, IOPORT_MODE_MUX_A);
         ioport_set_pin_peripheral_mode(PWMH2, IOPORT_MODE_MUX_A);
 		 ioport_set_pin_peripheral_mode(PWMH3, IOPORT_MODE_MUX_A);	// PIN_PWMH3_FLAGS


	   	ioport_set_pin_peripheral_mode(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	   	ioport_set_pin_peripheral_mode(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);

       #ifdef Debug_enable  
	    ioport_set_port_peripheral_mode(PINS_UART0_PORT, PINS_UART0, PINS_UART0_FLAGS);
	   #endif
	   
	   
		ioport_set_pin_peripheral_mode(SPI_MISO_GPIO, SPI_MISO_FLAGS);
		ioport_set_pin_peripheral_mode(SPI_MOSI_GPIO, SPI_MOSI_FLAGS);
		ioport_set_pin_peripheral_mode(SPI_SPCK_GPIO, SPI_SPCK_FLAGS);
		ioport_set_pin_peripheral_mode(SPI_NPCS0_GPIO, SPI_NPCS0_FLAGS);

		/* Configure the CAN1 TX and RX pin. */
		ioport_set_pin_peripheral_mode(PIN_CAN0_RX_IDX, PIN_CAN0_RX_FLAGS);
		ioport_set_pin_peripheral_mode(PIN_CAN0_TX_IDX, PIN_CAN0_TX_FLAGS);
//   	  	ioport_set_port_peripheral_mode(PINS_UART0_PORT, PINS_UART0,PINS_UART0_FLAGS);



}
