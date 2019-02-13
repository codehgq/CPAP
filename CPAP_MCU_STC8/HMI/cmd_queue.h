#ifndef _CMD_QUEUE
#define _CMD_QUEUE
#include "hmi_driver.h"

typedef unsigned char qdata;
typedef unsigned short qsize;

//#define QUEUE_MAX_SIZE 20 //128  /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/

//#define USER_CMD_HEAD 0XEE  //֡ͷ
//#define USER_CMD_TAIL 0XFFFCFFFF //֡β

typedef struct _QUEUE
{
	qsize _head; //����ͷ
	qsize _tail;  //����β
	qdata _data[QUEUE_MAX_SIZE];	//�������ݻ�����

	qsize _cmd_pos; //��ǰָ��ָ��λ��
}QUEUE;

/*! 
 *  \brief  ���ָ������
 */
void queue_reset(QUEUE *que);

/*! 
 * \brief  ���ָ������
 * \detial ���ڽ��յ����ݣ�ͨ���˺�������ָ����� 
 *  \param  _data ָ������
 */
void queue_push(QUEUE *que, qdata _data);

/*! 
 *  \brief  ��ָ�������ȡ��һ��������ָ��
 *  \param  cmd ָ����ջ�����
 *  \param  buf_len ָ����ջ�������С
 *  \return  ָ��ȣ�0��ʾ������������ָ��
 */
qsize queue_find_cmd(QUEUE *que, qdata *cmd,qsize buf_len);

void queue_pop(QUEUE *que, qdata* _data);

extern QUEUE lcd_que;
extern QUEUE user_que;

#endif
