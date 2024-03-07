/*
 * configuration.c
 *
 * Created: 12-07-2022 16:38:28
 *  Author: p.dharmaraj
 */

#include "main_header.h"
#include <math.h>
#include <limits.h>



 flash log_data = {.epoch_time = 0x01,.start_stop = 2,.log_code  = 3,.trouble_fire = 4,.value = 7};
uint32_t auto_exit_prev_tick_g_u32 = 0u;
bool blink_g_b = 1u;    /*Flag to determine the LED whether LED blink or not*/
uint16_t flash_write_status = 0;
 volatile uint8_t value = 0;
 uint32_t all_output_test_g_32 =0;
 uint32_t buzzer_fire_output_test_g_32 =0;
  uint32_t fire_output_test_g_32 =0;
	  volatile uint8_t ALEDS_bit=10;
	  volatile uint8_t ALEDS_bit_sec=0;
	 volatile  uint8_t buzz_bit=0;
	   volatile	uint8_t fire_bit=0;

/* Function : Enter into Configuration mode
 * Parameter: None
 * Return   : None
 */

void Configuration(void)
{





    if((count_1ms - test_sw_prev_tick_g_u32) >= SEC_5) //  15000u
    {
		FIRE_LED_PWM_EN(amerex_config_data.fire_LED_duty);
        Buzzer_Beep(OFF);
        uint32_t blink_prev_tick= count_1ms;             /*To store the Previous tick*/
        uint32_t test_exit_prev_tick = count_1ms;    /*To store the Previous tick*/
        uint32_t test_state_prev_tick= count_1ms;    /*To store the Previous tick For move to next state*/
		//uint32_t rgb_state_prev_tick;
        uint32_t config_state = 0;       /* To identify the configuration*/
        uint32_t button_status = 0u;      /* To check the inc/dec button pressed status*/
        uint32_t test_button_pressed = 0u; /* Set bit indicate button pressed*/
        uint32_t first_release = 0u;
        //my_data tmp_data = config_data; /*take the backup data */
		 //bool flag=1;

	ioport_set_pin_level(Amerex_R,0);
	ioport_set_pin_level(Amerex_G,0);
    ioport_set_pin_level(Amerex_B,0);

		/* Log the Configuration enter event*/
		Reset_Values();
        log_data.log_code = 11u; /*Indicating system reset button pressed*/
        log_data.start_stop = 1u; /*button pressed*/
        NOR_Flash_Log_Data(&log_data);
 		auto_exit_prev_tick_g_u32 = count_1ms;
		blink_g_b = 1u;
		LED_Blink(CONFIG_ALL_OFF,CONFIG_OFF);
		Delay(500u);
		Sound_Alert();
		//if(amerex_config_data.Default_trbl_rly_level)
		//{
		//ioport_set_pin_level(Alarm_silence,ON);
		//}

		while(1)
		{
			    	wdt_restart(WDT);
		              if(((count_1ms - blink_prev_tick) >= __500ms) && blink_g_b)
		              {
			              blink_prev_tick = count_1ms;
			              LED_Blink(config_state,CONF_BLINK);
					  }
					/***********If user inactivity of 20sec it will exit configuration mode automatically***********/
					if((count_1ms - auto_exit_prev_tick_g_u32) > __20_SEC)
					{
					ioport_set_pin_level(Amerex_R,0);
					ioport_set_pin_level(Amerex_G,0);
					ioport_set_pin_level(Amerex_B,0);
					if(amerex_config_data.CAN_baud_rate != 0u)
					can_enable(CAN0);
					Small_Beep(40u);
					break;
					}

					/********** Test button for move to next Configuration and exit the configuration mode***********/

					//if(!ROM_GPIOPinRead(TEST_SW_PORT,TEST_SW_PIN))
					if(!ioport_get_pin_level(TEST_SW_PIN))
					{

						if((count_1ms - test_exit_prev_tick) > 5000u)	   // SEC_5
						{

          				 test_sw_prev_tick_g_u32 = count_1ms;
                              LED_Blink(CONFIG_ALL_OFF,CONFIG_OFF);

								  amerex_config_data.Trigger =1u;
								  uint8_t* ptr = (uint8_t*) &amerex_config_data;
								  flash_write_status = flash_write_amerex_param(ptr);

							if(flash_write_status==1u)
							{

								 rstc_start_software_reset(RSTC);

							}
							Small_Beep(40u);
							break;/*Break the while statement if configuration data write unsuccessful in EEPROM*/
						}
						if(((count_1ms - test_state_prev_tick) >= __100ms) && first_release)
						{
							test_button_pressed = 1u;
						}

					}
					else
					{
						test_exit_prev_tick =  count_1ms;
						test_state_prev_tick = count_1ms;
						//rgb_state_prev_tick=count_1ms;
						first_release = 1u;
						if(test_button_pressed)
						{

							test_button_pressed = 0u;
							auto_exit_prev_tick_g_u32 = count_1ms; /*Update the auto exit count*/
							blink_g_b = 1u;
							LED_Blink(CONFIG_ALL_OFF,CONFIG_OFF);
							Config_Indication_LED(OFF);
							Buzzer_Freq_Set(BUZZER_FREQ_0_5KHZ, DUTY_0_5_KHZ);
							ioport_set_pin_level(Amerex_G,1);
							Delay(200);
							Buzzer_Freq_Set(BUZZER_FREQ_0_5KHZ, 0);
							ioport_set_pin_level(Amerex_G,0);
						    Sys_Reset_LED(OFF);
							config_state++;
							if(config_state ==3u)
							{
								ioport_set_pin_level(Amerex_R,OFF);
								ioport_set_pin_level(SYS_RST_LED,OFF);
								ioport_set_pin_level(Alarm_silence,OFF);
								ioport_set_pin_level(Amerex_G,OFF);
								ioport_set_pin_level(Amerex_B,OFF);						
							}
							if(config_state == 7u)
							{
								config_state = 0u;
								ioport_set_pin_level(Amerex_R,OFF);
							}
						}
					}
					  /******************************************************************************************/

        switch(config_state)
        {
        case CONFIG_ALL_LED_BRIGHT:

                    button_status = Button_Status();

                    if(button_status == INC_BUTTON_PRESSED)
                    {
                        if(amerex_config_data.all_LED_duty < 100u)
                        {
                            amerex_config_data.all_LED_duty += 10u;
                        }
                        else
                        {
                            //Buzzer_Freq_Set(BUZZER_FREQ_3KHZ, DUTY_3_KHZ);
                            Sound_Alert();
                        }
                    }
                    else if(button_status == DEC_BUTTON_PRESSED)
                    {
                        if(amerex_config_data.all_LED_duty > 10u)
                        {
                            amerex_config_data.all_LED_duty -= 10u;
                        }
                        else
                        {
							//Buzzer_Freq_Set(BUZZER_FREQ_3KHZ, DUTY_3_KHZ);
                             Sound_Alert();
                        }
                    }

                    if(button_status)
                    {
                        LED_Blink(config_state,CONFIG_ON);
                       	Amerex_LOGO_PWM_EN(amerex_config_data.all_LED_duty);
                       	LEDS_PWM_EN(amerex_config_data.all_LED_duty);
                    }

      break;

      case CONFIG_FIRE_LED_BRIGHT:

          blink_g_b = 0u;
          button_status = Button_Status();

          if(button_status == INC_BUTTON_PRESSED)
          {
              if(amerex_config_data.fire_LED_duty < 100u)
              {
                 amerex_config_data.fire_LED_duty += 10u;
              }
              else
              {
                   Sound_Alert();
              }
          }
          else if(button_status == DEC_BUTTON_PRESSED)
          {
              if(amerex_config_data.fire_LED_duty > 10u)
              {
                 amerex_config_data.fire_LED_duty -= 10u;
              }
              else
              {
                  Sound_Alert();
              }
          }
          if(button_status)
          {
              Fire_LED_Count();
          }
          Fire_LED(LED_FADING);
      break;
	  
	  case CONFIG_CAN_BUS_BAUD_RATE:
	  	   
	  	   blink_g_b = 0u;
	  	   button_status = Button_Status();
		if(amerex_config_data.CAN_baud_rate == 0u)
		{
			ioport_set_pin_level(Amerex_R,ON);
			ioport_set_pin_level(SYS_RST_LED,OFF);
			ioport_set_pin_level(Alarm_silence,OFF);
			ioport_set_pin_level(Amerex_G,OFF);
			ioport_set_pin_level(Amerex_B,OFF);	
		}
		else if(amerex_config_data.CAN_baud_rate == 1u)
		{
			ioport_set_pin_level(Amerex_R,OFF);
			ioport_set_pin_level(SYS_RST_LED,ON);
			ioport_set_pin_level(Alarm_silence,OFF);
			ioport_set_pin_level(Amerex_G,ON);
			ioport_set_pin_level(Amerex_B,OFF);	
		}
		else if(amerex_config_data.CAN_baud_rate == 2u)
		{
			ioport_set_pin_level(Amerex_R,OFF);
			ioport_set_pin_level(SYS_RST_LED,OFF);
			ioport_set_pin_level(Alarm_silence,ON);
			ioport_set_pin_level(Amerex_G,ON);	
			ioport_set_pin_level(Amerex_B,ON);	
		}
			 

	  	   if(button_status == INC_BUTTON_PRESSED)
	  	   {
		  	   if(amerex_config_data.CAN_baud_rate < 2u)
		  	   {
			  	   amerex_config_data.CAN_baud_rate += 1u;
		  	   }
		  	   else
		  	   {
			  	   Sound_Alert();
		  	   }
	  	   }
	  	   else if(button_status == DEC_BUTTON_PRESSED)
	  	   {
		  	   if(amerex_config_data.CAN_baud_rate > 0u)
		  	   {
			  	   amerex_config_data.CAN_baud_rate -= 1u;
		  	   }
		  	   else
		  	   {
			  	   Sound_Alert();
		  	   }
	  	   }

	  	   //if(button_status)
	  	   //{
		  	   //Config_Indication_LED(OFF);
	  	   //}
	  	   //Config_Indication_LED(amerex_config_data.CAN_baud_rate);
	  break;

//       case CONFIG_BUZZER_SOUND:
//
//                     blink_g_b = 1u;
//                     button_status = Button_Status();
//                     if(button_status == INC_BUTTON_PRESSED)
//                     {
//                         if(amerex_config_data.buzzer_duty < 50u)
//                         {
//                            amerex_config_data.buzzer_duty += 10u;
//                         }
//                         else
//                         {
//                              Sound_Alert();
//                         }
//                     }
//                     else if(button_status == DEC_BUTTON_PRESSED)
//                     {
//                         if(amerex_config_data.buzzer_duty > 10u)
//                         {
//                            amerex_config_data.buzzer_duty -= 10u;
//                         }
//                         else
//                         {
//                             Sound_Alert();
//                         }
//                     }
//                     if(button_status)
//                     {
//                         LED_Blink(CONFIG_BUZZER_LED,CONFIG_ON);
//                          Delay(800);
//                          Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, amerex_config_data.buzzer_duty);
// 						 Delay(500);
//                          Buzzer_Freq_Set(BUZZER_FREQ_2_5KHZ, 0);//Small_Beep(500);
//                     }
//
//        break;
	  case CONFIG_TRBL_RELAY_DELAY:

	        blink_g_b = 0u;
	        button_status = Button_Status();
	        _Trouble_LED(ALWAYS_ON);
	        if(button_status == INC_BUTTON_PRESSED)
	        {
		        if(amerex_config_data.trbl_relay_delay < 6000u)
		        {
			        amerex_config_data.trbl_relay_delay += 1000u;
		        }
		        else
		        {
			        Sound_Alert();
		        }
	        }
	        else if(button_status == DEC_BUTTON_PRESSED)
	        {
		        if(amerex_config_data.trbl_relay_delay > 1000u)
		        {
			        amerex_config_data.trbl_relay_delay -= 1000u;
		        }
		        else
		        {
			        Sound_Alert();
		        }
	        }
	        if(button_status)
	        {
		        Config_Indication_LED(OFF);
	        }

	        Config_Indication_LED(amerex_config_data.trbl_relay_delay / 1000u);

	  break;
	  case CONFIG_FIRE_RELAY_DELAY:

	        blink_g_b = 0u;
	        button_status = Button_Status();
	        LED_Blink(CONFIG_FIRE_LED,CONFIG_ON);
	        if(button_status == INC_BUTTON_PRESSED)
	        {
		        if(amerex_config_data.fire_relay_delay < 6000u)
		        {
			       amerex_config_data.fire_relay_delay += 1000u;
		        }
		        else
		        {
			        Sound_Alert();
		        }
	        }
	        else if(button_status == DEC_BUTTON_PRESSED)
	        {
		        if(amerex_config_data.fire_relay_delay > 1000u)
		        {
			       amerex_config_data.fire_relay_delay -= 1000u;
		        }
		        else
		        {
			        Sound_Alert();
		        }
	        }
	        if(button_status)
	        {
		        Config_Indication_LED(OFF);
	        }

	        Config_Indication_LED(amerex_config_data.fire_relay_delay / 1000u);

	  break;
// 	   	case CONFIG_ACTUATOR_CALIBRATION:
// 	    break;



	  case CONFIG_DEFAULT_TRBL_RLY_STATE:
	   
	   	  	   _Trouble_LED(ALWAYS_ON);
			   (amerex_config_data.Default_trbl_rly_level == 1u)?(ioport_set_pin_level(Alarm_silence,ON),ioport_set_pin_level(SYS_RST_LED,OFF),ioport_set_pin_level(Amerex_R,ON),ioport_set_pin_level(Amerex_G,OFF)):(ioport_set_pin_level(Alarm_silence,OFF),ioport_set_pin_level(SYS_RST_LED,ON),ioport_set_pin_level(Amerex_G,ON),ioport_set_pin_level(Amerex_R,OFF));
	   	  	   blink_g_b = 0u;
	   	  	   button_status = Button_Status();
 	   	  	   if(button_status == INC_BUTTON_PRESSED)
	   	  	   {
		   	  	   if(amerex_config_data.Default_trbl_rly_level <= 0u)
		   	  	   {
			   	  	   amerex_config_data.Default_trbl_rly_level = 1u;
		
		   	  	   }
		   	  	   else
		   	  	   {
			   	  	   Sound_Alert();
		   	  	   }
	   	  	   }
	   	  	   else if(button_status == DEC_BUTTON_PRESSED)
	   	  	   {
		   	  	   if(amerex_config_data.Default_trbl_rly_level == 1u)
		   	  	   {
			   	  	   amerex_config_data.Default_trbl_rly_level = 0u;

		   	  	   }
		   	  	   else
		   	  	   {
			   	  	   Sound_Alert();
		   	  	   }
	   	  	   }

	  break;


		case CONFIG_DEFAULT_FIRE_RLY_STATE:
             LED_Blink(CONFIG_FIRE_LED,CONFIG_ON);
 
		  (amerex_config_data.Default_fire_rly_level == 1u)?(ioport_set_pin_level(Alarm_silence,ON),ioport_set_pin_level(SYS_RST_LED,OFF),ioport_set_pin_level(Amerex_R,ON),ioport_set_pin_level(Amerex_G,OFF)):(ioport_set_pin_level(Alarm_silence,OFF),ioport_set_pin_level(SYS_RST_LED,ON),ioport_set_pin_level(Amerex_G,ON),ioport_set_pin_level(Amerex_R,OFF));		 //  ioport_set_pin_level(Amerex_R,ON);

		   blink_g_b = 0u;
		   button_status = Button_Status();

		   if(button_status == INC_BUTTON_PRESSED)
		   {
			  
			   if(amerex_config_data.Default_fire_rly_level <= 0u)
			   {
				   amerex_config_data.Default_fire_rly_level = 1u;
			   }
			   else
			   {
				   Sound_Alert();
			   }
		   }
		   else if(button_status == DEC_BUTTON_PRESSED)
		   {
			   if(amerex_config_data.Default_fire_rly_level == 1u)
			   {
				   amerex_config_data.Default_fire_rly_level = 0u;
			    }
			   else
			   {
				   Sound_Alert();
			   }
		   }

		break;




        default:

	    break;
		}
					/******************************************************************************************/
		}

	}




}



/* Function : Check the Increment and decrement button status
 * Parameter: None
 * Return   : any one of enum INC_BUTTON_PRESSED or DEC_BUTTON_PRESSED or NO_BUTTON_PRESSED
 */
uint32_t Button_Status(void)
{
    static uint8_t inc_db_count_sl_u8 = 0u; /*For increment button debounce count*/
    static uint8_t dec_db_count_sl_u8 = 0u; /*For decrement button debounce count*/
    static bool inc_status = 0u;
    static bool dec_status = 0u;

    if(!ioport_get_pin_level(ALARM_SILEN_PIN) && !inc_status)
    {
        dec_db_count_sl_u8 = 0u;
        inc_db_count_sl_u8++;
        if(inc_db_count_sl_u8 > CONFIG_DB_COUNT)
        {
            blink_g_b = 0u;
            Buzzer_Freq_Set(BUZZER_FREQ_1_5KHZ, DUTY_1_5_KHZ);
            //Small_Beep(100);
			Delay(100);
            inc_status = 1u;
            auto_exit_prev_tick_g_u32 = count_1ms;
		  // Buzzer_Freq_Set(BUZZER_FREQ_0, DUTY_1_5_KHZ);
		    Buzzer_Freq_Set(BUZZER_FREQ_1_5KHZ, 0);
            return INC_BUTTON_PRESSED ;
        }
    }
    else if(ioport_get_pin_level(ALARM_SILEN_PIN))
    {
        inc_db_count_sl_u8 = 0u;
        inc_status = 0u;
    }

    if(!ioport_get_pin_level(SYS_RESET_SW_PIN) && !dec_status)
    {
        inc_db_count_sl_u8 = 0u;
        dec_db_count_sl_u8++;
        if(dec_db_count_sl_u8 > CONFIG_DB_COUNT)
        {
            blink_g_b = 0u;
            Buzzer_Freq_Set(BUZZER_FREQ_1_5KHZ, DUTY_1_5_KHZ);

           // Small_Beep(100);
		   Delay(100);
            dec_status = 1u;
            auto_exit_prev_tick_g_u32 = count_1ms;
			//Buzzer_Freq_Set(BUZZER_FREQ_0, DUTY_1_5_KHZ);
			Buzzer_Freq_Set(BUZZER_FREQ_1_5KHZ, 0);

            return DEC_BUTTON_PRESSED;
        }
    }
    else if(ioport_get_pin_level(SYS_RESET_SW_PIN))
    {
        dec_status = 0u;
        dec_db_count_sl_u8 = 0u;
    }

    return NO_BUTTON_PRESSED;
}

/* Function : if the set value change to minimum or maximum alert to thr user by sound
 * Parameter: None
 * Return   : None
 */
void Sound_Alert(void)
{

     Small_Beep(100);
     Delay(50);
     Small_Beep(100);

}
/* Function : Blink the LED
 * Parameter: @param 1 - Which LED to be access @param2 - CONFIG_ON/CONFIG_OFF/CONF_BLINK enum
 * Return   : None
 */

void LED_Blink(uint8_t led, uint8_t C_Val)
{
    static bool on_off = 0u;
    if(C_Val == CONFIG_ON)
    {
        on_off = CONFIG_ON;
    }
    else if(C_Val == CONFIG_OFF)
    {
        on_off = CONFIG_OFF;
    }
    else
    {
        on_off = !on_off;
    }

    switch(led)
    {
    case CONFIG_ALL_LED:
        ALL_LED_BLINK(on_off); /*PWM Pin disable*/
        break;
    case CONFIG_FIRE_LED:
        on_off?FIRE_LED_PWM_EN(amerex_config_data.fire_LED_duty):FIRE_LED_PWM_EN(0);
        break;
    case CONFIG_BUZZER_LED:
		on_off?FIRE_LED_PWM_EN(amerex_config_data.fire_LED_duty):FIRE_LED_PWM_EN(0);
        ioport_set_pin_level(Trouble_LED,on_off);
       break;
    case CONFIG_TRBL_LED:
       ioport_set_pin_level(Trouble_LED,on_off);
        break;
    case CONFIG_CONFIG_LED:
       ioport_set_pin_level(sys_normal,on_off);
        break;
    default:
      ALL_LED_BLINK(OFF);  /*PWM Pin disable*/
      FIRE_LED_PWM_EN(0);
      break;
    }
}





/* Function : Calculate Fire LED count
 * Parameter: None
 * Return   : None
 */
void Fire_LED_Count(void)
{
    uint16_t fire_LED_value;
    fire_LED_value =  (FIRE_LED_FADING_TIME / amerex_config_data.fire_LED_duty);
    fire_LED_fading_count_g_u8 = (uint8_t)fire_LED_value;
    fire_LED_duty_max_g_u8 = (amerex_config_data.fire_LED_duty * 2u) - 1u;
}
