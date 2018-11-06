/**
  ******************************************************************************
  * @file    LED_Control.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the LEDs:
  *            + Control indication of the system state
  *            
  *            
  *              
**/

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "stm32l1xx.h"
#include "main.h"
#include "GPIO_PERIPH.h"

#define LS_First               (0x00)
#define LS_Second              (0xFF)
#define LS_Third               (0x0F)



void Led_Control(uint8_t lc_current_state, uint16_t error_type_o, uint16_t *fansStatetment, float temperature);

#endif