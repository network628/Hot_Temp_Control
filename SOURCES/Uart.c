/******************************************************************************
                                  智信科技
******************************************************************************/

// SQUIRREL_STC12C5A60S2开发板
// 串口非堵塞收发程序 中断驱动方式
// 并支持解析用户调试命令
// R207SC@SQUIRREL_STC12C5A60S2



#include "config.h"

// 屏蔽51编译器未调用函数警告
void NoCall_Uart(void);

//=============================================================================
// 发送接收队列部分
//=============================================================================
#define URBMax      20              // 接收缓存大小

U8  UartRecvBuf[URBMax];            // 接收缓存
U8  URWP  = 0;                      // 存入指针
U8  URRP  = 0;                      // 读取指针
U8  URNum = 0;                      // 数据数量



#define USBMax      20              // 发送缓存大小

U8  UartSendBuf[URBMax];            // 发送缓存
U8  USWP  = 0;                      // 存入指针
U8  USRP  = 0;                      // 读取指针
U8  USNum = 0;                      // 数据数量

U8  SendFlag = 0;

// 接收数据入队
U8  URQWrite(U8 Data)
{
//    P00 =~P00;
    if(URNum >= URBMax)return FALSE;// 失败
    UartRecvBuf[URWP] = Data;       // 数据入队
    URWP++;                         // 移动指针
    if(URWP >= URBMax)
    {
        URWP = 0;  
    }
    URNum++;
    return TRUE;                    // 成功
}

// 接收数据出队
U8  URQRead(U8 *Data)
{
    if(URNum == 0)return FALSE;     // 失败
    ES     =   0;                   // 关闭串口中断
//    P01 = ~P01;
    *Data = UartRecvBuf[URRP];      // 数据出队
    URRP++;                         // 移动指针
    if(URRP >= URBMax)
    {
        URRP = 0;  
    }
    URNum--;                         
    ES     =   1;                   // 允许串口中断
    return TRUE;                    // 成功
}




// 发送数据入队
U8  USQWrite(U8 Data)
{
    if(USNum >= USBMax)return FALSE;// 失败
    ES     =   0;                   // 关闭串口中断
    UartSendBuf[USWP] = Data;       // 数据入队
    USWP++;                         // 移动指针
    if(USWP >= USBMax)
    {
        USWP = 0;  
    }
    USNum++;
    if(SendFlag == 0)               // 如果不在发送状态
    {
        TI = 1;                     // 则启动发送
    }
    ES     =   1;                   // 允许串口中断
    return TRUE;                    // 成功
}

// 发送数据出队
U8  USQRead(U8 *Data)
{
    if(USNum == 0)return FALSE;     // 失败
    *Data = UartSendBuf[USRP];      // 数据出队
    USRP++;                         // 移动指针
    if(USRP >= USBMax)
    {
        USRP = 0;  
    }
    USNum--;                         
    return TRUE;                    // 成功
}
//=============================================================================
// KEIL printf 接口部分
//=============================================================================
char putchar (char chr)  
{
    // 如果发送缓冲区满 则直到发送成功才退出
    while(USQWrite(chr) == FALSE);
    return (chr);
}

char _getkey ()  
{
    char Data;
    // 如果接收缓冲区空 直到接收成功才退出
    while(URQRead((U8*)&Data) == FALSE);
    return Data;
}

//void UART_Send_ACk(U8 *str,U8 n)
//{
//    while(n--)
//    {
//       UART_SendChar( *str++);
//    }
//}
//=============================================================================
// 用户界面部分
//=============================================================================

// 串口发送字符
void UART_SendChar(U8 chr)
{
    // 如果发送缓冲区满 则直到发送成功才退出
    while(USQWrite(chr) == FALSE);
}
// 串口发送字符串
void UART_SendString(U8 *str)
{
    while(*str)
    {
        UART_SendChar(*str++);
    }
}

// 从串口接收字符 阻塞
U8  UART_RecvChar(void)
{
	U8 Data;
    // 如果接收缓冲区空 直到接收成功才退出
    while(URQRead(&Data) == FALSE);
    return Data;
}

// 从串口接收字符 非阻塞
U8  UART_RecvChar_N(U8 *Data)
{
    return URQRead(Data);
}

// 串口初始化
void UartInit(void)		
{
	SCON = 0x50;			//8位数据，可变波特率
	AUXR |= 0x01;			
	AUXR |= 0x04;			//定时器时钟为Fosc，1T模式
	T2L = 0xE0;				//定时器初值，内部时钟11.0592M，波特率9600
	T2H = 0xFE;				//定时器初值
	AUXR |= 0x10;			//启动定时器
	ES=1; 					//串口中断开关,采用查询法时不用打开中断      
	REN=1;      			//串口为工作方式1，允许接收数据
	SM0=0;						//SM0 SM1串口工作方式选择，01：8位异步收发，波特率由定时器决定
	SM1=1;
  
    TI = 0;             // 清零串口发送完成中断请求标志
//    ES = 1;             // 允许串口中断
//    EA = 1;             // 开总中断
    NoCall_Uart();      // 屏蔽51编译器未调用函数警告
}

//=============================================================================
// 串口中断部分
//=============================================================================
//extern U16 Receive_Cnt;
// 串口中断
void UART_Interrupt_Receive(void) interrupt 4
{
    U8 Data;
	 
    if(RI==1)
    {
        RI = 0;                     
        Data = SBUF;                // 接收串口数据
        URQWrite(Data);             // 接收到的数据如队列
//		Receive_Cnt++;
    }
    if(TI == 1)
    {
        TI = 0;
        if(USQRead(&Data) == TRUE)  // 如果获取数据成功
        {
            SBUF = Data;            // 发送数据
        	SendFlag = 1;           // 置发送状态
        }else{
            SendFlag = 0;           // 不在发送状态
        }
    }

}

//=============================================================================



//=============================================================================
// 本函数是为了屏蔽51编译器未调用函数警告 
// 帮助51编译器做覆盖分析 从而节约RAM 及 ROM 
// 实际不起任何作用 也没有任何函数被调用
//=============================================================================
void NoCall_Uart(void)
{
    U8 i;
    i=0;
    if(i)
    {
        // Uart.c
        // KEIL printf 接口部分
        putchar(0);  
        _getkey (); 
        // 串口发送字符
        UART_SendChar(0);
        // 串口发送字符串
        UART_SendString((U8 *)0);
        // 从串口接收字符 阻塞
        UART_RecvChar();
        // 从串口接收字符 非阻塞
        UART_RecvChar_N((U8 *)0);
    }
}
//=============================================================================

 





