/**
  ******************************************************************************
  * @file    adc.c
  * @author  LL
  * @version V0.0.0
  * @date    2014-04-23
  * @brief   
  ******************************************************************************
  ******************************************************************************
***/

#define __ADC_C__
#include "config.h"
    
#define ADCLEN 18

/*******************************************************************************
* Function Name  : ADC_Delay 
* Description    : ADC��ʱ
* Ϊ��ʹ�ɼ����׼ȷ����ͨ���л��󣬿���ת��ǰ����Ҫ��Ҫһ�βɼ�ʱ��
* ����ʱΪ50��ָ�����ڣ���ϵͳʱ��32MHz,ָ��Ƶ��8MHz = 32MHz/4ʱ��
* ��ʱʱ��Ϊ50/8MHz = 6.25uS. 
* Input          : None
* Output         : None
* Return         : None
* Date           : 2017-05-19
* Author         : LL
*******************************************************************************/
static void ADC_Delay(void)
{
    uint8_t i;
    for(i=0; i<50; i++)
    {
        NOP();
    }
}

/*******************************************************************************
* Function Name  : ADC_ConverSingle 
* Description    : ADC����һ��ת��
* Input          : channel:Ҫת����ͨ����
* Output         : None
* Return         : None
* Date           : 2016-04-11
* Author         : LL
*******************************************************************************/
static uint16_t  ADC_ConverSingle(uint8_t channel)
{
	//uint8_t retry = 200;
	uint16_t adcvalue; 
    
	ADCON0bits.CHS = channel; //ѡ��ת��ͨ��
    ADC_Delay();//��ʱһ��ʱ�䣬ʹ�ɼ��ȶ���
    ADCON0bits.GO_nDONE = 1; // start sampling...
    while (0==ADCON0bits.GO_nDONE); // �ȴ�conversion done
    
    adcvalue = ADRESH<<8; //get ADC valu
    adcvalue |= ADRESL;
    
	return adcvalue; 
}

/*******************************************************************************
* Function Name  : ADC_ConverStable
* Description    : ADC������ ����ADCת�������ת����ȡƽ��ֵ��ת��һ���ȶ�ֵ

������
����ADCLEN = 18 ��
ȥ�����ֵ����Сֵ������ֵ���ȡƽ��

* Input          : channel��Ҫת����ͨ��
* Output         : None
* Return         : averageval��β��������Ľ��
* Date           : 2016-06-20
* Author         : LL
*******************************************************************************/
uint16_t ADC_ConverStable(uint8_t channel)
{
	uint8_t i;
	uint16_t adcval;
	uint16_t minval=0xFFFF;
	uint16_t maxval=0;
	uint16_t averageval;
	uint32_t totalval=0;
	
	for (i=0; i<ADCLEN; i++)
	{
		adcval = ADC_ConverSingle(channel);
		totalval += adcval;
		if (minval>adcval) minval = adcval;
		if (maxval<adcval) maxval = adcval;
	}
	
	totalval = totalval - minval - maxval;
	averageval = totalval >> 4;
	
	return averageval;
}

/*******************************************************************************
* Function Name  : ADC_Init 
* Description    : ADC��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-04-11
* Author         : LL
*******************************************************************************/
void ADC_Init(void)
{
    
    //��������
    TRISCbits.TRISC3 = 1;
    ANSELCbits.ANSC3 = 1;
    TRISCbits.TRISC6 = 1;
    ANSELCbits.ANSC6 = 1;
    TRISCbits.TRISC7 = 1;
    ANSELCbits.ANSC7 = 1;

    ADCON1bits.ADFM = 1; //�����Ҷ���
    ADCON1bits.ADCS = 6; //ADת��ʱ��ѡ��
    ADCON1bits.ADNREF = 0; //�ο�Vref- Vss
    ADCON1bits.ADPREF = 0; //�ο�Vref+ Vdd

    ADACT = 0;//AD��ʹ��ת������Դ

    ADCON0bits.ADON = 1; // turn ADC ON
}

/*******************************************************************************
* Function Name  : ADC_Process 
* Description    : ADC������������NTC�¶ȼ��
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-04-11
* Author         : LL
*******************************************************************************/
void ADC_Process(void)
{
	uint32_t adc_value;
	
	if (FALSE == Flag.ADC_Scan) return;
	
	adc_value = ADC_ConverStable(25);
	DEBUG_PRINTFNUMBERHEX(adc_value);
	DEBUG_PRINTFNUMBERBCD((uint32_t)(adc_value*3300)>>10);
    adc_value = ADC_ConverStable(24);
	DEBUG_PRINTFNUMBERHEX(adc_value);
	DEBUG_PRINTFNUMBERBCD((uint32_t)(adc_value*3300)>>10);
    adc_value = ADC_ConverStable(27);
	DEBUG_PRINTFNUMBERHEX(adc_value);
	DEBUG_PRINTFNUMBERBCD((uint32_t)(adc_value*3300)>>10);
    adc_value = ADC_ConverStable(26);
	DEBUG_PRINTFNUMBERHEX(adc_value);
	DEBUG_PRINTFNUMBERBCD((uint32_t)(adc_value*3300)>>10);
	
	adc_value = adc_value;
	Flag.ADC_Scan = FALSE;  
}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */