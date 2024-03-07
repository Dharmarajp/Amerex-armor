/*
 * main_header.h
 *
 * Created: 12-07-2022 14:37:38
 *  Author: p.dharmaraj
 */


#ifndef MAIN_HEADER_H_
#define MAIN_HEADER_H_

/* Standard C header Files*/

struct tm _CurrentTime;
#include <string.h>
#include <time.h>
#include <asf.h>
#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <sysclk.h>
#include <pwm.h>
#include <conf_board.h>
#include <asf.h>
#include "stdlib.h"
#include "compiler.h"
#include "board.h"
#include "conf_board.h"
#include "ioport.h"


/*User defined header Files*/
#include "system_init.h"
#include "interrupt_handler.h"
#include "pwm_u.h"
#include "configuration.h"
#include "buzzer.h"
#include "led.h"
#include "switch_inputs.h"
#include "adc_u.h"
#include "fire.h"
#include "trouble.h"
#include "RV3028.h"
#include "RV3028_Defs.h"
#include "i2c_user.h"
#include "trouble.h"
#include "spi_u.h"
#include "can_u.h"
#include "conf_bootloader.h"
#include "memory_flash.h"
//paloadCount_t payload_cnt;

#define Debug_enable
//#define AT25DFX_SPI_MODULE

#endif /* MAIN_HEADER_H_ */