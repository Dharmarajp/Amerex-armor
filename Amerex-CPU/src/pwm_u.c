/*
 * pwm_u.c
 *
 * Created: 12-07-2022 15:31:47
 *  Author: p.dharmaraj
 */
 #include "main_header.h"



 void PWM_Init()
 {

  	pmc_enable_periph_clk(ID_PWM);
  	(Freq_selec_bit == 0) ? (clock_setting.ul_mck =  sysclk_get_cpu_hz()) : (clock_setting.ul_mck =  4000000UL);
	clock_setting.ul_clkb =  PWM_VALUE_25_KHZ* PERIOD_VALUE;
	//  pwm_channel_disable(PWM, PWMH2_Fire_LED);
	pwm_init(PWM, &clock_setting);
	// 		  	/* Period is left-aligned */
	g_pwm_channel_led.alignment = PWM_ALIGN_LEFT;
	// 		  	/* Output waveform starts at a low level */
	g_pwm_channel_led.polarity = PWM_HIGH;
	// 		  	/* Use PWM clock A as source clock */
	g_pwm_channel_led.ul_prescaler = PWM_CMR_CPRE_CLKB;
	// 		  	/* Period value of output waveform */
	g_pwm_channel_led.ul_period =PERIOD_VALUE;
	g_pwm_channel_led.channel = PWMH2_Fire_LED;
	// 		  	g_pwm_channel_led.ul_duty=0;
	pwm_channel_init(PWM, &g_pwm_channel_led);
	pwm_channel_enable(PWM, PWMH2_Fire_LED);
    g_pwm_channel_led.channel = PWMH1_LEDS;
	pwm_channel_init(PWM, &g_pwm_channel_led);
	pwm_channel_enable(PWM, PWMH1_LEDS);
	g_pwm_channel_led.channel = PWMH0_Amerex_LOGO;
	pwm_channel_init(PWM, &g_pwm_channel_led);
	pwm_channel_enable(PWM, PWMH0_Amerex_LOGO);
//     g_pwm_channel_led.channel = PWMH3_Buzzer;
// 	pwm_channel_init(PWM, &g_pwm_channel_led);
// 	pwm_channel_enable(PWM, PWMH3_Buzzer);


		Amerex_LOGO_PWM_EN(amerex_config_data.all_LED_duty);
		LEDS_PWM_EN(amerex_config_data.all_LED_duty);




 }

void Amerex_LOGO_PWM_EN(uint8_t ul_mask)
{
  	 g_pwm_channel_led.channel = PWMH0_Amerex_LOGO;
 	 pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_mask); /*g_pwm_channel_led.ul_duty;*/
 }

void LEDS_PWM_EN(uint8_t ul_mask)
{
 	g_pwm_channel_led.channel = PWMH1_LEDS;
 	pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_mask); /*g_pwm_channel_led.ul_duty;*/
 }

 void FIRE_LED_PWM_EN(uint8_t ul_mask)
 {
	 static uint8_t _mask_bit=0;
	if(_mask_bit != ul_mask)
	{
	  _mask_bit = ul_mask;
	   g_pwm_channel_led.channel = PWMH2_Fire_LED;
	   pwm_channel_update_duty(PWM, &g_pwm_channel_led, ul_mask); /*g_pwm_channel_led.ul_duty;*/
	}

 }

 /* Function : Duty cycle set For ALL LED except Fire LED
 * Parameter: None
 * Return   : None
 */
//void Duty_Cycle_Set(uint32_t duty_percentage)
//{
////	Amerex_LOGO_PWM_EN(duty_percentage);
////	LEDS_PWM_EN(duty_percentage);
//}





