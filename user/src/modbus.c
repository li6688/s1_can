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

#define DEVICE_NUM GlobalVar.DeviceID//设备ID
#define MODBUS_BROADCASTID 0xFE //广播地址

//功能码
#define MODBUS_FUNC_READCOILSTATUS 0x01 //1 读线圈状态
#define MODBUS_FUNC_READINPUTSTATUS 0x02 //2 读输入状态
#define MODBUS_FUNC_READHOLDREGISTER 0x03 //3 读保持寄存器
#define MODBUS_FUNC_READINPUTREGISTER 0x04 //4 读输入寄存器
#define MODBUS_FUNC_WRITESINGLECOIL 0x05 //5 强置单线圈
#define MODBUS_FUNC_WRITESINGLEREGISTER 0x06 //6 预置单寄存器
#define MODBUS_FUNC_READABNORMALSTATUS 0x07 //7 读取异常状态
#define MODBUS_FUNC_ECHODIAGNOSISCHECK 0x08 //8 回送诊断校验
#define MODBUS_FUNC_READEVENTCNT 0x0B //11 读取事件计数
#define MODBUS_FUNC_WRITEMULTIPLECOIL 0x0F//15 强置多线圈
#define MODBUS_FUNC_WRITEMULTIPLEREGISTER  0x10 //16 预置多寄存器
#define MODBUS_FUNC_REPORTSLAVEID 0x11 //17 报告从机标识
#define MODBUS_FUNC_RESETCOMMUNICATIONLINK 0x13 //19 重置通信链路
//#define MODBUS_FUNC_READDEVICENUM  0x41//65 用户扩展功能码，广播地址使用，读设备ID


//错误
#define MODBUS_FUNC_ERRBIT 0x80

//错误代码
#define MODEBUS_ERRC_INVALIDFUNC 0x01 //非法功能
#define MODEBUS_ERRC_INVALIDADDRESS 0x02 //非法地址
#define MODEBUS_ERRC_INVALIDVALUE 0x03 //非法数据
#define MODEBUS_ERRC_SLAVEFAULT 0x04 //从机故障
#define MODEBUS_ERRC_CONFIRM 0x05 //从机确认
#define MODEBUS_ERRC_SLAVEBUSY 0x07 //从机忙

/*
 * 一些功能码的处理宏
 * 当打开时会处理相关的功能码，
 * 关闭时不处理相关的功能码，相关处理函数也不会被编译，以减小程序量
 * 0关闭，1打开
 */
#define MODBUS_FUNCON_READCOILSTATUS 1 //1 读线圈状态
#define MODBUS_FUNCON_READINPUTSTATUS  0 //2 读输入状态
#define MODBUS_FUNCON_READHOLDREGISTER 1 //3 读保持寄存器
#define MODBUS_FUNCON_READINPUTREGISTER 0 //4 读输入寄存器
#define MODBUS_FUNCON_WRITESINGLECOIL 1 //5 强置单线圈
#define MODBUS_FUNCON_WRITESINGLEREGISTER 1 //6 预置单寄存器
#define MODBUS_FUNCON_READABNORMALSTATUS 0 //7 读取异常状态
#define MODBUS_FUNCON_ECHODIAGNOSISCHECK 0 //8 回送诊断校验
#define MODBUS_FUNCON_READEVENTCNT 0 //11 读取事件计数
#define MODBUS_FUNCON_WRITEMULTIPLECOIL 0//15 强置多线圈
#define MODBUS_FUNCON_WRITEMULTIPLEREGISTER  1 //16 预置多寄存器
#define MODBUS_FUNCON_REPORTSLAVEID 0 //17 报告从机标识
#define MODBUS_FUNCON_RESETCOMMUNICATIONLINK 0 //19 重置通信链路

static Modbus_StructType Modbus_DataPort1;//端口1的协议数据
Modbus_StructType *pModbus_DataPort1 = &Modbus_DataPort1;

/*******************************************************************************
* Function Name  : Modbus_PackageDeviceNum
* Description    : 打包设备ID
* Input          : buf:信息
                   len:信息长度
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
* Description    : 打包设备ID,和功能码，
* Input          : func:功能码
                   buf:信息
                   len:信息长度
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
* Description    : 发送错误响应，将功能码和错误代码，封包。
 正确接收到主机的一帧数据，出现异常非法情况，调用这个函数发送响应
* Input          : func:功能码
                   errc：错误码
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
* Description    : 发送响应，将功能码和要发送的数据，封包。
 正确接收到主机的命令，调用这个函数发送响应
* Input          : func:功能码
                   len：长度
                   data:数据
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
* Description    : 处理读线圈状态
* Input          : data:接收到的数据
                   len:数据量
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
			//数据非法
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
* Description    : 处理读保持寄存器
* Input          : data:接收到的数据
                   len:数据量
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
    
    if ((addr < MODBUS_REGISTER_MAX) && (2!=addr))//检查地址的合法性，串口波特率从1地址开始，占用两个地址，所以2地址不合法
    {
        if (num>(MODBUS_REGISTER_MAX-addr)) num= MODBUS_REGISTER_MAX-addr;//读寄存器的数量超过有效个数	
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
* Description    : 写单个线圈
* Input          : data:接收到的数据
                   len:数据量
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
		//休眠
        if (0x0000==sta)//OFF
        {
            Flag.Start.cursta= FALSE;
            Modbus_PackageFuncDeviceNum(MODBUS_FUNC_WRITESINGLECOIL,data, 4);
        }
        else
        {
            //数据非法
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
* Description    : 写单个寄存器
* Input          : data:接收到的数据
                   len:数据量
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
    
    if (0==addr)//设备ID 0地址，可以用单寄存器写
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
			//数据非法
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
* Description    : 写多个寄存器
* Input          : data:接收到的数据
                   len:数据量
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
			Modbus_PackageFuncDeviceNum(MODBUS_FUNC_WRITEMULTIPLEREGISTER,data,4);//写多个寄存器，只响应寄存器地址和数量
			while(1);//系统复位，重新配置参数
        }
		else
		{
			//数据非法,数据长度不正确
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
* Description    : 处理功能码类型
* Input          : data:接收到的数据
                   len:数据量
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
        case MODBUS_FUNC_READCOILSTATUS://读线圈
            Modbus_ReadCoilStatus(data+1,len-1);
            break;
#endif           
#if MODBUS_FUNCON_READHOLDREGISTER
        case MODBUS_FUNC_READHOLDREGISTER://读保持寄存器
            Modbus_ReadHoldRegister(data+1,len-1);
            break;
#endif
#if MODBUS_FUNCON_WRITESINGLECOIL
		case MODBUS_FUNC_WRITESINGLECOIL://写单个线圈
			Modbus_WriteSinglCoil(data+1,len-1);
			break;
#endif
#if MODBUS_FUNCON_WRITESINGLEREGISTER
		case MODBUS_FUNC_WRITESINGLEREGISTER://写单个寄存器
			Modbus_WriteSinglRegister(data+1,len-1);
			break;
#endif
#if MODBUS_FUNCON_WRITEMULTIPLEREGISTER
		case MODBUS_FUNC_WRITEMULTIPLEREGISTER://写多个寄存器
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
* Description    : 处理广播地址功能码类型
* Input          : data:接收到的数据
                   len:数据量
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
		case MODBUS_FUNC_READHOLDREGISTER://读保持寄存器
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
* Description    : 处理设备号
* Input          : data:接收到的数据
                   len:数据量
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
* Description    : RTU模式协议解析
* Input          : data:要解码的数据
 *                 len:数据长度
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
* Description    : 接收数据解码，对数据拆包处理,自定义协议格式
* Input          : data:要解码的数据
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
* Description    : 接收数据解码，对数据拆包处理,自定义协议格式
* Input          : data:要解码的数据
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
