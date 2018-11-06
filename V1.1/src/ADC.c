#include "ADC.h"

uint32_t ADC_Timeout = 0;

// Two different functions because there some delay between them 
void ADC_Start_4Ch_Inj_Conv(void) {

  ADC_SoftwareStartInjectedConv(ADC1);  

}

void ADC_Read_4Ch(data_str_ADC *data_ptr){


  // Wait end of conversion
  ADC_Timeout = 300000;
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC)){
    if((ADC_Timeout--) == 0) { break; }  
  };
  ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
  // 12 bit resolution(in settings)
  data_ptr-> power_in_VLT = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
  data_ptr-> fanR_VLT     = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_2);
  data_ptr-> fanm1_VLT    = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_3);
  data_ptr-> fanm2_VLT    = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_4);

}

void FOUR_CH_ADC_Init(void){

  // Enable Clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  // Configure PINS
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
   
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
      
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  // Base settings
  ADC_InitTypeDef ADCInitStruct;
  ADCInitStruct.ADC_Resolution = ADC_Resolution_12b;
  ADCInitStruct.ADC_ScanConvMode = ENABLE;
  ADCInitStruct.ADC_ContinuousConvMode = DISABLE;
  ADCInitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADCInitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC3;
  ADCInitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADCInitStruct.ADC_NbrOfConversion = 4;
  ADC_Init(ADC1, &ADCInitStruct);
  
  ADC_InjectedSequencerLengthConfig(ADC1, 4);
  
  // Configure injected channels
  // VBat channel
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_8,  1, ADC_SampleTime_48Cycles);
  // FANR_VLT channel
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_19, 2, ADC_SampleTime_48Cycles);
  // FANM1_VLT channel
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_20, 3, ADC_SampleTime_48Cycles);
  // FANM2_VLT channel
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_21, 4, ADC_SampleTime_48Cycles);

  ADC_Cmd(ADC1, ENABLE);
  //ADC_InjectedDiscModeCmd(ADC1, ENABLE);
  
  // Delays between conversions
  //ADC_AutoInjectedConvCmd(ADC1, DISABLE);

  

}

/*
void ONE_CH_ADC_Init(void) {
  
  // Configure PINS
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
   
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
      
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Enable Clock
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  // Configure regular channels group
  ADC_InitTypeDef ADCInitStruct;
  ADCInitStruct.ADC_Resolution = ADC_Resolution_8b;
  ADCInitStruct.ADC_ScanConvMode = DISABLE;
  ADCInitStruct.ADC_ContinuousConvMode = DISABLE;
  ADCInitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADCInitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC3;
  ADCInitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADCInitStruct.ADC_NbrOfConversion = 1;
  
  ADC_Init(ADC1, &ADCInitStruct);
  
  // Configure regular channel
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  1, ADC_SampleTime_48Cycles);

  ADC_Cmd(ADC1, ENABLE);

}*/