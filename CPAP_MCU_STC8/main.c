#include "uart.h"
#include "timer.h"
#include "io.h"
#include "eeprom.h"
#include "stepmotor.h"
#include "tm770x.h"

#include "HMI/hmi_driver.h"
#include "HMI/cmd_queue.h"
#include "HMI/cmd_process.h"

#include "management.h"
#include "misc.h"

#include <string.h>
#include <stdlib.h>

//RTX51 Tiny��ʱ�ӵδ�ΪFosc/12/INT_CLOCK
//Fosc=32MHz,INT_CLOCK=13333,����RTX51 Tiny��ʱ�ӵδ�ԼΪ5ms
//Fosc=24MHz,INT_CLOCK=10000,����RTX51 Tiny��ʱ�ӵδ�ԼΪ5ms
//�����os_wait()��ʱ����Լ200��ʱ�ӵδ�Ϊ1s����os_wait(K_TMO, 200, 0)

//-----------------------------------------------

/* main program */
void startupTask(void) _task_ TASK_STARTUP 
{
	//---------------��ʼ��------------------------------------
	IO_Init();
	Timer_Init(); //��ʱ����ʼ��

	queue_reset(&lcd_que);
	queue_reset(&user_que);

	Uart_Init();//���ڳ�ʼ��

	//AT24CXX_Init();

    EA = 1;//����ȫ���ж�
    //while(1)
    cDebug("hello CPAP!\r\n");
	//Uart_SendString("hello CPAP!\r\n");

//    while(1)
//    {
//        os_wait(K_TMO, 200, 0);
//        os_wait(K_TMO, 200, 0);
//        cDebug("hello CPAP!\r\n");
//        LEDM = !LEDM;    
//    }

    TM770X_Init(CHIP_ALL);
    InitMan();

    //WDT_CONTR = 0x3C;//�����Ź������ʱ��1.1377s

    //---------------��������-----------------------------
	os_create_task(TASK_UI); 	//��������1

	//---------------ɾ��������-----------------------------
 	os_delete_task(TASK_STARTUP);	//ɾ���Լ�(task0),ʹtask0�˳���������
}

void UITask(void) _task_ TASK_UI
{
    uint8_t size = 0;
    uint8_t i;
    uint32_t cnt;

//    cDebug("UITask is running...\r\n");

    os_wait(K_TMO, 200, 0);
    os_wait(K_TMO, 200, 0);
//    os_wait(K_TMO, 200, 0);
//    os_wait(K_TMO, 200, 0);
//    os_wait(K_TMO, 200, 0);

    //ҳ�������ʼ��
    //��Ũ��
	Int2String(man.iSetupOxygen, man.sSetupOxygen);
	SetTextValueLen(VAR_ADDR_SETUPOXYGEN, man.sSetupOxygen, 2);
    man.iLowOxygen = man.iSetupOxygen - 5;
    man.iHighOxygen = man.iSetupOxygen + 5;
    Int2String(man.iLowOxygen, man.sSetupOxygen);
	SetTextValueLen(VAR_ADDR_SETUPOXYGEN_LOW, man.sSetupOxygen, 2);
    Int2String(man.iHighOxygen, man.sSetupOxygen);
	SetTextValueLen(VAR_ADDR_SETUPOXYGEN_HIGH, man.sSetupOxygen, 2);
    //�¶�
    sprintf(man.stringTemp, "%f", man.fSetupTemper);
    SetTextValueLen(VAR_ADDR_SETUPTEMPER, man.stringTemp, 4);
    man.fLowTemper = man.fSetupTemper - 1.0;
    man.fHighTemper = man.fSetupTemper + 1.0;
    sprintf(man.stringTemp, "%f", man.fLowTemper);
    SetTextValueLen(VAR_ADDR_SETUPTEMPER_LOW, man.stringTemp, 4);
    sprintf(man.stringTemp, "%f", man.fHighTemper);
    SetTextValueLen(VAR_ADDR_SETUPTEMPER_HIGH, man.stringTemp, 4);

    //У׼ҳ�������ʼ��
    for(i=0;i<10;i++)
    {
        char s[10];
        sprintf(s, "%f", *(man.pCalib+i));
        SetTextValueLen(VAR_ADDR_CALIB_INNEROXYGEN_OFFSET+i*0x0004, s, 6);
        os_wait(K_TMO, 1, 0);
    }
    SetTextFontColor(VAR_ADDR_CALIB_INNEROXYGEN_OFFSET_PTR+man.calibIndex*0x0010, COLOR_RED);

    //PID����ҳ�������ʼ��
    for(i=0;i<9;i++)
    {
        char s[10];
        sprintf(s, "%f", *(&((man.pAdjustPID+i/3)->Proportion)+i%3));
        SetTextValueLen(VAR_ADDR_ADJUST_INNEROXYGEN_P+i*0x0004, s, 6);
        os_wait(K_TMO, 1, 0);
    }
    SetTextFontColor(VAR_ADDR_ADJUST_INNEROXYGEN_P_PTR+man.adjustPIDIndex*0x0010, COLOR_RED);

    //��ȡRTCʱ��
//    GetRTC();
//    cnt = 0;
//    while(!(man.autoCalibFlag&0x80) && (cnt++ < 50000))
//    {
//        uint8_t size = queue_find_cmd(&lcd_que, lcd_cmd_buffer, CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��			       
//		if(size>0)//���յ�ָ��
//		{
//			ProcessLCDMessage(lcd_cmd_buffer, size);//ָ���
//		}
//    }
//    if(cnt >= 50000)
//    {
//        cDebug("Can not read the rtc!\r\n");
//    }

    os_create_task(TASK_ADC);	//��������2
    os_create_task(TASK_ALARM);	//��������2
    os_create_task(TASK_TIMER);	//��������2 
       
    SetScreen(man.curPage);

    while (1)                       //loop
	{
        os_wait(K_TMO, 10, 0);

        //����
        Key_Scan();
        //if(!(man.alarmStatus&ALARM_TEMPER))//�¶ȳ���43����Ҫͣ��
        { 
    		Key_Process();
            Key_Process_LongPress(); //������������
        }

		//������������
		size = queue_find_cmd(&user_que, user_cmd_buffer, CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��			       
		if(size>0)//���յ�ָ��
		{
			ProcessUserMessage(user_cmd_buffer, size);//ָ���
		}
        size = queue_find_cmd(&lcd_que, lcd_cmd_buffer, CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��			       
		if(size>0)//���յ�ָ��
		{
			ProcessLCDMessage(lcd_cmd_buffer, size);//ָ���
		}

        AlarmCheck();

		LEDM = !LEDM;
		//MBEEP = !MBEEP; 
	}
}

void ADCTask(void) _task_ TASK_ADC
{
//    cDebug("ADCTask is running...\r\n");
//
//    cDebug("man.autoCalibFlag = %d\r\n", (int)man.autoCalibFlag);
    
    while(1)
    {
        os_wait(K_TMO, 10, 0);

        //��Ũ�Ⱥ��¶����ݲɼ�
        ADCGet();

        //��Ũ�ȵ���
        if(man.startFlag)
        {
            OxygenPercentControl();

            TemperControl();
        }
        else 
        {
            //��ͣʱҪ�رշ���װ��
            HeatPlate_Adjust(0);
            HeatLine_Adjust(0);

            if(man.oxygenMode != MODE_NONE)//�Զ�У׼
            {
                if(man.autoCalibFlag&0x01)
                    AutoCalibration(0);//��У׼99%
                    //AutoCalibration(1);
                else if(man.autoCalibFlag&0x02)
                    AutoCalibration(1);
            }       
        }
        //MBEEP = !MBEEP;
        //LEDM = !LEDM;
    }
   
}

void AlarmTask(void) _task_ TASK_ALARM
{
//    cDebug("AlarmTask is running...\r\n");

    while(1)
    {
        //ι��
		//WDT_CONTR = 0x3C;

        if(man.alarmStatus&ALARM_NO_MASK 
            || (!man.powerOn && man.alarmStatus&ALARM_OXYCURE && man.startFlag && !(man.silenceStatus&0x80)))
        {
            os_wait(K_TMO, 100, 0);
            man.beeperAlarm = 1;
            os_wait(K_TMO, 100, 0);
            man.beeperAlarm = 0;

            //man.beeperAlarm = 1;
            //LEDM = !LEDM;   
        }
        else if(man.beeperTimes)
        {
            if(man.beeperTimes)
                man.beeperTimes--;

            os_wait(K_TMO, 200, 0);
            man.beeperAlarm = 1;
            os_wait(K_TMO, 200, 0);
            man.beeperAlarm = 0;
        }
        else
            //man.beeperAlarm = 0;
            os_wait(K_TMO, 10, 0);

        //LEDM = !LEDM;
        //MBEEP = !MBEEP;    
    }    
}

void TimerTask(void) _task_ TASK_TIMER
{
    //int16_t powerOnCnt = POWERONNOALARMTIME;

//    cDebug("TimerTask is running...\r\n");

    while(1)
    {
        os_wait(K_TMO, 200, 0);
    
        if(man.powerOn)
        {
            man.powerOnCnt--;
            if(man.powerOnCnt < 0)
                man.powerOn = 0;
        }

        if(man.silenceStatus&0x80)
        {
            man.silenceTime--;
            if(man.silenceTime < 0)//����ʱ�䵽
            {
                man.silenceStatus = 0;

                //�ָ�����״̬
                if(man.alarmStatus&ALARM_OXYCURE)
                {
                    SetTextValueLen(VAR_ADDR_WARNINGWORKING, OXYCURE_WARNING_STRING, 8);
                    SetTextFontColor(VAR_ADDR_WARNINGWORKING_PTR, COLOR_RED); 
                }
                else
                {
                    SetTextValueLen(VAR_ADDR_WARNINGWORKING, OXYCURE_WORKING_STRING, 8);
                    SetTextFontColor(VAR_ADDR_WARNINGWORKING_PTR, COLOR_BLACK);    
                }
            }   
        }

        if(man.userTimeOutFlag)
        {
            man.userTimeCnt--;
            if(man.userTimeCnt <= 0)
                man.userTimeOutFlag = 0;    
        }
    }
}
