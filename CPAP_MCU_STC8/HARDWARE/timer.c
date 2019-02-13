#include "timer.h"
#include "io.h"
#include "stddef.h"
#include "stepmotor.h"
#include "management.h"

const uint8_t timer_TH_TL[7][2] = 
{
    {0x00, 0x00},
    {0x00, 0x80},

	{0x44, 0x80}, //250HZ
	{0xA2, 0x40}, //500HZ
	{0xD1, 0x20}, //1000HZ
	{0xED, 0x40}, //2500HZ
	{0xF6, 0xA0}, //5000HZ
	//{0xFB, 0x50}, //10000HZ
};

uint8_t stepMotorSpeedIndex = 2;
uint32_t stepMotorStep = 0;

//��ʱ����ʼ��
void Timer_Init(void)
{
	/*******************�������-��ʱ��0*********************************************/
//    TMOD &= 0x0F;                    //16λ�Զ���װ��
//	AUXR |= 0x80;  //��ʱ��0�ٶȿ��ƣ�0:12Tģʽ����CPUʱ��12��Ƶ��1:1Tģʽ��ֱ����CPUʱ��
//    TL0 = timer_TH_TL[stepMotorSpeedIndex][1];  //initial timer0 low byte
//    TH0 = timer_TH_TL[stepMotorSpeedIndex][0];  //initial timer0 high byte
//	TF0 = 0;
//    ET0 = 1;                         //enable timer0 interrupt
//	INTCLKO = 0x01;				 //ʹ�ܶ�ʱ��0��ʱ���������
//	TR0 = 0;                         //timer0 start running

//	/*******************����-��ʱ��3*********************************************/
//	T3L = (65536 - (FOSC/12/(HEAT_PEROID/2)));
//    T3H = (65536 - (FOSC/12/(HEAT_PEROID/2))) >> 8;
//	T4T3M &= ~0x0F; //bit4-ֹͣ��ʱ����bit3-��ʱģʽ��bit2-12���٣�bit0-��ʱʱ�����
//    //T4T3M |= 0x08;                               //������ʱ��
//    T4T3M &= ~0x08;
//    IE2 |= ET3;                                //ʹ�ܶ�ʱ���ж�
//
//	/*******************������-��ʱ��4*********************************************/
//	T4L = (65536 - (FOSC/12/(BEEPER_PEROID/2)));          
//    T4H = (65536 - (FOSC/12/(BEEPER_PEROID/2))) >> 8;
//	T4T3M &= ~0xF0;
//    //T4T3M |= 0x80;                               //������ʱ��
//    T4T3M &= ~0x80;
//    IE2 |= ET4;                                //ʹ�ܶ�ʱ���ж�


	/*******************�������-��ʱ��3*********************************************/
	T3L = timer_TH_TL[stepMotorSpeedIndex][1];
    T3H = timer_TH_TL[stepMotorSpeedIndex][0];
	T4T3M &= ~0x0F; //bit4-ֹͣ��ʱ����bit3-��ʱģʽ��bit2-12���٣�bit0-��ʱʱ�����
    //T4T3M |= 0x08;                               //������ʱ��
    T4T3M &= ~0x08;
    IE2 |= ET3;                                //ʹ�ܶ�ʱ���ж�

	/*******************������-������-������-��ʱ��4*********************************************/
	T4L = T100NS;          
    T4H = T100NS >> 8;
	T4T3M &= ~0xF0;
    T4T3M |= 0x80;                               //������ʱ��
    //T4T3M &= ~0x80;
    IE2 |= ET4;                                //ʹ�ܶ�ʱ���ж�

}

//-----------------------------------------------
/* Timer0 interrupt routine */
//void Timer0_Isr() interrupt 1 using 3
//{
////    TL0 = timer_TH_TL[stepMotorSpeedIndex][1];  //initial timer1 low byte
////    TH0 = timer_TH_TL[stepMotorSpeedIndex][0];  //initial timer1 high byte
//
////	if(stepMotorStep)
////		stepMotorStep--;
////	else
////	{
////		ET0 = 0;
////		TR0 = 0;
////	}
//
//	//MBEEP = !MBEEP;                                  //���Զ˿�
//}

void TM3_Isr() interrupt 19 using 3
{
    T3H = timer_TH_TL[stepMotorSpeedIndex][0];
    T3L = timer_TH_TL[stepMotorSpeedIndex][1];

    if(man.stepMotorStatus&0x20)
    {
        if(man.stepMotorStep)
    		man.stepMotorStep--;
    	else
        {
    		T4T3M &= ~0x08;
            man.stepMotorStatus &= ~0xA0;
        }
    }

    STM_PULSE = !STM_PULSE;

    //MBEEP = !MBEEP;                                  //���Զ˿�
    AUXINTIF &= ~T3IF;                          //���жϱ�־
}

void TM4_Isr() interrupt 20 using 3
{
    if(man.beeperAlarm) //������
    {
//        static uint16_t cnt = 0;
//        if(cnt++ > 1000)
//        {
//            cnt = 0;
//            MBEEP = !MBEEP;
//            LEDM = !LEDM; 
//        }

        static uint8_t cnt = 0;
        if(cnt++ > 10)
        {
            cnt = 0;

            if(!man.beeperStopFlag)
                MBEEP = !MBEEP;
        }
    }

#if 1
    if(man.heatControl&0x08) //����˿
    {
        if(man.lineHeatCnt++ > 100)//����100%
            man.lineHeatCnt = 0;
                
        if(man.lineHeatCnt < man.lineHeatPWMPercent)
            M_LINEHEAT = 0;
        else
            M_LINEHEAT = 1;  //�ر�MOS��   
    }
    else
        M_LINEHEAT = 1;  //�ر�MOS��
            
    if(man.heatControl&0x80) //������
    {
        if(man.plateHeatCnt++ > 100)//����100%
            man.plateHeatCnt = 0;
                
        if(man.plateHeatCnt < man.plateHeatPWMPercent)
            M_DISCHEAT = 0;
        else
            M_DISCHEAT = 1;  //�ر�MOS��                  
    }
    else
        M_DISCHEAT = 1;  //�ر�MOS��
#endif

    //MBEEP = !MBEEP;                                 //���Զ˿�
    AUXINTIF &= ~T4IF;                          //���жϱ�־
}

//----------------------------------------------------------------
void Delay1us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}

void Delay10us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 57;
	while (--i);
}

void Delay100us()		//@24.000MHz
{
	unsigned char i, j;

	i = 3;
	j = 82;
	do
	{
		while (--j);
	} while (--i);
}

void Delay500us()		//@24.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

//void Delay10ms()		//@24.000MHz
//{
//	unsigned char i, j, k;
//
//	_nop_();
//	_nop_();
//	i = 1;
//	j = 234;
//	k = 113;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}

void Delay10ms()		//@24.000MHz
{
	unsigned char i, j, k;

	i = 2;
	j = 56;
	k = 172;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay100ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
