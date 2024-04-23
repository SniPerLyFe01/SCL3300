#include "scl3300.h"


/* Static Functions */
static void cs_high()
{
	HAL_GPIO_WritePin(SPI_CS, SPI_CS_PIN, SET);
}
static void cs_low()
{
	HAL_GPIO_WritePin(SPI_CS, SPI_CS_PIN, RESET);
}
static uint8_t CRC8(uint8_t BitValue, uint8_t CRC_)
{
	uint8_t Temp;
	Temp = (uint8_t)(CRC_ & 0x80);
	if (BitValue == 0x01)
	{
		Temp ^= 0x80;
	}
	CRC_ <<= 1;
	if (Temp > 0)
	{
		CRC_ ^= 0x1D;
	}
	return CRC_;
}

uint8_t SCL3300_Init(){

    uint8_t rslt;
    uint32_t value;

    // Power On time
//    HAL_Delay(200);
    rslt = write_reg(WKUP_FROM_POW_DOWN);
    HAL_Delay(1);
    rslt = write_reg(SW_RESET);
    HAL_Delay(1);
    rslt = write_reg(CHANGE_TO_MODE2);
    rslt = write_reg(ENABLE_ANG_OUTPUTS);
    HAL_Delay(20);
    rslt = read_reg(READ_CMD, &value);
    HAL_Delay(1);
    rslt = read_reg(READ_STATUS_SUMMARY, &value);
    HAL_Delay(1);
    rslt = read_reg(READ_STATUS_SUMMARY, &value);
    HAL_Delay(1);
    rslt = read_reg(READ_STATUS_SUMMARY, &value);
    HAL_Delay(1);
    rslt = read_reg(READ_CMD, &value);
    HAL_Delay(1);
    rslt = write_reg(SWITCH_TO_BANK0);
    HAL_Delay(1);
    rslt = read_reg(READ_WHOAMI, &value);
    HAL_Delay(1);
    rslt = read_reg(READ_CMD, &value);

  return rslt;
}
uint8_t write_reg(uint32_t command)
{
	uint8_t read_frame[4];
	read_frame[0] = (command>>24) & 0xFF;
	read_frame[1] = (command>>16) & 0xFF;
	read_frame[2] = (command>>8) & 0xFF;
	read_frame[3] =  command & 0xFF;

	cs_high();
//	HAL_Delay(1);
	cs_low();
	HAL_SPI_Transmit(SCL_SPI, (uint8_t *)read_frame, 4, 1000);
	cs_high();
	return 0;
}
uint8_t read_reg(uint32_t command, uint32_t *data)
{
	uint8_t  CRC_Check;
	uint8_t  read_frame[4];
	uint32_t Rxdata;

	read_frame[0] = (command>>24) & 0xFF;
	read_frame[1] = (command>>16) & 0xFF;
	read_frame[2] = (command>>8) & 0xFF;
	read_frame[3] =  command & 0xFF;

	uint8_t reg_val[4];
	cs_high();
//	HAL_Delay(1);
	cs_low();
	HAL_SPI_Transmit(SCL_SPI, (uint8_t *)read_frame, 4, 1000);

	cs_high();
//	HAL_Delay(1);
	cs_low();

	HAL_SPI_Receive (SCL_SPI, (uint8_t *)reg_val, 4, 1000);
	cs_high();
	Rxdata = ((uint32_t)reg_val[0] << 24) | ((uint32_t)reg_val[1] << 16)
			| ((uint32_t)reg_val[2] << 8) | reg_val[3];

	*data = (Rxdata & 0xFFFFFF00) >> 8;
	CRC_Check = CalculateCRC(Rxdata);
	if(CRC_Check != reg_val[3])
	  {
	    return -1;
	  }

	return 0;
}
uint8_t CalculateCRC(uint32_t Data)
{
	uint8_t BitIndex;
	uint8_t BitValue;
	uint8_t CRC_;
	CRC_ = 0xFF;
	for (BitIndex = 31; BitIndex > 7; BitIndex--)
	{
		BitValue = (uint8_t)((Data >> BitIndex) & 0x01);
		CRC_ = CRC8(BitValue, CRC_);
	}
	CRC_ = (uint8_t)~CRC_;
	return CRC_;
}

uint8_t Acc_X(struct SCL *dev){

  uint8_t rslt;
      uint32_t value;
      uint8_t RC_check;
      double data;
      uint32_t garbage;
      rslt = read_reg(READ_STATUS_SUMMARY, &garbage);
//      HAL_Delay(1);

      rslt = read_reg(READ_STATUS_SUMMARY, &garbage);
//      HAL_Delay(1);

      rslt = read_reg(READ_ACC_X, &value);

      RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
      RC_check = RC_check << 6;
      RC_check = RC_check >> 6;
if(RC_check == RS_NORMAL)
{

      value = value & 0xFFFF;

      data = value / 12000;
      dev-> ACC_X = data;
}else{
      return -1;
}
  return rslt;
}
uint8_t Acc_Y(struct SCL *dev){

  uint8_t rslt;
      uint32_t value;
      uint8_t RC_check;
      double data;

      rslt = read_reg(READ_ACC_Y, &value);

      RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
      RC_check = RC_check << 6;
      RC_check = RC_check >> 6;
if(RC_check == RS_NORMAL)
{

      value = value & 0xFFFF;

      data = value / 12000;
      dev-> ACC_Y = data;
}else{
      return -1;
}
  return rslt;
}
uint8_t Acc_Z(struct SCL *dev){

  uint8_t rslt;
      uint32_t value;
      uint8_t RC_check;
      double data;

      rslt = read_reg(READ_ACC_Z, &value);

      RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
      RC_check = RC_check << 6;
      RC_check = RC_check >> 6;
if(RC_check == RS_NORMAL)
{

      value = value & 0xFFFF;

      data = value / 12000;
      dev-> ACC_Z = data;
}else{
      return -1;
}
  return rslt;
}
uint8_t Ang_X(struct SCL *dev){

  uint8_t rslt;
      uint32_t value;
      uint8_t RC_check;
      double data;

      rslt = read_reg(READ_ANG_X, &value);

      RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
      RC_check = RC_check << 6;
      RC_check = RC_check >> 6;
if(RC_check == RS_NORMAL)
{

      value = value & 0xFFFF;
      data = value / 182;
      dev-> ANG_X = data;
}else{
      return -1;
}
  return rslt;
}
uint8_t Ang_Y(struct SCL *dev){

  uint8_t rslt;
      uint32_t value;
      uint8_t RC_check;
      double data;

      rslt = read_reg(READ_ANG_Y, &value);

      RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
      RC_check = RC_check << 6;
      RC_check = RC_check >> 6;
if(RC_check == RS_NORMAL)
{

      value = value & 0xFFFF;
      data = value / 182;
//      data = (value / 16384)*90;
      dev-> ANG_Y = data;
}else{
      return -1;
}
  return rslt;
}
uint8_t Ang_Z(struct SCL *dev){

  uint8_t rslt;
      uint32_t value;
      uint8_t RC_check;
      double data;

      rslt = read_reg(READ_ANG_Z, &value);

      RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
      RC_check = RC_check << 6;
      RC_check = RC_check >> 6;
if(RC_check == RS_NORMAL)
{

      value = value & 0xFFFF;

      data = value / 182;
//      data = (value / 16384)*90;
      dev-> ANG_Z = data;
}else{
      return -1;
}
  return rslt;
}
uint8_t Temp(struct SCL *dev){
    uint8_t rslt;
    uint32_t value;
    uint8_t RC_check;
    double data;

    rslt = read_reg(READ_TEMPERATURE, &value);

    RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
    RC_check = RC_check << 6;
    RC_check = RC_check >> 6;
 if(RC_check == RS_NORMAL){

     value = value & 0xFFFF;

     data = (-273) + (value / 18.9);
      dev-> TEMP = data;

      }else{
          return -1;
      }
  return rslt;
}
uint8_t STO_Sensor(struct SCL *dev){
    uint8_t rslt;
    uint32_t value;
    uint8_t RC_check;
    uint16_t data = 1;

    rslt = read_reg(READ_STO, &value);
    RC_check = ((value & 0xFF0000) >> 16) & 0xFF;
    RC_check = RC_check << 6;
    RC_check = RC_check >> 6;
 if(RC_check == RS_NORMAL){

     value = value & 0xFFFF;

     data = value;
     dev-> STO = data;
      }else{
          return -1;
      }
  return rslt;
}


