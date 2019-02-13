/*! 
 *  \file hmi_driver.h
 *  \brief �����������ļ�
 *  \version 1.0
 *  \date 2012-2015
 *  \copyright ���ݴ�ʹ��Ƽ����޹�˾
 */

#ifndef _HMI_DRIVER_
#define _HMI_DRIVER_

#define FIRMWARE_VER 917   //��Ļ�̼��汾��

#define CRC16_ENABLE 0         /*!< �����ҪCRC16У�鹦�ܣ��޸Ĵ˺�Ϊ1(��ʱ��Ҫ��VisualTFT��������CRCУ��)*/
#define CMD_MAX_SIZE 20 //64        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define QUEUE_MAX_SIZE 60 //128  /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/

#define LCD_CMD_HEAD	0xA55A

#define USER_CMD_HEAD 0XEE  //֡ͷ
#define USER_CMD_TAIL 0XFFFCFFFF //֡β

#include "common.h"

void SEND_DATA(uint8 c);
void BEGIN_CMD();
void END_CMD();
void SendNU8(uint8 *pData, uint16 nDataLen);
void SendNU16(uint16 *pData, uint16 nDataLen);

#if(CRC16_ENABLE)

void AddCRC16(uint8 *buffer, uint16 n, uint16 *pcrc);

/*!
*  \brief  ��������Ƿ����CRC16У��
*  \param buffer ��У������ݣ�ĩβ�洢CRC16
*  \param n ���ݳ��ȣ�����CRC16
*  \return У��ͨ������1�����򷵻�0
*/
uint16 CheckCRC16(uint8 *buffer, uint16 n);

#endif
void SendStrings(uint8 *str);

void ResetDevice(void);

void GetVersion();

void SetBackLight(uint8 light_level, uint8 poweroff_save);
void GetBackLight();

void SetBuzzerTime(uint8 t);

void CalibrateTouchPane(void);

void SetScreen(uint16 screen_id);
void GetScreen(void);

void SetRTC(uint8 year, uint8 mon, uint8 date, uint8 hour, uint8 min, uint8 sec);
void GetRTC(void);

void SetTextValue(uint16 addr, char *str);
void SetTextValueLen(uint16 addr, char *str, uint8_t len);
void GetTextValue(uint16 addr, uint8 len);

void SetTextFontColor(uint16 vpAddr, uint16 color);


#endif
