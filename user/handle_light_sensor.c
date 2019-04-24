
#include "stm8s.h"

#include "handle_light_sensor.h"





bool open_light_sensor_time=FALSE;
bool handle_adc_process_flag=FALSE;
unsigned long light_sensor_time_count=0x00;


unsigned int adc_buff[20];
unsigned int light_value=1024;




/************************************************************************
*������:init_adc
*������void   void
*˵������ʼ��ADC1 �˿�PD5 AN5
************************************************************************/
void Init_Adc(void)
{
  GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
  //ADC1_DeInit();
 
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_5, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL5, DISABLE);
  
  ADC1_Cmd(ENABLE);
  ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_5, ADC1_ALIGN_RIGHT);
  ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);
  ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
  ADC1_AWDChannelConfig(ADC1_CHANNEL_5, DISABLE);
  //ADC1_Cmd(ENABLE);
  
  delay(1);
  
  ADC1_StartConversion();             //����ת��
   
  while(!(ADC1->CSR&0x80));          //�ȴ�ת������
   
  ADC1->CSR &= ~0x80;                //���ת��������־
   
}



/************************************************************************
*������:handle_adc_process
*������void   void
*˵����PD5 AN5  ADC ���նȲ���  ����ת��
************************************************************************/
void handle_adc_process(void)
{
    unsigned char i=0;
    
    Init_Adc();//---------2017-11-27-CAIWK
     
   disableInterrupts();//�����ٽ���
   
  for(i=0;i<20;i++)
  {
    
    ADC1_StartConversion();             //����ת��
  
    while(!(ADC1->CSR&0x80));          //�ȴ�ת������    
    
     ADC1->CSR &= ~0x80;                //���ת��������־
    
    adc_buff[i] = ADC1_GetConversionValue();
    
    delay(10);
    
    IWDG_ReloadCounter();

  }
  
  for(i=5;i<15;i++)
  {
    light_value+=adc_buff[i];
  }
  
  light_value=light_value/10;
  
  enableInterrupts(); //�˳��ٽ���
}