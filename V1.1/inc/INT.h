#ifndef INT_H
#define INT_H

#include "stm32l1xx.h"

void GPIO_TIM3_Init(void);
void GPIO_EXTI_Init(void);
  
void EXTI1_Init(void);
void NVIC1_Init(void);
    
void TIM3_Init(void);

#endif