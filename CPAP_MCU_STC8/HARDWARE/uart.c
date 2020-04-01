#include "uart.h"
#include "io.h"
#include "timer.h"

#include "cmd_queue.h"

uint8_t user_cmd_buffer[CMD_MAX_SIZE];
uint8_t lcd_cmd_buffer[CMD_MAX_SIZE];

//��־�������ڷ�������
static bit Uart_Busy;
static bit Uart2_Busy;

//���ڳ�ʼ�������ڲ����ʷ������õ��Ƕ�ʱ��1
void Uart_Init(void)
{
	/************************����1***********************************/
	PCON &= ~0x80;	//����1�����ʿ���λ��0����ģʽ�����ʲ��ӱ���1��ģʽ1��2��3�����ʼӱ�

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;            //8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;            //9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;            //9-bit variable UART, parity bit initial to 0
#endif

	TMOD &= ~0xF0;            //��ʱ��1Ϊ��ʱ����ģʽ0����16λ�Զ���װ��ģʽ

	AUXR |= 0x40;  //��ʱ��1�ٶȿ��ƣ�0:12Tģʽ����CPUʱ��12��Ƶ��1:1Tģʽ��ֱ����CPUʱ��
	AUXR &= ~0x20;  //����1������6���٣�0�������٣�1:6����
	AUXR &= ~0x01;  //����1�����ʷ�����ѡ��0����ʱ��1��1����ʱ��2

	TL1 = (65536 - (FOSC/4/USERUART_BAUD));
	TH1 = (65536 - (FOSC/4/USERUART_BAUD))>>8;

	TR1 = 1;                //Timer1 start run

	ES = 1;                 //Enable UART1 interrupt

	/************************����2***********************************/
#ifdef HARDWARE_VERSION_18A
	P_SW2 &= ~0x01;                              //RXD2/P1.0, TXD2/P1.1
#endif
#ifdef HARDWARE_VERSION_24A
    P_SW2 |= 0x01;                               //RXD2_2/P4.0, TXD2_2/P4.2
#endif

	S2CON = 0x50;  //8λ�ɱ䲨����

	AUXR &= ~0x08; //��ʱ��2��ʱģʽ
	AUXR |= 0x04; //��ʱ��2�ٶȿ��ƣ�0:12Tģʽ����CPUʱ��12��Ƶ��1:1Tģʽ��ֱ����CPUʱ��
	AUXR |= 0x10; //������ʱ��2
	T2L = (65536 - (FOSC/4/LCDUART_BAUD));
	T2H = (65536 - (FOSC/4/LCDUART_BAUD)) >> 8;	
	IE2 |= 0x01; //������2�ж�
}

/*----------------------------
UART interrupt service routine
----------------------------*/
void Uart_Isr() interrupt 4 using 1
{
	uint8_t Buf = SBUF;
    qsize pos;

    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag

		//8λ��Ƭ���жϺ������ܵ��ú���
		{
			//pos = (user_que._head+1)%QUEUE_MAX_SIZE;
            pos = user_que._head+1;  //ʹ��RTX tiny ��ֻ�ܰ�pos����������棬Ҳʹ�ò���%QUEUE_MAX_SIZE
            if(pos >= QUEUE_MAX_SIZE) pos = 0;

			if(pos != user_que._tail)//����״̬
			{
				user_que._data[user_que._head] = Buf;
				user_que._head = pos;
			}
		}

		//queue_push(&user_que, Buf);

    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        Uart_Busy = 0;           //Clear transmit busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: Data (data to be sent)
Output:None
----------------------------*/
void Uart_SendData(uint8_t Data)
{
    while (Uart_Busy);           //Wait for the completion of the previous data is sent
    ACC = Data;              //Calculate the even parity bit P (PSW.0)
    if (P)                  //Set the parity bit according to P
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;            //Set parity bit to 0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;            //Set parity bit to 1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;            //Set parity bit to 1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;            //Set parity bit to 0
#endif
    }
    Uart_Busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void Uart_SendString(uint8_t *s)
{
    while (*s)              //Check the end of the string
    {
        Uart_SendData(*s++);     //Send current char and increment string ptr
    }
}

//��дputchar����
char putchar(char c)
{
    Uart_SendData(c);
    return c;
}

/****************************************************
**
**����2
**
*****************************************************/
void Uart2Isr() interrupt 8 using 2
{
    if (S2CON & 0x02)
    {
        S2CON &= ~0x02;
        Uart2_Busy = 0;
    }
    if (S2CON & 0x01)
    {
        S2CON &= ~0x01;
        //buffer[wptr++] = S2BUF;
        //wptr &= 0x0f;

		{
			qsize pos = (lcd_que._head+1)%QUEUE_MAX_SIZE;
			if(pos!=lcd_que._tail)//����״̬
			{
				lcd_que._data[lcd_que._head] = S2BUF;
				lcd_que._head = pos;
			}
		}
    }
}

void Uart2Send(char dat)
{
    while (Uart2_Busy);
    Uart2_Busy = 1;
    S2BUF = dat;
}

void Uart2SendStr(char *p)
{
    while (*p)
    {
        Uart2Send(*p++);
    }
}
