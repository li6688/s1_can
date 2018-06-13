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
* Description    : ADC延时
* 为了使采集结果准确，在通道切换后，开启转换前，需要需要一段采集时间
* 本延时为50个指令周期，在系统时钟32MHz,指令频率8MHz = 32MHz/4时，
* 延时时间为50/8MHz = 6.25uS. 
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
* Description    : ADC启动一次转化
* Input          : channel:要转换的通道。
* Output         : None
* Return         : None
* Date           : 2016-04-11
* Author         : LL
*******************************************************************************/
static uint16_t  ADC_ConverSingle(uint8_t channel)
{
	//uint8_t retry = 200;
	uint16_t adcvalue; 
    
	ADCON0bits.CHS = channel; //选择转换通道
    ADC_Delay();//延时一段时间，使采集稳定。
    ADCON0bits.GO_nDONE = 1; // start sampling...
    while (0==ADCON0bits.GO_nDONE); // 等待conversion done
    
    adcvalue = ADRESH<<8; //get ADC valu
    adcvalue |= ADRESL;
    
	return adcvalue; 
}

/*******************************************************************************
* Function Name  : ADC_ConverStable
* Description    : ADC采样， 启动ADC转化，多次转化后取平均值，转化一次稳定值

处理方法
采样ADCLEN = 18 次
去掉最大值和最小值，其他值求和取平均

* Input          : channel：要转化的通道
* Output         : None
* Return         : averageval多次采样处理后的结果
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
* Description    : ADC初始化
* Input          : None
* Output         : None
* Return         : None
* Date           : 2016-04-11
* Author         : LL
*******************************************************************************/
void ADC_Init(void)
{
    
    //配置引脚
    TRISCbits.TRISC3 = 1;
    ANSELCbits.ANSC3 = 1;
    TRISCbits.TRISC6 = 1;
    ANSELCbits.ANSC6 = 1;
    TRISCbits.TRISC7 = 1;
    ANSELCbits.ANSC7 = 1;

    ADCON1bits.ADFM = 1; //数据右对齐
    ADCON1bits.ADCS = 6; //AD转换时钟选择
    ADCON1bits.ADNREF = 0; //参考Vref- Vss
    ADCON1bits.ADPREF = 0; //参考Vref+ Vdd

    ADACT = 0;//AD不使用转换触发源

    ADCON0bits.ADON = 1; // turn ADC ON
}

/*******************************************************************************
* Function Name  : ADC_Process 
* Description    : ADC处理，按键检测和NTC温度检测
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