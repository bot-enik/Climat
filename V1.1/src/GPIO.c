#include "GPIO.h"

/* ============================================================================
                            ##### Initialize #####
 ============================================================================*/

void All_GPIO_Init(void) {

      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);
      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOB, ENABLE);
      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOC, ENABLE);
      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOH, ENABLE);
      
      // Input PWRGD Pins PULL UP       
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = PWRGD1|PWRGD2;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOB, &GPIO_InitStruct);

      GPIO_InitStruct.GPIO_Pin = PWRGD3;
      GPIO_Init(GPIOC, &GPIO_InitStruct);
      
      // Output pins PushPull (without VREG_M, VREG_R)
      GPIO_InitStruct.GPIO_Pin = PWR2_EN|PWR1_EN|FANM2_EN|PWR3_EN;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
      GPIO_Init(GPIOA, &GPIO_InitStruct);
      
      GPIO_InitStruct.GPIO_Pin = FANM1_EN|FANR_EN;
      GPIO_Init(GPIOB, &GPIO_InitStruct);

      // LED pins PushPull
      GPIO_InitStruct.GPIO_Pin = LED0|LED1;
      GPIO_Init(GPIOH, &GPIO_InitStruct);
      
      // IO1_LEDR, IO0_LEDG
      GPIO_InitStruct.GPIO_Pin = IO1_LEDR|IO0_LEDG;
      GPIO_Init(GPIOC, &GPIO_InitStruct);
      
      
}
