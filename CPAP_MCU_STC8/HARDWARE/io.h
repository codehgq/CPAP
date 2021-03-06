#ifndef __IO_H__
#define __IO_H__

#include "common.h"

//硬件版本18A
#ifdef HARDWARE_VERSION_18A
//misc
sbit LEDM = P2^6;
sbit MBEEP = P3^4;

//sensor
//sbit MIN1 = P3^2;
//sbit MIN2 = P3^3;
sbit MIN1 = P3^3;
sbit MIN2 = P3^2;

//stepmotor
sbit STM_PULSE = P3^5;
sbit STM_DIR = P3^6;
sbit STM_EN = P4^5;

//加热
sbit M_DISCHEAT = P0^7;
sbit M_LINEHEAT = P0^5;
sbit I_OVER = P3^7;

//key
sbit KEY_SW1 = P0^2;
sbit KEY_SW2 = P0^1;
sbit KEY_SW3 = P0^3;
sbit KEY_SW4 = P4^3;
sbit KEY_SW5 = P0^4;
sbit KEY_SW6 = P0^0;
sbit KEY_SW7 = P0^6;

//EEPROM
//sbit IIC_WP = P2^7;
//sbit IIC_SCL = P2^5;
//sbit IIC_SDA = P2^4;

//LCD
sbit LCD_DOUT = P1^0;
sbit LCD_DIN = P1^1;
sbit LCD_BUSY = P2^0;

//串口
sbit M_RXD = P3^0;
sbit M_TXD = P3^1;

//AD
sbit M_CLKA = P5^4;
sbit A_RST = P4^4;
sbit A_SCLK = P1^5;
sbit A_DATA = P1^4;
sbit A_CS1 = P4^0;
sbit A_CS2 = P5^5;
sbit A_CS3 = P4^7;
sbit A_DRDY1 = P4^6;
sbit A_DRDY2 = P4^5;
sbit A_DRDY3 = P1^3;
#endif 

//硬件版本24A
#ifdef HARDWARE_VERSION_24A
//misc
sbit LEDM = P2^6;
sbit MBEEP = P3^4;

//sensor
//sbit MIN1 = P3^2;
//sbit MIN2 = P3^3;
sbit MIN1 = P3^3;
sbit MIN2 = P3^2;

//stepmotor
sbit STM_PULSE = P3^5;
sbit STM_DIR = P1^5;
sbit STM_EN = P4^5;

//步进电机驱动芯片DRV8825接口，脉冲等引脚与驱动器共用
sbit S2_SLEEP = P1^4;
sbit S2_MODE0 = P1^3;
sbit S2_MODE1 = P1^2;
sbit S2_MODE2 = P1^1;

//加热
sbit M_DISCHEAT = P0^7;
sbit M_LINEHEAT = P0^5;
sbit I_OVER = P3^6;

//key
sbit KEY_SW1 = P0^1;
sbit KEY_SW2 = P0^0;
sbit KEY_SW3 = P4^3;
sbit KEY_SW4 = P0^2;
sbit KEY_SW5 = P0^3;
sbit KEY_SW6 = P2^7;
sbit KEY_SW7 = P0^4;

//EEPROM
//sbit IIC_WP = P2^3;
//sbit IIC_SCL = P2^5;
//sbit IIC_SDA = P2^4;

//LCD
sbit LCD_DOUT = P4^0;
sbit LCD_DIN = P4^2;

//串口
sbit M_RXD = P3^0;
sbit M_TXD = P3^1;

//AD
sbit M_CLKA = P5^4;
sbit A_RST = P2^1;
sbit A_SCLK = P2^0;
sbit A_DATA = P2^2;
sbit A_CS1 = P3^7;
sbit A_CS2 = P4^7;
sbit A_DRDY1 = P4^1;
sbit A_DRDY2 = P5^5;

//WIFI模块
sbit WIFI_RST = P0^6;
sbit WIFI_EN = P1^0;
#endif 




//=========================================================
#define SW1_MASK 0x01	//氧浓度增加
#define SW2_MASK 0x02	//氧浓度减少
#define SW3_MASK 0x04	//温度增加
#define SW4_MASK 0x08	//温度减少
#define SW5_MASK 0x10	//启动/停止
#define SW6_MASK 0x20	//组合键
#define SW7_MASK 0x40	//静音

typedef struct
{
	uint8_t curKey;
	uint8_t lastKey;
	uint8_t fallingEdge;
	uint8_t rasingEdge;

	uint8_t oxygenChangeCnt;
	uint8_t temperChangeCnt;

//	uint16_t timerUnitCnt;
//	uint8_t timerFlag;
}Key_TypeDef; 

extern Key_TypeDef key;

extern const char OXYGEN_MODE_STRING[];
extern const char RHINOBYON_MODE_STRING[];
extern const char CAPA_MODE_STRING[];
extern const char FLUX_WARNING_STRING[];
extern const char FLUX_WORKING_STRING[];
extern const char CONNECT_WARNING_STRING[];
extern const char CONNECT_WORKING_STRING[];
extern const char OXYCURE_WARNING_STRING[];
extern const char OXYCURE_WORKING_STRING[];
extern const char TEMPER_WARNING_STRING[];
extern const char SILENCE_STATUS_STRING[];
extern const char TILDE_STRING[];
//extern const int16_t TEMPER_ALARM_STRING[];
extern const int8_t TEMPER_ALARM_STRING[];

void IO_Init(void);
void Key_Scan(void);
void Key_Process(void);
void Key_Process_LongPress(void);

#endif