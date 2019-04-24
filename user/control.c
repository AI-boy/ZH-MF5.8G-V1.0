
/*********************************继电器文件***************************************/

#include "stm8s.h"
#include "userdefine.h"      


//unsigned int sensor_time_ms_count=0x00; //继电器定时器毫秒变量

unsigned int sensor_time_se_count=0x00; //继电器定时器秒变量

//unsigned int sensor_time_mi_count=0x00; //继电器定时器分钟变量

unsigned char open_sensor_time_flag=0x00;  

unsigned int sensor_hold_time=5;                //系统默认5S 100% 输出  5s  30s 60s  180s  1200s 1800s   可设置

unsigned int sensor_dimming_period=5;           //系统默认5S 50%输出  0s 60s 180s  600s    1800s    可设置

unsigned char sensor_pwm_dim_level=20;          //10% 20% 30% 50%                                   可设置

unsigned char sensor_detection_area=100;        //感应探测区域 默认100%  

unsigned int sensor_daylight_level=1024;        //默认150lux  环境光亮度值   光度约暗 数值越小 5   15  30   50  100  150lux  Disable (1024) 
                                                //亮度值通过光敏二极管 AD采样获取  间接采集电压数值

unsigned char sensor_mode=0x00;

unsigned char MF_pluse_receive_flag=0x00;     //接受微波完成标志   处理微波

unsigned int MF_TIME_COUNT=0x00;

unsigned char MF_pluse_receive_count=0x00;

unsigned int mf_detection_time_count=0x00;
          
bool    handle_mf_detection_flag=FALSE;
      
bool    open_mf_detection_time=FALSE;
      
unsigned int mf_32_pulse_time_value[20];
 
unsigned int DEV_TEMP[4]={0};
 
unsigned char Have_person_flag=0x00;
 
unsigned int temp_num=0x00;
 
unsigned int pwm_dimming_time_count=0x00;
      
bool  handle_pwm_dimming_flag=FALSE;
      
bool  pwm_dimming_time_flag=FALSE;
     
 unsigned int NOW_VALUE=100;
 
 unsigned int END_VALUE=0x00;
 
 unsigned char COUNT_DIR=0x00;
 
 unsigned char relay_off_flag=0x00;
                                
 unsigned int relay_off_time_count=0x00;



/************************************************************************
*函数名:init_relay_control
*参数：void   void
*说明：初始化PC5端口  输出模式  控制继电器
************************************************************************/

void Init_Relay_Control(void)
{
  
  
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

  RELAY_OFF;
  
}




/************************************************************************
*函数名:Init_Time2
*参数：void  void
*说明：初始化Time2 PWM 模式1  边沿对齐 PA3
************************************************************************/
void Init_Time2_PWM(void)
{
    GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
    
     TIM2_DeInit();
    
     TIM2_TimeBaseInit(TIM2_PRESCALER_8, 1000);  //PWM 频率 2KHz=2MHz/1000 
     
     TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 100, TIM2_OCPOLARITY_HIGH);
             
     TIM2_ITConfig(TIM2_IT_CC2, DISABLE);         
     
     TIM2_OC3PolarityConfig(TIM2_OCPOLARITY_HIGH);
     
     TIM2_SetCounter(1000);
     
     TIM2_SetAutoreload(1000);
     
      TIM2_SetCompare3(100);
     
     TIM2_ClearITPendingBit(TIM2_IT_CC2);
     
     TIM2_ARRPreloadConfig(ENABLE);
     
     TIM2_OC3PreloadConfig(ENABLE);
     
     TIM2_CCxCmd(TIM2_CHANNEL_3, ENABLE);
     
     TIM2_Cmd(ENABLE);
     
     
}

/************************************************************************
*函数名:
*参数：
*说明：  
************************************************************************/
void Dimming_level(unsigned int lev)  //PWM 调光输出
{

  unsigned int pwm_adj_value;
  
  if(lev<=100)
  {
    pwm_adj_value = lev*10;
    
    TIM2_SetCompare3(1000-pwm_adj_value); //caiwk-2018-5-25
  }
  
}
  
void Dimming_level_up_down(unsigned int now_value,unsigned int end_value,bool enable_flag,unsigned char count_direction) 
{
  
//  if((now_value%5!=0)||(end_value%5!=0))
//  {
//    return ;
//  }
  
  
    if(enable_flag==TRUE)
  {
      RELAY_ON;
      
      pwm_dimming_time_count=0x00;
      
      handle_pwm_dimming_flag=FALSE;
      
      pwm_dimming_time_flag=TRUE;
      
      NOW_VALUE=now_value;
 
      END_VALUE=end_value;
      
      COUNT_DIR=count_direction;   //0 维持现状  1 增加占空比  2减少占空比
  }
}


  
  
  
 



/************************************************************************
*函数名:
*参数：
*说明：  
************************************************************************/
void Init_MF(void)
{
              //PA2 微波检测端口  初始化为默认浮动输入状态 
  
              GPIOA->DDR = GPIOA->DDR|0x04;
              
              GPIOA->ODR = GPIOA->ODR&0xfb;
              
              GPIOA->CR1 |= 0x04;
              
              GPIOA->CR2 |= 0x04;
   
   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_RISE_ONLY);  //仅上升沿 触发中断
}



/************************************************************************
*函数名:
*参数：
*说明：  
************************************************************************/
void Init_Time1_mf(void)
{
  
  
  TIM1_DeInit();
  
  TIM1_TimeBaseInit(160, TIM1_COUNTERMODE_UP, 600000, 0);
  
  
  TIM1->SMCR &= (uint8_t)(~TIM1_SMCR_SMS);
  
  
  TIM1_SetCounter(0x0000);
  
  TIM1_ClearFlag(TIM1_FLAG_UPDATE);
  
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
  

  
}






