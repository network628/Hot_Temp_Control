

#include "config.h"

struct NTC ntc;

void NO_CALL(void);

struct LIGNT Light;

void main(void)
{
	U8 Write_Temp;

	NO_CALL();
	INT0_init();
	INT1_init(1);
	Timer2_Init();
	Init_ADC();
	UartInit();
	DlyMs(100);
	PIDParament_Init();

	printf("----------------------------\r\n");
	printf("--------ntc.Set_T: %f--------\r\n", ntc.Set_T);
	printf("----------------------------\r\n");
	DlyMs(100);
	while (1)
	{
		key_handle();

		if (Light.Display_Cnt < 3 && Light.sw_flag)
		{
			disp_num = ntc.Set_T;
		}
		else if (Light.Display_Cnt >= 3 && Light.sw_flag)
		{
			if (ntc_cnt == 0)
			{
				ntc_cnt = 400;			// 50*1ms 50ms 执行一次
				ntc.Now_T = Get_Temp(); // 获取温度值
				disp_num = ntc.Now_T;
			}
		}

		if (Light.counter < 11 && Light.sw_flag) // 加热4秒停11秒
		{
			Light.AC_Voltage = 0;
		}
		else // 4s开
		{
			if (Write_Temp != ntc.Set_T)
			{
				Write_Temp = ntc.Set_T;
				PWM_Write_value(ntc.Set_T);
			}

			pid.curr = ntc.Now_T; // 当前温度
			pid.set = ntc.Set_T;  // 设定温度
			pid_calc();
			Light.AC_Voltage = pid.currpwm;

			printf("---ntc.Now_T: %3.1f -- ntc.Set_T: %3.1f ---\r\n", ntc.Now_T, ntc.Set_T);
		}
	}
}

void NO_CALL(void)
{
	P1M0 = 0x00;
	P1M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	Light.sw_flag = 0;
	Light.counter = 0;
	time_base_1ms = TIME_BASE_1MS;
}

/**************************************************************************/

/**************************************************************************/
