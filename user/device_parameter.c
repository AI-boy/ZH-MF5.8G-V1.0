
/*******************************************************************************
* FIle:存放系统参数
*
*
*******************************************************************************/
#include "stm8s.h"
#include "userdefine.h"





//微波感应检测区域
__root EEPROM  unsigned char Detection_area_data[] @0x4000={1};          
//1:100%  
//2:50%

//100%PWM调光输出保持时间
__root EEPROM  unsigned char Hold_time_data[] @0x4001={1};               
//1:5s
//2:30s
//3:60s
//4:180s
//5:1200s
//6:1800s

//节能时间(调光输出)时间
__root EEPROM  unsigned char Dim_period_data[] @0x4002={1};             
//1:0s
//2:60s
//3:180s
//4:600s
//5:1800s
//6:2000s

//节能水平  PWM占空比
__root EEPROM  unsigned char Dim_level_data[] @0x4003={1};               
//1:10%
//2:20%
//3:30%
//4:50%

//感光门限   低于一定亮度条件下 再微波雷达检测动作
__root EEPROM  unsigned char Daylight_sensor_data[] @0x4004={7};         
//1:5lux
//2:15lux
//3:30lux
//4:50lux
//5:100lux
//6:150lux
//7:Disable(1024)  禁止感光检测  只需微波检测  默认值


//微波检测触发累计历史次数
__root EEPROM  unsigned char MF_sensor_data[] @0x4005={0};    


//-----------------系统重启次数+断电次数
__root EEPROM  unsigned char System_reset_count_h[] @0x4006={0}; 

__root EEPROM  unsigned char System_reset_count_l[] @0x4007={0};


//---------------------拨码开关设置状态
__root EEPROM  unsigned char factory_switch_value[] @0x4008={0x55,0xaa};








unsigned int switch_current_status;

unsigned int switch_before_status;

void  get_device_parameter(void)
{
  
    unsigned char ee_byte;
    
    unsigned char temp_h,temp_l;
  
    FLASH_Unlock(FLASH_MEMTYPE_DATA);  //解锁EEPROM 
    
    //-------------------------------------------0x4000  检测区域范围
    ee_byte =  FLASH_ReadByte(0x4000);
    
    if(ee_byte==1)
    {
        sensor_detection_area = 100;
    }
    else if(ee_byte==2)
    {
        sensor_detection_area = 50;
    }
    else
    {
        sensor_detection_area = 100;  //默认值
    }
    //-------------------------------------------0x4001  
    ee_byte =  FLASH_ReadByte(0x4001);
    
    if(ee_byte==1)
    {
        sensor_hold_time = 5;
    }
    else if(ee_byte==2)
    {
        sensor_hold_time = 30;
    }
    else if(ee_byte==3)
    {
        sensor_hold_time = 60;
    }
    else if(ee_byte==4)
    {
        sensor_hold_time = 180;
    }
    else if(ee_byte==5)
    {
        sensor_hold_time = 300;
    }
    else if(ee_byte==6)
    {
        sensor_hold_time = 600;
    }
    else if(ee_byte==7)
    {
        sensor_hold_time = 1200;
    }
    else if(ee_byte==8)
    {
        sensor_hold_time = 1800;
    }
    else
    {
        sensor_hold_time = 5;  //默认值
    }
    
    //-------------------------------------------0x4002
    ee_byte =  FLASH_ReadByte(0x4002);
    
    if(ee_byte==1)
    {
        sensor_dimming_period = 0;
    }
    else if(ee_byte==2)
    {
        sensor_dimming_period = 10;
    }
    else if(ee_byte==3)
    {
        sensor_dimming_period = 60;
    }
    else if(ee_byte==4)
    {
        sensor_dimming_period = 180;
    }
    else if(ee_byte==5)
    {
        sensor_dimming_period = 300;
    }
    else if(ee_byte==6)
    {
        sensor_dimming_period = 600;
    }
    else if(ee_byte==7)
    {
        sensor_dimming_period = 1800;
    }
    else if(ee_byte==8)
    {
        sensor_dimming_period = 2000;
    }
    else
    {
        sensor_dimming_period = 0;  //默认值
    }
    
    //-------------------------------------------0x4003
    ee_byte =  FLASH_ReadByte(0x4003);
    
    if(ee_byte==1)
    {
        sensor_pwm_dim_level = 10;
    }
    else if(ee_byte==2)
    {
        sensor_pwm_dim_level = 20;
    }
    else if(ee_byte==3)
    {
        sensor_pwm_dim_level = 30;
    }
    else if(ee_byte==4)
    {
        sensor_pwm_dim_level = 50;
    }
    else
    {
        sensor_pwm_dim_level = 10;  //默认值
    }
    
    //-------------------------------------------0x4004
    ee_byte =  FLASH_ReadByte(0x4004);
    
    if(ee_byte==1)
    {
        sensor_daylight_level = 5;
    }
    else if(ee_byte==2)
    {
        sensor_daylight_level = 15;
    }
    else if(ee_byte==3)
    {
        sensor_daylight_level = 30;
    }
    else if(ee_byte==4)
    {
       sensor_daylight_level = 50;
    }
    else if(ee_byte==5)
    {
        sensor_daylight_level = 100;
    }
    else if(ee_byte==6)
    {
        sensor_daylight_level = 150;
    }
    else if(ee_byte==7)
    {
        sensor_daylight_level = 1024;
    }
    else
    {
        sensor_daylight_level = 1024;  //默认值
    }
    
    //---------------系统累记重启次数（包括断电次数）
    temp_h=  FLASH_ReadByte(0x4006);
    
    temp_l=  FLASH_ReadByte(0x4007);
    
    system_reset_count =(temp_h<<8)|temp_l;
    
    system_reset_count = system_reset_count+1;
    
    temp_h =(unsigned char )(system_reset_count>>8);
    
    temp_l =(unsigned char )system_reset_count;
    
    FLASH_ProgramByte(0x4006, temp_h);
    
    FLASH_ProgramByte(0x4007, temp_l);
    
    //-----------------拨码开关状态
    temp_h=  FLASH_ReadByte(0x4008);
    
    temp_l=  FLASH_ReadByte(0x4009);
    
    
    
    switch_before_status =(temp_h<<8)|temp_l;
    
    FLASH_Lock(FLASH_MEMTYPE_PROG);
    
}

/************************************************************************
*函数名:
*参数：
*说明：  AD采集电压数值 区分不同拨码状态
*PB4 1
*PD2 234
*PD3 567
*PB5 8
*PC6 9
*PD6 1234
*
************************************************************************/
void Init_switch_PD2_port(void)
{

//-------PD2 浮动输入无中断
  
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);

 
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
  
  ADC1_Cmd(ENABLE);
  ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3, ADC1_ALIGN_RIGHT);
  ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);
  ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
  ADC1_AWDChannelConfig(ADC1_CHANNEL_3, DISABLE);

  
  delay(1);
  
  ADC1_StartConversion();             //启动转换
   
  while(!(ADC1->CSR&0x80));          //等待转换结束
   
  ADC1->CSR &= ~0x80;                //清除转换结束标志


}

void Init_switch_PD3_port(void)
{
  
//-------PD3 浮动输入无中断
  
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);

 
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_4, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);
  
  ADC1_Cmd(ENABLE);
  ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_4, ADC1_ALIGN_RIGHT);
  ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);
  ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
  ADC1_AWDChannelConfig(ADC1_CHANNEL_4, DISABLE);

  
  delay(1);
  
  ADC1_StartConversion();             //启动转换
   
  while(!(ADC1->CSR&0x80));          //等待转换结束
   
  ADC1->CSR &= ~0x80;                //清除转换结束标志


}

void Init_switch_PD6_port(void)
{
  
//-------PD6 浮动输入无中断
  
  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);

 
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL6, DISABLE);
  
  ADC1_Cmd(ENABLE);
  ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_6, ADC1_ALIGN_RIGHT);
  ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);
  ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
  ADC1_AWDChannelConfig(ADC1_CHANNEL_6, DISABLE);

  
  delay(1);
  
  ADC1_StartConversion();             //启动转换
   
  while(!(ADC1->CSR&0x80));          //等待转换结束
   
  ADC1->CSR &= ~0x80;                //清除转换结束标志


}

void Read_Dip_Switch(void)
{
     unsigned int switch_temp;
         
    switch_temp=0x00;
    
    switch_current_status=0x00;
    
    //-------PB4   浮动输入无中断 
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
    
  
  //-------------------------------------------------------PB4  ---1
  
  if(GPIOB->IDR&GPIO_PIN_4)  
  {

      switch_temp|=0x1000;//(1<<12);   //   off
      
   }
  else if(!(GPIOB->IDR&GPIO_PIN_4) )
  {
      
     switch_temp&=0xefff;              // on
       
  }

     
      
  

  //---------------------------------------------------------PD2 -2-3-4
    Init_switch_PD2_port();
  
    delay(10);
    
    ADC1_StartConversion();             //启动转换
  
    while(!(ADC1->CSR&0x80));          //等待转换结束    
    
     ADC1->CSR &= ~0x80;                //清除转换结束标志
    
    adc_buff[0] = ADC1_GetConversionValue();
    
    delay(10);
    
//-----------拨码位置：on-on-on---采样值249-----设定范围：240-260
    if((adc_buff[0]>240)&&(adc_buff[0]<260))      //--on-on-on
    {
       switch_temp&=0xf7ff;             //~(1<<11);  //--2
       switch_temp&=0xfbff;              //~(1<<10);  //--3
       switch_temp&=0xfdff;              //~(1<<9);  //--4   
    }
    else if((adc_buff[0]>350)&&(adc_buff[0]<380)) //off-on-on
    {
       switch_temp|=0x0800;             //(1<<11);   //--2
       switch_temp&=0xfbff;             //--3
       switch_temp&=0xfdff;             //--4  
    }
    else if((adc_buff[0]>320)&&(adc_buff[0]<342)) //on-off-on
    {
       switch_temp&=0xf7ff;             //~(1<<11);   //--2
       switch_temp|=0x0400;             //(1<<10);    //--3
       switch_temp&=0xfdff;             //~(1<<9);   //--4  
    }
    else if((adc_buff[0]>559)&&(adc_buff[0]<579))//off-off-on
    {
       switch_temp|=0x0800;    //--2
       switch_temp|=0x0400;    //--3
       switch_temp&=0xfdff;   //--4  
    }
    else if((adc_buff[0]>300)&&(adc_buff[0]<320))//on-on-off
    {
       switch_temp&=0xf7ff;    //--2
       switch_temp&=0xfbff;    //--3
       switch_temp|=0x0200;    //--4  
    }
    else if((adc_buff[0]>1010)&&(adc_buff[0]<1025)) //off-off-off
    {
       switch_temp|=0x0800;    //--2
       switch_temp|=0x0400;    //--3
       switch_temp|=0x0200;    //--4  
    }
    else
    {
       switch_temp&=0xf7ff;  //--2
       switch_temp&=0xfbff;  //--3
       switch_temp&=0xfdff;  //--4   
    }
    
    //---------------------------------------------------------PD3 -5-6-7
   Init_switch_PD3_port();
  
    delay(10);
    
    ADC1_StartConversion();             //启动转换
  
    while(!(ADC1->CSR&0x80));          //等待转换结束    
    
     ADC1->CSR &= ~0x80;               //清除转换结束标志
    
    adc_buff[0] = ADC1_GetConversionValue();
    
    delay(10);
    
    if((adc_buff[0]>240)&&(adc_buff[0]<260)) //---on-on-on 
    {
       switch_temp&=0xfeff;//~(1<<8);  //--5
       switch_temp&=0xff7f;  //--6
       switch_temp&=0xffbf;  //--7  
    }
    else if((adc_buff[0]>355)&&(adc_buff[0]<375)) //off-on-on
    {
       switch_temp|=0x0100;//(1<<8);   //--5
       switch_temp&=0xff7f;  //--6
       switch_temp&=0xffbf;  //--7  
    }
    else if((adc_buff[0]>320)&&(adc_buff[0]<342)) //on-off-on
    {
       switch_temp&=0xfeff;   //--5
       switch_temp|=0x0080;    //--6
       switch_temp&=0xffbf;   //--7  
    }
    else if((adc_buff[0]>559)&&(adc_buff[0]<579)) //off-off-on
    {
       switch_temp|=0x0100;    //--5
       switch_temp|=0x0080;    //--6
       switch_temp&=0xffbf;   //--7  
    }
    else if((adc_buff[0]>300)&&(adc_buff[0]<320)) //on-on-off
    {
       switch_temp&=0xfeff;    //--5
       switch_temp&=0xff7f;    //--6
       switch_temp|=0x0040;     //--7  
    }
    else if((adc_buff[0]>1010)&&(adc_buff[0]<1025)) //off-off-off
    {
       switch_temp|=0x0100;    //--5
       switch_temp|=0x0080;    //--6
       switch_temp|=0x0040;    //--7  
    }
    else
    {
       switch_temp&=0xfeff;//~(1<<8);  //--5
       switch_temp&=0xff7f;  //--6
       switch_temp&=0xffbf;  //--7  
    }
    
  //------------------------------------------------PB5   浮动输入无中断   --8
   GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
  
  
  if (GPIOB->IDR&GPIO_PIN_5)  
  {

      switch_temp|=0x0020;   //bit5-1 off
      
   }
  else
  {

      switch_temp&=0xffdf; //bit5-1 on
      
  }
  
  //----------------------------------------------PC6   浮动输入无中断     --9
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
  
  
  if (GPIOC->IDR&GPIO_PIN_6)  
  {

      switch_temp|=0x0010;   //bit4-1 off
      
   }
  else
  {

      switch_temp&=0xffef; //bit4-1 on
      
  }
  //-------------------------------------------------PD6  10-11-12-13
   Init_switch_PD6_port();
  
    delay(10);
    
     ADC1_StartConversion();             //启动转换
  
    while(!(ADC1->CSR&0x80));          //等待转换结束    
    
     ADC1->CSR &= ~0x80;                //清除转换结束标志
    
     adc_buff[0] = ADC1_GetConversionValue();
    
    delay(10);
    
   if((adc_buff[0]>200)&&(adc_buff[0]<235)) //---on-on-on-on
    {
       switch_temp&=0xfff7;  //--3
       switch_temp&=0xfffb;  //--2
       switch_temp&=0xfffd;  //--1  
       switch_temp&=0xfffe;  //--0
    }
    else if((adc_buff[0]>290)&&(adc_buff[0]<305)) //of-on-on-on
    {
       switch_temp|=0x0008;  //--3
       switch_temp&=0xfffb;  //--2
       switch_temp&=0xfffd;  //--1  
       switch_temp&=0xfffe;  //--0
    }
    else if((adc_buff[0]>315)&&(adc_buff[0]<345)) //on-of-on-on
    {
       switch_temp&=0xfff7;  //--3
       switch_temp|=0x0004;  //--2
       switch_temp&=0xfffd;  //--1  
       switch_temp&=0xfffe;  //--0
    }
    else if((adc_buff[0]>550)&&(adc_buff[0]<586)) //of-of-on-on
    {
       switch_temp|=0x0008;  //--3
       switch_temp|=0x0004;  //--2
       switch_temp&=0xfffd;  //--1  
       switch_temp&=0xfffe;  //--0
    }
    else if((adc_buff[0]>305)&&(adc_buff[0]<315)) //on-on-of-on
    {
       switch_temp&=0xfff7;  //--3
       switch_temp&=0xfffb;  //--2
       switch_temp|=0x0002;  //--1  
       switch_temp&=0xfffe;  //--0
    }
    else if((adc_buff[0]>235)&&(adc_buff[0]<265)) //on-on-on-of
    {
       switch_temp&=0xfff7;  //--3
       switch_temp&=0xfffb;  //--2
       switch_temp&=0xfffd;  //--1  
       switch_temp&=0x0001;  //--0
    }
    else if((adc_buff[0]>1000)&&(adc_buff[0]<1025)) //of-of-of-of
    {
        switch_temp|=0x0008;  //--3
        switch_temp|=0x0004;  //--2
        switch_temp|=0x0002;  //--1  
        switch_temp|=0x0001;  //--0
    }
    else
    {
        switch_temp|=0x0008;  //--3
        switch_temp|=0x0004;  //--2
        switch_temp|=0x0002;  //--1  
        switch_temp|=0x0001;  //--0
    }
    
    switch_current_status = switch_temp;
    
    if(switch_current_status!=switch_before_status)                             //拨码开关位置改变
    {
        switch_before_status=switch_current_status;
        
        FLASH_ProgramByte(0x4008, (unsigned char)(switch_current_status>>8));  //------保存当前拨码开关位置信息
        
        FLASH_ProgramByte(0x4009, (unsigned char) switch_current_status);
        
        
        switch_temp=switch_current_status;
        
        set_by_switch_value(switch_current_status);
         
        
    }
    else
    {
        nop();
    }  
    
  
    
}





void set_by_switch_value(unsigned int switch_value)
{
        //-----------------------------------------------Detection area
        if(switch_value&(1<<12))        
        {
            sensor_detection_area=50;
                                  
            FLASH_ProgramByte(0x4000, 2); 
            
        }
        else if(!(switch_value&(1<<12)))
        {
            sensor_detection_area=100;
                                  
            FLASH_ProgramByte(0x4000, 1);  
        }
        else
        {
            sensor_detection_area=100;
                                  
            FLASH_ProgramByte(0x4000, 1);  
        }
        
        //-----------------------------------------------Hold time
        if( !(switch_value&(1<<11)) && !(switch_value&(1<<10)) && !(switch_value&(1<<9)) )  //on-on-on
        {
            sensor_hold_time=5;
                                  
            FLASH_ProgramByte(0x4001, 1);  
        }
        else if( (switch_value&(1<<11)) && !(switch_value&(1<<10)) && !(switch_value&(1<<9)) ) //off-on-on
        {
            sensor_hold_time=30;
                                  
            FLASH_ProgramByte(0x4001, 2); 
        }
        else if( !(switch_value&(1<<11)) && (switch_value&(1<<10)) && !(switch_value&(1<<9)) ) //on-off-on
        {
            sensor_hold_time=60;
                                  
            FLASH_ProgramByte(0x4001, 3); 
        }
        else if( (switch_value&(1<<11)) && (switch_value&(1<<10)) && !(switch_value&(1<<9)) ) //off-off-on
        {
            sensor_hold_time=180;
                                  
            FLASH_ProgramByte(0x4001, 4);
        }
        else if( !(switch_value&(1<<11)) && !(switch_value&(1<<10)) && (switch_value&(1<<9)) ) //on-on-off
        {
            sensor_hold_time=1200;
                                  
            FLASH_ProgramByte(0x4001, 7);
        }
        else if( (switch_value&(1<<11)) && (switch_value&(1<<10)) && (switch_value&(1<<9)) ) //of-of-off
        {
            sensor_hold_time=1800;
                                  
            FLASH_ProgramByte(0x4001, 8);
        }
        else
        {
            sensor_hold_time=5;
                                  
            FLASH_ProgramByte(0x4001, 1); 
        }
        
        //---------------------------------------------------Stand-by period
         if( !(switch_value&(1<<8)) && !(switch_value&(1<<7)) && !(switch_value&(1<<6)) )  //on-on-on
         {
            sensor_dimming_period=0;
                                  
            FLASH_ProgramByte(0x4002, 1); 
         }
        else if( (switch_value&(1<<8)) && !(switch_value&(1<<7)) && !(switch_value&(1<<6)) )  //of-on-on
        {
            sensor_dimming_period=60;
                                  
            FLASH_ProgramByte(0x4002, 3); 
        }
        else if( !(switch_value&(1<<8)) && (switch_value&(1<<7)) && !(switch_value&(1<<6)) )  //on-of-on
        {
            sensor_dimming_period=180;
                                  
            FLASH_ProgramByte(0x4002, 4); 
        }
        else if( (switch_value&(1<<8)) && (switch_value&(1<<7)) && !(switch_value&(1<<6)) )  //of-of-on
        {
            sensor_dimming_period=600;
                                  
            FLASH_ProgramByte(0x4002, 6); 
        }
        else if( !(switch_value&(1<<8)) && !(switch_value&(1<<7)) && (switch_value&(1<<6)) )  //on-on-of
        {
            sensor_dimming_period=1800;
                                  
            FLASH_ProgramByte(0x4002, 7); 
        }
        else if( (switch_value&(1<<8)) && (switch_value&(1<<7)) && (switch_value&(1<<6)) )  //of-of-of
        {
            sensor_dimming_period=2000;
                                  
            FLASH_ProgramByte(0x4002, 8); 
        }
        else
        {
            sensor_dimming_period=0;
                                  
            FLASH_ProgramByte(0x4002, 1); 
        }
        
        //--------------------------------------------------------Stand-by dimming level
        if( !(switch_value&(1<<5)) && !(switch_value&(1<<4))  ) //on-on
        {
            sensor_pwm_dim_level=10;
                                  
            FLASH_ProgramByte(0x4003, 1);
        }
        else if( !(switch_value&(1<<5)) && (switch_value&(1<<4))  )
        {
            sensor_pwm_dim_level=20;
                                  
            FLASH_ProgramByte(0x4003, 2);
        }
        else if( (switch_value&(1<<5)) && !(switch_value&(1<<4))  )
        {
            sensor_pwm_dim_level=30;
                                  
            FLASH_ProgramByte(0x4003, 3);
        }
        else if( (switch_value&(1<<5)) && (switch_value&(1<<4))  )
        {
            sensor_pwm_dim_level=50;
                                  
            FLASH_ProgramByte(0x4003, 4);
        }
        else
        {
            sensor_pwm_dim_level=10;
                                  
            FLASH_ProgramByte(0x4003, 1);
        }
        
        //-------------------------------------------------------------Daylight sensor
         if( !(switch_value&(1<<3)) && !(switch_value&(1<<2)) && !(switch_value&(1<<1)) && !(switch_value&(1<<0))  ) //on-on-on-on
         {
            sensor_daylight_level=5;
                                                
            FLASH_ProgramByte(0x4004, 1);  
         }
        else if( (switch_value&(1<<3)) && !(switch_value&(1<<2)) && !(switch_value&(1<<1)) && !(switch_value&(1<<0))  ) //of-on-on-on
        {
            sensor_daylight_level=15;
                                                
            FLASH_ProgramByte(0x4004, 2);  
        }
        else if( !(switch_value&(1<<3)) && (switch_value&(1<<2)) && !(switch_value&(1<<1)) && !(switch_value&(1<<0))  ) //on-of-on-on
        {
            sensor_daylight_level=30;
                                                
            FLASH_ProgramByte(0x4004, 3);  
        }
        else if( (switch_value&(1<<3)) && (switch_value&(1<<2)) && !(switch_value&(1<<1)) && !(switch_value&(1<<0))  ) //of-of-on-on
        {
            sensor_daylight_level=50;
                                                
            FLASH_ProgramByte(0x4004, 4);  
        }
        else if( !(switch_value&(1<<3)) && !(switch_value&(1<<2)) && (switch_value&(1<<1)) && !(switch_value&(1<<0))  ) //on-on-of-on
        {
            sensor_daylight_level=100;
                                                
            FLASH_ProgramByte(0x4004, 5);  
        }
        else if( !(switch_value&(1<<3)) && !(switch_value&(1<<2)) && !(switch_value&(1<<1)) && (switch_value&(1<<0))  ) //on-on-on-of
        {
            sensor_daylight_level=150;
                                                
            FLASH_ProgramByte(0x4004, 6);  
        }
        else if( (switch_value&(1<<3)) && (switch_value&(1<<2)) && (switch_value&(1<<1)) && (switch_value&(1<<0))  ) //of-of-of-of
        {
            sensor_daylight_level=1024;
                                                
            FLASH_ProgramByte(0x4004, 7);  
        }
        else
        {
            sensor_daylight_level=1024;
                                                
            FLASH_ProgramByte(0x4004, 7);
        }
    
}











                                                               