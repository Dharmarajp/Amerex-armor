/*
 * switch_inputs.c
 *
 * Created: 14-07-2022 10:21:13
 *  Author: p.dharmaraj
 */


#include "main_header.h"
#include "genclk.h"

uint32_t test_sw_prev_tick_g_u32 = 0u;  /*To store previous count value */
bool test_sw_pressed_g_b = 0u;          /*To store Switch status*/
bool alarm_sil_flag_g_b = 0u;          /*To store Switch status*/
bool Master_Clock_bit = 0u;
uint32_t system_reset_prev_tick_g_u32 = 0u;  /*To store previous count value */
uint32_t system_shutdown_tick_g_u32 = 0u;
uint32_t Clock_prev_tick_g_u32 = 0u;  /*To store previous count value */
volatile uint8_t Fire_Rst_vehicle_start_bit = 0u;
volatile uint8_t fire_rly_en_vehicle_start_bit = 0u;
uint32_t UpdateDFU_tick_g_u32 =0u;
uint32_t fireRST_prev_tick_g_u32 = 0u;
uint32_t fire_sw_prev_tick_g_u32=0;

bool manual_sw_trbl_g_b = 0u;
bool manual_sw_trbl_silence_b=0;
bool pres_sw_trbl_g_b = 0u;
bool man_sw_reset_state_g_b = 0u;
 uint32_t DFU_auto_exit_prev_tick_g_u32 = 0u;
volatile bool Freq_selec_bit = 0u;
/* Function : Test Button Status check
 * Parameter: None
 * Return   : None
 */

void Test_Button(void)
{

    if((!ioport_get_pin_level(TEST_SW_PIN)) && pri_pwr_trouble_g_b == NO_TROUBLE && (ioport_get_pin_level(ALARM_SILEN_PIN)) && (ioport_get_pin_level(SYS_RESET_SW_PIN)))
    {
        if((count_1ms - test_sw_prev_tick_g_u32) >= SEC_1)
        {
			   can_disable(CAN0);
             test_sw_pressed_g_b = PRESSED;


	    }
    }
    else
    {
        test_sw_prev_tick_g_u32 = count_1ms;
        if(test_sw_pressed_g_b)
        {
			FIRE_LED_PWM_EN(amerex_config_data.fire_LED_duty);
			if(amerex_config_data.CAN_baud_rate != 0u)
			can_enable(CAN0);
			test_sw_pressed_g_b = RELEASED;
			buzz_bit =0;
			value =0;
		//	Buzzer_Beep(OFF);  	   /* if test button released immediately switch OFF buzzer*/
			Buzzer_Freq_Set(BUZZER_FREQ_2KHZ, 0);  /* OFF PWM*/


        }

    }
}

/* Function : Alarm Silence Button status_check
 * Parameter: None
 * Return   : None
 */
void Alarm_Silence_Button(void)
{

    if((any_of_trouble_g_b  && !ioport_get_pin_level(ALARM_SILEN_PIN) && ioport_get_pin_level(TEST_SW_PIN)))	 //  fire_happen_g_b &&
    {
        if((((count_1ms - alarm_sil_prev_tick_g_u32) >= msec_200)	&& fire_happen_g_b)  || (((count_1ms - alarm_sil_prev_tick_g_u32) >= SEC_3)	&& !fire_happen_g_b))
        {
			alarm_sil_prev_tick_g_u32 =	count_1ms;
            if(!alarm_sil_flag_g_b) /*while pressing first time alone we have to log the data*/
            {

                /* Log the switch pressed event*/
                Reset_Values();
				(fire_happen_g_b == 1u)?(log_data.log_code = 10u):(log_data.log_code = 13u);
                 /*Indicating system reset button pressed*/
                log_data.start_stop = 1u; /*button pressed*/
                NOR_Flash_Log_Data(&log_data);

            }
			 tc_start(TC0,0);
			  printf("Start _timer0\n");

            alarm_sil_flag_g_b = PRESSED;
        }
    }
    else
    {
         alarm_sil_prev_tick_g_u32 =  count_1ms;

    }
}

void Fire_Switch(void)
{
	//static uint8_t fire_sw_count = 0u;  /*To store previous count value */
	static uint8_t sw_rel_count_sl_u8 = 0u; /* To count the switch OFF count*/
	static bool system_reset_sl_b = 1u; /* To indicate system got reset */

	if(!ioport_get_pin_level(Fire_input))
	{


						Get_New_trouble = MANUAL_SW_3_FLASH;

						if(!man_sw_reset_state_g_b) /* if manual fire switch closed while system power ON or after fire happened break the loop*/
						{
							manual_sw_trbl_silence_b =1u;
							manual_sw_trbl_g_b = TROUBLE;
							//break;
						}


						else if((count_1ms - fire_sw_prev_tick_g_u32) > __500ms)
						{
							fire_sw_prev_tick_g_u32 = count_1ms;

							man_sw_reset_state_g_b = 0u;
							fire_happen_g_b = FIRE;
							fire_happen_safety_g_b = FIRE;
							manual_sw_trbl_g_b = TROUBLE;
							manual_sw_trbl_silence_b =1u;
							Fire_Switch_Log(1u); /*value 1 indicate the manual actuation switch was closed*/
							//break;
						}

	}
	else
	{
		fire_sw_prev_tick_g_u32 =  count_1ms;
		if(!man_sw_reset_state_g_b)
		{
			sw_rel_count_sl_u8++;
			if(sw_rel_count_sl_u8 > 200u)
			{
				sw_rel_count_sl_u8 = 0u;
				man_sw_reset_state_g_b = 1u;
				manual_sw_trbl_g_b = NO_TROUBLE;
				if(!system_reset_sl_b)
				{
					Fire_Switch_Log(0u); /*value 0 indicate the manual actuation switch was change to normal state*/
				}
				system_reset_sl_b = 0u;
			}
		}
	}

}

/* Function : Cylinder Pressure switch status check
 * Parameter: None
 * Return   : None
 */

void Pressure_Sw(void)
{
     static uint32_t press_sw_prev_tick = 0u;
    static uint16_t db_count_sl_u8 = 0u;/*debounce count*/
    static bool prev_state_sl_b = 0u;

 if(ioport_get_pin_level(CYLPRS_SW_Pin))
 {
     db_count_sl_u8++;
     if(db_count_sl_u8 >= DEBOUNCE_COUNT)
     {
         db_count_sl_u8 = 0u;
         pres_sw_trbl_g_b = NO_TROUBLE;
         press_sw_prev_tick = count_1ms;
		 //printf("Pressure_Sw set\r\n");
     }
 }
 else
 {
     if((count_1ms - press_sw_prev_tick) >= __10Sec)
     {
		 Get_New_trouble = PRES_SW_TRBL_7_FLASH;
         pres_sw_trbl_g_b = TROUBLE;
		// printf("Pressure_Sw clear\r\n");
     }
 }

     if(pres_sw_trbl_g_b != prev_state_sl_b)/* Log the data*/
     {
         //LOG
         Reset_Values();

         log_data.log_code = PRES_SW_TRBL_7_FLASH;

         log_data.start_stop = pres_sw_trbl_g_b;

         log_data.trouble_fire = PRES_SW_TRBL_7_FLASH ; /*assign pressure switch trouble code*/

         NOR_Flash_Log_Data(&log_data);

         prev_state_sl_b = pres_sw_trbl_g_b;
     }
}


/* Function : System reset Button status_check
 * Parameter: None
 * Return   : None
 */
void System_Reset_Button(void)
{

if(!ioport_get_pin_level(SYS_RESET_SW_PIN))
{

	if((count_1ms - system_reset_prev_tick_g_u32) >= SEC_2)
	{

		if((!hd1_trouble_g_u8) && (!hd2_trouble_g_u8) && (fire_happen_g_b))// && (manual_sw_trbl_g_b))
		{
			Fire_Rst_vehicle_start_bit = 0u;
			fire_rly_en_vehicle_start_bit = 0u;
			Buzzer_Beep(ON);
			fire_happen_g_b = NO_FIRE;
			fire_happen_safety_g_b = NO_FIRE;
			pulse_flag_g_b = 0u;
			//timer_30min_sil_g_b = 0u;
			alarm_sil_flag_g_b = 0u;
			 tc_stop(TC0,0);
			Timer_cnt_msec= 0u;
			Timer_cnt_sec= 0u;
			 //Silence_Timer_cnt=0;
			hd1_fire_sl_b = 0u;
			hd2_fire_sl_b = 0u;
			manual_sw_trbl_silence_b =0u;
			ioport_set_pin_level(RELAY_B,amerex_config_data.Default_fire_rly_level);
			Fire_LED(OFF);
			Delay(25);
			Buzzer_Beep(OFF);
			//  Buzzer_Freq_Set(BUZZER_FREQ_2KHZ, 0);  /* OFF PWM*/
			//printf("System_Reset_Button\r\n");
			/* Log the switch pressed event*/

			Reset_Values();

			log_data.log_code = 9u; /*Indicating system reset button pressed*/
			log_data.start_stop = 1u; /*button pressed*/

			NOR_Flash_Log_Data(&log_data);
		}
	}


	else if ( fire_happen_g_b ==1 && Fire_Rst_vehicle_start_bit ==0 )
	{
		fireRST_prev_tick_g_u32 = count_1ms;
		Fire_Rst_vehicle_start_bit = 1u;
	}



}

else
{
	system_reset_prev_tick_g_u32 =  count_1ms;
	if (Fire_Rst_vehicle_start_bit == 1u)
	{
		fireRST_prev_tick_g_u32 = count_1ms - fireRST_prev_tick_g_u32;
			(fireRST_prev_tick_g_u32 >= 50u && fireRST_prev_tick_g_u32 <= 1500u)?(FireRly_Rst_For_Vehicle_Strt()):(Fire_Rst_vehicle_start_bit = 0);

	}

}
}

 void FireRly_Rst_For_Vehicle_Strt(void)
 {
	Fire_Rst_vehicle_start_bit = 2u;
	ioport_set_pin_level(RELAY_B,amerex_config_data.Default_fire_rly_level);
	Reset_Values();
	log_data.log_code = 12u; /*Indicating system reset button pressed*/
	log_data.start_stop = 1u; /*button pressed*/
	 NOR_Flash_Log_Data(&log_data);
 }


/* Function : Log the manual switch closed time
 * Parameter: Start / Stop
 * Return   : None
 */
void Fire_Switch_Log(uint32_t log_status)
{
    Reset_Values();

    log_data.log_code = MANUAL_SW_3_FLASH;

    log_data.start_stop = log_status;

  //  log_data.trouble_fire = MANUAL_SW_3_FLASH; /*assign pressure switch trouble code*/

    NOR_Flash_Log_Data(&log_data);
}

void Enter_DFU_mode()
{

	if(( !ioport_get_pin_level(SYS_RESET_SW_PIN)) && (!ioport_get_pin_level(TEST_SW_PIN)))
	{
		wdt_restart(WDT);
		
		if((count_1ms - UpdateDFU_tick_g_u32) >= SEC_3)
		{
			ota_start =1;
			
			reset_mailbox_conf(&tx_mailbox);
	 					tx_mailbox.ul_mb_idx = CAN_COMM_TXMB_ID;
	 					tx_mailbox.uc_obj_type = CAN_MB_TX_MODE;
	 					tx_mailbox.uc_tx_prio = CAN_TX_PRIO;
	 					tx_mailbox.uc_id_ver = 0;
	 					tx_mailbox.ul_id_msk = 0;
	 					can_mailbox_init(CAN0, &tx_mailbox);
			SysNormal_LED(OFF);
			//ioport_set_pin_level(MAIN_EN,0);
			//ioport_set_pin_level(CAPCHG_EN,0);
			//ioport_set_pin_level(HD_EN,0);
			//ioport_set_pin_level(BATT_EN,0);
			//ioport_set_pin_level(HM_EN,0);
			//ioport_set_pin_level(MEAS_EN,0);
			ioport_set_pin_level(SYS_RST_LED,0);
			ioport_set_pin_level(Alarm_silence,0);
			ioport_set_pin_level(Trouble_LED,0);
			ioport_set_pin_level(SEC_PWR_LED,0);
			ioport_set_pin_level(Amerex_R,0);
			ioport_set_pin_level(Amerex_B,1);
			//
			DFU_auto_exit_prev_tick_g_u32 = count_1ms;
			//ota_ACK = 1u;
			printf("enter CAN_DFU \r\n");
			while (1)
			{
			wdt_restart(WDT);
			if((count_1ms - DFU_auto_exit_prev_tick_g_u32) > 10000ul ) //__20_SEC  __20_SEC
			{
			ioport_set_pin_level(Amerex_R,1);
			ioport_set_pin_level(Amerex_B,0);
			printf("DFU_auto_exit \r\n");
			//can_enable(CAN0);
			Small_Beep(100u);
			Delay(1000u);
			Small_Beep(100u);
			Delay(1000u);
			Small_Beep(100u);
			Delay(1000u);
			Small_Beep(100u);
			Delay(1000u);
			rstc_start_software_reset(RSTC);
			}

			if (g_ul_recv_status == 1u)
			{
			wdt_restart(WDT);
			DFU_callback_Fun(&rx_mailbox);
			g_ul_recv_status =0;
			}
			}


		}
	}

	else
	{
		UpdateDFU_tick_g_u32 =  count_1ms;
	}

}



void Enter_Backup_mode()
{
	 if(( !ioport_get_pin_level(ALARM_SILEN_PIN)) && (!ioport_get_pin_level(TEST_SW_PIN)) && pri_pwr_trouble_g_b == TROUBLE)
	 {
		        if((count_1ms - system_shutdown_tick_g_u32) >= SEC_2)
		        {


					Reset_Values();
					log_data.log_code = 15u; /*Indicating system reset button pressed*/
					//log_data.start_stop = 1u; /*button pressed*/
					NOR_Flash_Log_Data(&log_data);
					windup_Music();
					freq_test_backup_mode();
				}
 	 }
 	      else
	      {
		      system_shutdown_tick_g_u32 =  count_1ms;
	      }

}

void Update_Master_Clock(void)
{

	if( pri_pwr_trouble_g_b == TROUBLE && Master_Clock_bit == 0)
	{
		if((count_1ms - Clock_prev_tick_g_u32) >= 10u)//
		{
			Master_Clock_bit = 1;
			Freq_selec_bit = 1;
			pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);
			/* Switch mainck to fast RC */
			pmc_osc_enable_fastrc(CKGR_MOR_MOSCRCF_8_MHz);
			pmc_switch_mainck_to_fastrc( CKGR_MOR_MOSCRCF_4_MHz);
			pmc_switch_mck_to_mainck(PMC_MCKR_PRES_CLK_1);
			while (!pmc_osc_is_ready_fastrc());        // wait until oscillator is ready
			pmc_osc_disable_xtal(0);
 			pmc_disable_pllack();
					#ifdef Debug_enable
					const sam_uart_opt_t uart_console_settings = {4000000UL, 19200UL,UART_MR_PAR_NO};
					uart_init(CONSOLE_UART, &uart_console_settings);
					#endif
			SysTick_Config(4000UL);
			configure_i2c_master();
			adc_init();
			clock_setting.ul_mck =  4000000UL;
			clock_setting.ul_clkb =  PWM_VALUE_25_KHZ* PERIOD_VALUE;
			pwm_init(PWM, &clock_setting);

				//Timer0_init();
 		}
	}

 	else if	( pri_pwr_trouble_g_b == NO_TROUBLE && Master_Clock_bit == 1)
      {
		  		if((count_1ms - Clock_prev_tick_g_u32) >= 10u)
		  		{
					Master_Clock_bit = 0;
					//printf("Master_Clock_bit : %d\r\n",Master_Clock_bit);
					Freq_selec_bit = 0;
					struct genclk_config gcfg;
					sysclk_init();
					genclk_config_set_source(&gcfg, GENCLK_PCK_SRC_PLLACK);
					genclk_config_set_divider(&gcfg, GENCLK_PCK_PRES_2);
					genclk_enable(&gcfg, GENCLK_PCK_0);
						#ifdef Debug_enable
						const sam_uart_opt_t uart_console_settings = {60000000UL, 115200UL,UART_MR_PAR_NO};
						uart_init(CONSOLE_UART, &uart_console_settings);
						#endif
					Systick_Config(60000UL);
					configure_i2c_master();
					adc_init();
					clock_setting.ul_mck = sysclk_get_cpu_hz();
					clock_setting.ul_clkb =  PWM_VALUE_25_KHZ* PERIOD_VALUE;
					pwm_init(PWM, &clock_setting);
						//Timer0_init();
				}
 		}
	else
	{
	Clock_prev_tick_g_u32 = count_1ms;
	}



}