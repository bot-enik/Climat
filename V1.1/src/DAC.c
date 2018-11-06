#include "DAC.h"

void TWO_DAC_Init(void) {

      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
      
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
           
      GPIO_Init(GPIOA, &GPIO_InitStruct);
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
      
      DAC_InitTypeDef DAC_InitStruct;
      DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;
      DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
      DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
      DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
      
      DAC_Init(DAC_Channel_1, &DAC_InitStruct);
      DAC_Init(DAC_Channel_2, &DAC_InitStruct);
  
      DAC_Cmd(DAC_Channel_1, ENABLE);
      DAC_Cmd(DAC_Channel_2, ENABLE);
}

void DAC_VREG_M_Set(uint16_t val) {
      
      // Min 0  ---  Max: 4090
      DAC_SetChannel1Data(DAC_Align_12b_R, val);

}

void DAC_VREG_R_Set(uint16_t val) {
      
      // Min 0  ---  Max: 4090
      DAC_SetChannel2Data(DAC_Align_12b_R, val); 
         
}

