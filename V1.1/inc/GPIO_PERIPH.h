/**
  ******************************************************************************
  * @file    GPIO_PERIPH.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the GPIO peripheral:
  *            + LED control       (GPIO)
  *            + Power control     (GPIO)
  *            + Heater control    (GPIO)
  *            + Fan speed control (GPIO and ADC)   
  *              
**/

#ifndef GPIO_PERIPH_H
#define GPIO_PERIPH_H

#include "stm32l1xx.h"
#include "Timers.h"
#include "DAC.h"
#include "main.h"

void Power_Control(uint8_t pwr);
void Heater1_Control(uint8_t pwr);
void Heater2_Control(uint8_t pwr);

void FanR_Set(uint8_t state, int16_t setVlt);
void FanM_Set(uint8_t state, int16_t setVlt);

void EMERG_LED(uint8_t led);
void NORM_LED(uint8_t led);

void GREEN_LED (uint8_t ledG);
void RED_LED (uint8_t ledR);
void ORANGE_LED (uint8_t ledO);

uint8_t Read_PWRGD1();
uint8_t Read_PWRGD2();
uint8_t Read_PWRGD3();



#endif