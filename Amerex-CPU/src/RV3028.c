/*****************************************************************************/
/**
* @file RV3028.c
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
* Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
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

#include "RV3028.h"
 #include "main_header.h"

#define RV3028_REG_SECONDS                  0x01

#define RV3032_NCLKE               0x06
#define RV3032_BSM                 0x04
#define RV3032_TCR                 0x02
#define RV3032_TCM                 0x00
#define RV3032_BSIE                0x04
#define RV3032_CONTROL             0x12
#define RV3032_EEPROM_PMU		   0xC0

//static rv3028_error_t ErrorCode;

/** @brief	    Convert a decimal value into a BCD value.
 *  @param Decimal  Decimal value.
 *  @return	    BCD value.
 */
static inline uint8_t DecimalToBCD(uint8_t Decimal)
{
   return ((Decimal / 0x0A) << 0x04) | (Decimal % 0x0A);
}

/** @brief	    Convert a BCD value into a decimal value.
 *  @param Decimal  BCD value.
 *  @return	    Decimal value.
 */
static inline uint8_t BCDToDecimal(uint8_t BCD)
{
   return ((BCD >> 0x04) * 0x0A) + (BCD & 0xF);
}

/** @brief		Read the content of one or more register from the RV3028.
 *  @param Reg_Addr	Register address.
 *  @param p_Reg_Data	Pointer to register data.
 *  @param Length	Data length.
 *  @param p_Device	Pointer to \ref rv3028_t device structure.
 *  @return		Communication error code.
 */
static rv3028_error_t RV3028_ReadRegister(uint8_t Reg_Addr, uint8_t* p_Reg_Data, uint32_t Length, rv3028_t* p_Device)
{

    return p_Device->p_Read(p_Device->DeviceAddr, Reg_Addr, p_Reg_Data, Length);
}

/** @brief		Write one or more data bytes into the RV3028.
 *  @param Reg_Addr	Register address.
 *  @param p_Reg_Data	Pointer to register data.
 *  @param Length	Data length.
 *  @param p_Device	Pointer to \ref rv3028_t device structure.
 *  @return		Communication error code.
 */
static rv3028_error_t RV3028_WriteRegister(uint8_t Reg_Addr, const uint8_t* p_Reg_Data, uint32_t Length, rv3028_t* p_Device)
{

    return p_Device->p_Write(p_Device->DeviceAddr, Reg_Addr, p_Reg_Data, Length);
}

/** @brief          Modify the value of a single register.
 *  @param Address  Register address.
 *  @param Mask	    Bit mask.
 *  @param Value    New value for masked bits.
 *  @param p_Device Pointer to \ref rv3028_t device structure.
 *  @return	    Communication error code.
 */
static rv3028_error_t RV3028_ModifyRegister(uint8_t Address, uint8_t Mask, uint8_t Value, rv3028_t* p_Device)
{
    uint8_t Temp = 0x00;
    uint8_t Addr_Temp = Address;

    ErrorCode = RV3028_ReadRegister(Addr_Temp, &Temp, sizeof(Temp), p_Device);
    if(ErrorCode != RV3028_NO_ERROR)
    {
	return ErrorCode;
    }

    Temp &= ~Mask;
    Temp |= Value & Mask;

    return RV3028_WriteRegister(Addr_Temp, &Temp, sizeof(Temp), p_Device);
}



// | ((p_Init->Resistance & 0x03) << RV3032_TCR) | (p_Init->TSMode & 0x03)

rv3028_error_t RV3032_BACKUP_SOURCE(rv3028_init_t* p_Init, rv3028_t* p_Device)
{
	ErrorCode = RV3028_ModifyRegister(RV3032_EEPROM_PMU,
	(0x01 << RV3032_NCLKE) | (0x03 << RV3032_BSM) | (0x03 << RV3032_TCR) |  (0x03 <<RV3032_TCM), (  (0<<6)|(p_Init->BatteryMode & 0x03) << 0x04) | (3<<2) | (0<< 0u) , p_Device);

	if(ErrorCode != RV3028_NO_ERROR)
	{
		return ErrorCode;
	}
	
	return RV3028_NO_ERROR;
}

rv3028_error_t RV3028_SetTime(rv3028_t* p_Device, RV3032_TimeDate *TimeDate)
{
    uint8_t tmp[7];
   
    /* Convert time values to BCD format used by the RTC */
    tmp[0] = DecimalToBCD(TimeDate->Second);
    tmp[1] = DecimalToBCD(TimeDate->Minute);
    tmp[2] = DecimalToBCD(TimeDate->Hour);
    tmp[3] = TimeDate->DayWeek;
    tmp[4] =  DecimalToBCD(TimeDate->Day);
    tmp[5] = DecimalToBCD(TimeDate->Month);
    tmp[6] = DecimalToBCD(TimeDate->Year);

    return RV3028_WriteRegister(RV3028_REG_SECONDS, tmp, sizeof(tmp), p_Device);
}

__int32_t RV3028_GetTime(rv3028_t* p_Device)
{
    uint8_t Temp[7];
	RV3032_TimeDate TimeDate;
	  struct tm t;


    ErrorCode = RV3028_ReadRegister(RV3028_REG_SECONDS, Temp, sizeof(Temp), p_Device);
    if(ErrorCode != RV3028_NO_ERROR)
    {
	return ErrorCode;
    }

    TimeDate.Second = BCDToDecimal(Temp[0]);
    TimeDate.Minute = BCDToDecimal(Temp[1]);
    TimeDate.Hour = BCDToDecimal(Temp[2]);
    TimeDate.DayWeek = BCDToDecimal(Temp[3]);
    TimeDate.Day = BCDToDecimal(Temp[4]);
    TimeDate.Month = BCDToDecimal(Temp[5]);
    TimeDate.Year = BCDToDecimal(Temp[6]);
	 //   printf(" %u:%u:%u,%u/%u/%u\r\n",TimeDate.Hour, TimeDate.Minute, TimeDate.Second,TimeDate.Day, TimeDate.Month,TimeDate.Year);
    //Convert to Unix tims stamp
    t.tm_year = (2000+TimeDate.Year)-1900;  // Year - 1900
    t.tm_mon = TimeDate.Month-1;           // Month, where 0 = jan
    t.tm_mday = TimeDate.Day;          // Day of the month
    t.tm_hour =TimeDate.Hour;
    t.tm_min =TimeDate.Minute;
    t.tm_sec = TimeDate.Second;
    t.tm_isdst = -1;
	
	return ((__int32_t)mktime(&t)<1695887402)?0:(__int32_t)mktime(&t); //Time invalid - If less than Tuesday, December 31,
 
}


 void RV3028_Set_TimeStamp (uint32_t t_of_day)
 {
	 if(t_of_day<1695887402)  //Time invalid - If less than THUS, APRIL 04, 2023 16:08:15 PM
	 return;
	 struct tm  ts;
	 time_t timeStamp;
	 timeStamp = (time_t)t_of_day;
	 RV3032_TimeDate TimeDate;
	 ts = *localtime(&timeStamp);

	TimeDate.Hour=ts.tm_hour;
	TimeDate.Minute=ts.tm_min;
	TimeDate.Second=ts.tm_sec;
	TimeDate.Day=ts.tm_mday;
	TimeDate.Month=ts.tm_mon+1;
	TimeDate.Year=(ts.tm_year-2000)+1900;
	RV3028_SetTime(&_RTC,&TimeDate);

 }
