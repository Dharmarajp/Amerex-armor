/*
 * led.c
 *
 * Created: 13-07-2022 12:53:52
 *  Author: p.dharmaraj
 */
 #include "main_header.h"
 const uint16_t ON_delay_500ms = 500u;       /*Secondary power LED Flash delay 500ms*/
 const uint16_t OFF_delay_10s = 10000u;      /*Secondary power LED OFF delay 10000ms*/
 uint16_t sec_pwr_interval_g_u32 = 500u;   /* used for interval 500ms */
 uint32_t sec_pwr_prevtick_g_u32 = 0u;       /*To store previous count value */
  uint32_t rgb_prevtick_g_u32 = 0u;       /*To store previous count value */
 bool sec_pwr_state_g_b = 0u;
 const uint16_t delay_250ms = 250u;         /*Secondary power LED Fault delay 250ms*/
 uint8_t  trouble_LED_count_g_u8 = 0u;             /*To store LED ON count value */
 uint16_t trouble_LED_interval_g_u16 = 500u;       /* used for interval*/
 uint32_t trouble_LED_prevtick_g_u32 = 0u;          /*To store previous count value */
 uint32_t fire_LED_prevtick_g_u32 = 0u;          /*To store previous count value */
 bool trouble_LED_state_g_b = 0u;                  /* TO store LED state*/
uint8_t fire_LED_fading_count_g_u8 = 2u;  /*to store the fading time. assign value must be FIRE_LED_FADING_TIME/ "config_data.fire_LED_duty" */
uint8_t fire_LED_duty_max_g_u8 = 199u;    /*Duty Cycle Percentage for brightness high to low, assign value must be "config_data.fire_LED_duty * 2 -1"*/
uint8_t  config_LED_count_g_u8 = 0u;        /*To store LED ON count value */
uint16_t config_LED_interval_g_u16 = 500u;  /* used for interval 500ms*/
uint32_t config_LED_prevtick_g_u32 = 0u;    /*To store previous count value */
bool config_LED_state_g_b = 0u;             /* TO store LED state*/
 uint8_t count_test=0;
volatile uint32_t Buzzer_bit=0;
volatile uint8_t Trouble_status;

void Amerex_LOGO_RGB(const uint32_t ul_mask)
{
pio_sync_output_write(PIOA,ul_mask);

}

 uint32_t RGB_ON[10]={RGB_LED_RED,RGB_LED_GREEN,RGB_LED_BLUE,RGB_LED_MAGENTA,RGB_LED_YELLOW,RGB_LED_CYAN,RGB_LED_WHITE};

/* Function : Alarm Silence indication
 * Parameter: None
 * Return   : None
 */
void Alarm_Sil_Ind(void)
{
    if(alarm_sil_flag_g_b )
    {

         Alarm_Silence_LED(ON);
		
		
    }
    else
    {
        Alarm_Silence_LED(OFF);
    }
}

/* Function : System Normal indication
 * Parameter: None
 * Return   : None
 */

void RGB_TEST()
{
//
// 	if(test_sw_pressed_g_b)
// 	{
//       if((count_1ms - rgb_prevtick_g_u32) >= 300) /*Compare Current time and previous time, then proceed*/
//       {
// 		rgb_prevtick_g_u32 = count_1ms; /*copy current time to previous counter value*/
// 		Amerex_LOGO_RGB(RGB_ON[count_test]);
// 		count_test++;
// 		if(count_test==7)count_test=0;
//
//       }
// 	}
// 	else
// 	{
// 	rgb_prevtick_g_u32 = count_1ms;
// 	}

}



void System_Normal_Ind(void)
{
    if((!any_of_trouble_g_b))
    {

        SysNormal_LED(ON);

    }
    else
    {
        SysNormal_LED(OFF);
    }
}
/* Function : System reset LED ON/OFF function
 * Parameter: 0 - OFF, 1 - ON
 * Return   : None
 */
void Sys_Reset_LED(bool on_off)
{
    ioport_set_pin_level(SYS_RST_LED,on_off);  /*ON/OFF LED*/
}

/* Function : Secondary power indication
 * Parameter: None
 * Return   : None
 */
void Secondary_pwr_Ind(void)
{
	
   if(!pri_pwr_trouble_g_b && !sec_pwr_trouble_g_b)
    {
        Secondary_Pwr_LED(OFF);
    }
    else if((sec_pwr_trouble_g_b) && (!pri_pwr_trouble_g_b))
    {
        Secondary_Pwr_LED(SEC_PWR_NOT_AVAIL);
    }
    else
    {
        Secondary_Pwr_LED(PRIMARY_PWR_NOT_AVAIL);
    }
}
/* Function : Trouble indication
   * Parameter: None
 * Return   : None
 */
void Trouble_Ind(void)
{

 if(!any_of_trouble_g_b)
    {
        _Trouble_LED(OFF);
		Trouble_status =0;
    }
    else if(hd1_trouble_g_u8)
    {
         _Trouble_LED(HD1_1_FLASH);
		Trouble_status = HD1_1_FLASH;
    }
    else if(hd2_trouble_g_u8)
    {
        _Trouble_LED(HD2_2_FLASH);
		Trouble_status = HD2_2_FLASH;
    }
    else if(manual_sw_trbl_g_b)
    {
        _Trouble_LED(MANUAL_SW_3_FLASH);
		Trouble_status = MANUAL_SW_3_FLASH;
    }
    else if(actuator_trbl_g_u8)
    {
        _Trouble_LED(ACT_TRBL_4_FLASH);
		Trouble_status = ACT_TRBL_4_FLASH;
    }
    else if(pri_pwr_trouble_g_b)
    {
        _Trouble_LED(PRI_PWR_5_FLASH);
		 Trouble_status = PRI_PWR_5_FLASH;
    }
    else if(sec_pwr_trouble_g_b)
    {
        _Trouble_LED(SEC_PWR_6_FLASH);
		 Trouble_status = SEC_PWR_6_FLASH;
    }
    else if(pres_sw_trbl_g_b)
    {
        _Trouble_LED(PRES_SW_TRBL_7_FLASH);
		Trouble_status = PRES_SW_TRBL_7_FLASH;
    }
    else
    {
        _Trouble_LED(OFF); /*if no trouble OFF the LED*/
    }
}


/* Function : Alarm Silence LED ON/OFF function
 * Parameter: 0 - OFF, 1 - ON
 * Return   : None
 */
void Alarm_Silence_LED(bool on_off)
{
    ioport_set_pin_level(Alarm_silence,on_off);  /*ON/OFF LED*/
}
 /* Function : System Normal LED ON / OFF function
 * Parameter: ON - 1, OFF - 0
 * Return   : None
 */

void SysNormal_LED(bool on_off)
{
ioport_set_pin_level(sys_normal,on_off);
}

/* Function : Secondary Power LED ON / OFF function
 * Parameter: @param1 - 0 - OFF, 100 - Continuous ON, 1 - 500ms ON 10Sec OFF, 2 - 500ms ON, 250ms OFF
 * Return   : None
 */

void Secondary_Pwr_LED(uint8_t count)
{
    if(count == OFF) /*if parameter is 0, OFF LED*/
    {
        ioport_set_pin_level(SEC_PWR_LED,OFF); /* OFF LED*/
        sec_pwr_prevtick_g_u32 = 0u;
    }
    else if(count == ALWAYS_ON) /*if parameter is 100, Solid ON the LED*/
    {
        ioport_set_pin_level(SEC_PWR_LED,ON);  /* ON LED*/
    }
    else
    {

      if((count_1ms - sec_pwr_prevtick_g_u32) >= sec_pwr_interval_g_u32) /*Compare Current time and previous time, then proceed*/
      {
        if(sec_pwr_state_g_b && (count == PRIMARY_PWR_NOT_AVAIL))
        {
            sec_pwr_interval_g_u32 = OFF_delay_10s; /*assign OFF delay time 10Sec*/
        }
        else if(sec_pwr_state_g_b && (count == SEC_PWR_NOT_AVAIL))
        {
            sec_pwr_interval_g_u32 = delay_250ms; /*assign OFF delay time 250ms*/
        }
        else
        {
            sec_pwr_interval_g_u32 = ON_delay_500ms; /*assign ON delay time 500ms*/
        }

        sec_pwr_state_g_b = !sec_pwr_state_g_b; /*Toggle the state variable*/

        sec_pwr_prevtick_g_u32 = count_1ms; /*copy current time to previous counter value*/

        ioport_set_pin_level(SEC_PWR_LED, sec_pwr_state_g_b);  /* OFF LED*/

// 						printf("count_test :%d\r\n",count_test);
// 						Amerex_LOGO_RGB(RGB_ON[count_test]);
// 						count_test++;
// 						if(count_test==7)count_test=0;

      }

    }
}


/* Function : Configuration LED blink/ON/OFF
 * Parameter: @param1 0 - OFF, 100 - ON, else - LED blink depends on "count" value
 * Return   : None
 */
void Config_Indication_LED(uint8_t count)
{

    if(count == OFF)
    {
       // ROM_PWMOutputState(PWM0_BASE,CONFIG_IND_LED_BLUE,PWM_DISABLE);  /*OFF LED*/

		 ioport_set_pin_level(Amerex_B,OFF);
        config_LED_count_g_u8 = 0u;
        config_LED_interval_g_u16 = ON_delay_500ms;
    }
    else if(count == ALWAYS_ON)
    {
       // ROM_PWMOutputState(PWM0_BASE,CONFIG_IND_LED_BLUE,PWM_ENABLE);  /*ON LED*/
	  // SysNormal_LED(ON);
	   ioport_set_pin_level(Amerex_B,ON);
        config_LED_count_g_u8 = 0u;
        config_LED_interval_g_u16 = ON_delay_500ms;
    }
    else
    {
        if(((count_1ms - config_LED_prevtick_g_u32) >= config_LED_interval_g_u16) && (config_LED_count_g_u8 < count))
        {
            config_LED_prevtick_g_u32 = count_1ms;
            config_LED_state_g_b = !config_LED_state_g_b;
            config_LED_interval_g_u16 = ON_delay_500ms;
            if(config_LED_state_g_b)
            {
                config_LED_count_g_u8++;
            }
        }
        else if(((count_1ms - config_LED_prevtick_g_u32) >= config_LED_interval_g_u16) && (config_LED_count_g_u8 >= count))
        {
            config_LED_count_g_u8 = 0u;
            config_LED_prevtick_g_u32 = count_1ms;
            config_LED_interval_g_u16 = 50000u;
            config_LED_state_g_b = !config_LED_state_g_b;
        }
		 	ioport_set_pin_level(Amerex_B,config_LED_state_g_b);
    }
}

/* Function : Trouble LED blink function
 * Parameter: @param1 0 - OFF, 100 - Continuous ON, 1 to 8 no of blink (500ms-ON, 250ms-OFF) interval 10Sec OFF
 * Return   : None
 */

void _Trouble_LED(uint8_t count)
{
    if(count == OFF)
    {
        ioport_set_pin_level(Trouble_LED,OFF);  /* OFF LED*/
        trouble_LED_count_g_u8 = 0u;
        trouble_LED_interval_g_u16 = delay_250ms;
    }
    else if(count == ALWAYS_ON) /*if parameter is 100, Solid On the LED*/
    {
        ioport_set_pin_level(Trouble_LED,ON);   /* ON LED*/
        trouble_LED_count_g_u8 = 0u;
    }

    else
    {
        if(((count_1ms - trouble_LED_prevtick_g_u32) >= trouble_LED_interval_g_u16) && (trouble_LED_count_g_u8 < count)) //Compare Current time, previous time and LED Flash count, then proceed
        {

          if(trouble_LED_state_g_b)
          {
              trouble_LED_interval_g_u16 = delay_250ms;  /*assign OFF delay time 250ms*/
          }
          else
          {
              trouble_LED_interval_g_u16 = ON_delay_500ms; /*assign ON delay time 500ms*/
              trouble_LED_count_g_u8++;
          }
          trouble_LED_state_g_b = !trouble_LED_state_g_b;
          trouble_LED_prevtick_g_u32 = count_1ms; /*copy current time to previous counter value*/
		   Buzzer_bit = 0;
		  
        }

        else if(((count_1ms - trouble_LED_prevtick_g_u32) >= trouble_LED_interval_g_u16) && (trouble_LED_count_g_u8 >= count)) //Compare Current time, previous time and LED Flash count, then proceed
        {
            trouble_LED_count_g_u8 = 0; //if Flash count reaches the function parameter
            trouble_LED_interval_g_u16 =OFF_delay_10s; //; /*assign OFF delay time 10Sec*/
            trouble_LED_state_g_b = !trouble_LED_state_g_b; //Toggle the Status variable
            trouble_LED_prevtick_g_u32 = count_1ms; /*copy current time to previous counter value*/
			Buzzer_bit = 1;
			

        }
		

		
		

            ioport_set_pin_level(Trouble_LED,trouble_LED_state_g_b);  /* Toggle LED*/
    }
}
/* Function : System Reset indication
 * Parameter: None
 * Return   : None
 */
void System_Reset_Ind(void)
{
    if(!fire_iso_out_g_b && !trbl_iso_out_g_b && !test_sw_pressed_g_b)
    {
        Sys_Reset_LED(OFF);
    }
    else
    {

        Sys_Reset_LED(ON);

    }
}
/* Function : Fire LED fading function.
 * Parameter:  @parm1 - 0 - OFF, not equal to 0 - LED fading
 * Return   : None
 */

void Fire_LED(uint8_t _value)
{
    static uint8_t fire_LED_intensity_sl_u8 = 0u;           /*For LED fading */

    static uint8_t fire_LED_count_sl_u8 = 0u;               /*For count and LED fading */
     if(_value == OFF)
     {
         FIRE_LED_PWM_EN(OFF);/* OFF LED*/
         fire_LED_count_sl_u8 = 0u;
         fire_LED_intensity_sl_u8 = 1u;
         fire_LED_prevtick_g_u32 = 0u;
     }
    else
    {
      //  ROM_PWMOutputState(PWM1_BASE, FIRE_LED_RED, PWM_ENABLE);  /*Enable PWM*/

        if((count_1ms - fire_LED_prevtick_g_u32) >= fire_LED_fading_count_g_u8)
        {
            fire_LED_count_sl_u8++;

            if(fire_LED_count_sl_u8 < amerex_config_data.fire_LED_duty)
            {
                fire_LED_intensity_sl_u8++;
            }
            else if(fire_LED_count_sl_u8 >= amerex_config_data.fire_LED_duty && fire_LED_count_sl_u8 < fire_LED_duty_max_g_u8)
            {
                fire_LED_intensity_sl_u8--;
            }
            else
            {

                fire_LED_intensity_sl_u8 = 1u;
                fire_LED_count_sl_u8 = 0u;
            }

            fire_LED_prevtick_g_u32 = count_1ms;
            if(!fire_LED_intensity_sl_u8)
            {
                fire_LED_intensity_sl_u8 = 1u;
            }
            //ROM_PWMPulseWidthSet(PWM1_BASE,FIRE_LED_RED_WIDTH,PWM_duty_value(fire_LED_intensity_sl_u8));
			FIRE_LED_PWM_EN(fire_LED_intensity_sl_u8);

        }
    }
}

/* Function : Fire indication
 * Parameter: None
 * Return   : None
 */
void Fire_Ind(void)
{
  if( (Fire_Rst_vehicle_start_bit!=2u && fire_happen_g_b))
  {

      Fire_LED(LED_FADING);
	  	Trouble_status = 8u;

  }
  else
  {
       Fire_LED(OFF);
  }
}

void ALL_LED_BLINK(bool _onoff)
{

	ioport_set_pin_level(SYS_RST_LED,_onoff);
	ioport_set_pin_level(Alarm_silence,_onoff);
	ioport_set_pin_level(SEC_PWR_LED,_onoff);
	ioport_set_pin_level(sys_normal,_onoff);
	ioport_set_pin_level(Trouble_LED,_onoff);

}
