/**
  ******************************************************************************
  * @file    Fan_Control.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to control Fans 
  *            + Depend of state
  *            + Uses hysteresis
  *            + Calculates the revolution
  *            + Control fans by simple functions
  *            + Calculates the ADC voltage
  *            + Uses Values from Fan_Test() (main.c)            
  *            
  *              
**/

#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

#include "stm32l1xx.h"
#include "main.h"
#include "GPIO_PERIPH.h"
#include "Timers.h"
#include "Fan_Speed.h"

#define HYST_HIGH      (0xFF)
#define HYST_PROM      (0x0F)
#define HYST_MEDIUM    (0xF0)
#define HYST_LOW       (0x00) 

#define LOCAL_1        (0xFF)   
#define LOCAL_2        (0x00)



void Fans_Control(uint8_t current_state, 
                  uint16_t MIN_REV, uint16_t MAX_REV, 
                  float temp_FC, uint32_t rev_real, uint16_t *fans_statement);


#endif