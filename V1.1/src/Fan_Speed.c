#include "Fan_Speed.h"

static volatile uint16_t pulse_counter;

static volatile uint32_t nowTime, pastTime, period;

uint16_t capture1 = 0, capture2 = 0;
volatile uint8_t capture_is_first = 1, capture_is_ready = 0;

uint8_t fan_to_measure = FANM1;

/* ============================================================================
                     #####  (PA6) USING TIMER #####
 ============================================================================*/

void TIM_Pulses_INIT_FANM1(void) {
  GPIO_InitTypeDef gpio_cfg;
  GPIO_StructInit(&gpio_cfg);
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  gpio_cfg.GPIO_Mode = GPIO_Mode_AF;
  // gpio_cfg.GPIO_PuPd = GPIO_PuPd_DOWN;
  gpio_cfg.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &gpio_cfg);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,GPIO_AF_TIM3);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_TimeBaseInitTypeDef timer_base;
  TIM_TimeBaseStructInit(&timer_base);
  timer_base.TIM_Prescaler = 16000 - 1; /// MUST BE correct
  TIM_TimeBaseInit(TIM3, &timer_base);

  TIM_ICInitTypeDef timer_ic;
  timer_ic.TIM_Channel = TIM_Channel_1;
  timer_ic.TIM_ICPolarity = TIM_ICPolarity_Rising;
  timer_ic.TIM_ICSelection = TIM_ICSelection_DirectTI;
  timer_ic.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  timer_ic.TIM_ICFilter = 0;
  TIM_ICInit(TIM3, &timer_ic);

  
  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
    
  NVIC_EnableIRQ(TIM3_IRQn);
  
  TIM_Cmd(TIM3, ENABLE);

}

void TIM_Pulses_INIT_FANM2(void) {
  GPIO_InitTypeDef gpio_cfg;
  GPIO_StructInit(&gpio_cfg);
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  gpio_cfg.GPIO_Mode = GPIO_Mode_AF;
  //gpio_cfg.GPIO_PuPd = GPIO_PuPd_DOWN;
  gpio_cfg.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &gpio_cfg);
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,GPIO_AF_TIM3);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_TimeBaseInitTypeDef timer_base;
  TIM_TimeBaseStructInit(&timer_base);
  timer_base.TIM_Prescaler = 16000 - 1; /// MUST BE correct
  TIM_TimeBaseInit(TIM3, &timer_base);

  TIM_ICInitTypeDef timer_ic;
  timer_ic.TIM_Channel = TIM_Channel_4;
  timer_ic.TIM_ICPolarity = TIM_ICPolarity_Rising;
  timer_ic.TIM_ICSelection = TIM_ICSelection_DirectTI;
  timer_ic.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  timer_ic.TIM_ICFilter = 0;
  TIM_ICInit(TIM3, &timer_ic);

  
  TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
    
  NVIC_EnableIRQ(TIM3_IRQn);
  
  TIM_Cmd(TIM3, ENABLE);

}

void TIM_Pulses_INIT_FANR(void) {
  GPIO_InitTypeDef gpio_cfg;
  GPIO_StructInit(&gpio_cfg);
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  gpio_cfg.GPIO_Mode = GPIO_Mode_AF;
  //gpio_cfg.GPIO_PuPd = GPIO_PuPd_DOWN;
  gpio_cfg.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &gpio_cfg);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7,GPIO_AF_TIM3);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_TimeBaseInitTypeDef timer_base;
  TIM_TimeBaseStructInit(&timer_base);
  timer_base.TIM_Prescaler = 16000 - 1; /// MUST BE correct
  TIM_TimeBaseInit(TIM3, &timer_base);

  TIM_ICInitTypeDef timer_ic;
  timer_ic.TIM_Channel = TIM_Channel_2;
  timer_ic.TIM_ICPolarity = TIM_ICPolarity_Rising;
  timer_ic.TIM_ICSelection = TIM_ICSelection_DirectTI;
  timer_ic.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  timer_ic.TIM_ICFilter = 0;
  TIM_ICInit(TIM3, &timer_ic);

  
  TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
    
  NVIC_EnableIRQ(TIM3_IRQn);
  
  TIM_Cmd(TIM3, ENABLE);

}

void TIM3_IRQHandler(void) {
switch(fan_to_measure)
{
  case FANM1:
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
      
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

      
      capture1 = capture2;
      capture2 = TIM_GetCapture1(TIM3);
      
      if (!capture_is_first)
        capture_is_ready = 1;

      capture_is_first = 0;

      
      if (TIM_GetFlagStatus(TIM3, TIM_FLAG_CC1OF) != RESET)
      {
        TIM_ClearFlag(TIM3, TIM_FLAG_CC1OF);
        // Miss the pulse
      }
    }
  break;
  case FANM2:
    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {
      
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

      
      capture1 = capture2;
      capture2 = TIM_GetCapture4(TIM3);
      
      if (!capture_is_first)
        capture_is_ready = 2;

      capture_is_first = 0;

      
      if (TIM_GetFlagStatus(TIM3, TIM_FLAG_CC4OF) != RESET)
      {
        TIM_ClearFlag(TIM3, TIM_FLAG_CC4OF);
        // Miss the pulse
      }
    }
  break;
  case FANR:
    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {
      
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

      
      capture1 = capture2;
      capture2 = TIM_GetCapture2(TIM3);
            
      if (!capture_is_first){
        capture_is_ready = 3;}

      capture_is_first = 0;

      
      if (TIM_GetFlagStatus(TIM3, TIM_FLAG_CC2OF) != RESET)
      {
        TIM_ClearFlag(TIM3, TIM_FLAG_CC2OF);
        // Miss the pulse
      }
    }
  break;
  default:
  break;
  
}

}

void setFanToMeasure(uint8_t fanId) {

  TIM_DeInit(TIM3);
  NVIC_DisableIRQ(TIM3_IRQn);
  capture_is_first = 1;
  
  switch(fanId)
  {
    case FANM1:
      fan_to_measure = FANM1;
      TIM_Pulses_INIT_FANM1();
    break;
    case FANM2:
      fan_to_measure = FANM2; 
      TIM_Pulses_INIT_FANM2();
    break;
    case FANR:
      fan_to_measure = FANR;
      TIM_Pulses_INIT_FANR();
    break;
    case FANS_OFF:
       //
    break;
    default:
    break;
  }
  blocking_Delay(10);
  
}

uint8_t whitchFanIsMeasuring(void) {
  
  return fan_to_measure;
  
}

uint16_t uint16_t_time_diff(uint16_t now, uint16_t before)
{
  return (now >= before) ? (now - before) : (UINT16_MAX - before + now);
}

uint16_t Get_Period(void) {
  
  uint16_t bf = 0;
  
  switch(fan_to_measure)
  {
    case FANM1:
        if (capture_is_ready == 1)
        {
          NVIC_DisableIRQ(TIM3_IRQn);
          capture_is_ready = 0;
          bf = uint16_t_time_diff(capture2, capture1);
          NVIC_EnableIRQ(TIM3_IRQn);
        }
    break;
    case FANM2:
        if (capture_is_ready == 2)
        {
          NVIC_DisableIRQ(TIM3_IRQn);
          capture_is_ready = 0;
          bf = uint16_t_time_diff(capture2, capture1);
          NVIC_EnableIRQ(TIM3_IRQn);
        }      
    break;
    case FANR:
        if (capture_is_ready == 3)
        {
          NVIC_DisableIRQ(TIM3_IRQn);
          capture_is_ready = 0;
          bf = uint16_t_time_diff(capture2, capture1);
          NVIC_EnableIRQ(TIM3_IRQn);
        }        
    break;
    case FANS_OFF:
       //
    break;
    default:
    break;
  }
  
  
  return bf;

}
  
uint16_t Get_Pulse_Value (void) {

  return (1000/Get_Period());
  
}
  

/* ============================================================================
                     ##### SENM1 (PA6) INTERRUPTS #####
 ============================================================================*/
/*
void Reset_Counter(void) {
  
  pulse_counter = 0;

}

uint16_t Get_Pulse_Value(void) {

  return pulse_counter;

}


void EXTI_SENM1_Full_Init_FanSpeed(void) {

      // Configuring pin name SENM1(PA6)
      GPIO_EXTI_SENM1_Init();
      EXTI_DeInit();
      //EXTI_SENM1_Init();  In files
      NVIC_SENM1_Init();
  
}

void GPIO_EXTI_SENM1_Init(void) {

      // pin name SENM1
      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA, ENABLE);  
      
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
      
      GPIO_Init(GPIOA, &GPIO_InitStruct);      
  
}

void EXTI_SENM1_Init(void) {

      //RCC_APB1PeriphClockCmd(RCC_APB2Periph_EXTIT,ENABLE);
      
      EXTI_InitTypeDef EXTIInitStruct;
      EXTIInitStruct.EXTI_Line = EXTI_Line6;
      EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
      EXTIInitStruct.EXTI_LineCmd = ENABLE;
     
      EXTI_Init(&EXTIInitStruct);

}

void NVIC_SENM1_Init(void) {

      NVIC_InitTypeDef NVICInitSrtuct;
      NVICInitSrtuct.NVIC_IRQChannel = EXTI9_5_IRQn;
      NVICInitSrtuct.NVIC_IRQChannelPreemptionPriority = 0x0F;
      NVICInitSrtuct.NVIC_IRQChannelSubPriority = 0x0F;
      NVICInitSrtuct.NVIC_IRQChannelCmd = ENABLE;
  
      NVIC_Init(&NVICInitSrtuct);
}

void EXTI9_5_IRQHandler(void) {
  // Interrupt handler
  // Search name for inerrupts funcntions in startup file

  if(EXTI_GetITStatus(EXTI_Line6)) {
            
    EXTI_ClearITPendingBit(EXTI_Line6);
    pastTime = nowTime;
    nowTime = GetTimeMs();
    period = nowTime - pastTime;
    pulse_counter++;
  } 
  
}


uint32_t Get_Period(void) {
    return period;
}





*/











