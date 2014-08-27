/*
 * sim900a.c
 *
 *  Created on: 2014��8��22��
 *      Author: jeqi
 */

#include "sim900a.h"

__IO u16 Sim900a_Timeout = 0;


u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(UART4_RX_STA&0X8000)		//
	{
		UART4_RX_BUF[UART4_RX_STA&0X7FFF]=0;//
		strx=strstr((const char*)UART4_RX_BUF,(const char*)str);
	}
	return (u8*)strx;
}

/*
 * CMD,ACK,WAITTIME
 * return: 0 -- success
 * 		   1 -- fail
 *
 * */
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0;
	UART4_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		USART_SendData(UART4,(uint16_t)cmd);
		while(USART_GetFlagStatus(UART4,USART_FLAG_TXE) == RESET)
			vTaskDelay(1/portTICK_RATE_MS);	//check if tx finished

	}else u4_printf("%s\r\n",cmd);//
	if(ack&&waittime)		//
	{
		Sim900a_Timeout = waittime;
		while(Sim900a_Timeout)	//
		{
			vTaskDelay(5/portTICK_RATE_MS);
			if(UART4_RX_STA&0X8000)//
			{
				if(sim900a_check_cmd(ack))break;		// receive valid data
				UART4_RX_STA=0;
			}
		}
		if(Sim900a_Timeout==0)res=1;
	}
	return res;
}


const u8 *port = "8086";//port
u8 mode = 0; //0->TCP,1->UDP
u8 ipbuf[16];	//ip buffer
u8 iplen = 0;	//ip length
u8 sim900a_gprs_test(void)
{
 	sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//�ر�����
	sim900a_send_cmd("AT+CIPSHUT","SHUT OK",100);		//�ر��ƶ�����
	if(sim900a_send_cmd("AT+CGCLASS=\"B\"","OK",1000))return 1;				//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ���
	if(sim900a_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",1000))return 2;//����PDP������,��������Э��,��������Ϣ
	if(sim900a_send_cmd("AT+CGATT=1","OK",500))return 3;					//����GPRSҵ��
	if(sim900a_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500))return 4;	 	//����ΪGPRS����ģʽ
	if(sim900a_send_cmd("AT+CIPHEAD=1","OK",500))return 5;	 				//���ý���������ʾIPͷ(�����ж�������Դ)
	return 0;
}



