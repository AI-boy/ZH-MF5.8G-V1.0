

#ifndef _HANDLE_LIGHT_SENSOR_H

#define _HANDLE_LIGHT_SENSOR_H







//------------��������
extern bool open_light_sensor_time;

extern bool handle_adc_process_flag;

extern unsigned long light_sensor_time_count;

extern unsigned int adc_buff[20];

extern unsigned int light_value;





//------------��������
extern void Init_Adc(void);

extern void handle_adc_process(void);




#endif