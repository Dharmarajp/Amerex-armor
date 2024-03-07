/*
 * fire.h
 *
 * Created: 14-07-2022 11:22:45
 *  Author: p.dharmaraj
 */


#ifndef FIRE_H_
#define FIRE_H_

extern uint32_t alarm_sil_prev_tick_g_u32;
extern uint32_t alarm_retry_sil_prev_tick_g_u32;
extern uint32_t fire_retry_sound_prev_tick_g_u32;
extern bool pulse_flag_g_b;
//extern bool timer_30min_sil_g_b;
//extern uint32_t hd1_prev_tick_g_u32;
#define SILENCE_TIME_30min  1800000lu /* 30min * 60 sec = 1800sec, 1800S * 1000ms = 1800000 */


extern uint8_t hd1_fire_count_g_u8;
extern uint32_t hd1_prev_tick_g_u32;
extern uint8_t hd2_fire_count_g_u8;
extern uint32_t hd2_prev_tick_g_u32;

void Actuator_Pulse(void);
void Check_Heat_Detector_Trbl(void);
void Fire_sound_retry_by_Timer(void);


#endif /* FIRE_H_ */