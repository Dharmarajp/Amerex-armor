/*
 * spi_u.c
 *
 * Created: 29-08-2022 16:42:53
 *  Author: p.dharmaraj
 */
#include "main_header.h"
#include <at25dfx.h>
#include <conf_at25dfx.h>

static char en_getlogArr[4];
 char Read_flash_data[8];


 union
 {
	 uint32_t EpochTime;
	 struct
	 {
		 unsigned char G_log_RawData[4];
	 }encode_data;
 }Encode_getlog;


my_data temp_data = {.all_LED_duty = 80u,.fire_LED_duty = 80u,.buzzer_duty = 30u,.trbl_relay_delay = 1000u,
.fire_relay_delay = 1000u,.actuator_value = 0u,.CAN_baud_rate = 1u,.Default_trbl_rly_level = 0u,.Default_fire_rly_level=0u};

volatile uint32_t spi_flash_addr_g_u32;
 #define none_blank(v32) (((v32)==0xFFFFFFFF)?0:(v32))
 #define TRIGGER_amerex_param (1)
 #define TRIGGER_Payloadcnt_param (1)


uint8_t crc8_cal(uint8_t *data, uint16_t length) {
	uint8_t crc = 0x00;

	uint8_t extract;
	uint8_t sum;


	for (uint16_t i = 0; i < length; i++) {
		extract = *data;
		for (uint8_t tempI = 8; tempI; tempI--) {
			sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum)
			crc ^= 0x8C;
			extract >>= 1;
		}
		data++;
	}

	return crc;
}





void write_flash_mid_param(uint8_t *data)
{
	void *mid_param_addr;
	//memory_init();
	mid_param_addr = (void *)flash_mid_param(0);
	//printf("Write info area ...\r\n");
	memory_erase(mid_param_addr, flash_mid_param_SIZE);

	memory_write(mid_param_addr, (uint8_t *) data);
}

uint8_t flash_write_amerex_param(uint8_t *data)
{
	uint8_t ret=0;

	void *amerex_param;
	//memory_init();
	amerex_param = (void *)Amerex_config(0);
	//  printf("Write info area ...\r\n");
	memory_erase(amerex_param, Amerex_config_SIZE);

	ret=memory_write(amerex_param, (uint8_t *) data);
	return ret;
}




void NOR_Flash_Log_Data(flash *mylog)
{
	//flash cmp_data;
	uint32_t sector_memory_adjust = 0;
	uint32_t location = 0;
    uint8_t ret = 1u;
	//uint32_t block_id = 0;

	/*
	 //  printf("Before payload_cntflash_data_counter %ld\r\n",payload_cnt.flash_data_counter);
	sector_id = (payload_cnt.flash_data_counter / MAX_PAYLOAD_PER_SECTOR);
	sector_id %= FALSH_SECTOR_PER_BLOCK;
	  // printf("sector_id %d\r\n",sector_id);
	block_id = payload_cnt.flash_data_counter / MAX_PAYLOAD_PER_BLOCK;
	  // printf("block_id %d\r\n",sector_id);
	 */
	//  printf("mylog code  %d\r\n",mylog->log_code);
	sector_memory_adjust = ((payload_cnt.flash_data_counter) / MAX_PAYLOAD_PER_SECTOR);
	sector_memory_adjust *= FREE_BYTES_IN_SECTOR;
	location += sector_memory_adjust;
	location += ((payload_cnt.flash_data_counter) * MAX_SIZE_OF_PAYLOAD);
	//if(payload_cnt.flash_data_counter <= 4774)
	  //location += 8192;
	  
	  printf("location %lu mid :%lu\r\n",location,payload_cnt.flash_data_counter);
	   wdt_restart(WDT);
	if ((payload_cnt.flash_data_counter) % MAX_PAYLOAD_PER_SECTOR == 0) {

		flash_erase(location, EXT_FLASH_SECTOR_SIZE);
		 printf("FLASH CLR %ld \r\n",location);
	}

	if(mylog->log_code == 5 && mylog->start_stop == 0)
	{
		wdt_restart(WDT);
		Delay(800u);
		wdt_restart(WDT);
	}



		mylog->epoch_time = RV3028_GetTime(&_RTC);//Convert_Epoch_time();

	//	printf("epoch_time %lu\r\n",mylog->epoch_time);
		mylog->crc = crc8_cal((uint8_t *)mylog, sizeof(flash));
		ret = at25dfx_write((uint8_t *)mylog, sizeof(flash),location);
        printf("FWR %d\r\n CRC %d\r\n",ret,mylog->crc);
		if (ret == 0)
		{
		
					//database_update_payload(payload_cnt.flash_data_counter);
		payload_cnt.flash_data_counter +=1;
		if (payload_cnt.flash_data_counter>=MAX_PAYLOAD_COUNT)
		{
		payload_cnt.flash_data_counter =0;
		}

		payload_cnt.Trigger =1;
		wdt_restart(WDT);
		uint8_t* ptr = (uint8_t*) &payload_cnt;
		write_flash_mid_param(ptr);
		}

}


void database_send_payload(uint16_t mid) {

	char str[200];
	uint32_t sector_memory_adjust = 0;
	uint32_t location = 0;
	flash sent_payload;
	sector_memory_adjust = (mid / MAX_PAYLOAD_PER_SECTOR);
	sector_memory_adjust *= FREE_BYTES_IN_SECTOR;
	location += sector_memory_adjust;
	location += (mid * MAX_SIZE_OF_PAYLOAD);

		//if(mid <= 4774)
	  //location += 8192;

	printf("location %ld mid :%d\r\n",location,mid);
	uint8_t* ptr = (uint8_t*) &sent_payload;
	uint8_t ret = at25dfx_read(ptr, sizeof(flash),location);
	if (ret == AT25_SUCCESS) {
		uint8_t crc_temp = sent_payload.crc;
		sent_payload.crc = 0;
		sent_payload.crc = crc8_cal(ptr, sizeof(flash));
		if (crc_temp == sent_payload.crc)
		{
			#ifdef Debug_enable
			sprintf(str,"%ld,%d,%d,%d,%d",sent_payload.epoch_time,sent_payload.log_code,sent_payload.trouble_fire,sent_payload.start_stop,sent_payload.value);

			printf("%s\r\n",str);
			#endif

				memset(en_getlogArr,0,sizeof(en_getlogArr));
				memset(Read_flash_data,0,sizeof(Read_flash_data));
				Encode_getlog.EpochTime = sent_payload.epoch_time;
				Encode_getlog.encode_data.G_log_RawData[4]    = Encode_getlog.EpochTime;
				memcpy(en_getlogArr, Encode_getlog.encode_data.G_log_RawData, 4);

			Read_flash_data[0] = en_getlogArr[0];
			Read_flash_data[1] = en_getlogArr[1];
			Read_flash_data[2] = en_getlogArr[2];
			Read_flash_data[3] = en_getlogArr[3];
			Read_flash_data[4] = ((sent_payload.log_code << 4) | sent_payload.start_stop );
			Read_flash_data[5] = sent_payload.trouble_fire;
			Read_flash_data[6] = sent_payload.value;
			Read_flash_data[7] = 5;

 			j1939_data_transfer(can_identifier,1);


		}
	}
}


void read_flash_mid_param(void *addr, paloadCount_t *Read_payload_cnt)
{

  	paloadCount_t *p = (paloadCount_t *)addr;

  	Read_payload_cnt->flash_data_counter    = none_blank(p->flash_data_counter);
  	Read_payload_cnt->Trigger    = none_blank(p->Trigger);

  	if (Read_payload_cnt->Trigger == TRIGGER_Payloadcnt_param)
  	{
	  //	printf("TRIGGER_read_flash_mid_param is Set\r\n");
  	}
  	else
  	{
	  //	printf("TRIGGER_read_flash_mid_param is not Set\r\n");
	  	Read_payload_cnt->flash_data_counter =0;
  	}

  printf("Read payload cnt %ld\r\n",Read_payload_cnt->flash_data_counter);

}

void flash_read_amerex_param(void *addr, my_data *amerex_param)
{
     my_data *p = (my_data *) addr;
     amerex_param->all_LED_duty    = none_blank(p->all_LED_duty);
     amerex_param->fire_LED_duty    = none_blank(p->fire_LED_duty);
     amerex_param->buzzer_duty    = none_blank(p->buzzer_duty);
     amerex_param->trbl_relay_delay    = none_blank(p->trbl_relay_delay);
     amerex_param->fire_relay_delay    = none_blank(p->fire_relay_delay);
     amerex_param->actuator_value    = none_blank(p->actuator_value);
     amerex_param->CAN_baud_rate    = none_blank(p->CAN_baud_rate);
	 amerex_param->Default_trbl_rly_level    = none_blank(p->Default_trbl_rly_level);
	 amerex_param->Default_fire_rly_level    = none_blank(p->Default_fire_rly_level);
     amerex_param->Trigger    = none_blank(p->Trigger);

     if (amerex_param->Trigger == TRIGGER_amerex_param)
     {
	    // printf("TRIGGER_amerex_param is Set\r\n");
     }
     else
     {
	   //  printf("TRIGGER_amerex_param is NOT Set\r\n");
	     amerex_config_data = temp_data;
     }


}


/*
 *Function  : Reset the values
 *Parameter : none
 *Return    : Epoch time
 */
void Reset_Values(void)
{
    memset(&log_data,0,sizeof(log_data));
}

  /*
 *Function  : Convert Human time to Epoch time
 *Parameter : time
 *Return    : Epoch time
 */

__int64_t Convert_Epoch_time(void)
{
		 struct tm t;
		 RV3028_GetTime(&_RTC);
		 //printf(" %u:%u:%u,%u/%u/%u\r\n",_CurrentTime.tm_hour, _CurrentTime.tm_min, _CurrentTime.tm_sec,_CurrentTime.tm_mday, _CurrentTime.tm_mon, _CurrentTime.tm_year);
  	     //Convert to Unix tims stamp
  	     t.tm_year = (2000+_CurrentTime.tm_year)-1900;  // Year - 1900
  	     t.tm_mon =  _CurrentTime.tm_mon-1;           // Month, where 0 = jan
  	     t.tm_mday = _CurrentTime.tm_mday;          // Day of the month
  	     t.tm_hour =_CurrentTime.tm_hour;
  	     t.tm_min =_CurrentTime.tm_min;
  	     t.tm_sec = _CurrentTime.tm_sec;
  	     t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
  	     return ((__int64_t)mktime(&t)<1661840105)?0:(__int64_t)mktime(&t); //Time invalid - If less than Tuesday, August 30, 2022 11:45:05 AM
}

 void at25dfx_init()
{
//	at25_status_t status;

	/* Initialize the SerialFlash */
	at25dfx_initialize();

	/* Set the SerialFlash active */
	at25dfx_set_mem_active(AT25DFX_MEM_ID);

	/* Check if the SerialFlash is valid */
	at25dfx_mem_check();

}







