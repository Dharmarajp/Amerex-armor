// /*
//  * adc_u.c
//  *
//  * Created: 14-07-2022 11:13:24
//  *  Author: p.dharmaraj
//  */
#include "main_header.h"

uint32_t adc_prev_tick_g_u32 = 0u;
unsigned long read_prevtick = 0;
uint8_t read_count = 0;
float vref = 3.30; //ADC Voltage reference value
bool fire_happen_g_b = 0u;          /*To store the fire event*/
uint8_t hd1_trouble_g_u8 = 0u;
uint8_t hd2_trouble_g_u8 = 0u;
bool fire_happen_safety_g_b = 0u;          /*this flag is for safety store same "fire_happen_g_b" value */
bool pri_pwr_trouble_g_b = 0u;
bool sec_pwr_trouble_g_b = 0u;
bool super_cap_state_g_b = 0u;
uint8_t actuator_trbl_g_u8 = 0u;
uint32_t primary_pwr_vol_g_u16 = 0u;
uint16_t sec_pwr_vol_g_u16 = 0u;
uint16_t hd1_measured_g_u16 = 0u;
uint16_t hd2_measured_g_u16 = 0u;
uint16_t super_capVolt_g_u16 = 0u;
uint16_t hmon_1A_2B_val_g_u16 = 0u;
uint16_t hmon_1B_2A_val_g_u16 = 0u;
uint16_t hmon_1A_gnd_flt_val_g_u16 = 0u;
uint16_t hmon_2A_gnd_flt_val_g_u16 = 0u;
bool ac_act_delay = 0;   //flag for after ON the AC  H-bridge ADC value stabilization (Gnd fault)
bool bd_act_delay = 0;   //flag for after ON the BD H-bridge ADC value stabilization  (Gnd fault)
bool ac_act_delay_1 = 0; //flag for after ON the AC - H-bridge ADC value stabilization (Actuator resistance fault)
bool bd_act_delay_1 = 0;  //flag for after ON the BD - H-bridge ADC value stabilization (Actuator resistance fault)
volatile uint16_t ac_adc_count = 0; //AC  H-bridge ADC value count (Gnd fault)
volatile uint16_t bd_adc_count = 0; //BD  H-bridge ADC value count (Gnd fault)
volatile uint16_t ac_adc_count_1 = 0; //AC - H-bridge ADC value count (Actuator resistance fault)
volatile uint16_t bd_adc_count_1 = 0; //BD - H-bridge ADC value count (Actuator resistance fault)
volatile  uint32_t health_mon[10] = {0}; // to store health monitor accumulate values
uint32_t h_mon_prev_tick = 0; //timer count
bool actuator_rdy = 0;      //flag to indicate actuator ready to check ADC values
static uint32_t delay_prev_tick_sl_b = 0u;
static uint32_t delay_Cap_Bank = 0u;
uint32_t sec_pwr_prev_tick_g_u32 = 0u;
uint32_t pri_pwr_prev_tick_g_u32 = 0u;
uint32_t sec_pwr_cutoff_tick_g_u32 = 0u;
uint32_t	Raw_value_HD1 =0;
uint32_t	Raw_value_HD2 =0;
uint32_t sec_rawvolt;
volatile uint32_t  Mux_A_value[3];
volatile uint32_t  Mux_B_value[3];
uint32_t ACT_HMON[10];


 pdc_packet_t g_pdc_rx_packet;
 Pdc *PDC_ptr_to_AFEC;
 struct afec_config afec_cfg;
 struct afec_ch_config afec_ch_cfg;
static void afec0_diff_data_ready(void);
static void afec1_channel_2_ready(void);
static void afec1_channel_3_ready(void);



 void afec0_diff_data_ready()
{
			uint8_t i;
				if ((afec_get_interrupt_status(AFEC0) & AFEC_ISR_ENDRX) == AFEC_ISR_ENDRX) {
					for (i = 0; i <NUM_CHANNELS; i++) {

						if(i%2==0)
						{
						us_value[i]&= AFEC_LCDR_LDATA_Msk;
						}
					}

					 	pdc_rx_init(PDC_ptr_to_AFEC, &g_pdc_rx_packet, 0);

				}
				else
				{
				//printf("fail\r\n");
				}
 }








void afec1_channel_2_ready()
{
	Mux_A_value[0] = afec_channel_get_value(AFEC1, AFEC_CHANNEL_2);
//	printf("channel_2 Mux_A_value %d\r\n",Mux_A_value[0]);

}

void afec1_channel_3_ready()
{
	Mux_B_value[0] = afec_channel_get_value(AFEC1, AFEC_CHANNEL_3);
//	printf("channel_2 Mux_B_value %d\r\n",Mux_B_value[0]);
}

  void adc_init(void)
 {


	afec_enable(AFEC0);
	afec_enable(AFEC1);


	afec_get_config_defaults(&afec_cfg,Freq_selec_bit);
	afec_init(AFEC0, &afec_cfg);
    afec_init(AFEC1, &afec_cfg);

	afec_ch_get_config_defaults(&afec_ch_cfg);
	afec_ch_set_config(AFEC0, AFEC_CHANNEL_0, &afec_ch_cfg);
 	afec_ch_set_config(AFEC0, AFEC_CHANNEL_1, &afec_ch_cfg);
 	afec_ch_set_config(AFEC0, AFEC_CHANNEL_2, &afec_ch_cfg);
 	afec_ch_set_config(AFEC0, AFEC_CHANNEL_3, &afec_ch_cfg);
 	afec_ch_set_config(AFEC0, AFEC_CHANNEL_4, &afec_ch_cfg);
 	afec_ch_set_config(AFEC0, AFEC_CHANNEL_5, &afec_ch_cfg);

 	afec_ch_set_config(AFEC1, AFEC_CHANNEL_2, &afec_ch_cfg);
 	afec_ch_set_config(AFEC1, AFEC_CHANNEL_3, &afec_ch_cfg);


	/*
	 * Because the internal ADC offset is 0x800, it should cancel it and shift
	 * down to 0.
	 */
 afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_0, 0x800);
 afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_1, 0x800);
 afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_2, 0x800);
 afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_3, 0x800);
 afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_4, 0x800);
 afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_5, 0x800);

 afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_2, 0x800);
 afec_channel_set_analog_offset(AFEC1, AFEC_CHANNEL_3, 0x800);


	afec_set_trigger(AFEC0, AFEC_TRIG_SW);
	afec_set_trigger(AFEC1, AFEC_TRIG_SW);

	/* Enable channel for potentiometer. */
  	afec_channel_enable(AFEC0, AFEC_CHANNEL_0);
 	afec_channel_enable(AFEC0, AFEC_CHANNEL_1);
 	afec_channel_enable(AFEC0, AFEC_CHANNEL_2);
 	afec_channel_enable(AFEC0, AFEC_CHANNEL_3);
 	afec_channel_enable(AFEC0, AFEC_CHANNEL_4);
 	afec_channel_enable(AFEC0, AFEC_CHANNEL_5);

	afec_channel_enable(AFEC1, AFEC_CHANNEL_2);
	afec_channel_enable(AFEC1, AFEC_CHANNEL_3);

PDC_ptr_to_AFEC=afec_get_pdc_base(AFEC0);


g_pdc_rx_packet.ul_addr = (uint32_t) (&us_value);		//g_pdc_rx_1_packet	raw_adc_values
g_pdc_rx_packet.ul_size = BUFFER_SIZE;



pdc_rx_init(PDC_ptr_to_AFEC, &g_pdc_rx_packet,0);
pdc_enable_transfer(PDC_ptr_to_AFEC, PERIPH_PTCR_RXTEN);



afec_set_callback(AFEC0, AFEC_INTERRUPT_END_RXBUF, afec0_diff_data_ready, 0);		// AFEC_INTERUPT_END_RXBUF


afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_2, afec1_channel_2_ready, 1);
afec_set_callback(AFEC1, AFEC_INTERRUPT_EOC_3, afec1_channel_3_ready, 1);
afec_start_software_conversion(AFEC0);
afec_start_software_conversion(AFEC1);
 }

 /* Overview : Low pass filter for heat detector 2
 * Parameter : ADC value
 * return    : Low pass filter ADC value
 * Note      : after controller reset if no of samples > 65 after that only filter array values updated
 */
uint32_t LPF_HD2(uint32_t x)
{
	   static uint32_t samples[64];
	   static uint8_t i = 0;
	   static uint32_t total = 0;

	   total -= samples[i] -  x;
	   samples[i] = x;
	   i = (i==63? 0 : i+1);

   return total/64;
}

 /*-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/
/* Overview : Low pass filter for heat detector 1
 * Parameter : ADC value
 * return    : Low pass filter ADC value
 * Note      : after controller reset if no of samples > 65 after that only filter array values updated
 */
uint32_t LPF_HD1(uint32_t x)
{
	   static uint32_t samples[64];
	   static uint8_t i = 0;
	   static uint32_t total = 0;

	   total -= samples[i] -  x;
	   samples[i] = x;
	   i = (i==63? 0 : i+1);

      return total/64;
}

 /*-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/
/* Overview : Low pass filter for secondary voltage read
 * Parameter : ADC value
 * return    : Low pass filter ADC value
 * Note      : after controller reset if no of samples > 65 after that only filter array values updated
 */
uint32_t LPF_Secondary_Voltage(uint32_t x)
{
	   static uint32_t samples[64];
	   static uint8_t i = 0;
	   static uint32_t total = 0;

	   total -= samples[i] -  x;
	   samples[i] = x;
	   i = (i==63? 0 : i+1);

      return total/64;
}
 /* Overview : Low pass filter for Primary voltage read
 * Parameter : ADC value
 * return    : Low pass filter ADC value
 * Note      : after controller reset if no of samples > 65 after that only filter array values updated
 */
uint32_t LPF_Primary_Voltage(uint32_t x)
{
	   static uint32_t samples[64];
	   static uint8_t i = 0;
	   static uint32_t total = 0;

	   total -= samples[i] -  x;
	   samples[i] = x;
	   i = (i==63? 0 : i+1);

       return total/64;
}
/* Super cap ADC to Voltage conversion */

 uint32_t LPF_Super_cap_Voltage(uint32_t x)
 {
	 static uint32_t samples[64];
	 static uint8_t i = 0;
	 static uint32_t total = 0;

	 total -= samples[i] -  x;
	 samples[i] = x;
	 i = (i==63? 0 : i+1);

	 return total/64;
 }



uint32_t HD2_volt(void)
{
	uint32_t tmp_HD2 = 0;
	tmp_HD2 = (((us_value[2] * vref) / 4095)) * 1000; //Heat detector zone 1 ADC in rawValue1[1]
	Raw_value_HD2  = tmp_HD2;
	tmp_HD2 = LPF_HD2(tmp_HD2);
	return tmp_HD2;
}

/* Heat detector zone 1 ADC to Voltage conversion */

uint32_t HD1_volt(void)
{
	uint32_t tmp_HD1 = 0;
	tmp_HD1 = (((us_value[0] * vref) / 4095)) * 1000;
	Raw_value_HD1 = tmp_HD1;
	tmp_HD1 = LPF_HD1(tmp_HD1);

	return tmp_HD1;
}


 /* Overview  : load the ADC value in low pass filter buffer, because after reset buffer values are zero
 * Parameter :  none
 * return    : none
  */
/*-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-*/
/* Secondary voltage read ADC to Voltage conversion */

uint32_t SEC_volt(void)
{

    	uint32_t Sec_raw = (((us_value[4] * vref) / 4095)) * 2511; //Secondary voltage read ADC in rawValue1[3]  2000
		sec_rawvolt =Sec_raw;
	return LPF_Secondary_Voltage(Sec_raw);
}
/* Primary voltage read ADC to Voltage conversion */

uint32_t PRI_volt(void)
{
	uint32_t  Pri_raw = (((us_value[10] * vref) / 4095)) *14695;//* 14325 //Primary voltage read ADC in rawValue1[2]  16830
	return LPF_Primary_Voltage(Pri_raw);
}





/* Actuator health monitor read ADC to Voltage conversion */

uint32_t ACT_Mux_B_volt(uint32_t hm_gf_value)
{
	return (((Mux_B_value[0] * vref) / 4095)) * 1000; //Actuator health monitor read ADC in rawValue2[0]
}

uint32_t ACT_Mux_A_volt(uint32_t hm_gf_value)
{
	return (((Mux_A_value[0] * vref) / 4095)) * 1000; //Actuator health monitor read ADC in rawValue2[0]
}

uint32_t SUPER_CAP_Volt(void)
{
	uint32_t Super_cap_raw = (((us_value[6] * vref) / 4095)) * 5334; //SuperCap voltage read ADC in rawValue1[6]  2000
	return LPF_Super_cap_Voltage(Super_cap_raw);

}



void LPF_buffer_load(void)
{
	   uint8_t adc_count = 0;
	   for(adc_count = 0;adc_count<130;adc_count++) //load ADC values into the low pass filter array buffer
	   {
		   	wdt_restart(WDT);
			 primary_pwr_vol_g_u16 =PRI_volt();
			 sec_pwr_vol_g_u16= SEC_volt();
			 hd1_measured_g_u16=HD1_volt();
			//  Check_G_Fault();
			 hd2_measured_g_u16=HD2_volt();
			//  Manual_Act_Sw();
			super_capVolt_g_u16= SUPER_CAP_Volt();
			  Delay(2u);
	   }
}


/* Function  : Actuator health monitor
* Parameter : None
* return    : None
*/
void Actuator_HM(void)
{
     static bool one_cycle_complete_sl_b = 0u;
	 static uint8_t actuator_prev_state_sl_u8 = 0u;
 if(!fire_happen_g_b)
 {

						if((count_1ms - delay_prev_tick_sl_b) >= 250u)
						{
									ioport_set_pin_level(ACTA_P,1);
									ioport_set_pin_level(ACTB_P,1);
									ioport_set_pin_level(ACTA_N,0);
									ioport_set_pin_level(ACTB_N,0);
						delay_prev_tick_sl_b = count_1ms;
						read_count++;
						}

						switch(read_count)
						{
						case 1u:

									if(!ac_act_delay)
									{
									ac_act_delay = 1u;
									delay_prev_tick_sl_b = count_1ms;
									ioport_set_pin_level(ACTB_P,1);
									ioport_set_pin_level(ACTB_N,1);
									ioport_set_pin_level(ACTA_P,0);
									ioport_set_pin_level(ACTA_N,0);
									}
									else if((count_1ms - delay_prev_tick_sl_b) >=20u) /* after turn ON the FET take sample after 20 ms*/
									{
										ac_adc_count++;
										health_mon[0] += ACT_Mux_B_volt(Mux_B_value[0]);

									}
						break;

					    case 2u:
									if(!bd_act_delay)
									{
										bd_act_delay = 1u;
										delay_prev_tick_sl_b = count_1ms;
								    ioport_set_pin_level(ACTB_P,0);
									ioport_set_pin_level(ACTB_N,0);
									ioport_set_pin_level(ACTA_P,1);
									ioport_set_pin_level(ACTA_N,1);

									}
									else if((count_1ms - delay_prev_tick_sl_b) > 20u) /* after turn ON the FET take sample after 20 ms*/
									{
										bd_adc_count++;
										health_mon[1] += ACT_Mux_A_volt(Mux_A_value[0]);
									}
                      break;

					  case 3u:

									if(!ac_act_delay_1)
									{
									ac_act_delay_1 = 1u;
									delay_prev_tick_sl_b = count_1ms;
									ioport_set_pin_level(ACTB_P,1);
									ioport_set_pin_level(ACTB_N,0);
									ioport_set_pin_level(ACTA_P,0);
									ioport_set_pin_level(ACTA_N,0);
									}
									else if((count_1ms - delay_prev_tick_sl_b) > 20u) /* after turn ON the FET take sample after 20 ms*/
									{
										 ac_adc_count_1++;
										 health_mon[2] += ACT_Mux_B_volt(Mux_B_value[0]);
									}
					  break;
					  case 4u:

									if(!bd_act_delay_1)
									{
									bd_act_delay_1 = 1u;
									delay_prev_tick_sl_b = count_1ms;
									ioport_set_pin_level(ACTB_P,0);
									ioport_set_pin_level(ACTB_N,0);
									ioport_set_pin_level(ACTA_P,1);
									ioport_set_pin_level(ACTA_N,0);
									}
									else if((count_1ms - delay_prev_tick_sl_b) > 20u) /* after turn ON the FET take sample after 20 ms*/
									{
									bd_adc_count_1++;
									health_mon[3] += ACT_Mux_A_volt(Mux_A_value[0]);
									}

					  break;



					  default:
					          read_count=1;
//
 							  hmon_1A_2B_val_g_u16 =  (health_mon[0] / ((uint32_t)ac_adc_count));
 							 // printf("ACT Direction B %u\r\n",hmon_1A_2B_val_g_u16);
 							  hmon_1B_2A_val_g_u16 =  (health_mon[1] / ((uint32_t)bd_adc_count));
 							 // printf("ACT Direction A %u\r\n",hmon_1B_2A_val_g_u16);
 							  hmon_1A_gnd_flt_val_g_u16 =  (health_mon[2] / ((uint32_t)ac_adc_count_1));
 							 // printf("Ground Fault B %u\r\n",hmon_1A_gnd_flt_val_g_u16);
 							  hmon_2A_gnd_flt_val_g_u16 =  (health_mon[3] / ((uint32_t)bd_adc_count_1));
 							//  printf("Ground Fault A %u\r\n",hmon_2A_gnd_flt_val_g_u16);


							  bd_adc_count_1=0;
							 ac_adc_count_1=0;
							 bd_adc_count=0;
							 ac_adc_count=0;
							ac_act_delay = 0;
							bd_act_delay = 0;
							ac_act_delay_1 = 0;
							bd_act_delay_1 = 0;
							one_cycle_complete_sl_b=1;
								memset((void *)health_mon,0,sizeof(health_mon));
							//	memset(ACT_HMON,0,sizeof(ACT_HMON));
					break;

			}

				if(!one_cycle_complete_sl_b)
				{
				return; /* if system restarted and should not show the trouble up to one cycle completion of health monitor
				Note : if we return the function here itself below condition never executed*/
				}

				    if((hmon_1A_2B_val_g_u16  < HMON_OPEN_FLT_RANGE)   &&  (hmon_1B_2A_val_g_u16 < HMON_OPEN_FLT_RANGE)
				    && (hmon_1A_gnd_flt_val_g_u16 > HMON_GND_FLT_RANGE) && (hmon_2A_gnd_flt_val_g_u16 > HMON_GND_FLT_RANGE))
				    {
					    actuator_trbl_g_u8 = 0u;
				    }
				    else if((hmon_1A_2B_val_g_u16  >= HMON_OPEN_FLT_RANGE)   ||  (hmon_1B_2A_val_g_u16 >= HMON_OPEN_FLT_RANGE))
				    {
					    actuator_trbl_g_u8 = 1u; /*1 indicating open fault trouble*/
				    }
				    else
				    {
					    actuator_trbl_g_u8 = 2u; /*2 indicating ground fault trouble*/
				    }

				    if(actuator_prev_state_sl_u8 != actuator_trbl_g_u8)
				    {
					    //LOG
					    actuator_prev_state_sl_u8 = actuator_trbl_g_u8; /*Update the value*/

					    /* Log the actuator trouble start/stop event */

					    Reset_Values();

					    log_data.log_code = 4u; /*Indicating actuator trouble code*/

					    if(!actuator_trbl_g_u8)
					    {
						    log_data.start_stop = 0u; /*indicating actuator trouble cleared*/
					    }
					    else
					    {
						    log_data.start_stop = 1u; /*indicating actuator trouble occurred*/
					    }
					    log_data.trouble_fire = actuator_trbl_g_u8;

					    NOR_Flash_Log_Data(&log_data);
						Get_New_trouble = ACT_TRBL_4_FLASH;

					}
				 }
}

/* Function  : Check Primary and secondary Power trouble
 * Parameter : None
 * return    : None
 */
void Check_Power_Trbl(void)
{
static bool pri_pwr_prev_state_sl_b = 0u;
static bool sec_pwr_prev_state_sl_b = 0u;

    /***********************For Secondary power check*************************/

    if(sec_pwr_vol_g_u16 > SEC_PWR_MIN_VOL && sec_rawvolt >SEC_PWR_MIN_VOL)
    {
        sec_pwr_prev_tick_g_u32 = count_1ms;
		sec_pwr_cutoff_tick_g_u32 = count_1ms;
        sec_pwr_trouble_g_b = NO_TROUBLE;
    }
	else if( sec_rawvolt < SEC_PWR_CUTOFF_VOL && sec_pwr_vol_g_u16 < SEC_PWR_CUTOFF_VOL && pri_pwr_trouble_g_b == TROUBLE) //sec_pwr_trouble_g_b == TROUBLE &&
	{
		if((count_1ms - sec_pwr_cutoff_tick_g_u32) >= 2000u)	//SEC_2
		{
			/* Log the secondary power event*/
			//LOG
			Reset_Values();
			log_data.log_code = 16u; /*Indicating secondary power log code*/
			//log_data.start_stop = 3; /*log start and stop event*/
			log_data.value = sec_pwr_vol_g_u16 / 50u;
			NOR_Flash_Log_Data(&log_data);
			printf("software Windup\r\n");
			windup_Music();
			freq_test_backup_mode();
		}

	}

    else
    {
        if((count_1ms - sec_pwr_prev_tick_g_u32) > 50000u)//__700ms /*check for 500ms and decide */
        {
            sec_pwr_trouble_g_b = TROUBLE;
			sec_pwr_cutoff_tick_g_u32 = count_1ms;
			sec_pwr_prev_tick_g_u32 = count_1ms;
			Get_New_trouble = SEC_PWR_6_FLASH;
        }
    }

    if(sec_pwr_prev_state_sl_b != sec_pwr_trouble_g_b)
    {
        /* Log the secondary power event*/
        //LOG
        Reset_Values();
        log_data.log_code = 6u; /*Indicating secondary power log code*/
        log_data.start_stop = sec_pwr_trouble_g_b; /*log start and stop event*/
        log_data.value = sec_pwr_vol_g_u16 / 50u;
        NOR_Flash_Log_Data(&log_data);
        sec_pwr_prev_state_sl_b = sec_pwr_trouble_g_b;
		//printf("sec_pwr_prev_state_sl_b %d",sec_pwr_prev_state_sl_b);
    }

/***********************For primary power check*************************/

    if(primary_pwr_vol_g_u16 >  PRI_PWR_MIN_VOL)
    {
        pri_pwr_prev_tick_g_u32 = count_1ms;
        pri_pwr_trouble_g_b = NO_TROUBLE;
    }
    else
    {
        if((count_1ms - pri_pwr_prev_tick_g_u32) > __700ms) /*check for 500ms and decide */
        {
		    pri_pwr_prev_tick_g_u32 = count_1ms;
            pri_pwr_trouble_g_b = TROUBLE;
			Get_New_trouble = PRI_PWR_5_FLASH;
        }
    }



    if(pri_pwr_prev_state_sl_b != pri_pwr_trouble_g_b)
    {
        /* Log the secondary power event*/
        //LOG
        Reset_Values();
        log_data.log_code = 5u; /*Indicating primary power log code*/
        log_data.start_stop = pri_pwr_trouble_g_b; /*log start and stop event*/
        log_data.value = primary_pwr_vol_g_u16 / 50u;
        NOR_Flash_Log_Data(&log_data);
        pri_pwr_prev_state_sl_b = pri_pwr_trouble_g_b;
		ioport_set_pin_level(CAN_ACT,0);
		//	printf("pri_pwr_prev_state_sl_b %d",pri_pwr_prev_state_sl_b);

    }

}

void Super_CAP_Charge_enable_disable(void)
{
	static  uint8_t SCAP_ReadCnt=30;
	static bool Super_CAP_Charg_prev_state = 0u;

	if((count_1ms - delay_Cap_Bank) > 1000u) /*check for 500ms and decide */	//__500ms
	{

		 if(super_capVolt_g_u16 > supercap_highcuttoff)
		 {
			 if((SCAP_ReadCnt--)<=1u)
			 {
				 Capacitor_Bank_Charge_disable(&Activate_PIN);
				 SCAP_ReadCnt =180u;
				 super_cap_state_g_b =0;
			 }

		 }
		 else if(super_capVolt_g_u16 < supercap_lowcuttoff)
		 {
			  if((SCAP_ReadCnt--)<=1u)
			 {
				 Capacitor_Bank_Charge_enable(&Activate_PIN);
 				 SCAP_ReadCnt =180u;
				   super_cap_state_g_b =1u;
			 }

		 }
		 else
		 {
			 SCAP_ReadCnt =180;
		 }

		     if(super_cap_state_g_b != Super_CAP_Charg_prev_state)
		     {
			     /* Log the SuperCAP  event*/
			     //LOG
			     Reset_Values();
			     log_data.log_code = 14u; /*Indicating SuperCap log code*/
			     log_data.start_stop = super_cap_state_g_b; /*log start and stop event*/
			     log_data.value = super_capVolt_g_u16 / 10u;
				 log_data.value /=50u;
			     NOR_Flash_Log_Data(&log_data);
			     Super_CAP_Charg_prev_state = super_cap_state_g_b;
		     }

		delay_Cap_Bank = count_1ms;
	}

}