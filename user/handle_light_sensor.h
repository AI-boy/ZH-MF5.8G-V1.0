

#ifndef _HANDLE_LIGHT_SENSOR_H

#define _HANDLE_LIGHT_SENSOR_H







//------------变量声明
extern bool open_light_sensor_time;

extern bool handle_adc_process_flag;

extern unsigned long light_sensor_time_count;

extern unsigned int adc_buff[20];

extern unsigned int light_value;





//------------函数声明
extern void Init_Adc(void);

extern void handle_adc_process(void);




#endif