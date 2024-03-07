/*****************************************************************************/
/**
* @file RV3028.h
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

#ifndef RV3028_H_
#define RV3028_H_

 #include "RV3028_Defs.h"
 
 typedef struct
 {
	 uint8_t     Second;     // Second (0 to 59)
	 uint8_t     Minute;     // Minute (0 to 59)
	 uint8_t     Hour;       // Hour (0 to 23 in 24-hour mode)
	 uint8_t     DayWeek;        // Day of month (1 to 31)
	 uint8_t     Day;    // Day of week (1 to 7)
	 uint8_t     Month;      // Month (1 to 12)
	 uint8_t     Year;       // Year (0 to 99)
 } RV3032_TimeDate;
 
  rv3028_error_t RV3032_BACKUP_SOURCE(rv3028_init_t* p_Init, rv3028_t* p_Device);

 rv3028_error_t RV3028_SetTime(rv3028_t* p_Device, RV3032_TimeDate *TimeDate);

 __int32_t RV3028_GetTime(rv3028_t* p_Device);

  rv3028_error_t RV3028_Interface(rv3028_t* p_Device);

#endif /* RV3028_H_ */