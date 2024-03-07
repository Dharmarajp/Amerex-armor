/*
 * buzzer.c
 *
 * Created: 12-07-2022 16:47:19
 *  Author: p.dharmaraj
 */
#include "main_header.h"

uint8_t Get_New_trouble = 0u;
bool hd1_fire_sl_b = 0u;
bool hd2_fire_sl_b = 0u;
uint8_t buzzer_state = 0u;
uint16_t buzzer_interval_g_u16 = 100u;
uint32_t buzzer_prev_tick_g_u32 = 0u;
extern uint16_t ON_delay_500ms;
extern  uint16_t OFF_delay_10s;


void Startup_Music(void)
{
	
	  //RGB_Status[count_test]
		Amerex_LOGO_RGB(RGB_LED_RED);
		SysNormal_LED(ON);
		Buzzer_Freq_Set(BUZZER_FREQ_0_5KHZ, DUTY_0_5_KHZ);
		Delay(100u);
	   	Buzzer_Freq_Set(BUZZER_FREQ_1KHZ, DUTY_1_KHZ);
		ioport_set_pin_level(SEC_PWR_LED,1);
		Delay(100u);
		Buzzer_Freq_Set(BUZZER_FREQ_1_5KHZ, DUTY_1_5_KHZ);
		ioport_set_pin_level(Trouble_LED,1);
		Delay(100u);
		Amerex_LOGO_RGB(RGB_LED_GREEN);
		Buzzer_Freq_Set(BUZZER_FREQ_2KHZ, DUTY_2_KHZ);
		ioport_set_pin_level(SYS_RST_LED,1);
		Delay(100u);
		Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, DUTY_2_5_KHZ);
		ioport_set_pin_level(Alarm_silence,1);
		Delay(100u);
		Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, 0);
		Amerex_LOGO_RGB(RGB_LED_BLUE);

		FIRE_LED_PWM_EN(amerex_config_data.fire_LED_duty);
		wdt_restart(WDT);
		LPF_buffer_load();
		// Delay(200u);
		wdt_restart(WDT);
		ioport_set_pin_level(Amerex_B,0);
		//Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ,0); //amerex_config_data.buzzer_duty
	    //SysNormal_LED(OFF);
		//FIRE_LED_PWM_EN(0);

		ioport_set_pin_level(RELAY_A,amerex_config_data.Default_trbl_rly_level);
		ioport_set_pin_level(RELAY_B,amerex_config_data.Default_fire_rly_level);

}

void windup_Music(void)
{
	wdt_restart(WDT);
	ioport_set_pin_level(SYS_RST_LED,0);
	Buzzer_Freq_Set(BUZZER_FREQ_0_5KHZ, DUTY_0_5_KHZ);
	Amerex_LOGO_RGB(RGB_LED_BLUE);
	Delay(100u);
	Buzzer_Freq_Set(BUZZER_FREQ_1KHZ, DUTY_1_KHZ);
	ioport_set_pin_level(Alarm_silence,1);
	Delay(100u);
	Amerex_LOGO_RGB(RGB_LED_GREEN);
	 	wdt_restart(WDT);
	Buzzer_Freq_Set(BUZZER_FREQ_1_5KHZ, DUTY_1_5_KHZ);
	ioport_set_pin_level(SYS_RST_LED,1);
	Delay(100u);
	Buzzer_Freq_Set(BUZZER_FREQ_2KHZ, DUTY_2_KHZ);
	ioport_set_pin_level(Trouble_LED,1);
	Delay(100u);
	Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, DUTY_2_5_KHZ);
	ioport_set_pin_level(SEC_PWR_LED,1);
	Delay(100u);
	Amerex_LOGO_RGB(RGB_LED_RED);
	SysNormal_LED(ON);
	Buzzer_Freq_Set(BUZZER_FREQ_0_5KHZ, DUTY_0_5_KHZ);
	Delay(100u);
	Buzzer_Freq_Set(BUZZER_FREQ_0_5KHZ, 0);
		wdt_restart(WDT);
}



/* Function  :  Changing Buzzer frequency and duty cycle
 * Parameter :  Set frequency,set duty cycle
 * Return    :   None
 */
void Buzzer_Freq_Set(uint32_t frequency, uint16_t duty_cycle)
{


 	static uint16_t last_frequency_sl_u16 = 0u;
 	static uint16_t last_duty_cycle_sl_u16 = 0u;

 	if((last_frequency_sl_u16 != frequency) || (last_duty_cycle_sl_u16 != duty_cycle))
 	{

	 	last_frequency_sl_u16 =  frequency;
	 	last_duty_cycle_sl_u16 = duty_cycle;
	 	clock_setting.ul_clka= frequency * PERIOD_VALUE;
		(Freq_selec_bit == 0) ? (clock_setting.ul_mck =  sysclk_get_cpu_hz()) : (clock_setting.ul_mck =  4000000UL);

	 	// clock_setting.ul_mck =   	sysclk_get_cpu_hz();	//	4000000;//
	 	pwm_init(PWM, &clock_setting);
		//pwm_channel_enable(PWM, PWMH3_Buzzer);
	 	g_pwm_channel_led.channel = PWMH3_Buzzer;
	 	g_pwm_channel_led.ul_duty=duty_cycle;
	 	/* Period is left-aligned */
	 	g_pwm_channel_led.alignment =  PWM_ALIGN_LEFT;//
	 	/* Output waveform starts at a low level */
	 	g_pwm_channel_led.polarity = PWM_HIGH;
	 	/* Use PWM clock A as source clock */
	 	g_pwm_channel_led.ul_prescaler = PWM_CMR_CPRE_CLKA;
	 	/* Period value of output waveform */
	 	g_pwm_channel_led.ul_period =PERIOD_VALUE;
	 	pwm_channel_init(PWM, &g_pwm_channel_led);
	 	pwm_channel_enable(PWM, PWMH3_Buzzer);
	 	//pwm_channel_update_duty(PWM, &g_pwm_channel_led, duty_cycle); /*g_pwm_channel_led.ul_duty;*/

 	}

}

/* Function  :  Buzzer beep sound
 * Parameter :  @param1 ON/OFF/Beep
 * Return    :   None
 */
void Buzzer_Beep(uint8_t State)
{
    if(State == OFF)
    {
	    Buzzer_Freq_Set(BUZZER_FREQ_2KHZ, 0);  /* OFF PWM*/
	    buzzer_interval_g_u16 = ON_delay_500ms;
	    buzzer_state = 1;
    }
    else if(State == ON)
    {
	    Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, Default_buzz_Duty);//  /* ON PWM*/  amerex_config_data.buzzer_duty
	    // buzzer_state = 0;
		buzzer_prev_tick_g_u32 	=count_1ms;
    }
    else
    {
	    if((count_1ms - buzzer_prev_tick_g_u32) >= buzzer_interval_g_u16)
	    {
		    buzzer_prev_tick_g_u32 = count_1ms;

		    if(Buzzer_bit == 1 && buzzer_state ==2)
		    {
			    buzzer_interval_g_u16 = OFF_delay_10s;
			    Buzzer_bit = 0;
			    buzzer_state = 1;
			    trouble_LED_prevtick_g_u32 = count_1ms;
			    //printf("Buzzer_bit %d\n",Buzzer_bit);
		    }

		    else if(buzzer_state == 1)
		    {
			    buzzer_interval_g_u16 = ON_delay_500ms;
			    buzzer_state = 0;
			    Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, Default_buzz_Duty);//amerex_config_data.buzzer_duty
		    }
		    else if(buzzer_state == 0)
		    {
			    buzzer_interval_g_u16 = 1u;
			    buzzer_state = 2;
			    Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, 0);
		    }

	    }

    }
}

/* Function  :  Sound indication
 * Parameter :  Set frequency,set duty cycle
 * Return    :   None
 */

void Buzzer_indication(void)
{
    static uint8_t prev_hd_val_sl_b = 0u;
    static uint8_t hd_val_sl_b = 0u;
	//static uint8_t Backup_Trouble = 0u;

   if(any_of_trouble_g_b && !fire_happen_g_b && (!alarm_sil_flag_g_b) )	//   && (!timer_30min_sil_g_b)
    {
        Buzzer_Beep(BEEP);
    }
    else if( Fire_Rst_vehicle_start_bit!=2u && fire_happen_g_b && (!alarm_sil_flag_g_b) )//  && (!timer_30min_sil_g_b)
    {
        Buzzer_Beep(ON);
    }
    else
    {
        Buzzer_Beep(OFF);
    }



      /* if fire occurred and user silence the  Buzzer by alarm silence button after some time in another heat detector fire occurred re sound the buzzer*/
if(fire_happen_g_b && Fire_Rst_vehicle_start_bit != 2u)
{
   if(hd1_trouble_g_u8 == 1u)
   {
       hd1_fire_sl_b = 1u;
   }
   if(hd2_trouble_g_u8 == 1u)
   {
       hd2_fire_sl_b = 1u;
   }

   hd_val_sl_b = hd1_fire_sl_b + hd2_fire_sl_b + manual_sw_trbl_silence_b;

   if(hd_val_sl_b != prev_hd_val_sl_b)
   {
      alarm_sil_flag_g_b = 0u;
	   tc_stop(TC0,0);
	   Timer_cnt_msec= 0u;
	   Timer_cnt_sec= 0u;
	   //Silence_Timer_cnt=0;
       //timer_30min_sil_g_b = 0u;
      alarm_sil_prev_tick_g_u32 = count_1ms;
   }
   prev_hd_val_sl_b = hd_val_sl_b;
}









}

/* Function  :  Small  beep
 * Parameter :  None
 * Return    :  None
 */
void Small_Beep(uint32_t delay)
{
    Buzzer_Beep(ON);
    Delay(delay);
    Buzzer_Beep(OFF);
}
