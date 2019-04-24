#ifndef _USERDEFINE_H
#define _USERDEFINE_H




#include "led.h"
#include "handle_light_sensor.h"
#include "control.h" 
#include "remote.h"



//------------定义系统时钟频率

#define SYE_Freq     16000000





//------------变量声明

extern unsigned long system_run_time_count;

extern unsigned int system_reset_count;
  
extern unsigned int switch_current_status;

extern unsigned int switch_before_status;






//------------函数声明

 extern void Init_System(void);
 
 extern void Auto_Mode_Process(void);
 
 extern void Dim_Test_Mode_Process(void);
 
 extern void Reset_Mode_Process(void);
 
 extern void On_Off_Mode_Process(void);
 
 extern void  get_device_parameter(void);
 
 extern void Init_switch_PD2_port(void);
 
 extern void Init_switch_PD3_port(void);
 
 extern void Init_switch_PD6_port(void);
 
 extern void Read_Dip_Switch(void);
 
 extern void set_by_switch_value(unsigned int switch_value);





#endif