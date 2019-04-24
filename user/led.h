#ifndef _LED_H

#define _LED_H


#include "stm8s.h"


//#define RUN_LED_ON              (GPIOC->ODR|=GPIO_PIN_3)
//#define RUN_LED_OFF            (GPIOC->ODR&=(~GPIO_PIN_3))
//#define RUN_LED_BLINKS         (GPIOC->ODR^=GPIO_PIN_3)

#define RUN_LED_ON              (GPIOD->ODR|=GPIO_PIN_6)
#define RUN_LED_OFF            (GPIOD->ODR&=(~GPIO_PIN_6))
#define RUN_LED_BLINKS         (GPIOD->ODR^=GPIO_PIN_6)




//------------变量声明
extern unsigned int DELAY_MS_COUNT;




//------------函数声明
extern void delay_ms(unsigned int ms);

extern void Init_Led(void);

extern void delay(unsigned short Count);

extern void delay_us(unsigned int us);


extern void led_blinks(void);



#endif