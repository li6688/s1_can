/**
  ******************************************************************************
  * @file    modbus.h
  * @author  LL
  * @version V0.0.0
  * @date    2017-05-11
  * @brief
  ******************************************************************************
  ******************************************************************************
***/
#define __MODBUS_C__

#include "config.h"

#define DEVICE_NUM GlobalVar.DeviceID//�豸ID
#define MODBUS_BROADCASTID 0xFE //�㲥��ַ

//������
#define MODBUS_FUNC_READCOILSTATUS 0x01 //1 ����Ȧ״̬
#define MODBUS_FUNC_READINPUTSTATUS 0x02 //2 ������״̬
#define MODBUS_FUNC_READHOLDREGISTER 0x03 //3 �����ּĴ���
#define MODBUS_FUNC_READINPUTREGISTER 0x04 //4 ������Ĵ���
#define MODBUS_FUNC_WRITESINGLECOIL 0x05 //5 ǿ�õ���Ȧ
#define MODBUS_FUNC_WRITESINGLEREGISTER 0x06 //6 Ԥ�õ��Ĵ���
#define MODBUS_FUNC_READABNORMALSTATUS 0x07 //7 ��ȡ�쳣״̬
#define MODBUS_FUNC_ECHODIAGNOSISCHECK 0x08 //8 �������У��
#define MODBUS_FUNC_READEVENTCNT 0x0B //11 ��ȡ�¼�����
#define MODBUS_FUNC_WRITEMULTIPLECOIL 0x0F//15 ǿ�ö���Ȧ
#define MODBUS_FUNC_WRITEMULTIPLEREGISTER  0x10 //16 Ԥ�ö�Ĵ���
#define MODBUS_FUNC_REPORTSLAVEID 0x11 //17 ����ӻ���ʶ
#define MODBUS_FUNC_RESETCOMMUNICATIONLINK 0x13 //19 ����ͨ����·
//#define MODBUS_FUNC_READDEVICENUM  0x41//65 �û���չ�����룬�㲥��ַʹ�ã����豸ID


//����
#define MODBUS_FUNC_ERRBIT 0x80

//�������
#define MODEBUS_ERRC_INVALIDFUNC 0x01 //�Ƿ�����
#define MODEBUS_ERRC_INVALIDADDRESS 0x02 //�Ƿ���ַ
#define MODEBUS_ERRC_INVALIDVALUE 0x03 //�Ƿ�����
#define MODEBUS_ERRC_SLAVEFAULT 0x04 //�ӻ�����
#define MODEBUS_ERRC_CONFIRM 0x05 //�ӻ�ȷ��
#define MODEBUS_ERRC_SLAVEBUSY 0x07 //�ӻ�æ

/*
 * һЩ������Ĵ����
 * ����ʱ�ᴦ����صĹ����룬
 * �ر�ʱ��������صĹ����룬��ش�����Ҳ���ᱻ���룬�Լ�С������
 * 0�رգ�1��
 */
#define MODBUS_FUNCON_READCOILSTATUS 1 //1 ����Ȧ״̬
#define MODBUS_FUNCON_READINPUTSTATUS  0 //2 ������״̬
#define MODBUS_FUNCON_READHOLDREGISTER 1 //3 �����ּĴ���
#define MODBUS_FUNCON_READINPUTREGISTER 0 //4 ������Ĵ���
#define MODBUS_FUNCON_WRITESINGLECOIL 1 //5 ǿ�õ���Ȧ
#define MODBUS_FUNCON_WRITESINGLEREGISTER 1 //6 Ԥ�õ��Ĵ���
#define MODBUS_FUNCON_READABNORMALSTATUS 0 //7 ��ȡ�쳣״̬
#define MODBUS_FUNCON_ECHODIAGNOSISCHECK 0 //8 �������У��
#define MODBUS_FUNCON_READEVENTCNT 0 //11 ��ȡ�¼�����
#define MODBUS_FUNCON_WRITEMULTIPLECOIL 0//15 ǿ�ö���Ȧ
#define MODBUS_FUNCON_WRITEMULTIPLEREGISTER  1 //16 Ԥ�ö�Ĵ���
#define MODBUS_FUNCON_REPORTSLAVEID 0 //17 ����ӻ���ʶ
#define MODBUS_FUNCON_RESETCOMMUNICATIONLINK 0 //19 ����ͨ����·

static Modbus_StructType Modbus_DataPort1;//�˿�1��Э������
Modbus_StructType *pModbus_DataPort1 = &Modbus_DataPort1;

/*******************************************************************************
* Function Name  : Modbus_PackageDeviceNum
* Description    : ����豸ID
* Input          : buf:��Ϣ
                   len:��Ϣ����
* Output         : None
* Return         : None
* Date           : 2017-03-01
* Author         : LL
*******************************************************************************/
void Modbus_PackageDeviceNum(uint8_t *buf, uint16_t len)
{
    uint8_t buft[25];
    uint8_t i;

	buft[0] = DEVICE_NUM & 0xFF;

	for(i=0; i<len; i++)
	{
		buft[i+1] = buf[i];
	}
	UART_PrintfNormal(buft, len+1); 
}

/*******************************************************************************
* Function Name  : Modbus_PackageFuncDeviceNum
* Description    : ����豸ID,�͹����룬
* Input          : func:������
                   buf:��Ϣ
                   len:��Ϣ����
* Output         : None
* Return         : None
* Date           : 2017-03-01
* Author         : LL
*******************************************************************************/
void Modbus_PackageFuncDeviceNum(uint8_t func, uint8_t *buf, uint16_t len)
{
    uint8_t buft[25];
    uint8_t i;

	buft[0] = DEVICE_NUM & 0xFF;
	buft[1] = func;

	for(i=0; i<len; i++)
	{
		buft[i+2] = buf[i];
	}
	UART_PrintfNormal(buft, len+2); 
}


/*******************************************************************************
* Function Name  : Modbus_SendErrorResponse
* Description    : ���ʹ�����Ӧ����������ʹ�����룬�����
 ��ȷ���յ�������һ֡���ݣ������쳣�Ƿ�����������������������Ӧ
* Input          : func:������
                   errc��������
* Output         : None
* Return         : None
* Date           : 2017-05-18
* Author         : LL
*******************************************************************************/
static void Modbus_SendErrorResponse(uint8_t func, uint8_t errc)
{
    uint8_t buf[3];
    
    buf[0] = func | MODBUS_FUNC_ERRBIT;
    buf[1] = errc;
	Modbus_PackageDeviceNum(buf,2);
}

/*******************************************************************************
* Function Name  : Modbus_SendResponse
* Description    : ������Ӧ�����������Ҫ���͵����ݣ������
 ��ȷ���յ���������������������������Ӧ
* Input          : func:������
                   len������
                   data:����
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
static void Modbus_SendResponse(uint8_t func, uint16_t len, uint8_t *data)
{
    uint8_t i;
    uint8_t buf[20];
    
    if (len>18) len = 18;
    
    buf[0] = func;
    buf[1] = len & 0xFF;
    for(i=0; i<len; i++)
    {
        buf[i+2] = data[i];
    }
    switch (func)
    {
        case MODBUS_FUNC_READCOILSTATUS:
        case MODBUS_FUNC_READHOLDREGISTER:
		case MODBUS_FUNC_WRITESINGLECOIL:
		case MODBUS_FUNC_WRITESINGLEREGISTER:
		case MODBUS_FUNC_WRITEMULTIPLEREGISTER:
            Modbus_PackageDeviceNum(buf,len+2);
            break;
        default:break;
    }
}

#if MODBUS_FUNCON_READCOILSTATUS
/*******************************************************************************
* Function Name  : Modbus_ReadCoilStatus
* Description    : �������Ȧ״̬
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
static void Modbus_ReadCoilStatus(uint8_t *data, uint16_t len)
{
    uint16_t addr;
    uint16_t num;
    uint8_t buf[5];
    //if (len<4) return;
    
    addr = data[1] | (data[0]<<8);
    num = data[3] | (data[2]<<8);
    
    if (0==addr)
    {
        if (1==num)
        {
        	Modbus_SendResponse(MODBUS_FUNC_READCOILSTATUS,1,buf);
        }
		else
		{
			//���ݷǷ�
			Modbus_SendErrorResponse( MODBUS_FUNC_READCOILSTATUS,MODEBUS_ERRC_INVALIDVALUE);
		}
    }
    else
    {
        Modbus_SendErrorResponse(MODBUS_FUNC_READCOILSTATUS,MODEBUS_ERRC_INVALIDADDRESS);
    }   
}
#endif /* MODBUS_FUNCON_READCOILSTATUS */

#if MODBUS_FUNCON_READHOLDREGISTER
/*******************************************************************************
* Function Name  : Modbus_ReadHoldRegister
* Description    : ��������ּĴ���
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
static void Modbus_ReadHoldRegister(uint8_t *data, uint16_t len)
{
    uint8_t i;
    uint16_t addr;
    uint16_t num;
    uint8_t buf[20];
    //if (len<4) return;
    
    addr = data[1] | (data[0]<<8);
    num = data[3] | (data[2]<<8);
    
    if ((addr < MODBUS_REGISTER_MAX) && (2!=addr))//����ַ�ĺϷ��ԣ����ڲ����ʴ�1��ַ��ʼ��ռ��������ַ������2��ַ���Ϸ�
    {
        if (num>(MODBUS_REGISTER_MAX-addr)) num= MODBUS_REGISTER_MAX-addr;//���Ĵ���������������Ч����	
        i=0;
		switch (addr)
		{
			case 0:
				buf[i++] = GlobalVar.DeviceID;
				buf[i++] = 0;
			case 1:
				buf[i++] = (GlobalVar.UartBaudRate>>24) & 0xFF;
				buf[i++] = (GlobalVar.UartBaudRate>>16) & 0xFF;
		    	buf[i++] = (GlobalVar.UartBaudRate>>8) & 0xFF;
				buf[i++] = GlobalVar.UartBaudRate & 0xFF;
			case 3:
				buf[i++] = (GlobalVar.GrayCode>>8) & 0xFF;
				buf[i++] = GlobalVar.GrayCode & 0xFF;
				break;
			default:break;
				
		}
        Modbus_SendResponse(MODBUS_FUNC_READHOLDREGISTER,num*2,buf);
    }
    else
    {
        Modbus_SendErrorResponse( MODBUS_FUNC_READHOLDREGISTER,MODEBUS_ERRC_INVALIDADDRESS);
    }
}
#endif /* MODBUS_FUNCON_READHOLDREGISTER */

#if MODBUS_FUNCON_WRITESINGLECOIL
/*******************************************************************************
* Function Name  : Modbus_WriteSinglCoil
* Description    : д������Ȧ
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-06-30
* Author         : LL
*******************************************************************************/
static void Modbus_WriteSinglCoil(uint8_t *data, uint16_t len)
{
    uint16_t addr;
	uint16_t sta;
   
    //if (len<4) return;
    
    addr = data[1] | (data[0]<<8);
    sta = data[3] | (data[2]<<8);

	if (1==addr)
	{
		//����
        if (0x0000==sta)//OFF
        {
            Flag.Start.cursta= FALSE;
            Modbus_PackageFuncDeviceNum(MODBUS_FUNC_WRITESINGLECOIL,data, 4);
        }
        else
        {
            //���ݷǷ�
            Modbus_SendErrorResponse( MODBUS_FUNC_WRITESINGLECOIL,MODEBUS_ERRC_INVALIDVALUE);
        }
	}
	else
	{
		Modbus_SendErrorResponse( MODBUS_FUNC_WRITESINGLECOIL,MODEBUS_ERRC_INVALIDADDRESS);
	}
}
#endif /* MODBUS_FUNCON_WRITESINGLECOIL */

#if MODBUS_FUNCON_WRITESINGLEREGISTER
/*******************************************************************************
* Function Name  : Modbus_WriteSinglRegister
* Description    : д�����Ĵ���
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-06-30
* Author         : LL
*******************************************************************************/
static void Modbus_WriteSinglRegister(uint8_t *data, uint16_t len)
{
	uint8_t id;
    uint16_t addr;
    //uint8_t buf[10];
    //if (len<4) return;
    
    addr = data[1] | (data[0]<<8);
    
    if (0==addr)//�豸ID 0��ַ�������õ��Ĵ���д
    {
		id = data[2];
        if ((id<MODBUS_DEVICEID_MAX) && (id>0)) 
        {
			Eeprom_Write(MODBUS_DEVICEID_ADDR,&id,1);
			Modbus_PackageFuncDeviceNum(MODBUS_FUNC_WRITESINGLEREGISTER,data, 4);
			GlobalVar.DeviceID = id;
        }
		else
		{
			//���ݷǷ�
			Modbus_SendErrorResponse( MODBUS_FUNC_WRITESINGLEREGISTER,MODEBUS_ERRC_INVALIDVALUE);
		}
    }
    else
    {
        Modbus_SendErrorResponse( MODBUS_FUNC_WRITESINGLEREGISTER,MODEBUS_ERRC_INVALIDADDRESS);
    }
}
#endif /* MODBUS_FUNCON_WRITESINGLEREGISTER */


#if MODBUS_FUNCON_WRITEMULTIPLEREGISTER
/*******************************************************************************
* Function Name  : Modbus_WriteMultipleRegister
* Description    : д����Ĵ���
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-06-30
* Author         : LL
*******************************************************************************/
static void Modbus_WriteMultipleRegister(uint8_t *data, uint16_t len)
{
    uint8_t i;
	//uint8_t bytenum;
    uint16_t addr;
    uint16_t num;
    uint8_t buf[10];
    //if (len<8) return;
    
    addr = data[1] | (data[0]<<8);
    num = data[3] | (data[2]<<8);
	//bytenum = data[4];
    
    if (1==addr)
    {
        if (2==num)
        {
			for(i=0;i<4;i++)
			{
				buf[i] = data[i+5]; 
			}
			Eeprom_Write(MODBUS_UARTBAUDRATE_ADDR,buf,4);
			Modbus_PackageFuncDeviceNum(MODBUS_FUNC_WRITEMULTIPLEREGISTER,data,4);//д����Ĵ�����ֻ��Ӧ�Ĵ�����ַ������
			while(1);//ϵͳ��λ���������ò���
        }
		else
		{
			//���ݷǷ�,���ݳ��Ȳ���ȷ
			Modbus_SendErrorResponse( MODBUS_FUNC_WRITESINGLEREGISTER,MODEBUS_ERRC_INVALIDVALUE);
		}
    }
    else
    {
        Modbus_SendErrorResponse( MODBUS_FUNC_WRITEMULTIPLEREGISTER,MODEBUS_ERRC_INVALIDADDRESS);
    }
}
#endif /* MODBUS_FUNCON_WRITEMULTIPLEREGISTER */

/*******************************************************************************
* Function Name  : Modbus_FunctionCodeType
* Description    : ������������
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
static void Modbus_FunctionCodeType(uint8_t *data, uint16_t len)
{
    if (0 == len) return;
	
	switch (data[0])
	{
#if MODBUS_FUNCON_READCOILSTATUS
        case MODBUS_FUNC_READCOILSTATUS://����Ȧ
            Modbus_ReadCoilStatus(data+1,len-1);
            break;
#endif           
#if MODBUS_FUNCON_READHOLDREGISTER
        case MODBUS_FUNC_READHOLDREGISTER://�����ּĴ���
            Modbus_ReadHoldRegister(data+1,len-1);
            break;
#endif
#if MODBUS_FUNCON_WRITESINGLECOIL
		case MODBUS_FUNC_WRITESINGLECOIL://д������Ȧ
			Modbus_WriteSinglCoil(data+1,len-1);
			break;
#endif
#if MODBUS_FUNCON_WRITESINGLEREGISTER
		case MODBUS_FUNC_WRITESINGLEREGISTER://д�����Ĵ���
			Modbus_WriteSinglRegister(data+1,len-1);
			break;
#endif
#if MODBUS_FUNCON_WRITEMULTIPLEREGISTER
		case MODBUS_FUNC_WRITEMULTIPLEREGISTER://д����Ĵ���
			Modbus_WriteMultipleRegister(data+1,len-1);
			break;
#endif
        default:
            Modbus_SendErrorResponse(data[0],MODEBUS_ERRC_INVALIDFUNC);
            break;
	}
}

/*******************************************************************************
* Function Name  : Modbus_BroadcastFunctionCodeType
* Description    : ����㲥��ַ����������
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-05-18
* Author         : LL
*******************************************************************************/
static void Modbus_BroadcastFunctionCodeType(uint8_t *data, uint16_t len)
{
    if (0 == len) return;
	
	switch (data[0])
	{
#if MODBUS_FUNCON_READHOLDREGISTER
		case MODBUS_FUNC_READHOLDREGISTER://�����ּĴ���
			Modbus_ReadHoldRegister(data+1,len-1);
			break;
#endif
        default:
            Modbus_SendErrorResponse(data[0],MODEBUS_ERRC_INVALIDFUNC);
            break;
	}
}

/*******************************************************************************
* Function Name  : Modbus_DeviceNum
* Description    : �����豸��
* Input          : data:���յ�������
                   len:������
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void Modbus_DeviceNum(uint8_t *data, uint16_t len)
{
    uint8_t devicenum;

    if (0==len) return;

    devicenum = data[0];

	if (MODBUS_BROADCASTID==devicenum)
	{
		Modbus_BroadcastFunctionCodeType(data+1,len-1);
	}
	else if (DEVICE_NUM==devicenum)
	{
		Modbus_FunctionCodeType(data+1,len-1);
	}
	else
	{
		DEBUG_PRINTFMASSAGES("Device err");
        DEBUG_PRINTFNUMBERHEX(devicenum);
	}

}

/*******************************************************************************
* Function Name  : Modbus_DataDecode_RTU
* Description    : RTUģʽЭ�����
* Input          : data:Ҫ���������
 *                 len:���ݳ���
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void Modbus_DataDecode_RTU(uint8_t *data,  uint16_t len)
{
    uint16_t crc16_check;
    uint16_t crc16_r;
    if (len<2) return;
    
    crc16_check = CRC_16(data, len-2);
    crc16_r = data[len-2] | (data[len-1]<<8);
    if (crc16_check == crc16_r)
    {
        Modbus_DeviceNum(data,len-2);
    }
    else
    {
        DEBUG_PRINTFMASSAGES("");
        DEBUG_PRINTFMASSAGES("**CRC check error**");
        DEBUG_PRINTFMASSAGES("Recive CRC:");
        DEBUG_PRINTFNUMBERHEX(crc16_r);
        DEBUG_PRINTFMASSAGES("Check CRC:");
        DEBUG_PRINTFNUMBERHEX(crc16_check);
        DEBUG_PRINTFMASSAGES("Data:");
        DEBUG_PRINTFMASSAGESHEX(data, len-2);
    }
}

/*******************************************************************************
* Function Name  : Modbus_Init
* Description    : �������ݽ��룬�����ݲ������,�Զ���Э���ʽ
* Input          : data:Ҫ���������
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void Modbus_Init(void)
{
    pModbus_DataPort1 = &Modbus_DataPort1;
    pModbus_DataPort1->step = 0;
    //pModbus_DataPort1->cnt = 0;
    //pModbus_DataPort1->len = 0;
}

/*******************************************************************************
* Function Name  : Modbus_Test
* Description    : �������ݽ��룬�����ݲ������,�Զ���Э���ʽ
* Input          : data:Ҫ���������
* Output         : None
* Return         : None
* Date           : 2017-05-11
* Author         : LL
*******************************************************************************/
void Modbus_Test(uint8_t data)
{

	static uint8_t step = 0;
	
    switch (step)
    {
        case 0:
            if ('A' == data)
            {
                step = 1;		
            }
            break;
        case 1:
            if ('B' == data)
            {
                step = 2;
            }
            else
            {
                step = 0;
            }
            break;
        case 2:
            //Modbus_PrintfCommand(UPDATA_COMMAND_INUPDATAMODE);
			step = 0;
            break;
        default:
            step = 0;
            break;
    }

}

/* ************ ****** ************ THE FILE END  ************ ****** ************ */
