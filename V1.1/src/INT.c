#include "INT.h"

void EXTI1_Init(void) {

      // Configuring external interrupt
      
      //RCC_APB1PeriphClockCmd(RCC_APB2Periph_EXTIT,ENABLE);
      
      EXTI_InitTypeDef EXTIInitStruct;
      EXTIInitStruct.EXTI_Line = EXTI_Line2;
      EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
      EXTIInitStruct.EXTI_LineCmd = ENABLE;
     
      EXTI_Init(&EXTIInitStruct);

}

void NVIC1_Init(void) {

      NVIC_InitTypeDef NVICInitSrtuct;
      NVICInitSrtuct.NVIC_IRQChannel = EXTI2_IRQn;
      NVICInitSrtuct.NVIC_IRQChannelPreemptionPriority = 0x0F;
      NVICInitSrtuct.NVIC_IRQChannelSubPriority = 0x0F;
      NVICInitSrtuct.NVIC_IRQChannelCmd = ENABLE;
  
      NVIC_Init(&NVICInitSrtuct);
}

void TIM3_Init(void) {
      
      // Timer whitch flashing pin3 without interrupt
  
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
      
      TIM_TimeBaseInitTypeDef TimInitStruct;  
      TimInitStruct.TIM_Prescaler = 24000;
      TimInitStruct.TIM_Period = 1000;
      TimInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
      TIM_TimeBaseInit(TIM3, &TimInitStruct);    
      
      TIM_OCInitTypeDef TIM_OCInitStruct; 
      TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle;
      TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
      TIM_OCInitStruct.TIM_Pulse = 1000;
      TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
      TIM_OC3Init(TIM3, &TIM_OCInitStruct);
      
      TIM_Cmd(TIM3,ENABLE);
        
}

void GPIO_TIM3_Init(void) {

      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOB, ENABLE);  
      
      GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
      
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
      
      GPIO_Init(GPIOB, &GPIO_InitStruct);

      
      
}

void GPIO_EXTI_Init(void) {

      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);  
      
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
      
      GPIO_Init(GPIOA, &GPIO_InitStruct);      
  
}

void EXTI2_IRQHandler(void) {

  // Interrupt handler
  // Search name for inerrupts funcntions in startup file
  // uint8_t flag2 = 0;
  
  if(EXTI_GetITStatus(EXTI_Line2)) {
            

  } 

}