/**
  ******************************************************************************
  * @file    Fan_Speed.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to count the Fans speed 
  *            + Uses external interrupts
  *            + OR using timers (input capture mode)       
  *            
  *              
**/

#ifndef FAN_SPEED_H
#define FAN_SPEED_H

#include "main.h" 
#include "stm32l1xx.h"
#include "Timers.h"

// Initializing data structures
typedef struct forward_data_str_Speed
{
      uint32_t fanM1_Speed, fanM2_Speed, fanR_Speed;
}data_str_Speed;

#define FANM1      (0x00)
#define FANM2      (0x0F)
#define FANR       (0xFF)
#define FANS_OFF   (0xF0)

/// Using timers
void TIM_Pulses_INIT_FANM1(void);
void TIM_Pulses_INIT_FANM2(void);
void TIM_Pulses_INIT_FANR(void);

void TIM3_IRQHandler(void);
void setFanToMeasure(uint8_t fanId);
uint8_t whitchFanIsMeasuring(void);

uint16_t uint16_t_time_diff(uint16_t now, uint16_t before);
uint16_t Get_Period(void);
uint16_t Get_Pulse_Value(void);


/*
/// Using external interrupts
void EXTI_SENM1_Full_Init_FanSpeed(void);
void GPIO_EXTI_SENM1_Init(void);
void EXTI_SENM1_Init(void);
void NVIC_SENM1_Init(void);
void EXTI9_5_IRQHandler(void);


void Reset_Counter(void);
uint16_t Get_Pulse_Value(void);

uint32_t Get_Period(void);
*/



#endif