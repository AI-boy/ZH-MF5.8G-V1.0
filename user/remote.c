

#include "stm8s.h"
#include "userdefine.h"
#include "remote.h"

  unsigned char  ird_status=0x00;

  unsigned char   receive_bit_count=0x00;

  unsigned char   ird_receive_flag=0x01;

  unsigned char   ird_receive_time_count=0x00;

  unsigned long   ird_data=0x00FF629D;
  


  unsigned char lamp_flag=0x00;
  
  
  YSYTEM_MODE  system_mode_status=AUTO_MODE;

/************************************************************************
*������:
*������
*˵����  
************************************************************************/
void Init_Ird(void)
{

              //PC4  ������ܶ˿�    �½��ش����ж� 
  
              GPIOC->DDR = GPIOC->DDR&0xEF;             
              
              GPIOC->CR1 |= 0x10;     //PC4  ��������
              
              GPIOC->CR2 |= 0x10;     //�ⲿ�ж�ʹ��
              
              EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);  //�˿�C ���½��ش����ж�

}

/************************************************************************
*������:
*������
*˵����  
************************************************************************/
void Key_Menu(unsigned long key_value)
{
    unsigned char code;
    
    unsigned int  remote_mac;
    
    code=(unsigned char)(key_value>>8);
    
    //0x4ECA807F   ����4ECAΪ�û��� Ҳ������ң�����豸�� MAC
    
    remote_mac=key_value>>16;
    
    if(remote_mac!=ZH_MAC)   //0x4ECA
    {
        return;
    }
    
    led_blinks();
    
    switch(code)
    {

        case on_off         :       //-------------------------------------------------�ֶ�����ģʽ 
                              {
                                    system_mode_status=ON_OFF_MODE;  
                                    
                                    lamp_flag=lamp_flag^0xff ;
                              }
                              break;
            
        case reset          :     //--------------------------------------------------���ݲ��뿪�ػָ���������
                              {
                                
                                  open_sensor_time_flag=0x00;
                               
                                  Have_person_flag=0x00;                                
                                
                                  sensor_time_se_count=0x00; 
                                  
                                  mf_detection_time_count=0x00;
                
                                  handle_mf_detection_flag=FALSE;
                                  
                                  open_mf_detection_time=FALSE;  //��ͣ΢�����
                                  
                                  system_mode_status=RESET_MODE;  
                                                                   
                              }
                              break;
            
        case auto_mode      :       //---------------------------------------------------�Զ���Ӧģʽ                         
                                
                                system_mode_status=AUTO_MODE; 
                               
                                while(1);
                              
                                break;
            
        case dim_test       :       //-------------------------------------------------�������ģʽ
                               open_sensor_time_flag=0x00;
                               
                               Have_person_flag=0x00;
                               
                               NOW_VALUE=0x00;
                               
                               END_VALUE=0x00;
                              
                               sensor_time_se_count=0x00; 
                               
                               Dimming_level_up_down(NOW_VALUE,100,TRUE,1); //ռ�ձȵ���
                              
                               system_mode_status=DIM_TEST_MODE; 
                              
                              break;
        
        case QS1            :   
                              {
                                  sensor_detection_area=100;    
                                  FLASH_ProgramByte(0x4000, 1);  //�������
                                  
                                  sensor_hold_time=30; 
                                  FLASH_ProgramByte(0x4001, 2);  //����ʱ��
                                  
                                  sensor_dimming_period=60; 
                                  FLASH_ProgramByte(0x4002, 3);  //���ܵ���ʱ��
                                  
                                  sensor_pwm_dim_level=10;  
                                  FLASH_ProgramByte(0x4003, 1);  //����PWMռ�ձ�
                                  
                                  sensor_daylight_level=5;
                                  FLASH_ProgramByte(0x4004, 1);  //�й����ֵ
                              }   
                              break;
        
        case QS2            :       
                              {
                                  sensor_detection_area=100;    
                                  FLASH_ProgramByte(0x4000, 1);  //�������
                                  
                                  sensor_hold_time=60; 
                                  FLASH_ProgramByte(0x4001, 3);  //����ʱ��
                                  
                                  sensor_dimming_period=180; 
                                  FLASH_ProgramByte(0x4002, 4);  //���ܵ���ʱ��
                                  
                                  sensor_pwm_dim_level=10;  
                                  FLASH_ProgramByte(0x4003, 1);  //����PWMռ�ձ�
                                  
                                  sensor_daylight_level=15;
                                  FLASH_ProgramByte(0x4004, 2);  //�й����ֵ
                              }   
                              break;
        
        case QS3            :      
                              {
                                  sensor_detection_area=100;    
                                  FLASH_ProgramByte(0x4000, 1);  //�������
                                  
                                  sensor_hold_time=300; 
                                  FLASH_ProgramByte(0x4001, 5);  //����ʱ��
                                  
                                  sensor_dimming_period=600; 
                                  FLASH_ProgramByte(0x4002, 6);  //���ܵ���ʱ��
                                  
                                  sensor_pwm_dim_level=10;  
                                  FLASH_ProgramByte(0x4003, 1);  //����PWMռ�ձ�
                                  
                                  sensor_daylight_level=30;
                                  FLASH_ProgramByte(0x4004, 3);  //�й����ֵ
                              }   
                              break;
        
        case QS4            :         
                              {
                                  sensor_detection_area=100;    
                                  FLASH_ProgramByte(0x4000, 1);  //�������
                                  
                                  sensor_hold_time=600; 
                                  FLASH_ProgramByte(0x4001, 6);  //����ʱ��
                                  
                                  sensor_dimming_period=1800; 
                                  FLASH_ProgramByte(0x4002, 7);  //���ܵ���ʱ��
                                  
                                  sensor_pwm_dim_level=10;  
                                  FLASH_ProgramByte(0x4003, 1);  //����PWMռ�ձ�
                                  
                                  sensor_daylight_level=1024;
                                  FLASH_ProgramByte(0x4004, 7);  //�й����ֵ
                              }   
                              break;
        
        case detection_area_25:  
                              {
                                  sensor_detection_area=50;
                                  
                                  FLASH_ProgramByte(0x4000, 2);  
                              }                                         
                              break;                                    
                                       
                               
        
        case detection_area_50:   
                              {
                                  sensor_detection_area=50;
                                  
                                  FLASH_ProgramByte(0x4000, 2);  
                              }                                         
                              break; 
        
        case detection_area_75:  
                              {
                                  sensor_detection_area=50;
                                  
                                  FLASH_ProgramByte(0x4000, 2);  
                              }                                         
                              break; 
        
        case detection_area_100:
                               {
                                  sensor_detection_area=100;
                                  
                                  FLASH_ProgramByte(0x4000, 1);  
                                }                                         
                              break; 
        
        case hold_time_5: 
                              {
                                  sensor_hold_time=5;
                                  
                                  FLASH_ProgramByte(0x4001, 1);  
                               }                                         
                              break; 
        
        case hold_time_30:  
                              {
                                  sensor_hold_time=30;
                                  
                                  FLASH_ProgramByte(0x4001, 2);  
                               }                                         
                              break; 
        
        case hold_time_60:   
                              {
                                  sensor_hold_time=60;
                                  
                                  FLASH_ProgramByte(0x4001, 3);  
                               }                                         
                              break; 
        
        case hold_time_180:      
                                {
                                  sensor_hold_time=180;
                                  
                                  FLASH_ProgramByte(0x4001, 4);  
                               }                                         
                              break; 
        
        case hold_time_300:      
                                {
                                  sensor_hold_time=300;
                                  
                                  FLASH_ProgramByte(0x4001, 5);  
                               }                                         
                              break; 
        
        case hold_time_600:     
                                {
                                  sensor_hold_time=600;
                                  
                                  FLASH_ProgramByte(0x4001, 6);  
                               }                                         
                              break; 
        
        case hold_time_1200:   
                                {
                                  sensor_hold_time=1200;
                                  
                                  FLASH_ProgramByte(0x4001, 7);  
                               }                                         
                              break; 
        
        case hold_time_1800:    
                                {
                                  sensor_hold_time=1800;
                                  
                                  FLASH_ProgramByte(0x4001, 8);  
                               }                                         
                              break;
        
        
        case dim_period_0:    
                              {
                                  sensor_dimming_period=0;
                                  
                                  FLASH_ProgramByte(0x4002, 1);  
                               } 
                              break;    //����ģʽ  ����ʱ��0s
        
        case dim_period_10:            
                              {
                                  sensor_dimming_period=10;
                                  
                                  FLASH_ProgramByte(0x4002, 2);  
                               }
                              break;   //����ģʽ  ����ʱ��10s
        
        case dim_period_60:                                 
                              {
                                  sensor_dimming_period=60;
                                  
                                  FLASH_ProgramByte(0x4002, 3);  
                               }
                               break;   //����ģʽ  ����ʱ��60s
        
        case dim_period_180:                                
                                {
                                  sensor_dimming_period=180;
                                  
                                  FLASH_ProgramByte(0x4002, 4);  
                               }
                                break;  //����ģʽ  ����ʱ��180s
        
        case dim_period_300:                                
                                {
                                  sensor_dimming_period=300;
                                  
                                  FLASH_ProgramByte(0x4002, 5);  
                               }
                                break;  //����ģʽ  ����ʱ��300s
        
        case dim_period_600:                                
                                {
                                  sensor_dimming_period=600;
                                  
                                  FLASH_ProgramByte(0x4002, 6);  
                               }
                                break;  //����ģʽ  ����ʱ��600s
        
        case dim_period_1800:                               
                                {
                                  sensor_dimming_period=1800;
                                  
                                  FLASH_ProgramByte(0x4002, 7);  
                               }
                                break; //����ģʽ  ����ʱ��1800s
        
        case dim_period_2000:                              
                                {
                                  sensor_dimming_period=2000;
                                  
                                  FLASH_ProgramByte(0x4002, 8);  
                               }
                                break;  //����1800sʱ���峤ʱ��������ģʽ
        case dim_level_10:   
                              {
                                  sensor_pwm_dim_level=10;
                                  
                                  FLASH_ProgramByte(0x4003, 1);  
                               } 
                               break;    //����ģʽ  PWMռ�ձ�10%
        
        case dim_level_20:      
                              {
                                  sensor_pwm_dim_level=20;
                                  
                                  FLASH_ProgramByte(0x4003, 2);  
                              } 
                              break;    //����ģʽ  PWMռ�ձ�20%
        
        case dim_level_30:                                  
                              {
                                  sensor_pwm_dim_level=30;
                                  
                                  FLASH_ProgramByte(0x4003, 3);  
                               } 
                              break;    //����ģʽ  PWMռ�ձ�30%
        
        case dim_level_50:                                 
                              {
                                  sensor_pwm_dim_level=50;
                                  
                                  FLASH_ProgramByte(0x4003, 4);  
                               } 
                              break;    //����ģʽ  PWMռ�ձ�50%
        
        case daylight_sensor_level_5lux:                  
                                              {
                                                sensor_daylight_level=5;
                                                
                                                FLASH_ProgramByte(0x4004, 1);  
                                              }
                                              break;
        
        case daylight_sensor_level_15lux:                    
                                              {
                                                sensor_daylight_level=15;
                                                
                                                FLASH_ProgramByte(0x4004, 2);  
                                              }
                                              break;
        
        case daylight_sensor_level_30lux:                 
                                              {
                                                sensor_daylight_level=30;
                                                
                                                FLASH_ProgramByte(0x4004, 3);  
                                              }
                                              break;
        
        case daylight_sensor_level_50lux:                   
                                              {
                                                sensor_daylight_level=50;
                                                
                                                FLASH_ProgramByte(0x4004, 4);  
                                              }
                                              break;
        
        case daylight_sensor_level_100lux:                  
                                              {
                                                sensor_daylight_level=100;
                                                
                                                FLASH_ProgramByte(0x4004, 5);  
                                              }
                                              break;
        
        case daylight_sensor_level_150lux:                   
                                              {
                                                sensor_daylight_level=150;
                                                
                                                FLASH_ProgramByte(0x4004, 6);  
                                              }
                                              break;
        
        case daylight_sensor_disable:            //��ֹ��ȸ�Ӧ          
                                              {
                                                sensor_daylight_level=1024;
                                                
                                                FLASH_ProgramByte(0x4004, 7);  
                                              }
                                              break;
        
        case test_mode:                                     break;
        
        default:break;
    
    
    
    
    }
    
  
}


/************************************************************************
*������:
*������
*˵����  
************************************************************************/
void Scan_Ird_Process(void)
{
    if(ird_receive_flag==0x01)
        {
          
              Key_Menu(ird_data);
          
              receive_bit_count=0x00;
              
              ird_status=0x00;
              
              ird_data=0x00;
              
              TIM1->CNTRH=0x00;
              
              TIM1->CNTRL=0x00;//��ʱ������             
              
              mf_detection_time_count=0x00;
                
              handle_mf_detection_flag=FALSE;
              
              open_mf_detection_time=TRUE;  //����΢�����
              
              system_run_time_count=0x00;
              
              ird_receive_flag=0x00;
              
             
        }

}