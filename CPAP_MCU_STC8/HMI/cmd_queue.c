#include "cmd_queue.h"
#include "uart.h"

#ifdef __cplusplus
extern "C" {
#endif

QUEUE lcd_que = {0,0,0};  //ָ�����
QUEUE user_que = {0,0,0};

void queue_reset(QUEUE *que)
{
	que->_head = que->_tail = 0;
	
	que->_cmd_pos = 0;
}

//void queue_push(QUEUE *que, qdata _data)
//{
//	qsize pos = (que->_head+1)%QUEUE_MAX_SIZE;
//	if(pos!=que->_tail)//����״̬
//	{
//		que->_data[que->_head] = _data;
//		que->_head = pos;
//	}
//}

//�Ӷ�����ȡһ������
void queue_pop(QUEUE *que, qdata* _data)
{
	if(que->_tail!=que->_head)//�ǿ�״̬
	{
		*_data = que->_data[que->_tail];
		que->_tail = (que->_tail+1)%QUEUE_MAX_SIZE;
	}
}

//��ȡ��������Ч���ݸ���
static qsize queue_size(QUEUE *que)
{
	return ((que->_head+QUEUE_MAX_SIZE-que->_tail)%QUEUE_MAX_SIZE);
}

qsize queue_find_cmd(QUEUE *que, qdata *buffer, qsize buf_len)
{
    if(que == &user_que)//�û��ӿ�������ȡ
    {
        static uint32_t cmd_state = 0;
        qsize cmd_size = 0;
    	qdata _data = 0;

    	while(queue_size(que)>0)
    	{
    		//ȡһ������
    		queue_pop(que, &_data);
    
    		if(que->_cmd_pos==0&&_data!=USER_CMD_HEAD)//ָ���һ�ֽڱ�����֡ͷ����������
    		    continue;
    
    		if(que->_cmd_pos<buf_len)//��ֹ���������
    			buffer[que->_cmd_pos++] = _data;
    
    		cmd_state = ((cmd_state<<8)|_data);//ƴ�����4���ֽڣ����һ��32λ����
    
    		//���4���ֽ���֡βƥ�䣬�õ�����֡
    		if(cmd_state==USER_CMD_TAIL)
    		{
    			cmd_size = que->_cmd_pos; //ָ���ֽڳ���
    			cmd_state = 0;  //���¼��֡β
    			que->_cmd_pos = 0; //��λָ��ָ��
    
    			return cmd_size;
    		}
    	}
    }
    else
    {
        uint32_t cmdSize = 0;
    	qdata _data;
    	static uint16_t cmdHead = 0;
    	static uint8_t cmdLen = 3;
    
    	while(queue_size(que)>0)
    	{
    		queue_pop(que, &_data);
    
    		cmdHead = (cmdHead << 8) | _data;
    
    		if(que->_cmd_pos == 0)
    		{
    			if(cmdHead != LCD_CMD_HEAD)//ָ���һ���ֽڱ�����֡ͷ����������
    				continue;
    			else
    			{
                    buffer[que->_cmd_pos++] = (LCD_CMD_HEAD>>8);
    			}
    		}

            buffer[que->_cmd_pos++] = _data;

    		if(que->_cmd_pos == 3)
                cmdLen = _data + 3;
    
    		//�չ�5�ֽڵõ�����֡
    		if (que->_cmd_pos >= cmdLen)
    		{
    			cmdLen = 3;
    			cmdSize = que->_cmd_pos; //ָ���ֽڳ���
    			que->_cmd_pos = 0; //��λָ��ָ��
    			cmdHead = 0;

    			return cmdSize;
    		} 
    	}     
    }

	return 0;//û���γ�������һ֡   
}

#ifdef __cplusplus
}
#endif
