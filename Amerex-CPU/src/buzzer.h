/*
 * buzzer.h
 *
 * Created: 12-07-2022 16:46:25
 *  Author: p.dharmaraj
 */


#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_FREQ_0        0u
#define BUZZER_FREQ_0_5KHZ 500u
#define BUZZER_FREQ_1KHZ   1000u
#define BUZZER_FREQ_1_5KHZ 1500u
#define BUZZER_FREQ_2KHZ   2000u
#define BUZZER_FREQ_2_5KHZ 2500u
#define BUZZER_FREQ_3KHZ   3000u
#define BUZZER_FREQ_3_5KHZ 3500u
#define BUZZER_FREQ_4KHZ   4000u

#define FIRE_LED_FREQ_25KHZ  25000u


#define DUTY_0       0
#define DUTY_0_5_KHZ 50u
#define DUTY_1_KHZ   50u
#define DUTY_1_5_KHZ 50u
#define DUTY_2_KHZ   50u
#define DUTY_2_5_KHZ 50u
#define DUTY_3_KHZ   50u
#define DUTY_3_5_KHZ 50u
#define DUTY_4_KHZ   50u
#define Default_buzz_Duty 50u



 /** PWM period value */
 #define PERIOD_VALUE   100


extern uint8_t Get_New_trouble;
extern bool hd1_fire_sl_b;
extern bool hd2_fire_sl_b;
extern uint8_t buzzer_state;
extern uint32_t buzzer_prev_tick_g_u32;
uint16_t buzzer_interval_g_u16;


void Buzzer_Freq_Set(uint32_t frequency, uint16_t duty_cycle);
/*void Buzzer_Sound(uint16_t freq, uint16_t duty_cycle);*/
void Startup_Music(void);
void windup_Music(void);
void Buzzer_Beep(uint8_t);
void Buzzer_indication(void);
void Small_Beep(uint32_t delay);




#endif /* BUZZER_H_ */