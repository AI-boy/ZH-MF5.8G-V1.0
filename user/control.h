 #ifndef _CONTROL_H

#define _CONTROL_H


#include "stm8s.h"


//------------�궨��


#define RELAY_ON_OFF            (GPIOC->ODR^=GPIO_PIN_5)
#define RELAY_ON                (GPIOC->ODR&=(~GPIO_PIN_5)) 
#define RELAY_OFF               (GPIOC->ODR|=GPIO_PIN_5)   

//------------��������

//extern unsigned int sensor_time_ms_count;       //�̵�����ʱ���������

extern unsigned int sensor_time_se_count;       //�̵�����ʱ�������

//extern unsigned int sensor_time_mi_count;       //�̵�����ʱ�����ӱ���

extern unsigned char open_sensor_time_flag;     //ϵͳ״̬��ʱ����־

extern unsigned int sensor_hold_time;           //ϵͳĬ��5S 100% ���

extern unsigned int sensor_dimming_period;      //ϵͳĬ��5S 20%���

extern unsigned char sensor_pwm_dim_level;      //10% 20% 30% 50% ������

extern unsigned char sensor_detection_area;     //��Ӧ̽������ Ĭ��100% 

extern unsigned char sensor_mode;

extern unsigned int sensor_daylight_level;



extern unsigned char sensor_mode;

extern unsigned char MF_pluse_receive_flag;

extern unsigned int MF_TIME_COUNT;

extern unsigned char MF_pluse_receive_count;



extern unsigned int mf_detection_time_count;
          
extern   bool    handle_mf_detection_flag;
      
extern   bool    open_mf_detection_time;

extern  unsigned int mf_32_pulse_time_value[20];

extern  unsigned int DEV_TEMP[4];

extern unsigned char Have_person_flag;

extern  unsigned int temp_num;

extern  unsigned int pwm_dimming_time_count;
      
extern     bool  handle_pwm_dimming_flag;
      
extern    bool  pwm_dimming_time_flag;


extern unsigned int NOW_VALUE;
 
extern unsigned int END_VALUE;

extern  unsigned char COUNT_DIR;

extern  unsigned char relay_off_flag;
                                
 extern  unsigned int relay_off_time_count;



//------------��������

extern void Init_Relay_Control(void);

//extern void handle_relay_process(unsigned char mode );



/*********************Time2  PWM***********************/

extern void Init_Time2_PWM(void);

extern void Dimming_level(unsigned int lev);

extern   void Dimming_level_up_down(unsigned int now_value,unsigned int end_value,bool enable_flag,unsigned char count_direction);

extern void Init_MF(void);

/*********************Tim3  ��ʱ�� MF΢�����ڼ���*****************************/

extern void Init_Time1_mf(void);



#endif