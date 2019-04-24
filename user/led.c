/*********************************LED文件***************************************/

#include "stm8s.h"
#include "userdefine.h"


unsigned int DELAY_MS_COUNT=0x00;


/************************************************************************
*函数名:delay
*参数：void   void
*说明：1ms延时
************************************************************************/
void delay(unsigned short Count)
{
  u8 i,j;
  while (Count--)//Count形参控制延时次数
  {
    for(i=0;i<50;i++)
      for(j=0;j<40;j++);
  }
}
  
//精确ms延时
void delay_ms(unsigned int ms)
{
  DELAY_MS_COUNT=ms;
  
  while(DELAY_MS_COUNT);

}
//us级延时
void delay_us(unsigned int us)
{
    while(us--)
    {
      nop();  //0.5us
      nop();  //0.5us
    }
}

/************************************************************************
*函数名:init_led
*参数：void   void
*说明：初始化PC3端口 输出模式  推挽输出
************************************************************************/

void Init_Led(void)
{
    unsigned char i;
    
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_SLOW);  //PC3 运行指示灯-->PD6
    
    for(i=0;i<20;i++)
    {
      RUN_LED_BLINKS;
    
      delay(300);
    }
  
    RUN_LED_OFF;
    
}

void led_blinks(void)
{
     RUN_LED_BLINKS;
     delay(200);
     RUN_LED_BLINKS;
     RUN_LED_OFF;
}



