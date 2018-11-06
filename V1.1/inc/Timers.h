/**
  ******************************************************************************
  * @file    Timers.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the VIRTUAL TIMERS:
  *            
  *            
  *              
**/

#ifndef TIMERS_H
#define TIMERS_H

#include "main.h"

/* ----------------------------- Settings ----------------------------- */

#include "stm32l1xx.h"

#define SEC  (1000)
#define MIN  (60*SEC)
#define HOUR (60*MIN)

#define MAX_TIMERS (3)

#define GET_VAL_CONVERTION   0
#define SPEED_CONVERTION     1
#define DOP_TIMER            2

/* -------------------------- End of settings -------------------------- */

void SysTick_Handler(void);

void MainTimerInit(void);
uint32_t GetTimeMs(void);
uint32_t MainTimerSet(uint32_t dop_time);
uint8_t MainTimerIsExpired(uint32_t Timer);
uint32_t MainTimerRemainingMs(uint32_t Timer);
void blocking_Delay(uint32_t delay_time);

void TimersInit(void);
uint32_t GetTimer(uint8_t Timer);
void ResetTimer(uint8_t Timer);

#endif