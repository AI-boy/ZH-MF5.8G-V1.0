/*********************************LED�ļ�***************************************/

#include "stm8s.h"
#include "userdefine.h"


unsigned int DELAY_MS_COUNT=0x00;


/************************************************************************
*������:delay
*������void   void
*˵����1ms��ʱ
************************************************************************/
void delay(unsigned short Count)
{
  u8 i,j;
  while (Count--)//Count�βο�����ʱ����
  {
    for(i=0;i<50;i++)
      for(j=0;j<40;j++);
  }
}
  
//��ȷms��ʱ
void delay_ms(unsigned int ms)
{
  DELAY_MS_COUNT=ms;
  
  while(DELAY_MS_COUNT);

}
//us����ʱ
void delay_us(unsigned int us)
{
    while(us--)
    {
      nop();  //0.5us
      nop();  //0.5us
    }
}

/************************************************************************
*������:init_led
*������void   void
*˵������ʼ��PC3�˿� ���ģʽ  �������
************************************************************************/

void Init_Led(void)
{
    unsigned char i;
    
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_SLOW);  //PC3 ����ָʾ��-->PD6
    
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



