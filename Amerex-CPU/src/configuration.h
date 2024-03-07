/*
 * configuration.h
 *
 * Created: 12-07-2022 16:37:15
 *  Author: p.dharmaraj
 */


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_


#define __20_SEC 20000u
#define __80_SEC 80000u
 typedef struct
 {
	 uint16_t all_LED_duty;
	 uint16_t fire_LED_duty;
	 uint16_t buzzer_duty;
	 uint16_t trbl_relay_delay;
	 uint16_t fire_relay_delay;
	 uint16_t actuator_value;
	 uint8_t CAN_baud_rate;
	 uint8_t Default_trbl_rly_level;
	 uint8_t Default_fire_rly_level;
	 uint8_t Trigger;
 }my_data;

 my_data amerex_config_data;

enum config_LED
{
	CONFIG_ALL_LED = 0u, /*All LED except Fire LED*/
	CONFIG_FIRE_LED = 1u,
	CONFIG_BUZZER_LED = 2u,
	CONFIG_TRBL_LED = 3u,
	CONFIG_CONFIG_LED = 4u,
	CONFIG_ALL_OFF = 50u,
	CONFIG_DB_COUNT = 100u,/*For push button debounce count*/
	CONFIG_ON = 1u,
	CONFIG_OFF = 0u,
	CONF_BLINK = 3u,
};
enum config_state
{
	CONFIG_ALL_LED_BRIGHT = 0u,
	CONFIG_FIRE_LED_BRIGHT = 1u,
	//CONFIG_BUZZER_SOUND    = 2u,
	CONFIG_CAN_BUS_BAUD_RATE = 2u,
	CONFIG_TRBL_RELAY_DELAY = 3u,
	CONFIG_FIRE_RELAY_DELAY = 4u,
	//CONFIG_ACTUATOR_CALIBRATION = 5u,
	CONFIG_DEFAULT_TRBL_RLY_STATE = 5u,  //High or Low	CONFIG_TRBL_RELAY_DELAY
	CONFIG_DEFAULT_FIRE_RLY_STATE =6u

};
enum button
{
	INC_BUTTON_PRESSED = 2u,
	DEC_BUTTON_PRESSED = 1u,
	NO_BUTTON_PRESSED = 0u
};

 typedef struct
 {
	 uint32_t epoch_time;
	 uint8_t log_code;
	 uint8_t trouble_fire;
	 uint8_t start_stop;
	 uint8_t value;
	 uint8_t crc;
 }flash;


extern flash log_data;
 extern volatile uint8_t value ;
extern  uint32_t all_output_test_g_32;
extern uint32_t buzzer_fire_output_test_g_32;
extern uint32_t CAN_tx_bit;
 extern volatile	uint8_t fire_bit;
extern volatile  uint8_t buzz_bit;
void Fire_LED_Count(void);
void LED_Blink(uint8_t led, uint8_t);
void Configuration(void);
void ALL_LED_BLINK(bool _onoff);
void Sound_Alert(void);
uint32_t Button_Status(void);
#endif /* CONFIGURATION_H_ */