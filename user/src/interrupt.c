/**
******************************************************************************
* @file    interrupt.c
* @author  LL
* @version V0.0.0
* @date    2017-05-17
* @brief   
******************************************************************************
******************************************************************************
***/
#define __INTERRUPT_C__

#include "config.h"

#if LOWPOWER_EN
#define LOWPOWER_ENAUTO 0
#endif

typedef struct
{
	uint16_t led_time;
	uint16_t onesec_time;
	uint16_t gray_time;
	uint16_t sample_time;//����ģʽ�£��ɼ�ʱ��
}Delay_StructType;

Delay_StructType Delay;

#define T_10MS 1
#define T_100MS T_10MS*10
#define T_200MS T_10MS*20
#define T_300MS T_10MS*30
#define T_500MS T_10MS*50
#define T_1S T_500MS*2
#define T_2S T_1S*2
#define T_5S T_1S*5
#define T_1M T_1S*60

#define REOUTPUT_OPENTIME T_5S


void interrupt  myIsr(void)
{
    if (TMR2IE && TMR2IF) 
	{
		TMR2IF=0;
        if (++Delay.onesec_time >= T_1S)
        {
            Delay.onesec_time = 0;
            Flag.flag = TRUE;
        }
		#if LOWPOWER_ENAUTO
		if (++GlobalVar.PortConnectTime >= T_5S)
		{
			GlobalVar.PortConnectTime = 0;
			Flag.Start.cursta = FALSE;
		}
		#endif
		if (++Delay.gray_time >= T_1S)
		{
			Delay.gray_time = 0;
			Flag.Gray_Scan = TRUE;
		}
		
	}

    if (TMR4IE && TMR4IF)
    {
        TMR4IF = 0;
        static unsigned char cntbkp = 0;
        static unsigned char idletmr = 0;
        if (pModbus_DataPort1->cnt > 0)  //���ռ�����������ʱ��������߿���ʱ��
        {
            if (cntbkp != pModbus_DataPort1->cnt)  //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
            {
                cntbkp = pModbus_DataPort1->cnt;
                idletmr = 0;
            }
            else
            {
                if (idletmr < 30)  //���ռ�����δ�ı䣬�����߿���ʱ���ۻ�����ʱ��
                {
                    idletmr++;
                    if (idletmr >= 30)  //����ʱ�䳬��30ms����Ϊһ֡����������
                    {
                        pModbus_DataPort1->len = pModbus_DataPort1->cnt;
                        pModbus_DataPort1->cnt = 0;
                        Flag.ReciveOK = TRUE; //����������־
                    }
                }
            }
        }
        else
        {
            cntbkp = 0;
        }
    }
 
    if (RCIE && RCIF)
    {
        uint8_t data;
        
        RCIF = 0;
   
		if (0!=FREE) //֡����
		{
			data = RC1REG;
		}
		else if (0!=OERR)//�������
		{
			CREN = 0;
		}
		else
        {  
			data = RC1REG;
            if (pModbus_DataPort1->cnt < DATACODEBUFF_MAXSIZE) //���ջ�������δ����ʱ��
            {
                if (FALSE==Flag.ReciveOK)
                {
                    pModbus_DataPort1->DataCodeBuff[pModbus_DataPort1->cnt] = data; //��������ֽڣ�������������
                    pModbus_DataPort1->cnt++;
                }
            }
        }
    }
    
    //����жϱ�־
    //PIR0 = 0;
    //PIR1 = 0;
    //PIR2 = 0;
    //PIR3 = 0;
    //PIR4 = 0;
    
}



/* ************ ****** ************ THE FILE END  ************ ****** ************ */	
