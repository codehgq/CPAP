#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include  "common.h"
#include "PID.h"

//ʹ������һ�Ƿ�����ü����޸�
#define WORKMODE_KEYCHANGE      1

#define OS_TICK    (200)
#define TM7706_OUTPUT_HZ 15 //tm7706����������ʣ�������ʱos_wait()


//�¶�ADC�ɼ���
#define TEMPER_ADC_INPUT_VOLTAGE_RANGE_MV  20.0 //ADC�����ѹ20mV��Χ������128���棬2.5V�ο���ѹ��2.5V/128=20mV
#define TEMPER_ADC_PULLUP_RESISTOR_OHM     4990.0 //��������
#define TEMPER_ADC_PULLDOWN_RESISTOR_OHM   100.0 //��������
#define TEMPER_ADC_POWER_VOLTAGE_MV        5000.0 //ADC��Դ��ѹ��ͨ������������������裨����Pt100����ѹ��ΪTM7707������
//�¶�ADC���˵������ѹ
#define TEMPER_ADC_NEGATIVE_INPUT_VOLTAGE_MV  (TEMPER_ADC_POWER_VOLTAGE_MV*TEMPER_ADC_PULLDOWN_RESISTOR_OHM/(TEMPER_ADC_PULLUP_RESISTOR_OHM+TEMPER_ADC_PULLDOWN_RESISTOR_OHM)) 

//��Ũ��ADC�ɼ���
#define OXYGEN_ADC_INPUT_VOLTAGE_RANGE_MV  80.0 //ADC�����ѹ80mV��Χ������32���棬2.5V�ο���ѹ��2.5V/32=80mV

//ADC�ɼ���������С
#define OXYGENBUFFER_SIZE 5
#define TEMPERBUFFER_SIZE OXYGENBUFFER_SIZE

//������ҳ�����
#define PAGE_LOGO           0
#define PAGE_PAUSE          1
#define PAGE_RUNNING        2
#define PAGE_MODESELECT     3
#define PAGE_CALIBRATION    4
#define PAGE_ADJUSTED       5
#define PAGE_OVERTEMPER43   6
#define PAGE_UNPLUGOXYGEN   7
#define PAGE_UNPLUGAIR      8
#define PAGE_REPLACE_S1     9
#define PAGE_REPLACE_S2     10
#define PAGE_CALIBRATING    11
#define PAGE_CALIBCOMPLETE  12
#define PAGE_MODESELECT_3   13

//��������ɫ����
//#define COLOR_RED 0xD800
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x0600
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF

//������������ַ
#define VAR_ADDR_SETUPOXYGEN 				0x0000
#define VAR_ADDR_SETUPTEMPER         		0x0002

#define VAR_ADDR_CURRENTMASKOXYGEN			0x0006
#define VAR_ADDR_CURRENTMASKTEMPER      	0x0008
#define VAR_ADDR_CURRENTOXYGEN 		        0x000C
#define VAR_ADDR_CURRENTTEMPER              0x000E

#define VAR_ADDR_SETUPOXYGEN_LOW 			0x0012
#define VAR_ADDR_SETUPOXYGEN_HIGH 			0x0014
#define VAR_ADDR_SETUPTEMPER_LOW 	        0x0016
#define VAR_ADDR_SETUPTEMPER_HIGH	        0x001A

#define VAR_ADDR_MODE		 				0x0050
#define VAR_ADDR_WARNINGFLUX 				0x0060
#define VAR_ADDR_WARNINGCONNECT				0x0070
#define VAR_ADDR_WARNINGWORKING				0x0080

#define VAR_ADDR_CALIB_INNEROXYGEN_OFFSET	0x0400
#define VAR_ADDR_ADJUST_INNEROXYGEN_P	    0x0600

//����ָ���ַ
#define VAR_ADDR_WARNINGFLUX_PTR 				0x00F0
#define VAR_ADDR_WARNINGCONNECT_PTR				0x0100
#define VAR_ADDR_WARNINGWORKING_PTR				0x0110

#define VAR_ADDR_CURRENTMASKOXYGEN_PTR			0x00C0
#define VAR_ADDR_CURRENTMASKTEMPER_PTR	        0x00D0
#define VAR_ADDR_CURRENTOXYGEN_PTR 		        0x0090
#define VAR_ADDR_CURRENTTEMPER_PTR              0x00A0

#define VAR_ADDR_CALIB_INNEROXYGEN_OFFSET_PTR   0x0800
#define VAR_ADDR_ADJUST_INNEROXYGEN_P_PTR       0x0A00

//��������
#define SETUP_OXYGEN_MIN  21 
#define SETUP_OXYGEN_MAX  99
#define SETUP_TEMPER_MIN  30.0 
#define SETUP_TEMPER_MAX  40.0

#define TEMPER_MIN        15.0
#define TEMPER_MAX        43.0

#define UNCONNECT_TEMPER_MAX    (79.0-10.0)
//#define TEMPER_DIFFERENCE       (4.0)
#define TEMPER_DIFFERENCE       (24.0)

//��������������
#define KEY_CONTINUAL_COUNT 20

//����ģʽ
#define MODE_NONE       0
#define MODE_ANABIOSIS  1
#define MODE_OXYGENAIR  2
#define MODE_CPAP       3

//��������
#define ALARM_TEMPER            0x10
#define ALARM_FLUX              0x08
#define ALARM_CONNECT           0x04
#define ALARM_OXYCURE_OXYGEN    0x02
#define ALARM_OXYCURE_TEMPER    0x01

#define ALARM_NO_MASK           (ALARM_TEMPER|ALARM_FLUX|ALARM_CONNECT)
#define ALARM_OXYCURE           (ALARM_OXYCURE_OXYGEN|ALARM_OXYCURE_TEMPER)

//EEPROM ADDR
#define EEPROM_BASEADDR_DEFAULT     0x2000
#define EEPROM_BASEADDR_CALIB       0x2200
#define EEPROM_BASEADDR_ADJUST      0x2400
#define EEPROM_BASEADDR_AUTOCALIB   0x2600
#define EEPROM_BASEADDR_WORKMODE    0x2800

//ʱ��
#define SILENCETIME             (3*60)   //����3����  
#define POWERONNOALARMTIME      (10*60)  //15���Ӷ�ʱ��
//#define SILENCETIME             10 //(3*60)   //����3����  
//#define POWERONNOALARMTIME      30 //(15*60)  //15���Ӷ�ʱ��

#define COMBOKEYBEEPERSTOPCOUNT 5
#define COMBOKEYAUTOCALIBCOUNT  2
#define COMBOKEYCALIBCOUNT      3
#define COMBOKEYADJUSTCOUNT     5
#define COMBOKEYDEFAULTRESETCOUNT     10
#define COMBOKEYMODECHANGECOUNT 3
#define COMBOKEYWORKMODECOUNT   15


#define AUTOCALIBRATIONPEROID   30 //�Զ�У׼���ڣ�Ĭ��30��

typedef struct
{
    uint8_t oxygenMode; //����ģʽ 1������ģʽ��2������ģʽ��3��CPAPģʽ

    uint8_t startFlag;
    uint8_t powerOn; //������һ�����ڿ���������15���ӲŻ����Ʊ���
    int16_t powerOnCnt;//

    uint8_t curPage; //��ǰҳ��
    uint8_t prevPage; //��һҳ��
    uint8_t comboKeyAutoCalibCnt; //��10����ϼ������Զ�У׼ҳ��
    uint8_t comboKeyCalibCnt; //��10����ϼ�����У׼ҳ��
    uint8_t comboKeyAdjustCnt; //��10����ϼ�����PID����ҳ��
    uint8_t comboKeyDefaultResetCnt;
    uint8_t comboKeyModeChangeCnt;
    uint8_t comboKeyBeeperStopCnt;
    uint8_t comboKeyWorkModeCnt;

    uint8_t autoCalibFlag; //�Զ�У׼��־λ
    uint8_t autoCalibState; //�Զ�У׼����״̬��
    uint32_t autoCalibDate; //��һ���Զ�У׼������

    /*����״̬            
    bit8: 1-�����У�0-ֹͣ
    bit7: 1-ǰ����0-����
    bit6��1-ʹ�ò���
    */
	uint8_t stepMotorStatus; //���״̬
    uint16_t stepMotorStep; //����

    //�ַ�����ʾ
	char sSetupOxygen[2]; //������Ũ���ַ���
   	char sSetupTemperInteger[2]; //�����¶������ַ���
	char sSetupTemperDecimal[1]; //�����¶�С���ַ���
    char stringTemp[10];

	char sCurOxygen[2]; //��ǰ��Ũ���ַ���
   	char sCurTemperInteger[2]; //��ǰ�¶������ַ���
	char sCurTemperDecimal[1]; //��ǰ�¶�С���ַ���

    //����ֵ
	uint8_t iSetupOxygen; //������Ũ��
    float fSetupOxygen; //PID������
	float fSetupTemper; //�����¶�

    uint8_t iLowOxygen; //��С��Ũ��
    uint8_t iHighOxygen; //�����Ũ��
    float fLowTemper; //��С�¶�
    float fHighTemper; //����¶�

    //У׼��
    float innerOxygenCalibOffset;
    float innerOxygenCalibRatio;
    float maskOxygenCalibOffset;
    float maskOxygenCalibRatio;
    float innerTemperCalibOffset;
    float innerTemperCalibRatio;
    float cubeTemperCalibOffset;
    float cubeTemperCalibRatio;
    float maskTemperCalibOffset;
    float maskTemperCalibRatio;
    float *pCalib;
    int8_t calibIndex;

    //��ǰʵ������
    uint8_t curInnerOxygen; //��ǰ�ڲ���Ũ��
    uint8_t curMaskOxygen; //��ǰ������Ũ��
    float fCurInnerOxygen; //PID������
    float fCurMaskOxygen; //PID������
    float curInnerTemper; //��ǰ�ڲ��¶�
    float curCubeTemper; //��ǰ�����¶�
    float curMaskTemper; //��ǰ�����¶�

    //��Ũ�Ȳɼ�����
    uint16_t innerOxygenBufferADC[OXYGENBUFFER_SIZE];
    uint16_t maskOxygenBufferADC[OXYGENBUFFER_SIZE];
    uint16_t innerOxygenADC;
    uint16_t maskOxygenADC;

    //�¶Ȳɼ�����
    uint32_t innerTemperBufferADC[TEMPERBUFFER_SIZE];
    uint32_t cubeTemperBufferADC[TEMPERBUFFER_SIZE];
    uint32_t maskTemperBufferADC[TEMPERBUFFER_SIZE];
    uint32_t innerTemperADC;
    uint32_t cubeTemperADC;
    uint32_t maskTemperADC;

    //����
    uint8_t beeperAlarm; //��������
    uint8_t alarmStatus; //bit4�¶ȳ���43�汨��; bit3-��������; bit2-���ӱ���; bit1-����_��Ũ�ȱ���; bit0-����_�¶ȱ���
    uint8_t alarmStatusLast;
    uint8_t alarmStatusRaisingEdge;
    uint8_t alarmStatusFallingEdge;

    uint8_t silenceStatus; //����״̬��bit7-���뾲��״̬,bit0-����ʱ�䵽
    int16_t silenceTime; //����ʱ��ͳ��
    uint8_t beeperTimes;//���������м���

    //����
    uint8_t heatControl; //���ȿ��� bit7-�������������ȣ�bit3-��������������
    uint8_t plateHeatCnt; //�����̼���������Ҫ���ڼ���PMM�İٷֱ�
    uint8_t plateHeatPWMPercent; //�����̵��ڵİٷֱ�
    uint8_t lineHeatCnt; //�����߼���������Ҫ���ڼ���PMM�İٷֱ�
    uint8_t lineHeatPWMPercent; //�����ߵ��ڵİٷֱ�

    //PID
    PID_TypeDef oxygenPID;  //��Ũ��PID
    PID_TypeDef plateHeatTemperPID; //�������¶�PID
    PID_TypeDef lineHeatTemperPID; //�������¶�PID
    PID_TypeDef *pAdjustPID;
    int8_t adjustPIDIndex;

    uint8_t oxygenAdjustState; //��Ũ�ȵ���״̬��

    uint8_t beeperStopFlag; //���ڽ�ֹ������

    //ģʽѡ��
    uint8_t workMode; //0������һ��1������һ
    uint8_t pageModeSelect;
}Man_TypeDef;

//������
typedef struct
{
	float temperature;
	float resistorKOhm;
}Pt100_T_R_TypeDef;

//�����
typedef struct
{
    float oxygenPercent;
	float voltageMV;	
}OxygenSensor_V_P_TypeDef;

extern Man_TypeDef man;

void InitMan(void);

void OxygenPercentControl(void);
void TemperControl(void);

void ADCGet(void);
void AlarmCheck(void);

void AutoCalibration(uint8_t flag);

#endif
