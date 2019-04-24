 

#ifndef _REMOTE_H

#define _REMOTE_H





#define  IRD_PC4       (GPIOC->IDR&GPIO_PIN_4)




//-----------------------------------------------------------------------遥控器按键值
#define  on_off                          0x28//0x80
        
#define  reset                           0xEA//0x40         
        
#define  auto_mode                       0x82//0xC0
        
#define  dim_test                        0x62//0x20
    
#define  QS1                             0x90//0x88
    
#define  QS2                             0xB8//0x48
    
#define  QS3                             0xF8//0xC8
    
#define  QS4                             0xB0//0x28
    
#define  detection_area_25               0x98//0x84
    
#define  detection_area_50               0xD8//0x44
    
#define  detection_area_75               0x88//0xC4
    
#define  detection_area_100              0xA8//0x24
    
#define  hold_time_5                     0xE8//0x8C
    
#define  hold_time_30                    0x48//0x4C
    
#define   hold_time_60                   0x9A//0xCC
    
#define  hold_time_180                   0x10//0x2C
    
#define  hold_time_300                   0x0A//0x82
    
#define  hold_time_600                   0xAA//0x42
    
#define  hold_time_1200                  0x12//0xC2 
    
#define  hold_time_1800                  0x52//0x22
    
#define   dim_level_10                   0x7A//0x8A //节能模式  PWM占空比10%
    
#define  dim_level_20                    0x60//0x4A   //节能模式  PWM占空比20%
    
#define  dim_level_30                    0xA0//0xCA    //节能模式  PWM占空比30%
    
#define  dim_level_50                    0xC0//0x2A    //节能模式  PWM占空比50%
    
#define  dim_period_0                    0xE2//0x86    //节能模式  节能时间0s
    
#define  dim_period_10                   0xE0//0x46   //节能模式  节能时间10s
    
#define   dim_period_60                  0x02//0xC6   //节能模式  节能时间60s
    
#define  dim_period_180                  0x40//0x26  //节能模式  节能时间180s
    
#define  dim_period_300                  0x18//0x8E  //节能模式  节能时间300s
    
#define  dim_period_600                  0x22//0x4E  //节能模式  节能时间600s
    
#define  dim_period_1800                 0xF0//0xCE //节能模式  节能时间1800s
    
#define  dim_period_2000                 0x8A//0x2E  //大于1800s时定义长时间调光节能模式
    
#define  daylight_sensor_level_5lux      0x50//0x81
    
#define  daylight_sensor_level_15lux     0x78//0x41
    
#define  daylight_sensor_level_30lux     0x70//0xC1
    
#define  daylight_sensor_level_50lux     0x58//0x21
    
#define  daylight_sensor_level_100lux    0xCA//0x89
    
#define  daylight_sensor_level_150lux    0x4A//0x49
    
#define  daylight_sensor_disable         0x80//0xC9
    
#define  test_mode                       0xA2//0x29

//---------------------------------------------------------遥控器设备地址MAC

#define  ZH_MAC                         0x00FF



typedef enum  mode
       {
         AUTO_MODE      =0x00,
         ON_OFF_MODE    =0x01,
         RESET_MODE     =0x02,
         DIM_TEST_MODE  =0x03,
         NOP_MODE       =0x04
        
       } YSYTEM_MODE ;




//---------------------------------------------------------------------------
//------------变量声明
 extern unsigned char  ird_status;

 extern  unsigned char   receive_bit_count;

 extern unsigned char   ird_receive_flag;

 extern unsigned char   ird_receive_time_count;

 extern  unsigned long   ird_data;



extern unsigned char  ird_status;



extern unsigned char lamp_flag;

extern YSYTEM_MODE  system_mode_status;





//------------函数声明

extern void Init_Ird(void);

extern void Key_Menu(unsigned long key_value);

extern void Scan_Ird_Process(void);


#endif