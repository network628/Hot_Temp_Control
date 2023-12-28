
#include "config.h"
 
u8 time_base_1ms = 0;
U8 time_base_10ms;
// �жϷ������
void t2int() interrupt 12 // �ж����
{
	if (hot_time > 0) // 10us
		hot_time--;

	hot_handle();

	--time_base_1ms;
	if (time_base_1ms == 0) // 10us*100=1ms
	{
		time_base_1ms = TIME_BASE_1MS;
 
		Disp_Num_Function();

		if (ntc_cnt > 0) //
			--ntc_cnt;

		ntc.T2_CNT++;		   // 1s
		if (ntc.T2_CNT > 1000) // 1s
		{
			ntc.T2_CNT = 0;

			if (Light.Display_Cnt < 8)
				Light.Display_Cnt++;

			Light.counter++;
			if (Light.counter > 15)
				Light.counter = 0;

			if (Light.Delay_Open_Cnt > 0)
			{
				if (Light.Delay_Open_Cnt == 1) // ����ʱ�رգ�3s���
				{							   // Ȼ���ٿ��жϼ���
					EX1 = 1;
					ET0 = 1;
				}
				Light.Delay_Open_Cnt--;
			}
		}

		--time_base_10ms;
		if (time_base_10ms == 0) // 10us*100=1ms
		{
			time_base_10ms = TIME_BASE_10MS;
			Key_Scan_Cnt = 1; // ��ʱ������1msִ��һ��
		}
	}
}

//-----------------------------------------------
// #define FOSC 11059200L
// #define T2MS (65536 - FOSC / 1000) // 1Tģʽ 1000us
//-----------------------------------------------
void Timer2_Init(void)
{
	AUXR |= 0x04; // ��ʱ��2Ϊ1Tģʽ
	//  AUXR &= ~0x04;//��ʱ��2Ϊ12Tģʽ
	// T2L = T2MS; // ��ʼ����ʱֵ
	// T2H = T2MS >> 8;

	T2L = 0x91;	  // ���ö�ʱ��ʼֵ //10us
	T2H = 0xFF;	  // ���ö�ʱ��ʼֵ
	AUXR |= 0x10; // ��ʱ��2��ʼ��ʱ

	IE2 |= 0x04; // ����ʱ��2�ж�
	EA = 1;
}
