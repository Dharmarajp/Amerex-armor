/*
 * adc_u.h
 *
 * Created: 14-07-2022 11:13:42
 *  Author: p.dharmaraj
 */


#ifndef ADC_U_H_
#define ADC_U_H_

  #define NUM_CHANNELS   12
  #define BUFFER_SIZE     NUM_CHANNELS
#define HMON_OPEN_FLT_RANGE         2280u
#define HMON_GND_FLT_RANGE          2300u



extern bool fire_happen_g_b;          /*To store the fire event*/
extern uint8_t hd1_trouble_g_u8;
extern uint8_t hd2_trouble_g_u8 ;
extern bool fire_happen_safety_g_b;
extern bool pri_pwr_trouble_g_b;
extern bool sec_pwr_trouble_g_b;
extern bool super_cap_state_g_b;
extern uint8_t actuator_trbl_g_u8;
extern uint32_t adc_prev_tick_g_u32;
extern  volatile uint8_t uc_ch_num[NUM_CHANNELS];
extern  volatile uint16_t us_value[NUM_CHANNELS];
extern uint32_t primary_pwr_vol_g_u16;
extern uint16_t sec_pwr_vol_g_u16;
extern uint16_t hd1_measured_g_u16;
extern uint16_t hd2_measured_g_u16;
extern uint16_t super_capVolt_g_u16;

extern uint32_t sec_pwr_prev_tick_g_u32;
extern uint32_t pri_pwr_prev_tick_g_u32;
extern  uint32_t sec_pwr_cutoff_tick_g_u32;
extern 	uint32_t Raw_value_HD1;
extern 	uint32_t Raw_value_HD2;
extern 	uint32_t sec_rawvolt;

 enum heat_detector
 {
	 OPEN_TROUBLE = 3200u,
	 HD_OK_MIN    = 2900u,
	 HD_OK_MAX    = 3100u,
	 HD_SHORT_MIN = 1000u,
	 HD_SHORT_MAX = 2800u,
	 HD_FIRE_MIN  = 500u,
	// HD_FIRE_MAX  = 3010u,
	 FIRE_COUNT   = 100u,
	 __10Sec      = 10000u
 };

enum power
{
	SEC_PWR_MIN_VOL     = 5100u,  //4500u
	PRI_PWR_MIN_VOL    = 8700u,	  //8500
	PRI_PWR_MAX_VOL     = 40000u,
	SEC_PWR_CUTOFF_VOL  = 5000u,  //4400
	supercap_highcuttoff =14700u, // 5% tolerance
	supercap_lowcuttoff = 13500u// 5% tolerance 12350u
};



void adc_init(void);
void LPF_buffer_load(void);
uint32_t HD1_volt(void);
uint32_t HD2_volt(void);
uint32_t PRI_volt(void);
uint32_t SEC_volt(void);
uint32_t LPF_Primary_Voltage(uint32_t); //low pass filter for primary voltage measurement
uint32_t LPF_Secondary_Voltage(uint32_t); //low pass filter for secondary voltage measurement
 uint32_t LPF_Super_cap_Voltage(uint32_t);
uint32_t LPF_HD1(uint32_t x); //low pass filter for heat detector 1
uint32_t LPF_HD2(uint32_t x);
uint32_t SUPER_CAP_Volt(void);
void Actuator_HM(void);
uint32_t ACT_Mux_B_volt(uint32_t);
uint32_t ACT_Mux_A_volt(uint32_t);
void Check_Power_Trbl(void);
void Super_CAP_Charge_enable_disable(void);
#endif /* ADC_U_H_ */