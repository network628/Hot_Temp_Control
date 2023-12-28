

#ifndef     __UART_H__
#define     __UART_H__


#ifndef TURE
 #define TRUE              1
#endif

#ifndef FALSE
 #define FALSE             0
#endif

//=============================================================================
// KEIL printf �ӿڲ���
//=============================================================================
char putchar (char chr);  
char _getkey ();  

//=============================================================================
// �û����沿��
//=============================================================================
U8  URQRead(U8 *Data);

// ���ڷ����ַ�
void UART_SendChar(U8 chr);
// ���ڷ����ַ���
void UART_SendString(U8 *str);
//void UART_Send_ACk(U8 *str,U8 n);
// �Ӵ��ڽ����ַ� ����
U8  UART_RecvChar(void);
// �Ӵ��ڽ����ַ� ������
U8  UART_RecvChar_N(U8 *Data);
// ���ڳ�ʼ��
void UartInit(void);

void Uart_Contorl(void);


#endif // end of __UART_H__





