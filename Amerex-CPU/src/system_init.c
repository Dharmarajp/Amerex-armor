// /*
//  * system_init.c
//  *
//  * Created: 12-07-2022 14:57:00
//  *  Author: p.dharmaraj
//  */
 #include "main_header.h"



void Pherperial_clock_Enable()
{
pmc_enable_periph_clk(ID_PIOA);
pmc_enable_periph_clk(ID_PIOB);
}


void Pherperial_Power_Enable()
{

LED_BUZZER_SUPPLY_set(&Activate_PIN,MAIN_EN);
LED_BUZZER_SUPPLY_enable(&Activate_PIN);


// Internal_Battery_Pack_set(&Activate_PIN,NIMH);
// Internal_Battery_Pack_enable(&Activate_PIN);


 Capacitor_Bank_Charge_set(&Activate_PIN,CAPCHG_EN);
// Capacitor_Bank_Charge_enable(&Activate_PIN);


HeatDetector_set(&Activate_PIN,HD_EN);
HeatDetector_enable(&Activate_PIN);


//SecondaryPower_set(&Activate_PIN,SEC_EN);
//SecondaryPower_enable(&Activate_PIN);


//SecondaryPower_NiMH_set(&Activate_PIN,BATT_EN);
//SecondaryPower_NiMH_disable(&Activate_PIN);

Act_HealthMonitor_set(&Activate_PIN, HM_EN);
Act_HealthMonitor_enable(&Activate_PIN);


VoltMeasurement_set(&Activate_PIN, MEAS_EN);
VoltMeasurement_enable(&Activate_PIN);


RtcFlash_set(&Activate_PIN,  RTC_Flash_EN);
RtcFlash_enable(&Activate_PIN);


//	ioport_set_pin_level(CAN_ACT,0);
//	ioport_set_pin_level(MUX_A1,0);
	ioport_set_pin_level(DIMMER_IN,1);

	ioport_set_pin_level(PIN_CAN0_TR_EN_IDX, 0);
	ioport_set_pin_level(CAN_Vio, 1);
	//ioport_set_pin_level(WakeUp_input, 0);

}

void Systick_Config(uint64_t _count)
{

//SysTick_Config(sysclk_get_cpu_hz() / 1000);
SysTick_Config(_count);
}

/* Function : Enable the Watch dog interrupt for 3 Sec
 * Parameter: None
 * Return   : None
 */
void Watchdog_Config(uint32_t set_time)
{
	uint32_t  timeout_value,wdt_mode;
 	/* Get timeout value. */
 	timeout_value = wdt_get_timeout_value(set_time * 1000,BOARD_FREQ_SLCK_XTAL);
     wdt_mode = WDT_MR_WDRSTEN;
	 wdt_init(WDT, wdt_mode, timeout_value, timeout_value);

}
/* Function : User delay
 * Parameter: Specifies the delay (1 count = 1ms)
 * Return   : None
 */
void Delay(uint32_t count)
{
    uint32_t prev_count = count_1ms;
    while(count)
    {
         if((count_1ms - prev_count) >= count)
        {
                 break;
        }
    }
}



/**
 * \brief Enable .
 *
 * \param p_component Pointer to LED_BUZZER control structure.
 */
void LED_BUZZER_SUPPLY_enable(Power_Enable_index *p_component)
{
	/* Raise EN pin of LED_BUZZER_CTRL to High Level (Vcc). */
	 ioport_set_pin_level(p_component->pio_main_enable_idx, EN_HIGH);
}

/**
 * \brief Enable .
 *
 * \param p_component Pointer to LED_BUZZER control structure.
 */
void LED_BUZZER_SUPPLY_disable(Power_Enable_index *p_component)
{
	/* Raise EN pin of LED_BUZZER_CTRL to High Level (Vcc). */
	ioport_set_pin_level(p_component->pio_main_enable_idx, EN_LOW);
}

/**
 * \brief Initialize EN pin for transceiver.
 *
 * \param p_component Pointer to LED_BUZZER_SUPPLY_PIN control structure.
 * \param pin_idx     The pin index value for LED_BUZZER_SUPPLY_PIN EN pin.
 */
void LED_BUZZER_SUPPLY_set(Power_Enable_index *p_component, uint8_t pin_idx)
{
	p_component->pio_main_enable_idx = pin_idx;
}






void Capacitor_Bank_Charge_enable(Power_Enable_index *p_component)
{
ioport_set_pin_level(p_component->pio_Capacitor_Bank_Charge_idx,EN_HIGH );
}

void Capacitor_Bank_Charge_set(Power_Enable_index *p_component, uint8_t pin_idx)
{
 p_component->pio_Capacitor_Bank_Charge_idx = pin_idx;
}

void Capacitor_Bank_Charge_disable(Power_Enable_index *p_component)
{
ioport_set_pin_level(p_component->pio_Capacitor_Bank_Charge_idx, EN_LOW);
}


void HeatDetector_enable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_HeatDetectorEnable_idx, EN_HIGH);
}

void HeatDetector_set(Power_Enable_index *p_component, uint8_t pin_idx)
{
	p_component->pio_HeatDetectorEnable_idx = pin_idx;
}

void HeatDetector_disable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_HeatDetectorEnable_idx, EN_LOW);
}




void SecondaryPower_NiMH_enable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_SecondaryPowerPack_NiMHEnable, EN_HIGH);
}

void SecondaryPower_NiMH_set(Power_Enable_index *p_component, uint8_t pin_idx)
{
	p_component->pio_SecondaryPowerPack_NiMHEnable = pin_idx;
}

void SecondaryPower_NiMH_disable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_SecondaryPowerPack_NiMHEnable, EN_LOW);
}

void Act_HealthMonitor_enable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_ActuatorHealthMonitorEnable, EN_HIGH);
}

void Act_HealthMonitor_set(Power_Enable_index *p_component, uint8_t pin_idx)
{
	p_component->pio_ActuatorHealthMonitorEnable = pin_idx;
}

void Act_HealthMonitor_disable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_ActuatorHealthMonitorEnable, EN_LOW);
}


void VoltMeasurement_enable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_VoltageMeasureEnable, EN_HIGH);
}

void VoltMeasurement_set(Power_Enable_index *p_component, uint8_t pin_idx)
{
	p_component->pio_VoltageMeasureEnable = pin_idx;
}

void VoltMeasurement_disable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_VoltageMeasureEnable, EN_LOW);
}


void RtcFlash_enable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_RTC_FlashEnable_idx, EN_HIGH);
}

void RtcFlash_set(Power_Enable_index *p_component, uint8_t pin_idx)
{
	p_component->pio_RTC_FlashEnable_idx = pin_idx;
}

void RtcFlash_disable(Power_Enable_index *p_component)
{
	ioport_set_pin_level(p_component->pio_RTC_FlashEnable_idx, EN_LOW);
}



