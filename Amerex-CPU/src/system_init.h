/*
 * system_init.h
 *
 * Created: 12-07-2022 14:58:09
 *  Author: p.dharmaraj
 */


#ifndef SYSTEM_INIT_H_
#define SYSTEM_INIT_H_


#define EN_HIGH true
#define EN_LOW  false

/** Watchdog period 3000ms */
#define WDT_3SEC                        3000 //3000
#define WDT_0_5SEC                       500 //3000



typedef struct {
	/** PIO dedicated to MAIN_EN pin index. */
	uint8_t pio_main_enable_idx;
	uint8_t pio_Internal_Batt_Pack_idx;
	uint8_t pio_Capacitor_Bank_Charge_idx;
	uint8_t pio_HeatDetectorEnable_idx;
	uint8_t pio_SecondaryPowerPack_ExEnable;
	uint8_t pio_SecondaryPowerPack_NiMHEnable;
	uint8_t pio_ActuatorHealthMonitorEnable;
	uint8_t pio_VoltageMeasureEnable;
	uint8_t pio_RTC_FlashEnable_idx;

}Power_Enable_index;


  Power_Enable_index Activate_PIN;

void Pherperial_Power_Enable(void);
void Delay(uint32_t count);
void Systick_Config( uint64_t cunt);
void Pherperial_clock_Enable(void);

void LED_BUZZER_SUPPLY_enable(Power_Enable_index *p_component);
void LED_BUZZER_SUPPLY_disable(Power_Enable_index *p_component);
void LED_BUZZER_SUPPLY_set(Power_Enable_index *p_component, uint8_t pin_idx);

//void Internal_Battery_Pack_enable(Power_Enable_index *p_component);
//void Internal_Battery_Pack_disable(Power_Enable_index *p_component);
//void Internal_Battery_Pack_set(Power_Enable_index *p_component, uint8_t pin_idx);

void Capacitor_Bank_Charge_enable(Power_Enable_index *p_component);
void Capacitor_Bank_Charge_disable(Power_Enable_index *p_component);
void Capacitor_Bank_Charge_set(Power_Enable_index *p_component, uint8_t pin_idx);

void HeatDetector_enable(Power_Enable_index *p_component);
void HeatDetector_disable(Power_Enable_index *p_component);
void HeatDetector_set(Power_Enable_index *p_component, uint8_t pin_idx);

//void SecondaryPower_enable(Power_Enable_index *p_component);
//void SecondaryPower_disable(Power_Enable_index *p_component);
//void SecondaryPower_set(Power_Enable_index *p_component, uint8_t pin_idx);

void SecondaryPower_NiMH_enable(Power_Enable_index *p_component);
void SecondaryPower_NiMH_disable(Power_Enable_index *p_component);
void SecondaryPower_NiMH_set(Power_Enable_index *p_component, uint8_t pin_idx);

void Act_HealthMonitor_enable(Power_Enable_index *p_component);
void Act_HealthMonitor_disable(Power_Enable_index *p_component);
void Act_HealthMonitor_set(Power_Enable_index *p_component, uint8_t pin_idx);

void VoltMeasurement_enable(Power_Enable_index *p_component);
void VoltMeasurement_disable(Power_Enable_index *p_component);
void VoltMeasurement_set(Power_Enable_index *p_component, uint8_t pin_idx);

void RtcFlash_enable(Power_Enable_index *p_component);
void RtcFlash_disable(Power_Enable_index *p_component);
void RtcFlash_set(Power_Enable_index *p_component, uint8_t pin_idx);

void Watchdog_Config(uint32_t value);

#endif /* SYSTEM_INIT_H_ */