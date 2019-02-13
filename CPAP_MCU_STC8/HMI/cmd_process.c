#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"

#include "uart.h"
#include "stepmotor.h"
#include "io.h"

#include "misc.h"
#include "management.h"

#ifdef __cplusplus
extern "C" {
#endif

void ProcessLCDMessage(uint8 *msg, uint16 size)
{
	uint8 cmd = msg[3];
	switch (cmd)
	{
    	case 0x81: //��ϵͳ����
        {
            uint8 addr = msg[4];
    		uint8 len = msg[5];
    		
    		if (addr == 0x20)//RTC
    		{
    			uint8 year = msg[6];
    			uint8 mon = msg[7];
    			uint8 date = msg[8];
    			uint32 d = BCD2Int(year) * 365
    				+ BCD2Int(mon) * 12
    				+ BCD2Int(date);

                cDebug("rtc d = %d\r\n", (int)d);
                cDebug("rtc autoCalibDate = %ld\r\n", (man.autoCalibDate));

                if(d >= (man.autoCalibDate + AUTOCALIBRATIONPEROID))
                {
                    man.autoCalibFlag = 0x01;

                    man.autoCalibDate = d;

                    cDebug("This is timeout to calibration!\r\n");
                }
                
                man.autoCalibFlag |= 0x80;

                cDebug("the rtc is 20%02x-%02x-%02x %02x:%02x:%02x\r\n", (int)year, (int)mon, (int)date, (int)msg[10], (int)msg[11], (int)msg[12]);
    		}    
        }
		break;
	    case 0x83: //���û�����
		{
            uint16 addr = msg[4] << 8 | msg[5];
    		uint8 len = msg[6];
    
    		//if (addr == 0x000C)
    			//qDebug() << "read data is : " << dataBuffer.data() + 7;
        }
		break;
	    default:
		break;
	}
}

/*! 
 *  \brief  ��Ϣ�������̣��˴�һ�㲻��Ҫ����
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessUserMessage(uint8 *msg, uint16 size)
{
    uint8_t cmdType = msg[1];
    uint8_t *cmdPara = &msg[2];
	uint8_t i;

//	for(i=0;i<size;i++)
////		//Uart2Send(msg[i]);
//		Uart_SendData(msg[i]);

    switch(cmdType)
    {
        case 0x80: //beeper
            if(cmdPara[0])
                man.beeperAlarm = 1;
            else
                man.beeperAlarm = 0;    
        break;
        case 0x81: //�������
            if(cmdPara[0] == 0)
                StepMotor_Stop();
            else if(cmdPara[0] == 1)
            {
                StepMotor_SetSpeed(cmdPara[1]);
                StepMotor_CW();
            }
            else if(cmdPara[0] == 2)
            {
                StepMotor_SetSpeed(cmdPara[1]);
                StepMotor_CCW();
            }
        break;
        case 0x82: //������
            if(!cmdPara[0])
            {
                HeatPlate_Adjust(cmdPara[1]);    
            }
            else
            {
                HeatLine_Adjust(cmdPara[1]);
            }
        break;
        case 0x83: //ҳ����ת
            SetScreen(cmdPara[0]);
        break;
        case 0x84: //�޸�У׼����
            switch(cmdPara[0]&0xF0) 
            {
                case 0x00: //�ڲ���������
                    if(cmdPara[0]&0x04) //����
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.innerOxygenCalibRatio += 0.1;
                        else
                            man.innerOxygenCalibRatio -= 0.1;

                       cDebug("man.innerOxygenCalibRatio = %f\r\n", man.innerOxygenCalibRatio); 
                    }
                    else //ƫ��
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.innerOxygenCalibOffset += 0.1;
                        else
                            man.innerOxygenCalibOffset -= 0.1;
                            
                        cDebug("man.innerOxygenCalibOffset = %f\r\n", man.innerOxygenCalibOffset);     
                    }
                break;
                case 0x10: //������������
                    if(cmdPara[0]&0x04) //����
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.maskOxygenCalibRatio += 0.1;
                        else
                            man.maskOxygenCalibRatio -= 0.1;

                       cDebug("man.maskOxygenCalibRatio = %f\r\n", man.maskOxygenCalibRatio); 
                    }
                    else //ƫ��
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.maskOxygenCalibOffset += 0.1;
                        else
                            man.maskOxygenCalibOffset -= 0.1;
                            
                        cDebug("man.maskOxygenCalibOffset = %f\r\n", man.maskOxygenCalibOffset);     
                    }
                break;
                case 0x20: //�ڲ��¶ȴ�����
                    if(cmdPara[0]&0x04) //����
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.innerTemperCalibRatio += 0.1;
                        else
                            man.innerTemperCalibRatio -= 0.1;

                        cDebug("man.innerTemperCalibRatio = %f\r\n", man.innerTemperCalibRatio);
                    }
                    else //ƫ��
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.innerTemperCalibOffset += 0.1;
                        else
                            man.innerTemperCalibOffset -= 0.1;
                            
                        cDebug("man.innerTemperCalibOffset = %f\r\n", man.innerTemperCalibOffset);    
                    }
                break;
                case 0x30: //�����¶ȴ�����
                    if(cmdPara[0]&0x04) //����
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.cubeTemperCalibRatio += 0.1;
                        else
                            man.cubeTemperCalibRatio -= 0.1;

                        cDebug("man.cubeTemperCalibRatio = %f\r\n", man.cubeTemperCalibRatio);
                    }
                    else //ƫ��
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.cubeTemperCalibOffset += 0.1;
                        else
                            man.cubeTemperCalibOffset -= 0.1;
                            
                        cDebug("man.cubeTemperCalibOffset = %f\r\n", man.cubeTemperCalibOffset);    
                    }
                break;
                case 0x40: //�����¶ȴ�����
                    if(cmdPara[0]&0x04) //����
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.maskTemperCalibRatio += 0.1;
                        else
                            man.maskTemperCalibRatio -= 0.1;

                        cDebug("man.maskTemperCalibRatio = %f\r\n", man.maskTemperCalibRatio);
                    }
                    else //ƫ��
                    {
                        if(cmdPara[0]&0x01) //��0.1
                            man.maskTemperCalibOffset += 0.1;
                        else
                            man.maskTemperCalibOffset -= 0.1;
                            
                        cDebug("man.maskTemperCalibOffset = %f\r\n", man.maskTemperCalibOffset);    
                    }
                break;
                default:
                break;            
            }
        break;
        case 0x85: //ģ�����
            switch(cmdPara[0])
            {
                case 0:
                    
                break;
                case 1:
                break;
                case 2:
                break;
                case 3:
                break;
                case 4:
                break;
                case 5:
                break;
                default:
                break;
            }
        break;
        default:
        break;
    }
}

#ifdef __cplusplus
}
#endif
