/*
 * interrupt_handler.h
 *
 * Created: 12-07-2022 15:21:13
 *  Author: p.dharmaraj
 */


#ifndef INTERRUPT_HANDLER_H_
#define INTERRUPT_HANDLER_H_
 #define TC			TC0
 #define CHANNEL		0
 #define ID_TC		ID_TC0
 #define TC_Handler  TC0_Handler
 #define TC_IRQn     TC0_IRQn

extern volatile uint32_t count_1ms;
extern volatile uint32_t g_ul_recv_status;
#define IRQ_PRIOR_PIO    0
extern uint8_t g_ul_log_en;
extern uint8_t ota_start;
extern volatile uint8_t Timer_cnt_msec;
extern volatile uint8_t Timer_cnt_sec;

typedef enum
{
	Amerex_rtc_set,
	Amerex_config_set,
	Amerex_Boardno_set,
	Amerex_get_config,
	Amerex_Get_Log,
	Amerex_Software_RST,
	Amerex_packet_end
} Amerex_packet_type_t;


typedef union
{
	 uint8_t CANRESP[10];
	struct
	{
		uint32_t CANRespL_1;
		uint32_t CANRespH_1;
	}CANRESP_ST;
}CAN_Res_pkt_t;
CAN_Res_pkt_t CAN_Res_pkt;


void ota_process_start(uint8_t *);
void ota_process_end(void);
//void switch_image(uint32_t u32Size);
void Timer0_init(void);
//void region_info_write(regions_info_t info_d);
uint16_t crc16(uint16_t crc, unsigned char val);
#endif /* INTERRUPT_HANDLER_H_ */