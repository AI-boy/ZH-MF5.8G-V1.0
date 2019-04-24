/**
  ******************************************************************************
  * @file    stm8s_it.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "stm8s.h"

    
/**********自定义**********/     
#include "userdefine.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)    //仅上升沿 触发中断
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 
   
   if(ird_status==0x01)
   {
        nop();
        return;
   }

    
    //-------------------------记录32个脉冲个数
    if(MF_pluse_receive_count<31)
    {
              MF_pluse_receive_count++;
        
              //状态模式1   高速推挽输出低电平 PA2 放电                      
            
              GPIOA->DDR = GPIOA->DDR|0x04;
              
              GPIOA->ODR = GPIOA->ODR&0xfb;
              
              GPIOA->CR1 |= 0x04;
              
              GPIOA->CR2 |= 0x04;
              
              delay_us(15);   // 具体延时时间参数需根据具体电路RC数值而定 保证放电到低电平
            
              //状态模式2  浮动输入 中断
              
              GPIOA->DDR = GPIOA->DDR&0xfb;
             
              
              GPIOA->CR1 &= 0xfb;
              
              GPIOA->CR2 |= 0x04;
              
             
              
         

        
    }
    else  //记满32个周期脉冲  获取时间
    {
                       
      
        mf_32_pulse_time_value[ temp_num++]=TIM1_GetCounter();
        
        temp_num = temp_num%6;
        
        
        if(temp_num==0x05)    
        {
            MF_pluse_receive_flag=0x01;
            
            nop();
        }
        
        GPIOA->DDR = GPIOA->DDR|0x04;
              
        GPIOA->ODR = GPIOA->ODR&0xfb;
              
        GPIOA->CR1 |= 0x04;
              
        GPIOA->CR2 |= 0x04;
        
        TIM1_Cmd(DISABLE);
      
        MF_pluse_receive_count=0x00;
        
        
    }
    
      
    
    
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)    //端口C PC4 仅下降沿触发中断   4.5ms+9ms=13.5ms
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    unsigned   int   time_cnt;
  
    ird_receive_time_count=100;//100ms超时时间
    
    if((receive_bit_count<32)&&(ird_receive_flag==0x00))
    {
          if(ird_status==0x00)
          {
              ird_status=0x01;
              
              TIM1->CNTRH=0x00;
              
              TIM1->CNTRL=0x00;//定时器清零
              
              TIM1_Cmd(ENABLE);
              
              mf_detection_time_count=0x00;
                
              handle_mf_detection_flag=FALSE;
              
              open_mf_detection_time=FALSE;  //暂停微波检测
              
              system_run_time_count=0x00;
              
              
          }
          else if(ird_status==0x01)
          {
                time_cnt=TIM1_GetCounter();
                
                if((time_cnt>1000)&&(time_cnt<1700))
                {
                    receive_bit_count=0x00; //数据计数清零
                      
                    ird_data=0x00;
                    
                    TIM1->CNTRH=0x00;
              
                    TIM1->CNTRL=0x00;//定时器清零
                }
                else if((time_cnt>50)&&(time_cnt<150)) //数据 ‘0’
                {
                    receive_bit_count++;
                    
                    ird_data=ird_data<<1;
                    
                    TIM1->CNTRH=0x00;
              
                    TIM1->CNTRL=0x00;//定时器清零
                }
                else if((time_cnt>150)&&(time_cnt<300)) //数据 ‘1’
                {
                    receive_bit_count++;
                    
                    ird_data=(ird_data<<1)+1;
                    
                    TIM1->CNTRH=0x00;
              
                    TIM1->CNTRL=0x00;//定时器清零
                }
          }
    }
    
    
  
  
  
   
  
  
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x) 
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8AF52Ax) */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
 
/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8AF622x)
/**
  * @brief UART4 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART4 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8AF622x) */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#else /* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */
/**
  * @brief ADC1 interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   
   
   ADC1_ClearITPendingBit(ADC1_IT_EOC);
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#else /* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
  * @brief Timer4 Update/Overflow Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)   //1ms系统时基 Tick
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
   if(system_run_time_count<1000)  //1s
   {     
      system_run_time_count++;     
   }
   else
   {
     
       if(open_sensor_time_flag==0x01)   //hold-time 定时器标志
       {
            if(sensor_time_se_count<sensor_hold_time)
            {
                sensor_time_se_count++;
                
                 
            }
            else
            {
                           
                open_sensor_time_flag=0x02;  
                
                sensor_time_se_count=0x00;
                              
                
            }
       }
       else if(open_sensor_time_flag==0x03)  //stand-by-period 定时器标志
       {
         
            if(sensor_dimming_period<=1800)
            {
                if(sensor_time_se_count<sensor_dimming_period)
                {
                    sensor_time_se_count++;
                                   
                }
                else
                {                
                    
                    open_sensor_time_flag=0x04;  
                    
                    sensor_time_se_count=0x00;
                    
                }
            }
            else     //"+&"状态  处于长时间 Dimming-level 状态
            {
                sensor_time_se_count=0x00;
            }
             
       }                 
           
      system_run_time_count=0x00;
            
   }
  
   
   /***********************************微波检测任务周期50ms***********************************************/
   
   if((open_mf_detection_time==TRUE)&&(handle_mf_detection_flag==FALSE))
   {
      if(mf_detection_time_count<50)
      {
          mf_detection_time_count++;
        
      }
      else
      {
          mf_detection_time_count=0x00;
          
          handle_mf_detection_flag=TRUE;
      }
   }
   //-----------------------------------------------------------------------------------------
   
   if(pwm_dimming_time_flag==TRUE)
   {
      if(pwm_dimming_time_count<50)
      {
          pwm_dimming_time_count++;
      }
      else
      {
          pwm_dimming_time_count=0x00;
          
          handle_pwm_dimming_flag=TRUE;
      }
   
   }
   
   
   /***************************精确1ms延时***************************************/
   if(DELAY_MS_COUNT>0)
   {
      DELAY_MS_COUNT--;
      
      if(DELAY_MS_COUNT==0x00)
      {
           RUN_LED_OFF;
      }
      
     
   }
   /********************************延时变量控制******************************************/
   if(ird_receive_time_count>0)
   {
      ird_receive_time_count--;
      
      if(ird_receive_time_count==0x00)
      {
          if(receive_bit_count==32)
          {
              ird_receive_flag=0x01;
              
              receive_bit_count=0x00;
          }
          else
          {
              receive_bit_count=0x00;
              
              ird_status=0x00;
              
              TIM1->CNTRH=0x00;
              
              TIM1->CNTRL=0x00;//定时器清零             
              
              mf_detection_time_count=0x00;
                
              handle_mf_detection_flag=FALSE;
              
              open_mf_detection_time=TRUE;  //开启微波检测
              
              system_run_time_count=0x00;
          }
      }
   }
  //----------------------------------继电器关闭后延时一段时间再检测微波
    if(relay_off_time_count>0)  
    {
        relay_off_time_count--;
        
        if(relay_off_time_count==0x00)
        {
            relay_off_flag=0x00;
        }
    }
      
   
   TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
   
 }
#endif /* (STM8S903) || (STM8AF622x)*/

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/