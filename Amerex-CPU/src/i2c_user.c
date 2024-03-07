/*
 * i2c_user.c
 *
 * Created: 09-08-2022 11:09:21
 *  Author: p.dharmaraj
 */
#include "main_header.h"
#include "RV3028.h"
#include "RV3028_Defs.h"

 
 /** TWI Bus Clock 400kHz */
 #define TWI_CLK     100000

/** I2C related structure */
 twi_options_t opt;
 twi_packet_t packet_rx,packet_tx;
 
   rv3028_init_t _RV3028_RTC_Init = {
	   // Use this settings to enable the battery backup
	   .BatteryMode = RV3028_BAT_DSM,  //	 		,//	  	RV3028_BAT_LSM				 RV3028_BAT_DSM

   };
  
 rv3028_error_t RV3028_Write(uint8_t Device_Addr, uint8_t Reg_Addr,  uint8_t* p_Reg_Data, uint32_t Length)
 {
	  uint8_t Resp = RV3028_NO_ERROR;

	 packet_tx.chip = Device_Addr;
	 // TWI address/commands to issue to the other chip (node)
	 packet_tx.addr[0] = Reg_Addr;

	 // Length of the TWI data address segment (1-3 bytes)
	 packet_tx.addr_length = 1;
	 // Where to find the data to be written
	 packet_tx.buffer =  p_Reg_Data;
	 // How many bytes do we want to write
	 packet_tx.length = Length;

	 // perform a write access
	 Resp = twi_master_write(EDBG_I2C_MODULE, &packet_tx);

	 return Resp;
 }

 rv3028_error_t RV3028_Read(uint8_t Device_Addr, uint8_t Reg_Addr, uint8_t* p_Reg_Data, uint32_t Length)
 {

	 uint8_t Resp = RV3028_NO_ERROR;
	 // TWI chip address to communicate with
	 packet_rx.chip = Device_Addr;
	 // TWI address/commands to issue to the other chip (node)
	 packet_rx.addr[0] = Reg_Addr;

	 // Length of the TWI data address segment (1-3 bytes)
	 packet_rx.addr_length = 1;
	 // Where to find the data to be written
	 packet_rx.buffer =p_Reg_Data;
	 // How many bytes do we want to write
	 packet_rx.length = Length;
		    Delay(100u);
	 // perform a write access
	   Resp = twi_master_read(EDBG_I2C_MODULE, &packet_rx);

	 return Resp;
 }



void configure_i2c_master(void)
{

	/* Enable the peripheral clock for TWI */
	pmc_enable_periph_clk(EDBG_I2C_MODULE_ID);

 (Freq_selec_bit == 0) ? (opt.master_clk = sysclk_get_cpu_hz()) : (opt.master_clk =  4000000UL);
	/* Configure the options of TWI driver */
	opt.speed      = TWI_CLK;

 (twi_master_init(EDBG_I2C_MODULE, &opt) == TWI_SUCCESS)?RV3028_Interface(&_RTC):TWI_NO_CHIP_FOUND;
//	pmc_enable_periph_clk(EDBG_I2C_MODULE_ID);



}

 rv3028_error_t RV3028_Interface(rv3028_t* p_Device)
 {
	// uint8_t Resp = RV3028_NO_ERROR;

	 p_Device->p_Read = RV3028_Read;
	// p_Device->p_Write = RV3028_Write;
	 p_Device->DeviceAddr = RV3028_ADDRESS;

	//Resp= RV3032_BACKUP_SOURCE(&_RV3028_RTC_Init, &_RTC);
   return RV3028_NO_ERROR;
 
 }


