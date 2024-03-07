/*
 * switch_inputs.h
 *
 * Created: 14-07-2022 10:21:01
 *  Author: p.dharmaraj
 */


#ifndef SWITCH_INPUTS_H_
#define SWITCH_INPUTS_H_

#define SEC_1  1000u/*1Sec Delay count*/
#define SEC_2  2000u/*2Sec Delay count*/
#define SEC_5  5000u /*5 Sec Delay count*/
#define SEC_3  3000u /*5 Sec Delay count*/
#define Msec_50 50u
#define msec_200 200u


extern bool test_sw_pressed_g_b;
extern uint32_t system_reset_prev_tick_g_u32;
extern uint32_t system_shutdown_tick_g_u32;
extern bool manual_sw_trbl_g_b;
extern bool alarm_sil_flag_g_b;
extern bool pres_sw_trbl_g_b;
extern bool Master_Clock_bit;
extern volatile bool Freq_selec_bit;
extern uint32_t test_sw_prev_tick_g_u32;
extern uint32_t Clock_prev_tick_g_u32;  /*To store previous count value */
extern volatile bool Fire_Alart_bit;
extern volatile uint8_t ALEDS_bit;
extern volatile uint8_t Fire_Rst_vehicle_start_bit;
extern volatile uint8_t fire_rly_en_vehicle_start_bit;
extern uint32_t DFU_auto_exit_prev_tick_g_u32;
extern bool manual_sw_trbl_silence_b;
extern uint32_t fire_sw_prev_tick_g_u32;

void Test_Button(void);
void Alarm_Silence_Button(void);
void System_Reset_Button(void);
void Fire_Switch(void);
void Pressure_Sw(void);
void Enter_Backup_mode(void);
void freq_test_backup_mode(void);
void Update_Master_Clock(void);
void Fire_Switch_Log(uint32_t log_status);
void Enter_DFU_mode(void);
void FireRly_Rst_For_Vehicle_Strt(void);


#endif /* SWITCH_INPUTS_H_ */