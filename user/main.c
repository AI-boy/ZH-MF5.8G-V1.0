/********************************************************************************
  * @file    user/main.c 
  * @author  蔡维凯
  * @version V1.0
  * @date    2017-11-15
  * @brief   Main program body
*******************************************************************************/ 

/****************************************************************************
*  IO口定义：
*       PA2:输入中断模式  I/O 下降沿触发  微波信号采集
*       PC4:输入中断模式  红外接受  
*       PA3:输出模式  PWM 调光 1-10V 占空比：10%-20%-30%-50%
*       PC5:输出模式  contrl 继电器控制
*       PD5:输入模式  AD采样 感光检测
*       PC3:输出模式  LED 运行指示灯
****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"



/****************************数据变量定义*************************************/

 unsigned long sys_clk_freq=0 ;   //系统时钟频率

 unsigned long system_run_time_count=0;

 unsigned int system_reset_count=0x00;




void Init_System(void)
{
  CLK_DeInit();
  
  CLK_HSICmd(ENABLE);                                   //启用内部高速振荡器RC(16MHz)
    
  CLK_ClockSwitchCmd(DISABLE);
  
  //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);        //8分频  指令周期=16/8=2MHz
  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  delay(100);
 
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  
//-------PB4   浮动输入无中断 
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
  
//-------PB5   浮动输入无中断
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
  
//-------PC6   浮动输入无中断 
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
  
  
  ADC1_DeInit();
  
  /**********TIM4系统时基定时器*************/
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_64, 250);              //预分频 8  自动重装载值 250 :250*64/16=1000us=1ms 
  TIM4_UpdateDisableConfig(DISABLE);                     //运行计数溢出更新
  TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);   //寄存器更新(计数器溢出)时立即发送一个中断请求
  TIM4_SelectOnePulseMode(TIM4_OPMODE_REPETITIVE);       //计数器不停止计数
  TIM4_ARRPreloadConfig(ENABLE);                         //允许自动装载
  TIM4_SetCounter(0);                                    //设置定时器初始值
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);                      //清除更新标志
  TIM4_ClearITPendingBit(TIM2_IT_UPDATE);                //清除中断标志
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);                 //使能TIMER4 定时器中断
  
    
  EXTI_DeInit();
            
  ITC_DeInit();
  
  
}


void main(void)
{

 
  
  Init_System();                                       //系统初始化
  
  sys_clk_freq = CLK_GetClockFreq();                    //获取系统时钟频率
  
  Init_Led();                                           //初始化运行指示LED
  
  Init_Adc();                                           //初始化ADC1
  
  while(!(sys_clk_freq==SYE_Freq));                     //确认系统时钟2MHz?
  
  Init_Relay_Control();                                 //初始化继电器端口
  
  Init_Time2_PWM();                                     //初始化PWM输出 通道3
  
  Init_MF();                                           //初始化微波检测端口
   
  Init_Time1_mf();                                     //初始化Time1定时器
   
  Init_Ird();                                          //初始化红外接受端口
  
  get_device_parameter();                               //获取感应器工作参数 读取eeprom数据
  
  //Read_Dip_Switch();                                    //读取拨码开关状态
  
  IWDG_Enable();                                        //使能看门狗
   
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);         // 允许写访问预分频器和重载寄存器
  
  IWDG_SetPrescaler(IWDG_Prescaler_256);                //预分频器
  
  IWDG_SetReload(0xff);                                 //重载寄存器
  
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);        // 禁止写访问预分频器和重载寄存器
  
  
  
  open_mf_detection_time=TRUE;                           //启动微波雷达检测定时器
   
  TIM4_Cmd(ENABLE);                                     //启用定时器Timer4
  
  enableInterrupts();                                  //使能CPU中断
  
  
      while(1)
      {
        
          Scan_Ird_Process();   //红外接受进程
                    
          switch(system_mode_status)
          {
            case AUTO_MODE:     Auto_Mode_Process();                                    break;                            //正常工作模式
              
            case ON_OFF_MODE:   On_Off_Mode_Process();  system_mode_status= NOP_MODE;   break;                          //手动开关模式
              
            case RESET_MODE:    Reset_Mode_Process();   system_mode_status=AUTO_MODE;   break;                        //复位设置模式  所有参数依据DIP SWITCH 薄码开关而定
              
            case DIM_TEST_MODE:  Dim_Test_Mode_Process();                               break;                      //调光测试模式 完成后自动返回正常工作模式
            
            case NOP_MODE:  nop();                                                      break;                     //空闲 挂起状态
              
            default:break;
          }
                   
          IWDG_ReloadCounter();
      
      }
      
 }


/************************************************************************
*函数名:
*参数：
*说明：  
************************************************************************/
void Dim_Test_Mode_Process(void)
{
       if(handle_pwm_dimming_flag==TRUE)
         {
           
                   if(COUNT_DIR==0x01)
                   {
                        if( NOW_VALUE< END_VALUE)
                        {
                            NOW_VALUE=NOW_VALUE+1; //1%占空比增量
                            
                            Dimming_level(NOW_VALUE);
                        }
                        else
                        {
                            
                          
                            Dimming_level(END_VALUE);
                            
                            pwm_dimming_time_count=0x00;
                  
                            handle_pwm_dimming_flag=FALSE;
                            
                            pwm_dimming_time_flag=FALSE;
                            
                            Dimming_level_up_down(100,0,TRUE,2); //占空比递减
                        }
                   }
                   else if(COUNT_DIR==0x02)
                   {
                        if( NOW_VALUE>END_VALUE)
                        {
                            NOW_VALUE=NOW_VALUE-1; //1%占空比
                            
                            Dimming_level(NOW_VALUE);
                        }
                        else
                        {
                            if(END_VALUE==0x00)
                            {
                                delay_ms(500);
                                
                                RELAY_OFF ;
                            }
                          
                            Dimming_level(END_VALUE);
                            
                            pwm_dimming_time_count=0x00;
                  
                            handle_pwm_dimming_flag=FALSE;
                            
                            pwm_dimming_time_flag=FALSE;
                            
                            system_mode_status=AUTO_MODE;
                            
                            open_sensor_time_flag=0x00;
                               
                            Have_person_flag=0x00;
                               
                            NOW_VALUE=100;
                               
                            END_VALUE=0x00;
                              
                            sensor_time_se_count=0x00; 
                        }
                   }
                   else
                   {
                        Dimming_level(NOW_VALUE);
                   }
                        
                       
            handle_pwm_dimming_flag=FALSE;
            
         }
}

/************************************************************************
*函数名:
*参数：
*说明：  
************************************************************************/
void Reset_Mode_Process(void)
{
  
      
      mf_detection_time_count=0x00;
                
      handle_mf_detection_flag=FALSE;
                                  
       open_mf_detection_time=FALSE;  //暂停微波检测
       
       set_by_switch_value(switch_current_status);   //通过拨码开关设置系统参数
       
       open_mf_detection_time=TRUE;//开启微波检测
       
       while(1);//重启系统
}

/************************************************************************
*函数名:
*参数：
*说明：  
************************************************************************/
  void On_Off_Mode_Process(void)
  {
      if(lamp_flag)
      {
          RELAY_ON;
          
          Dimming_level(100);
          
          RUN_LED_ON;
      }
      else
      {
          RELAY_OFF;
          
          Dimming_level(0);
          
          RUN_LED_OFF;
      }
  }



/************************************************************************
*函数名:
*参数：
*说明：  
************************************************************************/
 void Auto_Mode_Process(void)
   {
     
     unsigned char  lex_cnt;
     
     unsigned int Threshold_Value_L;
     
     unsigned int Threshold_Value_H;
     
     Threshold_Value_L=0x00;
     
     Threshold_Value_H=0x00;
     
     lex_cnt=0x00;
              
       //----------------------------------------------------------------------  
          if(sensor_daylight_level==1024)//Disable 不检测光照度  只需微波感应
          {
              
                 if(open_sensor_time_flag==0x00)
                  {                                         
                       if(Have_person_flag==0x01)
                        {
                          
                              RELAY_ON;                                                           
                          
                              Dimming_level_up_down(70,100,TRUE,1); //占空比递增
      
                              sensor_time_se_count=0x00;
                              
                              system_run_time_count=0x00;
                              
                              open_sensor_time_flag=0x01;  //启动继电器定时器标志
                              
                              Have_person_flag=0x00;
                              
                        }                     
                      
                  }
                  else if(open_sensor_time_flag==0x01)
                  {
                      if(Have_person_flag==0x01)
                         {
                              sensor_time_se_count=0x00;
      
                              system_run_time_count=0x00;
                              
                              Have_person_flag=0x00;
                              
                         }
                  }
                 else if(open_sensor_time_flag==0x02)
                 {
                      Dimming_level_up_down(NOW_VALUE,sensor_pwm_dim_level,TRUE,2); //节能模式占空比
                      
                      open_sensor_time_flag=0x03;
                 }
                 else if(open_sensor_time_flag==0x03)
                 {
                      if(Have_person_flag==0x01)
                         {
                           
                              Dimming_level_up_down(70,100,TRUE,1); //占空比递增
                              
                              open_sensor_time_flag=0x01;
                              
                              sensor_time_se_count=0x00;
      
                              system_run_time_count=0x00;
                              
                              Have_person_flag=0x00;
                              
                         }
                 }
                 else if(open_sensor_time_flag==0x04)
                 {
                        Dimming_level_up_down(NOW_VALUE,0,TRUE,2); //占空比递减
                        
                        open_sensor_time_flag=0x00;
                 }
                 
                          
           }
          else 
          {
                 if(open_sensor_time_flag==0x00)
                  {  
                         if(Have_person_flag==0x01)
                         {
                              mf_detection_time_count=0x00;
                             
                              open_mf_detection_time=FALSE;    
                              
                              RUN_LED_OFF;
                                                                               
                              handle_adc_process();   
                              
                              open_mf_detection_time=TRUE;
                            
                           if (light_value<(sensor_daylight_level+100) )
                            {
                                  RUN_LED_OFF;
                                  
                                  RELAY_ON;
                              
                                  Dimming_level_up_down(70,100,TRUE,1); //占空比递增
          
                                  sensor_time_se_count=0x00;
                                  
                                  system_run_time_count=0x00;
                                  
                                  open_sensor_time_flag=0x01;  //启动继电器定时器标志
                                  
                                  Have_person_flag=0x00;
                                  
                            } 
                          
                           
                           
                              Have_person_flag=0x00;
                         }                                               
                     
                  }
                  else if(open_sensor_time_flag==0x01)
                  {
                      if(Have_person_flag==0x01)
                         {
                              sensor_time_se_count=0x00;
      
                              system_run_time_count=0x00;
                              
                              Have_person_flag=0x00;
                              
                         }
                  }
                 else if(open_sensor_time_flag==0x02)
                 {
                      Dimming_level_up_down(NOW_VALUE,sensor_pwm_dim_level,TRUE,2); //占空比递减
                      
                      open_sensor_time_flag=0x03;
                 }
                 else if(open_sensor_time_flag==0x03)
                 {
                      if(Have_person_flag==0x01)
                         {
                              Dimming_level_up_down(70,100,TRUE,1); //占空比递增
                              
                              open_sensor_time_flag=0x01;
                              
                              sensor_time_se_count=0x00;
      
                              system_run_time_count=0x00;
                              
                              Have_person_flag=0x00;
                              
                         }
                 }
                 else if(open_sensor_time_flag==0x04)
                 {
                        Dimming_level_up_down(NOW_VALUE,0,TRUE,2); //占空比递减
                        
                        open_sensor_time_flag=0x00;
                 }
             
                
          }
          //--------------------------------------------------------------------
          if(MF_pluse_receive_flag==0x01)
          {
                  if(mf_32_pulse_time_value[4]>0)
                  {
                      //---------------------------------------------------
                      if(mf_32_pulse_time_value[2]>=mf_32_pulse_time_value[1])
                       {
                          DEV_TEMP[0]= mf_32_pulse_time_value[2]-mf_32_pulse_time_value[1];
                       }
                       else
                       {
                          DEV_TEMP[0]= mf_32_pulse_time_value[1]-mf_32_pulse_time_value[2];
                       }
                      //------------------------------------------------
                      if(mf_32_pulse_time_value[4]>=mf_32_pulse_time_value[3])
                       {
                          DEV_TEMP[1]= mf_32_pulse_time_value[4]-mf_32_pulse_time_value[3];
                       }
                       else
                       {
                          DEV_TEMP[1]= mf_32_pulse_time_value[3]-mf_32_pulse_time_value[4];
                       }
                        //-------------------------------------------------
                       /*if(mf_32_pulse_time_value[6]>=mf_32_pulse_time_value[5])
                       {
                          DEV_TEMP[2]= mf_32_pulse_time_value[6]-mf_32_pulse_time_value[5];
                       }
                       else
                       {
                          DEV_TEMP[2]= mf_32_pulse_time_value[5]-mf_32_pulse_time_value[6];
                       }
                       
                       
                       //-----------------------------------------------
                       if(mf_32_pulse_time_value[8]>=mf_32_pulse_time_value[7])
                       {
                          DEV_TEMP[3]= mf_32_pulse_time_value[8]-mf_32_pulse_time_value[7];
                       }
                       else
                       {
                          DEV_TEMP[3]= mf_32_pulse_time_value[7]-mf_32_pulse_time_value[8];
                       }*/
                       //---------------------------------------------------------
                          lex_cnt=0x00;
                      
                          Threshold_Value_L=(100-sensor_detection_area)*2+40;
                          
                          Threshold_Value_H=1500;
                      
                         if (( DEV_TEMP[0] > Threshold_Value_L )&&( DEV_TEMP[0] < Threshold_Value_H ))
                         {
                            lex_cnt++;
                         }
                         if(( DEV_TEMP[1] > Threshold_Value_L )&&( DEV_TEMP[1] < Threshold_Value_H ))
                         {
                            lex_cnt++;
                         }
                       /* if(( DEV_TEMP[2] > Threshold_Value_L )&&( DEV_TEMP[2] < Threshold_Value_H ))
                        {
                            lex_cnt++;
                        }
                        
                        if(( DEV_TEMP[3] > Threshold_Value_L )&&( DEV_TEMP[3] < Threshold_Value_H ))
                        {
                            lex_cnt++;
                        }*/
                      //------------------------------------------------------------
                      if(( lex_cnt>=1 )&&(!relay_off_flag))
                       {
                            Have_person_flag=0x01;                          
                      
                            DELAY_MS_COUNT=200; //LED延时点亮200ms
                          
                            RUN_LED_ON ;      
                            
                       
                       }
                       
                      
                  }
                 
                    MF_pluse_receive_flag=0x00;
          }
         //-------------------------------------------------------------------------------- 
          
         if((handle_mf_detection_flag==TRUE)&&(ird_status==0x00))
         {
              MF_pluse_receive_count=0x00;
          
              MF_pluse_receive_flag=0x00;
            
              GPIOA->DDR = GPIOA->DDR|0x04;
              
              GPIOA->ODR = GPIOA->ODR&0xfb;
              
              GPIOA->CR1 |= 0x04;
              
              GPIOA->CR2 |= 0x04;
              
              delay_us(15);                   //PA2推挽输出低电平   电容放电
            
              GPIOA->DDR = GPIOA->DDR&0xfb;   //PA2浮动输入中断    充电充电
                           
              GPIOA->CR1 &= 0xfb;
              
              GPIOA->CR2 |= 0x04;   
              
              if(ird_status==0x00)
              {
                  Init_Time1_mf();
            
                  TIM1_Cmd(ENABLE);   //开启定时器Time1          
              }
  
                                                
              handle_mf_detection_flag=FALSE;
            
         }
   
        //-------------------------------------------------------------------------
         if(handle_pwm_dimming_flag==TRUE)
         {
           
                

                        if( NOW_VALUE< END_VALUE)
                        {
                            NOW_VALUE=NOW_VALUE+1; //5%占空比增量
                            
                            Dimming_level(NOW_VALUE);
                        }
                        else if( NOW_VALUE>END_VALUE)
                         {
                            NOW_VALUE=NOW_VALUE-1; //5%占空比
                            
                            Dimming_level(NOW_VALUE);  
                         }
                        else
                        {
                            if(END_VALUE==0x00)
                            {
                                relay_off_flag=0x01;
                                
                                relay_off_time_count=1000;
                                
                                delay_ms(500);
                                
                                RELAY_OFF ;
                            }
                          
                            Dimming_level(END_VALUE);
                            
                            pwm_dimming_time_count=0x00;
                  
                            handle_pwm_dimming_flag=FALSE;
                            
                            pwm_dimming_time_flag=FALSE;
                            
                              
                            sensor_time_se_count=0x00; 
                        }
                   
                  
                       
            handle_pwm_dimming_flag=FALSE;
            
         }
    
    
          IWDG_ReloadCounter();
    }
  



