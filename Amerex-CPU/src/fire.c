/*
 * fire.c
 *
 * Created: 14-07-2022 11:23:07
 *  Author: p.dharmaraj
 */
#include "main_header.h"

uint32_t alarm_sil_prev_tick_g_u32 = 0u;
uint32_t alarm_retry_sil_prev_tick_g_u32 = 0u;
uint32_t fire_retry_sound_prev_tick_g_u32 =0u;
bool pulse_flag_g_b = 0u;
//bool timer_30min_sil_g_b = 0;
 uint32_t hd1_prev_tick_g_u32 = 0u;
 uint32_t hd2_prev_tick_g_u32 = 0u;



/* Function : Actuate the actuator when fire occur
 * Parameter: None
 * Return   : None
 */
void Actuator_Pulse(void)
{
    if(fire_happen_g_b == 1u)
    {
        ioport_set_pin_level(ACTA_P,1);
        ioport_set_pin_level(ACTB_P,1);
        ioport_set_pin_level(ACTA_N,0);
        ioport_set_pin_level(ACTB_N,0);
        if((fire_happen_safety_g_b) && (!pulse_flag_g_b) && fire_happen_g_b)
        {
            pulse_flag_g_b = 1u;
            Delay(5);
			ioport_set_pin_level(ACT_FIRE_ON,1);
            Delay(50);
            ioport_set_pin_level(ACT_FIRE_ON,0);
            Delay(500);
            ioport_set_pin_level(ACT_FIRE_ON,1);
            Delay(50);
            ioport_set_pin_level(ACT_FIRE_ON,0);
            Delay(50);

            /* Log the fire event*/
            //LOG
                Reset_Values();

                log_data.log_code = 8u; /*8 to indicate the fire happened in system*/

                log_data.start_stop = 1u;

                log_data.trouble_fire = (manual_sw_trbl_g_b << 2) | (hd2_trouble_g_u8 ==1u?2u:0) | (hd1_trouble_g_u8 ==1u?1:0) ;  /*assign the fire event
                due to manual actuation or HD1 or HD2*/

                NOR_Flash_Log_Data(&log_data);
        }

    }
}

void Check_Heat_Detector_Trbl(void)
	{

	static  uint8_t hd1_prev_state_sl_u8 = 0u;
	static  uint8_t hd2_prev_state_sl_u8 = 0u;

	 /**********************For Heat Detector 1*****************************/

	if((hd1_measured_g_u16 >= HD_OK_MIN && hd1_measured_g_u16 < HD_OK_MAX) && (Raw_value_HD1 >= HD_OK_MIN && Raw_value_HD1 < HD_OK_MAX))
	{
		hd1_trouble_g_u8 = NO_TROUBLE;  /* No trouble in zone*/
		hd1_prev_tick_g_u32 = count_1ms ;

	}

	else if(hd1_measured_g_u16 <= HD_FIRE_MIN && Raw_value_HD1 <= HD_FIRE_MIN)
	{
			if(fire_happen_g_b && fire_happen_safety_g_b)
			{
				hd1_trouble_g_u8 = 1u; /* Fire Trouble*/

			}

			else if((count_1ms - hd1_prev_tick_g_u32) > __500ms) // &&  pri_pwr_trouble_g_b == false
			{
				hd1_prev_tick_g_u32 = count_1ms ;
				fire_happen_g_b =  FIRE;
				fire_happen_safety_g_b = FIRE;
				hd1_trouble_g_u8 = 1u; /* Fire Trouble*/
			}


	}
	else if(hd1_measured_g_u16 >= OPEN_TROUBLE && Raw_value_HD1 >= OPEN_TROUBLE)
	{
		if((count_1ms - hd1_prev_tick_g_u32) > __250ms)
		{
			hd1_prev_tick_g_u32 = count_1ms ;

			hd1_trouble_g_u8 = 2u;  /*Open trouble in zone*/
		}
	}
	else if( (hd1_measured_g_u16 >= HD_SHORT_MIN) && (hd1_measured_g_u16 < HD_SHORT_MAX)  &&  ((Raw_value_HD1 >= HD_SHORT_MIN) && (Raw_value_HD1 < HD_SHORT_MAX))) //
	{
		if((count_1ms - hd1_prev_tick_g_u32) > __250ms)
		{
		   hd1_prev_tick_g_u32 = count_1ms ;
			hd1_trouble_g_u8 = 3u;  /* Some resistance short  trouble in zone*/
		}
	}
	else if( hd1_measured_g_u16 > HD_FIRE_MIN &&  Raw_value_HD1 > HD_FIRE_MIN)
	{
		if((count_1ms - hd1_prev_tick_g_u32) > __250ms )
		{
			hd1_prev_tick_g_u32 = count_1ms ;

			hd1_trouble_g_u8 = 4u;
		}
	}







	if(hd1_prev_state_sl_u8 != hd1_trouble_g_u8)/*if any trouble happened log the data*/
	{
		//LOG
	Reset_Values();

		log_data.log_code = HD1_1_FLASH;

		if(!hd1_trouble_g_u8) /*if trouble cleared log the data*/
		{
			log_data.start_stop = 0u;
		}
		else
		{
			log_data.start_stop = 1u;
		}

		log_data.trouble_fire = hd1_trouble_g_u8 ; /*log the Heat detector trouble*/

		log_data.value = hd1_measured_g_u16 / 50u; /*divide the value  by 50 to accommodate the value in 1 byte*/

		NOR_Flash_Log_Data(&log_data);

		hd1_prev_state_sl_u8 = hd1_trouble_g_u8;

		 Get_New_trouble=HD1_1_FLASH;
	}



	/*********************For Heat Detector 2*****************************/

	if((hd2_measured_g_u16 >= HD_OK_MIN && hd2_measured_g_u16 < HD_OK_MAX) && (Raw_value_HD2 >= HD_OK_MIN && Raw_value_HD2 < HD_OK_MAX))
	{
		hd2_trouble_g_u8 = NO_TROUBLE;  /* No trouble in zone*/
		hd2_prev_tick_g_u32 = count_1ms;
		//Get_New_trouble =0;
	}

	else if(hd2_measured_g_u16 <= HD_FIRE_MIN && Raw_value_HD2 <= HD_FIRE_MIN)
	{

					if(fire_happen_g_b && fire_happen_safety_g_b)
					{
						hd2_trouble_g_u8 = 1u; /* Fire Trouble*/
						//break;    /*if already fire happened break the loop*/
					}

					else if((count_1ms - hd2_prev_tick_g_u32) > __500ms)
					{
						hd2_prev_tick_g_u32 = count_1ms ;
						fire_happen_g_b =  FIRE;
						fire_happen_safety_g_b = FIRE;
						hd2_trouble_g_u8 = 1u; /* Fire Trouble*/
					}

	}
	else if(hd2_measured_g_u16 >= OPEN_TROUBLE && Raw_value_HD2 >= OPEN_TROUBLE)
	{
		if((count_1ms - hd2_prev_tick_g_u32) > __250ms)
		{
				hd2_prev_tick_g_u32 = count_1ms;
			hd2_trouble_g_u8 = 2u;  /*Open trouble in zone*/
		}
	}
	else if( (hd2_measured_g_u16 >= HD_SHORT_MIN) && (hd2_measured_g_u16 < HD_SHORT_MAX) &&  ((Raw_value_HD2 >= HD_SHORT_MIN) && (Raw_value_HD2 < HD_SHORT_MAX)))
	{
		if((count_1ms - hd2_prev_tick_g_u32) > __250ms)
		{
				hd2_prev_tick_g_u32 = count_1ms;
			hd2_trouble_g_u8 = 3u;  /* Some resistance short  trouble in zone*/
		}
	}
	else if(hd2_measured_g_u16 > HD_FIRE_MIN &&  Raw_value_HD2 > HD_FIRE_MIN)
	{
		if((count_1ms - hd2_prev_tick_g_u32) > __250ms) //
		{
				hd2_prev_tick_g_u32 = count_1ms;
			hd2_trouble_g_u8 = 4u;
		}
	}

	if(hd2_prev_state_sl_u8 != hd2_trouble_g_u8)/*if any trouble happened log the data*/
	{
		//LOG
		Reset_Values();

		log_data.log_code = HD2_2_FLASH;

		if(!hd2_trouble_g_u8) /*if trouble cleared log the data*/
		{
			log_data.start_stop = 0u;
		}
		else
		{
			log_data.start_stop = 1u;
		}

		log_data.trouble_fire = hd2_trouble_g_u8 ; /*log the Heat detector trouble*/

		log_data.value = hd2_measured_g_u16 / 50u; /*divide the value  by 50 to accommodate the value in 1 byte*/

		NOR_Flash_Log_Data(&log_data);

		hd2_prev_state_sl_u8 = hd2_trouble_g_u8;

		  Get_New_trouble=HD2_2_FLASH;
	}




}

/* Function : Silence alarm by timer
 * Parameter: None
 * Return   : None
 */


void Fire_sound_retry_by_Timer(void)
{

	if(fire_happen_g_b && Fire_Rst_vehicle_start_bit == 2u && test_sw_pressed_g_b == 0)
	{
		if((count_1ms - fire_retry_sound_prev_tick_g_u32) >= 10000u)	 // SILENCE_TIME_30min
		{

			Fire_Rst_vehicle_start_bit = 0u;
			fire_rly_en_vehicle_start_bit =1u;

		}
	}
	else
	{
		fire_retry_sound_prev_tick_g_u32 = count_1ms;
	}
}