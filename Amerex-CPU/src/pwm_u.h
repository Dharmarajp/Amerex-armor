/*
 * pwm_u.h
 *
 * Created: 12-07-2022 16:01:04
 *  Author: p.dharmaraj
 */


#ifndef PWM_U_H_
#define PWM_U_H_




  /** PWM period value */
 #define PERIOD_VALUE   100
 #define PWM_VALUE_25_KHZ 25000U//25000u
 #define PWM_VALUE_2_5_KHZ 2500u

//   /** PWM channel instance for LEDs */
//   pwm_channel_t g_pwm_channel_amerex_logo,g_pwm_channel_LEDS,g_pwm_channel_Fire,g_pwm_channel_buzzer;
//   pwm_clock_t clock_setting_amerex_logo,clock_setting_LEDS,clock_setting_Fire_led,clock_setting_buzzer;
  /** PWM channel instance for LEDs */
  pwm_channel_t g_pwm_channel_led;
  pwm_clock_t clock_setting;



  void PWM_Init(void);
  void Amerex_LOGO_PWM_EN(uint8_t ul_mask);
  void LEDS_PWM_EN(uint8_t ul_mask);
  void FIRE_LED_PWM_EN( uint8_t ul_mask);
  void Buzzer_PWM_EN( uint8_t ul_mask);
  void fire_Freq_Set(uint32_t frequency, uint16_t duty_cycle);
  //void Duty_Cycle_Set(uint32_t ul_mask);


#endif /* PWM_U_H_ */