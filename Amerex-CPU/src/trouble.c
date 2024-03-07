/*
 * trouble.c
 *
 * Created: 15-07-2022 23:50:38
 *  Author: p.dharmaraj
 */
#include "main_header.h"
bool any_of_trouble_g_b = 0u;
bool fire_iso_out_g_b = 0u;
bool trbl_iso_out_g_b = 0u;

/* Function : Check any trouble exist in the panel
 * Parameter: None
 * Return   : None
 */

void Check_any_Trbl(void)
{
    if(pri_pwr_trouble_g_b || sec_pwr_trouble_g_b || hd1_trouble_g_u8 || hd2_trouble_g_u8 || actuator_trbl_g_u8 || manual_sw_trbl_g_b || pres_sw_trbl_g_b || fire_happen_g_b)
    {


         any_of_trouble_g_b = TROUBLE;
    }
    else
    {
        any_of_trouble_g_b = NO_TROUBLE;
		Get_New_trouble  = NO_TROUBLE;
    }
}
/* Function : Reset the trouble isolated output
 * Parameter: None
 * Return   : None
 */

void Trbl_Fire_Iso_Output_Reset(void)
{
static uint32_t trouble_prev_tick_ls_u32 = 0u;

     if(trbl_iso_out_g_b)
    {

       if((!any_of_trouble_g_b) && !ioport_get_pin_level(SYS_RESET_SW_PIN))
       {
          if((count_1ms -  trouble_prev_tick_ls_u32) >= SEC_2)
              {
                Buzzer_Beep(ON);
                trbl_iso_out_g_b = NO_TROUBLE;
                fire_iso_out_g_b = NO_TROUBLE;
               // Port_Expander_Write(RELAY_A | RELAY_B,OFF);
				ioport_set_pin_level(RELAY_A,amerex_config_data.Default_fire_rly_level);
				ioport_set_pin_level(RELAY_B,amerex_config_data.Default_trbl_rly_level);
                Delay(25);
                Buzzer_Beep(OFF);
              }
       }
       else
       {
           trouble_prev_tick_ls_u32 = count_1ms;
       }
    }
}

/* Function : Turn ON the fire isolated output after user configured delay
 * Parameter: None
 * Return   : None
 */
void Fire_Iso_Output(void)
{
static uint32_t fire_iso_prev_tick_ls_u32 = 0u;
        uint32_t Fire_Rly=0;

    if(fire_happen_g_b && Fire_Rst_vehicle_start_bit!=2 )
    {

	    (fire_rly_en_vehicle_start_bit == 0) ? (Fire_Rly =  amerex_config_data.fire_relay_delay) : (Fire_Rly =  5000u);
        if((count_1ms - fire_iso_prev_tick_ls_u32) >= Fire_Rly)
        {
			fire_rly_en_vehicle_start_bit = 0;
            fire_iso_prev_tick_ls_u32 =  count_1ms;
            /*Port_Expander_Write(RELAY_B,ON);*/
			ioport_set_pin_level(RELAY_B,!amerex_config_data.Default_fire_rly_level); //ioport_set_pin_level(RELAY_B,1); // amerex_config_data.Default_rly_level
            fire_iso_out_g_b = ON;
        }
    }
    else
    {
        fire_iso_prev_tick_ls_u32 =  count_1ms;
    }
}








/* Function : Turn ON the trouble isolated output after user configured delay
 * Parameter: None
 * Return   : None
 */
void Trouble_Iso_Output(void)
{
static uint32_t trbl_iso_prev_tick_ls_u32 = 0u;

    if(any_of_trouble_g_b)
    {
        if((count_1ms - trbl_iso_prev_tick_ls_u32) >= amerex_config_data.trbl_relay_delay)
        {
            trbl_iso_prev_tick_ls_u32 =  count_1ms;
           /* Port_Expander_Write(RELAY_A,ON);*/
		   	ioport_set_pin_level(RELAY_A,!amerex_config_data.Default_trbl_rly_level);
            trbl_iso_out_g_b = ON;
        }
    }
    else
    {
        trbl_iso_prev_tick_ls_u32 =  count_1ms;
    }
}