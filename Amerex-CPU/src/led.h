/*
 * led.h
 *
 * Created: 13-07-2022 12:53:36
 *  Author: p.dharmaraj
 */


#ifndef LED_H_
#define LED_H_

#define RGB_LED_RED (1<<31)
#define RGB_LED_GREEN (1<<29)
#define RGB_LED_BLUE (1<<30)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE )
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)



 #define FIRE_LED_FADING_TIME  500 /*0.5 sec rise and 0.5 sec fall time. so 1000ms/2 = 500
 divide by 1 - specifies systick timer configured as 1ms interrupt*/

enum LED
{
	OFF = 0u,
	ON = 1u,
	BEEP = 3u,
	ALWAYS_ON = 100u, /*This is Specially For particular function*/
	LED_FADING = 1u,  /*used for Fire, Charge LED fading*/
	FIRE = 1u,
	NO_FIRE = 0u,
	PRESSED = 1u,
	RELEASED = 0u,
	SEC_PWR_NOT_AVAIL = 2u, /*if system working in primary power and secondary power not available*/
	PRIMARY_PWR_NOT_AVAIL = 1u,
	__100ms = 100u,         /* Count for 100ms delay*/
	__250ms = 250u,
	__500ms = 500u,
	__700ms = 700u,
	TROUBLE = 1u,
	NO_TROUBLE = 0u,
	DEBOUNCE_COUNT = 1250u
}IND;
enum error
{
	HD1_1_FLASH = 1u,
	HD2_2_FLASH = 2u,
	MANUAL_SW_3_FLASH = 3u,
	ACT_TRBL_4_FLASH = 4u,
	PRI_PWR_5_FLASH = 5u,
	SEC_PWR_6_FLASH = 6u,
	PRES_SW_TRBL_7_FLASH = 7u,
};
extern uint32_t fire_LED_prevtick_g_u32;
extern uint8_t count_test;
extern uint32_t trouble_LED_prevtick_g_u32;
volatile extern uint32_t Buzzer_bit;
volatile extern uint8_t Trouble_status;



void SysNormal_LED(bool on_off);
void Secondary_Pwr_LED(uint8_t);
void _Trouble_LED(uint8_t);
 void Fire_LED(uint8_t);
void Sys_Reset_LED(bool on_off);// __attribute__((always_inline));
void Alarm_Silence_LED(bool on_off);
// void Charge_Indication_LED(bool on_off);
 void Config_Indication_LED(uint8_t);
// void System_Error_LED(uint8_t count);
void System_Normal_Ind(void);
void Secondary_pwr_Ind(void);
void Trouble_Ind(void);
void Fire_Ind(void);
void System_Reset_Ind(void);
void Alarm_Sil_Ind(void);
void Amerex_LOGO_RGB(const uint32_t ul_mask);
void FireLED_Freq_Set(uint32_t frequency, uint16_t duty_cycle);
void RGB_TEST(void);

extern uint8_t fire_LED_duty_max_g_u8;
extern uint8_t fire_LED_fading_count_g_u8;
 //extern volatile uint32_t data__,__RGB_TEST;

#endif /* LED_H_ */