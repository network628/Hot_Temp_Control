#include "pid.h"
#include "config.h"

PID pid;
// extern u16 pidcalcms;
void PIDParament_Init() //
{
  ntc.Set_T = PWM_Read_value();
  pid.set = ntc.Set_T; // 设定温度
  pid.pwm_cycle = 220; ////pwm的周期
  pid.pid_P = 5;
  pid.pid_I = 4;
  pid.pid_D = 3;
  //	pid.calc_cycle=50;  //每隔50*10mS运算一次
}

void pid_calc(void) // pid
{
  // int dk;

  short PID_out;

  //	if(pidcalcms<pid.calc_cycle) return ;

  pid.Ek = pid.set - pid.curr; //
                               //	pid.Ek=pid.curr-pid.set;
  PID_out = ((pid.pid_P) * ((pid.Ek) - (pid.Ek_1))) +
            ((pid.pid_I) * (pid.Ek)) +
            ((pid.pid_D) * ((pid.Ek) - (2 * (pid.Ek_1)) + (pid.Ek_2))); 
  pid.currpwm += PID_out;
  pid.Ek_2 = pid.Ek_1;  
  pid.Ek_1 = pid.Ek;    
  //  printf("--->PWM---> %d %d\r\n", pid.currpwm ,PID_out);
  if (pid.currpwm <= 0) 
  {
    pid.currpwm = 0;                    //
                                        //	  return pid.currpwm;
  }                                     // PID
  else if (pid.currpwm > pid.pwm_cycle) // 1000    //1s  PWM
  {
    pid.currpwm = pid.pwm_cycle; //1000
                                 //	 return pid.currpwm;
  }

  // pidcalcms=0;
  //	return pid.currpwm;
}

