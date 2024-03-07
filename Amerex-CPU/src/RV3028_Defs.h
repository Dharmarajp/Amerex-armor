/*****************************************************************************/
/**
* @file RV3028_Defs.h
*
* Micro Crystal RV3028 I2C extreme low power RTC driver.
*
* GNU GENERAL PUBLIC LICENSE:
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---  --------    -----------------------------------------------
* 1.00  dk   03/11/2020  First release
*
* </pre>
******************************************************************************/

#ifndef RV3028_DEFS_H_
#define RV3028_DEFS_H_

 #include <time.h>
 #include <stdint.h>
 #include <stddef.h>
 #include <stdbool.h>

 /**@brief RV3028 I2C device slave address.
  */
 #define RV3028_ADDRESS                      	0x51	// 0xDE//

 typedef enum
 {
    RV3028_NO_ERROR		= 0x00,			    /**< No error. */
    RV3028_INVALID_PARAM	= 0x01,			    /**< Invalid parameter passed to function call. */
    RV3028_TIMEOUT		= 0x02,			    /**< Communication timeout. */
    RV3028_NOT_INITIALIZED	= 0x03,			    /**< Device is not initialized. Please call the
								 RV3028_Init function. */
    RV3028_NOT_READY		= 0x04,			    /**< Device function is not ready. Please initialize them first. */
    RV3028_WP_ACTIVE		= 0x05,			    /**< Device is write protected. Please unprotect the device first. */
    RV3028_COMM_ERROR		= 0x06,			    /**< Communication error. */
 } rv3028_error_t;

 typedef enum
 {
    RV3028_BAT_DISABLED		= 0x03,			    /**< Battery switchover disabled. */
    RV3028_BAT_DSM		= 0x01,			    /**< Battery direct switching mode (DSM). */
    RV3028_BAT_LSM		= 0x02,			    /**< Battery level switching mode (LSM). */
 } rv3028_bat_t;


 /**@brief		Bus communication function pointer which should be mapped to the platform specific read functions of the user.
  * @param Device_Addr	I2C device address.
  * @param Reg_Addr	Register address.
  * @param Reg_Data	Data from the specified address.
  * @param Length	Length of the reg_data array.
  * @return		Communication error code.
  */
 typedef uint8_t (*rv3028_read_fptr_t)(uint8_t Device_Addr, uint8_t Reg_Addr, uint8_t* p_Reg_Data, uint32_t Length);

 /**@brief		Bus communication function pointer which should be mapped to the platform specific write functions of the user.
  * @param Device_Addr	I2C device address.
  * @param Reg_Addr	Register address.
  * @param Reg_Data	Data to the specified address.
  * @param Length	Length of the reg_data array.
  * @return		Communication error code.
  */
 typedef uint8_t (*rv3028_write_fptr_t)(uint8_t Device_Addr, uint8_t Reg_Addr, const uint8_t* p_Reg_Data, uint32_t Length);

 /**@brief RV3028 device initialization object structure.
  */
 typedef struct
 {
    rv3028_bat_t	BatteryMode;			    /**< Battery mode used by the RV3028. */
 } rv3028_init_t;

 /**@brief RV3028 device object structure.
  */
 typedef struct
 {
	uint8_t		DeviceAddr;			    /**< RTC device address. */
	rv3028_bat_t	BatteryMode;			    /**< Battery mode used by the RV3028. */
	rv3028_read_fptr_t	p_Read;				    /**< Pointer to RV3028 I2C read function. */
	rv3028_write_fptr_t p_Write;			    /**< Pointer to RV3028 I2C write function. */
 } rv3028_t;


#endif /* RV3028_DEFS_H_ */
