// /*
//  * interrupt_handler.c
//  *
//  * Created: 12-07-2022 15:26:20
//  *  Author: p.dharmaraj
//  */
  #include "main_header.h"
#include "conf_bootloader.h"
volatile uint32_t count_1ms = 0u;
volatile uint8_t Timer_cnt_msec=0;
volatile uint8_t Timer_cnt_sec=0;
volatile bool Fire_Alart_bit=0;

//can_mb_conf_t  rx_mailbox;
//volatile bool g_ul_recv_status = 0;;
/** Receive status */
volatile uint32_t g_ul_recv_status = 0;

// void read_data(int);
uint8_t g_ul_log_en=0;
uint8_t ota_start=0u;
uint8_t falsh_state=0;
static uint32_t File_size = 0;
#define TRIGGER_BOOTLAOAD (1)
uint16_t RX_imageCRC = 0;

 union
 {
	 uint32_t data;
	 struct
	 {
		 unsigned char RawData[4];
	 }rx_data;
 }decode;
 
 enum eDownImageStates {
	 initiate = 1,
	 getimagesize,
	 getimageCRC,
	 crcVerify,
	 //imageDownloadResponse,
	 //imageDownloadComplete,
	 imageDownloading
 };
 
 
 
  /** Boot informations */
  typedef struct regions_info {
	  /** Firmware length in regions */
	  uint32_t length;
	  /** Software trigger to force action */
	  uint32_t trigger;
	  uint8_t crc;
	  uint32_t OAD_image_CRC;

  } regions_info_t;

uint16_t crc16(uint16_t crc, unsigned char val) {
	const uint16_t poly = 0x1021;
	unsigned char cnt;

	for (cnt = 0; cnt < 8; cnt++, val <<= 1) {
		unsigned char msb = (crc & 0x8000) ? 1 : 0;

		crc <<= 1;

		if (val & 0x80) {
			crc |= 0x0001;
		}

		if (msb) {
			crc ^= poly;
		}
	}

	return crc;
}
 bool decode_can_msg(can_mb_conf_t *p_mailbox)
{
	static char datalow[4]={0};
	static char datahigh[4]={0};
	uint32_t timestamp = 0;
	uint8_t temp;
	uint8_t res;
	
	memset(datahigh,0,sizeof(datahigh));
	memset(datalow,0,sizeof(datalow));

	decode.data = p_mailbox->ul_datal ;
	decode.rx_data.RawData[4]    = decode.data;

	//printf("datahigh %ld\r\n",decode.data);
	memcpy(datahigh, decode.rx_data.RawData, 4);

// 	 	for(int i =0;i<4;i++)
// 	 	{
//
// 		 	printf("Hello World %u\n",datahigh[i]);
//
// 	 	}


	decode.data = p_mailbox->ul_datah;

      timestamp =decode.data;
	decode.rx_data.RawData[4]    = decode.data;
	memcpy(datalow, decode.rx_data.RawData, 4);
 	//for(int i =0;i<4;i++)
 	//{
//
 		//printf("Hello World %u\n",datalow[i]);
//
 	//}


	  switch (datahigh[0]) {

		 case Amerex_rtc_set:
							printf("Amerex_rtc_set %ld\r\n",timestamp);
							RV3028_Set_TimeStamp(timestamp);
							printf("Time at Bootup : %lu\r\n", RV3028_GetTime(&_RTC));
							//printf(" %u:%u:%u,%u/%u/%u\r\n",_CurrentTime.tm_hour, _CurrentTime.tm_min, _CurrentTime.tm_sec,_CurrentTime.tm_mday, _CurrentTime.tm_mon, _CurrentTime.tm_year);
							break;
		case Amerex_config_set:
						//	printf("Amerex_config_set\r\n");
							if(datalow[0]>=10 && datalow[0]<=100)
							{
								amerex_config_data.all_LED_duty = datalow[0];
							}
							else return 0;
							

							 if(datalow[1]>=10 && datalow[1]<=100)
							amerex_config_data.fire_LED_duty = datalow[1];
							else return 0;

							 temp = (datalow[2] >> 4) & 0x0F;
							 if(temp>=1 && temp<=6)
								amerex_config_data.trbl_relay_delay = temp *1000;
							else return 0;
							 temp = datalow[2]  & 0x0F ;
							  if(temp>=1 && temp<=6)
								amerex_config_data.fire_relay_delay =  temp *1000;
							else return 0;
							 temp = datalow[3] & 0x0F ;
							if(temp>=1 && temp<=2)
							amerex_config_data.CAN_baud_rate = temp;
							else return 0;
							amerex_config_data.Trigger =1u;
							uint8_t* ptr = (uint8_t*) &amerex_config_data;
							res = flash_write_amerex_param(ptr);
							if(res==1)
							{
								rstc_start_software_reset(RSTC);
							}

							break;
		case Amerex_Boardno_set:
							//printf("Amerex_Boardno_set\r\n");
							break;
		case Amerex_get_config:
							//printf("Amerex_config_get\r\n");

							break;
	    case Amerex_Get_Log:
							printf("Amerex_Get_Log\r\n");
							g_ul_log_en =1;
							break;
	    case Amerex_Software_RST:
							printf("Amerex_Software_RST\r\n");
							 rstc_start_software_reset(RSTC);
							break;
		default:
		                   // printf("unknown commands\r\n") ;
		                   break;


                      }
					 return 0; 
}


/**
 * \brief Default interrupt handler for CAN 1.	   // Set transfer id on CAN Bus Analyzer rx_mailbox.ul_id = CAN_MID_MIDvA(CAN_TRANSFER_ID) ex:transmit id: 7
 */
void CAN0_Handler(void)
{
	uint32_t ul_status=0;

	ul_status = can_mailbox_get_status(CAN0, 0);	//  && g_ul_recv_status == 0
	if (ul_status & CAN_MSR_MRDY ) {   //&& g_ul_recv_status == 0
		
		rx_mailbox.ul_status = ul_status;
		can_mailbox_read(CAN0, &rx_mailbox);
		//can_disable_interrupt(CAN0, CAN_IDR_MB0);
		g_ul_recv_status = 1;

	}
}
#define CAN_MSG_TOGGLE_LED_0        0x44332244
#define CAN_MSG_TOGGLE_LED_1        0xAABBCCDD
#define CAN_MSG_DUMMY_DATA          0x05060708
#define TEST1_CAN_COMM_MB_IDX    0
#define TEST1_CAN_TRANSFER_ID    0x03
#define TEST1_CAN0_TX_PRIO       15
void DFU_callback_Fun(can_mb_conf_t *p_mailbox)
{
//static bool on_off_f=0;


	can_disable_interrupt(CAN0, CAN_IDR_MB0);
	
		uint16_t imageCRC = 0;
		static volatile uint32_t remFileSize = 0;
		static uint32_t flashPos = 0;
		static char datalow[4]={0};
		static char datahigh[4]={0};
		static uint8_t Merge_Arr[8];
		//uint8_t cnt = 0;
		static uint8_t temp_crc = 0;
		static enum eDownImageStates state = initiate;
		static char Strcrc =0;
		
		DFU_auto_exit_prev_tick_g_u32 =count_1ms;
		//static uint8_t otabit=0;
		memset(datahigh,0,sizeof(datahigh));
		memset(datalow,0,sizeof(datalow));
		decode.data = p_mailbox->ul_datah ;
		decode.rx_data.RawData[4]    = decode.data;
		memcpy(datahigh, decode.rx_data.RawData, 4);
		decode.data = p_mailbox->ul_datal;
		decode.rx_data.RawData[4]    = decode.data;
		memcpy(datalow, decode.rx_data.RawData, 4);

	
		if((datahigh[0] == 1) && (datahigh[1] == 1) && (datahigh[2] == 1) && (datahigh[3] == 1) && state == initiate)
		{
		ioport_set_pin_level(Amerex_R,1);
		ioport_set_pin_level(Amerex_G,0);
		ioport_set_pin_level(Amerex_B,0);
			
			
		temp_crc=0;
		temp_crc= crc8_cal((uint8_t *)datalow, 2);
		if (datalow[2] == temp_crc)
		{
		for(int i=0;i<2;i++)
		CAN_Res_pkt.CANRESP[i]    = datalow[i];
		tx_mailbox.ul_datal = (CAN_Res_pkt.CANRESP_ST.CANRespL_1 << 8) | temp_crc;
		}
		else
		{
			tx_mailbox.ul_datal = (0xCCEE << 8) | temp_crc;
		}
		tx_mailbox.ul_id = CAN_MID_MIDvA(TEST1_CAN_TRANSFER_ID);
		tx_mailbox.uc_length = MAX_CAN_FRAME_DATA_LEN;
		tx_mailbox.ul_datah =0;
		can_mailbox_write(CAN0, &tx_mailbox);
	/* Send out the information in the mailbox. */
	can_global_send_transfer_cmd(CAN0, CAN_TCR_MB5);
	
	//on_off_f=!on_off_f;
	//ioport_set_pin_level(Amerex_B,on_off_f);

		}
		
	else if((datahigh[0] == 2) && (datahigh[1] == 2) && (datahigh[2] == 2) && (datahigh[3] == 2) && state == initiate)
	{
		ioport_set_pin_level(Amerex_R,0);
		ioport_set_pin_level(Amerex_G,1);
		ioport_set_pin_level(Amerex_B,0);
		File_size = remFileSize =	 (datalow[2]<<16 | datalow[1]<<8 | datalow[0]);

        temp_crc=0;
		temp_crc= crc8_cal((uint8_t *)datalow, 3);
		if ((datalow[3] == temp_crc) && (remFileSize>2000u))
		{
			flashPos = 0;
			ota_start =1;
			remFileSize =remFileSize;
			falsh_state=at25dfx_erase_block_OTA(0);
			ioport_set_pin_level(Amerex_R,0);
	
		for(int i=0;i<3;i++)
		CAN_Res_pkt.CANRESP[i]    = datalow[i];
		tx_mailbox.ul_datal = (CAN_Res_pkt.CANRESP_ST.CANRespL_1 << 8) | temp_crc;
		tx_mailbox.uc_length = 8;
		}
		else
		{
	    tx_mailbox.ul_datal = (0xCCEE << 8) | temp_crc;	
		tx_mailbox.uc_length = 8;
		}
		
		tx_mailbox.ul_id = CAN_MID_MIDvA(3);
		tx_mailbox.ul_datah =0;
		can_mailbox_write(CAN0, &tx_mailbox);
		can_global_send_transfer_cmd(CAN0,CAN_TCR_MB5);
		printf("File_size %lu\n",remFileSize);
	}
	
		if((datahigh[0] == 3) && (datahigh[1] == 3) && (datahigh[2] == 3) && (datahigh[3] == 3) && state == initiate)
	{

		ioport_set_pin_level(Amerex_R,0);
		ioport_set_pin_level(Amerex_G,0);
		ioport_set_pin_level(Amerex_B,1);
		RX_imageCRC =	 (datalow[2]<<16 | datalow[1]<<8 | datalow[0]);
		printf("RX_imageCRC %x\n",RX_imageCRC);

		temp_crc=0;
		temp_crc= crc8_cal((uint8_t *)datalow, 3);
		if (datalow[3] == temp_crc)
		{
			for(int i=0;i<3;i++)
			CAN_Res_pkt.CANRESP[i]    = datalow[i];
			tx_mailbox.ul_datal = (CAN_Res_pkt.CANRESP_ST.CANRespL_1 << 8) | temp_crc;

		}
		else
		{
			tx_mailbox.ul_datal = (0xCCEE << 8) | temp_crc;

		}
		tx_mailbox.uc_length = MAX_CAN_FRAME_DATA_LEN;
		tx_mailbox.ul_datah =0;
		tx_mailbox.ul_id = CAN_MID_MIDvA(3);
		
		can_mailbox_write(CAN0, &tx_mailbox);
		
		can_global_send_transfer_cmd(CAN0,CAN_TCR_MB5);
	}
	
	

		else if((datahigh[0] == 4) && (datahigh[1] == 4) && (datahigh[2] == 4) && (datahigh[3] == 4) && state == initiate)
		{
			ioport_set_pin_level(Amerex_R,1);
			ioport_set_pin_level(Amerex_G,1);
			ioport_set_pin_level(Amerex_B,1);

			tx_mailbox.ul_datal = datalow[0];
			tx_mailbox.ul_id = CAN_MID_MIDvA(3);
			tx_mailbox.uc_length = 8;
			tx_mailbox.ul_datah =0;
			printf("%x\r\n", datalow[0]);
			Strcrc = datalow[0];
		   state = imageDownloading;
			can_mailbox_write(CAN0, &tx_mailbox);

			can_global_send_transfer_cmd(CAN0,CAN_TCR_MB5);
		}
		
		       else if( remFileSize!=0 &&  state == imageDownloading)
		       	 {
			       	 // Calculate the size of the merged array
			       	 char size1 = sizeof(datahigh) / sizeof(datahigh[0]);
			       	 char size2 = sizeof(datalow) / sizeof(datalow[0]);
			       	 // Copy elements from array1 to mergedArray
			       	 memcpy(Merge_Arr, datalow, size2 * sizeof(char));
			       	 // Copy elements from array2 to mergedArray
			       	 memcpy(Merge_Arr + size2, datahigh, size1 * sizeof(char));
			       	 temp_crc= crc8_cal((uint8_t *)Merge_Arr, 8);
			       	 if(Strcrc == temp_crc)
			       	 {
				       	 falsh_state=at25dfx_write(Merge_Arr, p_mailbox->uc_length,flashPos);
				       	flashPos +=(remFileSize > p_mailbox->uc_length) ?(p_mailbox->uc_length) : remFileSize;
				       	 remFileSize -=(remFileSize > p_mailbox->uc_length) ?(p_mailbox->uc_length) : remFileSize;
				       	 memset(Merge_Arr, 0, sizeof(Merge_Arr));
							
				       	 //state = crcVerify;
			       	 }
			       	 else if((Strcrc != temp_crc) && (remFileSize!=0))
			       	 {
				       	 //	state = crcVerify;
				       	 printf(" Not SameCRC\r\n");
			       	 }
						state = initiate;
			       	 if (remFileSize == 0u)
			       	 {
				       	 //ota_process_end();
				       	 remFileSize =File_size;
				       	 flashPos=0;
				       	 //otabit=0;
				       	 //ioport_set_pin_level(Amerex_B,0);
				       	 do
				       	 {
					       	 memset(Merge_Arr, 0, sizeof(Merge_Arr));
					       	 
					       	 falsh_state=at25dfx_read(Merge_Arr, sizeof(Merge_Arr),flashPos);
					       	 
					       	 for (int cnt = 0; cnt < 8;  cnt++)
					       	 {
						       	 imageCRC = crc16(imageCRC,Merge_Arr[cnt]);
						       	 //   printf("imageCRC %x\r\n",imageCRC);
						       	 
					       	 }
					       	 flashPos +=(remFileSize > 8) ?(8) : remFileSize;
					       	 remFileSize -=(remFileSize > 8) ?(8) : remFileSize;
					       	 
					       	 //otabit = ~otabit;
					       	 //ioport_set_pin_level(Amerex_G,otabit);
					       	 // printf("\n");
					       	 wdt_restart(WDT);
					       	 
				       	 } while (remFileSize>0);
				       	 printf("Final imageCRC %x \n RX_imageCRC %x\r\n",imageCRC,RX_imageCRC);
				       	 
				       	 
				       	 
				       	 regions_info_t info;
				       	 void *info_addr;
				       	 if (imageCRC == RX_imageCRC)
				       	 {
					       	 info.length = File_size;	//flashpos
					       	 info.trigger = TRIGGER_BOOTLAOAD;
					       	 info.OAD_image_CRC = imageCRC;
				       	 }
				       	 else
				       	 {
					       	 info.length = 0;
					       	 info.trigger = 0;
					       	 info.OAD_image_CRC = 0;
				       	 }
				       	 wdt_restart(WDT);
				       	 //memory_init();
				       	 info_addr = (void *)INFO_ADDR(0);
				       	 //	printf("write info area ...\r\n");
				       	 memory_erase(info_addr, INFO_SIZE);
				       	 memory_write(info_addr, &info);

				       	 rstc_start_software_reset(RSTC);
				       	 //rstc_start_software_reset(rstc);
				       	 //rstc_start_software_reset(rstc);
			       	 }
			       	 

			       	 ota_process_start(&temp_crc);

		       	 }
								/* Enable CAN1 mailbox interrupt. */
								can_enable_interrupt(CAN0, 1 << CAN_COMM_RXMB_ID);
								//can_enable_interrupt(CAN0, CAN_IER_MB1);
								cpu_irq_enable();
								/* Configure and enable interrupt of CAN1. */
								NVIC_EnableIRQ(CAN0_IRQn);
								g_ul_recv_status =0;	


		       	 }
	

								

/**
 * \brief Decode CAN messages.
 *
 *  \param p_mailbox Pointer to CAN Mailbox structure.
 */
  void ota_process_start(uint8_t *BinCRC)
  {
	  /* Write transmit information into mailbox. */
	  tx_mailbox.ul_id = CAN_MID_MIDvA(3);	//j1939_pgn(TP_DATA_PGN);
	  //tx_mailbox.ul_datal = CAN_MSG_DUMMY_DATA;
	  tx_mailbox.uc_length = 1;
	   tx_mailbox.ul_datal = *BinCRC;
	   tx_mailbox.ul_datah = 0;
	  can_mailbox_write(CAN0, &tx_mailbox);

	  /* Send out the information in the mailbox. */
	  can_global_send_transfer_cmd(CAN0,CAN_TCR_MB5);	// CAN_TCR_MB1




  }

  void ota_process_end(void)
  {

	  /* Write transmit information into mailbox. */
	  tx_mailbox.ul_id = CAN_MID_MIDvA(2);

	  tx_mailbox.uc_length = 2;


	  tx_mailbox.ul_datal = 61149u;
	  can_mailbox_write(CAN0, &tx_mailbox);

	  /* Send out the information in the mailbox. */
	  can_global_send_transfer_cmd(CAN0,CAN_TCR_MB1);	// CAN_TCR_MB1

  }


void Timer0_init()
    {
	    pmc_enable_periph_clk(ID_TC);
	    tc_init(TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK5 | TC_CMR_CPCTRG);
	    tc_write_rc(TC0, 0, BOARD_FREQ_SLCK_XTAL);
	    tc_enable_interrupt(TC, CHANNEL, TC_IER_CPCS);
	    NVIC_ClearPendingIRQ(TC_IRQn);
	    NVIC_SetPriority(TC_IRQn,0);
	     NVIC_EnableIRQ(TC_IRQn);

    }


 void SysTick_Handler(void)
 {
	 count_1ms++;
 }

   void TC_Handler(void)
   {

	   if (tc_get_status(TC0, 0) & TC_SR_CPCS)
	   {
		   Timer_cnt_msec ++;
		   if (Timer_cnt_msec>=57u)
		   {
			   	Timer_cnt_msec =0;
				   Timer_cnt_sec++;

			   if (Timer_cnt_sec>=30)
			   {
				   Timer_cnt_sec=0;
				   
					printf("Stop _timer0\n");
					alarm_sil_flag_g_b = 0u;
					tc_stop(TC0,0);
					

			   }

		   }

	   }

   }






// configure gpio///////////////
// ioport_configure_port_pin(&PORTD, PIN1_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);



/*
 * -# Build the program
 * The bootloader code and application code are compiled separately. The factory code should
 * contain both bootloader code and application code. So, it is required to combine
 * both the images (.hex) before programing on the board. Open Bootloader example
 * (HOST_FIRMWARE_UPGRADE_BOOTLOADER_EXAMPLE) from ASF. Compile and generate .hex file.
 * Combine application .hex (this project) and bootloader .hex using srec_cat
 * (refer: https://sourceforge.net/projects/srecord/files/srecord/) and download the
 * combined image into the board.
 * -# On the computer, open and configure a terminal application as the follows.
 */