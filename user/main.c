/********************************************************************************
  * @file    user/main.c 
  * @author  ��ά��
  * @version V1.0
  * @date    2017-11-15
  * @brief   Main program body
*******************************************************************************/ 

/****************************************************************************
*  IO�ڶ��壺
*       PA2:�����ж�ģʽ  I/O �½��ش���  ΢���źŲɼ�
*       PC4:�����ж�ģʽ  �������  
*       PA3:���ģʽ  PWM ���� 1-10V ռ�ձȣ�10%-20%-30%-50%
*       PC5:���ģʽ  contrl �̵�������
*       PD5:����ģʽ  AD���� �й���
*       PC3:���ģʽ  LED ����ָʾ��
****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"



/****************************���ݱ�������*************************************/

 unsigned long sys_clk_freq=0 ;   //ϵͳʱ��Ƶ��

 unsigned long system_run_time_count=0;

 unsigned int system_reset_count=0x00;




void Init_System(void)
{
  CLK_DeInit();
  
  CLK_HSICmd(ENABLE);                                   //�����ڲ���������RC(16MHz)
    
  CLK_ClockSwitchCmd(DISABLE);
  
  //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);        //8��Ƶ  ָ������=16/8=2MHz
  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  delay(100);
 
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  
//-------PB4   �����������ж� 
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
  
//-------PB5   �����������ж�
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
  
//-------PC6   �����������ж� 
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
  
  
  ADC1_DeInit();
  
  /**********TIM4ϵͳʱ����ʱ��*************/
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_64, 250);              //Ԥ��Ƶ 8  �Զ���װ��ֵ 250 :250*64/16=1000us=1ms 
  TIM4_UpdateDisableConfig(DISABLE);                     //���м����������
  TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);   //�Ĵ�������(���������)ʱ��������һ���ж�����
  TIM4_SelectOnePulseMode(TIM4_OPMODE_REPETITIVE);       //��������ֹͣ����
  TIM4_ARRPreloadConfig(ENABLE);                         //�����Զ�װ��
  TIM4_SetCounter(0);                                    //���ö�ʱ����ʼֵ
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);                      //������±�־
  TIM4_ClearITPendingBit(TIM2_IT_UPDATE);                //����жϱ�־
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);                 //ʹ��TIMER4 ��ʱ���ж�
  
    
  EXTI_DeInit();
            
  ITC_DeInit();
  
  
}


void main(void)
{

 
  
  Init_System();                                       //ϵͳ��ʼ��
  
  sys_clk_freq = CLK_GetClockFreq();                    //��ȡϵͳʱ��Ƶ��
  
  Init_Led();                                           //��ʼ������ָʾLED
  
  Init_Adc();                                           //��ʼ��ADC1
  
  while(!(sys_clk_freq==SYE_Freq));                     //ȷ��ϵͳʱ��2MHz?
  
  Init_Relay_Control();                                 //��ʼ���̵����˿�
  
  Init_Time2_PWM();                                     //��ʼ��PWM��� ͨ��3
  
  Init_MF();                                           //��ʼ��΢�����˿�
   
  Init_Time1_mf();                                     //��ʼ��Time1��ʱ��
   
  Init_Ird();                                          //��ʼ��������ܶ˿�
  
  get_device_parameter();                               //��ȡ��Ӧ���������� ��ȡeeprom����
  
  //Read_Dip_Switch();                                    //��ȡ���뿪��״̬
  
  IWDG_Enable();                                        //ʹ�ܿ��Ź�
   
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);         // ����д����Ԥ��Ƶ�������ؼĴ���
  
  IWDG_SetPrescaler(IWDG_Prescaler_256);                //Ԥ��Ƶ��
  
  IWDG_SetReload(0xff);                                 //���ؼĴ���
  
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);        // ��ֹд����Ԥ��Ƶ�������ؼĴ���
  
  
  
  open_mf_detection_time=TRUE;                           //����΢���״��ⶨʱ��
   
  TIM4_Cmd(ENABLE);                                     //���ö�ʱ��Timer4
  
  enableInterrupts();                                  //ʹ��CPU�ж�
  
  
      while(1)
      {
        
          Scan_Ird_Process();   //������ܽ���
                    
          switch(system_mode_status)
          {
            case AUTO_MODE:     Auto_Mode_Process();                                    break;                            //��������ģʽ
              
            case ON_OFF_MODE:   On_Off_Mode_Process();  system_mode_status= NOP_MODE;   break;                          //�ֶ�����ģʽ
              
            case RESET_MODE:    Reset_Mode_Process();   system_mode_status=AUTO_MODE;   break;                        //��λ����ģʽ  ���в�������DIP SWITCH ���뿪�ض���
              
            case DIM_TEST_MODE:  Dim_Test_Mode_Process();                               break;                      //�������ģʽ ��ɺ��Զ�������������ģʽ
            
            case NOP_MODE:  nop();                                                      break;                     //���� ����״̬
              
            default:break;
          }
                   
          IWDG_ReloadCounter();
      
      }
      
 }


/************************************************************************
*������:
*������
*˵����  
************************************************************************/
void Dim_Test_Mode_Process(void)
{
       if(handle_pwm_dimming_flag==TRUE)
         {
           
                   if(COUNT_DIR==0x01)
                   {
                        if( NOW_VALUE< END_VALUE)
                        {
                            NOW_VALUE=NOW_VALUE+1; //1%ռ�ձ�����
                            
                            Dimming_level(NOW_VALUE);
                        }
                        else
                        {
                            
                          
                            Dimming_level(END_VALUE);
                            
                            pwm_dimming_time_count=0x00;
                  
                            handle_pwm_dimming_flag=FALSE;
                            
                            pwm_dimming_time_flag=FALSE;
                            
                            Dimming_level_up_down(100,0,TRUE,2); //ռ�ձȵݼ�
                        }
                   }
                   else if(COUNT_DIR==0x02)
                   {
                        if( NOW_VALUE>END_VALUE)
                        {
                            NOW_VALUE=NOW_VALUE-1; //1%ռ�ձ�
                            
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
*������:
*������
*˵����  
************************************************************************/
void Reset_Mode_Process(void)
{
  
      
      mf_detection_time_count=0x00;
                
      handle_mf_detection_flag=FALSE;
                                  
       open_mf_detection_time=FALSE;  //��ͣ΢�����
       
       set_by_switch_value(switch_current_status);   //ͨ�����뿪������ϵͳ����
       
       open_mf_detection_time=TRUE;//����΢�����
       
       while(1);//����ϵͳ
}

/************************************************************************
*������:
*������
*˵����  
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
*������:
*������
*˵����  
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
          if(sensor_daylight_level==1024)//Disable �������ն�  ֻ��΢����Ӧ
          {
              
                 if(open_sensor_time_flag==0x00)
                  {                                         
                       if(Have_person_flag==0x01)
                        {
                          
                              RELAY_ON;                                                           
                          
                              Dimming_level_up_down(70,100,TRUE,1); //ռ�ձȵ���
      
                              sensor_time_se_count=0x00;
                              
                              system_run_time_count=0x00;
                              
                              open_sensor_time_flag=0x01;  //�����̵�����ʱ����־
                              
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
                      Dimming_level_up_down(NOW_VALUE,sensor_pwm_dim_level,TRUE,2); //����ģʽռ�ձ�
                      
                      open_sensor_time_flag=0x03;
                 }
                 else if(open_sensor_time_flag==0x03)
                 {
                      if(Have_person_flag==0x01)
                         {
                           
                              Dimming_level_up_down(70,100,TRUE,1); //ռ�ձȵ���
                              
                              open_sensor_time_flag=0x01;
                              
                              sensor_time_se_count=0x00;
      
                              system_run_time_count=0x00;
                              
                              Have_person_flag=0x00;
                              
                         }
                 }
                 else if(open_sensor_time_flag==0x04)
                 {
                        Dimming_level_up_down(NOW_VALUE,0,TRUE,2); //ռ�ձȵݼ�
                        
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
                              
                                  Dimming_level_up_down(70,100,TRUE,1); //ռ�ձȵ���
          
                                  sensor_time_se_count=0x00;
                                  
                                  system_run_time_count=0x00;
                                  
                                  open_sensor_time_flag=0x01;  //�����̵�����ʱ����־
                                  
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
                      Dimming_level_up_down(NOW_VALUE,sensor_pwm_dim_level,TRUE,2); //ռ�ձȵݼ�
                      
                      open_sensor_time_flag=0x03;
                 }
                 else if(open_sensor_time_flag==0x03)
                 {
                      if(Have_person_flag==0x01)
                         {
                              Dimming_level_up_down(70,100,TRUE,1); //ռ�ձȵ���
                              
                              open_sensor_time_flag=0x01;
                              
                              sensor_time_se_count=0x00;
      
                              system_run_time_count=0x00;
                              
                              Have_person_flag=0x00;
                              
                         }
                 }
                 else if(open_sensor_time_flag==0x04)
                 {
                        Dimming_level_up_down(NOW_VALUE,0,TRUE,2); //ռ�ձȵݼ�
                        
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
                      
                            DELAY_MS_COUNT=200; //LED��ʱ����200ms
                          
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
              
              delay_us(15);                   //PA2��������͵�ƽ   ���ݷŵ�
            
              GPIOA->DDR = GPIOA->DDR&0xfb;   //PA2���������ж�    �����
                           
              GPIOA->CR1 &= 0xfb;
              
              GPIOA->CR2 |= 0x04;   
              
              if(ird_status==0x00)
              {
                  Init_Time1_mf();
            
                  TIM1_Cmd(ENABLE);   //������ʱ��Time1          
              }
  
                                                
              handle_mf_detection_flag=FALSE;
            
         }
   
        //-------------------------------------------------------------------------
         if(handle_pwm_dimming_flag==TRUE)
         {
           
                

                        if( NOW_VALUE< END_VALUE)
                        {
                            NOW_VALUE=NOW_VALUE+1; //5%ռ�ձ�����
                            
                            Dimming_level(NOW_VALUE);
                        }
                        else if( NOW_VALUE>END_VALUE)
                         {
                            NOW_VALUE=NOW_VALUE-1; //5%ռ�ձ�
                            
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
  



