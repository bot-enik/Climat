/**
  ******************************************************************************
  * @file    SpeedCounter.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to count the Fans speed continiously 
  *            + Counts continiously (counts pulses per time)
  *            + Uses in Feedback loop  
  *            
  *              
**/

#ifndef SPEEDCOUNTER_H
#define SPEEDCOUNTER_H

#include "stm32l1xx.h"
#include "main.h"
#include "Fan_Speed.h"

#define START_SC       (0xF0)
#define CHECK_FANM1    (0x00)
#define CHECK_FANM2    (0x0F)
#define CHECK_FANR     (0x60)
#define CONCLUSION     (0x06)

void Speed_Count_FSM (struct forward_data_str_ADC *ADC_ptr,
                      uint16_t *fans_state, uint16_t minimum_rev,
                      uint16_t maximum_rev,
                      uint8_t *good_Fan_p, uint8_t *feedback_state_p, 
                      uint8_t *curent_state_p, uint8_t *error_type_p,
                      struct forward_data_str_Speed *ptrSpeeds);

#endif
