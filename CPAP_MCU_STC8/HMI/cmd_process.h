#ifndef _CMD_PROCESS_H
#define _CMD_PROCESS_H
#include "hmi_driver.h"

#define CMD_PARAMETER_SETTING			0X81  //���ò���
#define CMD_MOTORINIT_START				0X82  //��������ʼ��
#define CMD_MOTORINIT_STOP				0X83  //ֹͣ����ʼ��
#define CMD_WAVECONTROL_SETTING			0X84  //�趨����������
#define CMD_MOTORSTEP_TURNING			0X85  //���ת��ָ������
#define CMD_MOTOR_TURNING				0X86  //��������Զ�ת��
#define CMD_PARAMETER_READ				0X88  //��ȡ����
#define CMD_VERSION_READ                0X89  //��ȡ�ֿ�����汾
#define CMD_AU_READ		                0X8A  //��ȡAUֵ


#define PTR2U16(PTR) ((((uint8 *)(PTR))[0]<<8)|((uint8 *)(PTR))[1])  //�ӻ�����ȡ16λ����
#define PTR2U32(PTR) ((((uint8 *)(PTR))[0]<<24)|(((uint8 *)(PTR))[1]<<16)|(((uint8 *)(PTR))[2]<<8)|((uint8 *)(PTR))[3])  //�ӻ�����ȡ32λ����

void ProcessLCDMessage(uint8 *msg, uint16 size);
void ProcessUserMessage(uint8 *msg, uint16 size);

#endif
