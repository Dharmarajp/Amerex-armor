/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include "main_header.h"
#include "genclk.h"
#include "can_u.h"
#include <at25dfx.h>
#include <conf_at25dfx.h>



volatile uint8_t uc_ch_num[NUM_CHANNELS];
volatile uint16_t us_value[NUM_CHANNELS];
uint8_t rv3028_status;

static void Amerex_configuration(void);
static void Amerex_application(void);



static void configure_console(void)
{

#ifdef Debug_enable
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
#endif

}

 /** Wakeup pin for wait mode: Touchscreen controller IRQ pin */
 #define WAKEUP_WAIT_INPUT_ID    (1u << 1)
 /** Wakeup pin for backup mode: Touchscreen controller IRQ pin */
 #define WAKEUP_BACKUP_INPUT_ID  (1u << 13)


 void freq_test_backup_mode()
 {

			SysNormal_LED(OFF);
			ioport_set_pin_level(MAIN_EN,0);
			ioport_set_pin_level(CAPCHG_EN,0);
			ioport_set_pin_level(HD_EN,0);
			//ioport_set_pin_level(BATT_EN,0);
			ioport_set_pin_level(HM_EN,0);
			ioport_set_pin_level(MEAS_EN,0);
			ioport_set_pin_level(SYS_RST_LED,0);
			ioport_set_pin_level(Alarm_silence,0);
			ioport_set_pin_level(Trouble_LED,0);
			ioport_set_pin_level(SEC_PWR_LED,0);
			ioport_set_pin_level(Amerex_R,0);

			/* Disable PIO pull-up for PB10(USB_DDM), PB11(USB_DDP) */
			pio_pull_up(PIOB, (0x3 << 10), 0);


			pio_set_input(PIOA, 0xFFFFFFFF, PIO_OPENDRAIN);
			pio_set_input(PIOB, 0xFFFFFFFF, PIO_OPENDRAIN);
			pio_set_input(PIOD, 0xFFFDFFFF, PIO_OPENDRAIN);

			// 		/* Enable the PIO for wake-up */
			supc_set_wakeup_inputs(SUPC, WAKEUP_BACKUP_INPUT_ID, WAKEUP_BACKUP_INPUT_ID);

			/* Switch MCK to slow clock  */
			pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);	 // 	  PMC_MCKR_PRES_CLK_4

			/* Disable unused clock to save power */
			pmc_osc_disable_xtal(0);
			//example_disable_pll();
			pmc_disable_pllack();
			/* Enter into backup mode */
			pmc_enable_backupmode();

 }


static void Amerex_configuration()
{
			Test_Button();
					 if((count_1ms - all_output_test_g_32) >1u)
					 {

							all_output_test_g_32 = count_1ms;
							fire_bit =!fire_bit;

							pio_sync_output_write(PIOB,1<<ALEDS_bit);
							ALEDS_bit++;
							ioport_set_pin_level(sys_normal,fire_bit);
							if(ALEDS_bit == 15)
							{

							ALEDS_bit = 10;
							}


					 }


					 if((count_1ms - buzzer_fire_output_test_g_32) > 600u)
					 {

							buzzer_fire_output_test_g_32 = count_1ms;
							buzz_bit = !buzz_bit;
							if(buzz_bit == 0)
							{
							Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, 0);
							FIRE_LED_PWM_EN(50);
							}

							else
							{
							Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, Default_buzz_Duty);
							FIRE_LED_PWM_EN(5)	;
							}
					 }

			Configuration();
}

static void Amerex_application()
{
			if((count_1ms - adc_prev_tick_g_u32) >= 10u) /*Take an ADC sample for every 10ms*/
			{

				afec_start_software_conversion(AFEC0);
				adc_prev_tick_g_u32 = count_1ms;
			}
			afec_start_software_conversion(AFEC1);
			System_Normal_Ind();
			Secondary_pwr_Ind();
			Trouble_Ind();
			System_Reset_Ind();
			Alarm_Sil_Ind();
			Fire_Ind();
			Buzzer_indication();
			System_Reset_Button();
			Alarm_Silence_Button();
			Test_Button();
			Fire_Switch();
			Enter_Backup_mode();
			Check_Heat_Detector_Trbl();
			Pressure_Sw();
			Check_Power_Trbl();
			Check_any_Trbl();
			Actuator_HM();
			Actuator_Pulse();
			Trbl_Fire_Iso_Output_Reset();
			Fire_Iso_Output();
			Trouble_Iso_Output();
			Fire_sound_retry_by_Timer();
			Super_CAP_Charge_enable_disable();
			primary_pwr_vol_g_u16 =PRI_volt();
			//printf("p: %ld\r\n",primary_pwr_vol_g_u16);
			sec_pwr_vol_g_u16= SEC_volt();
		    // printf("s: %ld\r\n",sec_pwr_vol_g_u16);
			hd1_measured_g_u16=HD1_volt();
			//printf("hd1: %ld\r\n",hd1_measured_g_u16);
			//printf("hd1_raw: %ld\r\n",Raw_value_HD1);
			hd2_measured_g_u16=HD2_volt();
			//printf("hd2: %ld\r\n",hd2_measured_g_u16);
			//printf("hd2_raw: %ld\r\n",Raw_value_HD2);
			super_capVolt_g_u16= SUPER_CAP_Volt();
			// printf("SC : %ld\r\n",super_capVolt_g_u16);
			CAN_process();
			Update_Master_Clock();
			Enter_DFU_mode();
			if (g_ul_recv_status == 1)
			{
				 decode_can_msg(&rx_mailbox);
				 g_ul_recv_status=0;
			}

}

 uint32_t setUNIX_Time=	1704774450;


int main (void)
{


			 sysclk_init();
			  /* Disable the watchdog */
		     //Watchdog_Config(WDT_3SEC);
			WDT->WDT_MR = WDT_MR_WDDIS;
			 board_init();
			configure_console();
			matrix_set_system_io(0xC20);
			Pherperial_clock_Enable();
			Systick_Config(60000U);
			Pherperial_Power_Enable();
			ioport_set_pin_level((PIO_PD17_IDX), 1);
			pmc_enable_periph_clk(ID_CAN0);
		     Delay(100u);
			 configure_i2c_master();
			at25dfx_init();
			memory_init();
			flash_read_amerex_param((void *)Amerex_config(0), &amerex_config_data);
			read_flash_mid_param((void *)flash_mid_param(0), &payload_cnt); //payload_cnt
			Fire_LED_Count(); /*update the Fading count values*/
			PWM_Init();
			adc_init();
			CAN_Open();
			Startup_Music();
		    Timer0_init();
		//	at25dfx_erase_block_OTA(0);
		   	hd1_prev_tick_g_u32 = count_1ms;
			hd2_prev_tick_g_u32 = count_1ms;
			system_reset_prev_tick_g_u32 = count_1ms;
			sec_pwr_prev_tick_g_u32 = count_1ms;
			pri_pwr_prev_tick_g_u32 = count_1ms;
			system_shutdown_tick_g_u32 = count_1ms;
			Clock_prev_tick_g_u32 =  count_1ms;
			adc_prev_tick_g_u32 =  count_1ms;
			buzzer_prev_tick_g_u32 =  count_1ms;
			fire_rly_en_vehicle_start_bit=0u;
			Fire_Rst_vehicle_start_bit=0u;
			//RV3028_Set_TimeStamp (setUNIX_Time);
		    #ifdef Debug_enable
			 printf("Time at Bootup : %lu\r\n", RV3028_GetTime(&_RTC));
	        #endif


	while(1)
	{

				(test_sw_pressed_g_b == 1) ? (Amerex_configuration()):(Amerex_application());
				wdt_restart(WDT);
	}
}

































































// static void ProcessButtonEvt(uint8_t uc_button)
//  {
// 	// [main_button1_evnt_process]
// 	if (uc_button == 0) {
// 		g_b_led0_active = !g_b_led0_active;
// 		if (!g_b_led0_active) {
// 			ioport_set_pin_level(SYS_RST_LED, IOPORT_PIN_LEVEL_HIGH);
// 		}
// 		else
// 		{
// 			ioport_set_pin_level(SYS_RST_LED, 0);
//
// 		}
//
// 	}
//
// 		// [main_button1_evnt_process]
// 	else if (uc_button == 1) {
// 			g_b_led0_active = !g_b_led0_active;
// 			if (!g_b_led0_active) {
// 				ioport_set_pin_level(Alarm_silence,1);
// 			}
// 			else
// 			{
// 				ioport_set_pin_level(Alarm_silence,0);
//
// 			}
//
// 		}
//
//
//
// 			// [main_button1_evnt_process]
// 			else if (uc_button == 2) {
// 				//g_b_led0_active = !g_b_led0_active;
// 				if (g_b_led0_active) {
// 					ioport_set_pin_level(ACT_FIRE,1);
// 					g_pwm_channel_led.channel = PWMH2_CHANNEL;
// 					pwm_channel_init(PWM, &g_pwm_channel_led);
// 					pwm_channel_enable(PWM, PWMH2_CHANNEL);
// 					pwm_channel_update_duty(PWM, &g_pwm_channel_led, 10);
// 					g_b_led0_active=0;
// 								g_pwm_channel_led.channel = PWMH3_CHANNEL;
// 								clock_setting.ul_clka = BUZZER_FREQ_2_5KHZ * PERIOD_VALUE;
// 								pwm_init(PWM, &clock_setting);
// 								pwm_channel_init(PWM, &g_pwm_channel_led);
// 								pwm_channel_enable(PWM, PWMH3_CHANNEL);
// 								pwm_channel_update_duty(PWM, &g_pwm_channel_led, 10);
// 				}
// 				else
// 				{
// 					ioport_set_pin_level(ACT_FIRE,0);
// 					pwm_channel_disable(PWM, PWMH2_CHANNEL);
// 					g_b_led0_active=1;
// 					pwm_channel_disable(PWM, PWMH3_CHANNEL);
//
//
// 				}
//
// 			}
//
//
// }
//
//
// //// [main_button1_handler]
// static void Button1_Handler(uint32_t id, uint32_t mask)
// {
// 	if (PIN_PUSHBUTTON_1_ID == id && PIN_PUSHBUTTON_1_MASK == mask) {
// 		ProcessButtonEvt(0);
// 	}
// }
//
// static void Button2_Handler(uint32_t id, uint32_t mask)
// {
// 	if (PIN_PUSHBUTTON_4_ID == id && PIN_PUSHBUTTON_4_MASK == mask) {
// 		ProcessButtonEvt(1);
// 	}
// }
//
// static void Button3_Handler(uint32_t id, uint32_t mask)
// {
// 	if (PIN_PUSHBUTTON_3_ID == id && PIN_PUSHBUTTON_3_MASK == mask) {
// 		ProcessButtonEvt(2);
// 	}
// }
// //// [main_button1_handler]
//
//
//  void configure_buttons()
// {
// 	//GPIO_PUSH_BUTTON_2
// 	/* Configure Pushbutton 2 */
// 	pmc_enable_periph_clk(PIN_PUSHBUTTON_1_ID);//
// 	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
// 	/* Interrupt on rising edge  */
// 	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,
// 	PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_3_ATTR, Button1_Handler);
// 	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
// 	pio_handler_set_priority(PIN_PUSHBUTTON_1_PIO,
// 	(IRQn_Type) PIN_PUSHBUTTON_1_ID, IRQ_PRIOR_PIO);
// 	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);
//
//
//
// 	/* Configure Pushbutton 1 */
// 	pmc_enable_periph_clk(PIN_PUSHBUTTON_4_ID);//
// 	pio_set_debounce_filter(PIN_PUSHBUTTON_4_PIO, PIN_PUSHBUTTON_4_MASK, 10);
// 	/* Interrupt on rising edge  */
// 	pio_handler_set(PIN_PUSHBUTTON_4_PIO, PIN_PUSHBUTTON_4_ID,
// 	PIN_PUSHBUTTON_4_MASK, PIN_PUSHBUTTON_4_ATTR, Button2_Handler);
// 	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_4_ID);
// 	pio_handler_set_priority(PIN_PUSHBUTTON_4_PIO,
// 	(IRQn_Type) PIN_PUSHBUTTON_4_ID, IRQ_PRIOR_PIO);
// 	pio_enable_interrupt(PIN_PUSHBUTTON_4_PIO, PIN_PUSHBUTTON_4_MASK);
//
//
//
//
//
// 		/* Configure Pushbutton 1 */
// 		pmc_enable_periph_clk(PIN_PUSHBUTTON_3_ID);//
// 		pio_set_debounce_filter(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK, 10);
// 		/* Interrupt on rising edge  */
// 		pio_handler_set(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_ID,
// 		PIN_PUSHBUTTON_3_MASK, PIN_PUSHBUTTON_3_ATTR, Button3_Handler);
// 		NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_3_ID);
// 		pio_handler_set_priority(PIN_PUSHBUTTON_3_PIO,
// 		(IRQn_Type) PIN_PUSHBUTTON_3_ID, IRQ_PRIOR_PIO);
// 		pio_enable_interrupt(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK);
//
//
// }
//
//






// 		#if 0
// 		ioport_set_pin_level(Amerex_R,0);
// 		ioport_set_pin_level(Amerex_G,0);
// 		ioport_set_pin_level(Amerex_B,0);
// 		delay_ms(3000);
// 		ioport_set_pin_level(Amerex_R,1);
// 		ioport_set_pin_level(Amerex_G,0);
// 		ioport_set_pin_level(Amerex_B,0);
// 		delay_ms(3000);
// 		ioport_set_pin_level(Amerex_R,0);
// 		ioport_set_pin_level(Amerex_G,1);
// 		ioport_set_pin_level(Amerex_B,0);
// 		delay_ms(3000);
// 		ioport_set_pin_level(Amerex_R,0);
// 		ioport_set_pin_level(Amerex_G,0);
// 		ioport_set_pin_level(Amerex_B,1);
// 		delay_ms(3000);
// 		ioport_set_pin_level(Amerex_R,1);
// 		ioport_set_pin_level(Amerex_G,1);
// 		ioport_set_pin_level(Amerex_B,0);
// 		delay_ms(3000);
// 		ioport_set_pin_level(Amerex_R,1);
// 		ioport_set_pin_level(Amerex_G,0);
// 		ioport_set_pin_level(Amerex_B,1);
// 		delay_ms(3000);
// 		ioport_set_pin_level(Amerex_R,0);
// 		ioport_set_pin_level(Amerex_G,1);
// 		ioport_set_pin_level(Amerex_B,1);
// 		delay_ms(3000);
// 		ioport_set_pin_level(Amerex_R,1);
// 		ioport_set_pin_level(Amerex_G,1);
// 		ioport_set_pin_level(Amerex_B,1);
// 		delay_ms(3000);
// 		#endif
//
	