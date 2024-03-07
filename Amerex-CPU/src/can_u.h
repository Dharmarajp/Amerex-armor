/*
 * can_u.h
 *
 * Created: 05-12-2022 11:43:34
 *  Author: p.dharmaraj
 */


#ifndef CAN_U_H_
#define CAN_U_H_


#define STRING_EOL    "\r"
#define CAN_COMM_RXMB_ID    0
#define CAN_COMM_TXMB_ID    5
#define CAN_TX_PRIO         15
#define CAN_TRANSFER_ID     7
/** The transfer data length */
#define CAN_TRANSFER_DATA_LEN         8

#define PGN_MODULE_1 64347  //Vehicle Fire Suppression System Module 1 Detector Data

#define PGN_MODULE_2 64348 //Vehicle Fire Suppression System Module 2 Detector Data

#define PGN_AUXILLIARY_1 ((uint16_t) 64351u) //Vehicle Fire Suppression System Module 1 Auxilliary Data

#define PGN_AUXILLIARY_2 64352 //Vehicle Fire Suppression System Module 2 Auxilliary Data

#define TP_PGN ((uint16_t) 60416u) //PGN for Transfer protocol

#define BROAD_CAST ((uint8_t) 32u)

#define TP_DATA_PGN ((uint16_t) 60160u) //PGN for data transfer in transport protocol

/** CAN frame max data length */
#define MAX_CAN_FRAME_DATA_LEN      8

/** Receive status */
extern volatile uint32_t primary_pwr_data,secondary_pwr_data,hd2_data,hd1_data;
extern uint32_t can_identifier ; // used to store 29 bit CAN identifier
extern uint32_t set_can_baudrate;

//void read_data(void);
void trnsfer_protocol_connection(uint32_t identifier, uint16_t data_byte, uint8_t packet_size, uint32_t pgn_number);
void j1939_data_read(void);
uint32_t j1939_pgn(uint16_t pgn_number);
void CAN_Open(void);
void CAN_process(void);
 bool decode_can_msg( can_mb_conf_t *p_mailbox);
 /*Used to Set Time In Unix Stamp */
void RV3028_Set_TimeStamp (uint32_t t_of_day);
void j1939_data_transfer(uint32_t , uint8_t);
 void reset_mailbox_conf(can_mb_conf_t *p_mailbox);
 void DFU_callback_Fun( can_mb_conf_t *p_mailbox);
extern can_mb_conf_t tx_mailbox, rx_mailbox;
// ///////////////////////////////////////////////
typedef union
{
	uint8_t MtrRawData[10];
	struct
	{
		uint32_t datal_1;
		uint32_t datah_1;
	}Para;
}MeterData_t;
MeterData_t MeterData;
/////////////////////////////////////////////////////

typedef union
{
	unsigned char TCPCON[10];
	struct
	{
		uint32_t TPCl_1;
		uint32_t TPCh_1;
	}GET_TPC;
}TPC_t;
TPC_t TPC;

struct format
{
	uint16_t Priority :3;    // Priority for CAN message
	uint16_t DataPage :1;    //J1939 data page
	uint16_t Res      :1;    //used for reserved purpose
	uint8_t  SourceAddress;  //J1939 source address
	uint16_t DataLength : 4; //data length
	uint8_t	Data[8];         // data
};


struct pgn1
{
	uint8_t sequence_num;
	uint8_t data[25];
};
///////////////////////////////////////////////////////////
#endif /* CAN_U_H_ */