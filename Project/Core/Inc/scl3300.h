#ifndef APPLICATION_USER_INC_SCL3300_H_
#define APPLICATION_USER_INC_SCL3300_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"
#include <stdint.h>

  struct SCL{

    double ACC_X;
    double ACC_Y;
    double ACC_Z;
    uint16_t STO;
    double ANG_X;
    double ANG_Y;
    double ANG_Z;
    double TEMP;
  };

  // Change SPI handle and CS pins according to your hardware setup
#define SCL_SPI					(&hspi4)
#define SPI_CS					GPIOD
#define SPI_CS_PIN				GPIO_PIN_5

#define RS_STARTUP 				0x00
#define RS_NORMAL  				0x01
#define RS_SELFTEST   			0x02
#define RS_ERR  				0x03

#define READ_ACC_X	 			0x040000F7
#define READ_ACC_Y 				0x080000FD
#define READ_ACC_Z 				0x0C0000FB
#define READ_STO  				0x100000E9
#define	ENABLE_ANG_OUTPUTS		0xB0001F6F
#define READ_ANG_X				0x240000C7
#define READ_ANG_Y				0x280000CD
#define READ_ANG_Z				0x2C0000CB
#define READ_TEMPERATURE		0x140000EF
#define READ_STATUS_SUMMARY 	0x180000E5
#define	READ_ERR_FLAG1			0x1C0000E3
#define	READ_ERR_FLAG2			0x200000C1
#define READ_CMD				0x340000DF
#define CHANGE_TO_MODE1			0xB400001F
#define CHANGE_TO_MODE2			0xB4000102
#define CHANGE_TO_MODE3			0xB4000225
#define CHANGE_TO_MODE4			0xB4000338
#define SET_POWER_DOWN_MODE 	0xB400046B
#define WKUP_FROM_POW_DOWN 		0xB400001F
#define SW_RESET				0xB4002098
#define READ_WHOAMI  			0x40000091
#define	READ_SERIAL1			0x640000A7
#define READ_SERIAL2			0x680000AD
#define READ_CURRENT_BANK		0x7C0000B3
#define	SWITCH_TO_BANK0			0xFC000073
#define SWITCH_TO_BANK1			0xFC00016E
#define WHOAMI_VALUE 			0xC1

uint8_t SCL3300_Init();

uint8_t write_reg(uint32_t command);
uint8_t read_reg(uint32_t command, uint32_t *data);
uint8_t CalculateCRC(uint32_t Data);

uint8_t Acc_X(struct SCL *dev);
uint8_t Acc_Y(struct SCL *dev);
uint8_t Acc_Z(struct SCL *dev);
uint8_t Ang_X(struct SCL *dev);
uint8_t Ang_Y(struct SCL *dev);
uint8_t Ang_Z(struct SCL *dev);
uint8_t Temp(struct SCL *dev);
uint8_t STO_Sensor(struct SCL *dev);



#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* APPLICATION_USER_INC_SCL3300_H_ */
