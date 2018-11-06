/**
  ******************************************************************************
  * @file    DAC.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the DAC peripheral:
  *            + 2 channel DAC init
  *            + control of each channel
  *              
**/

#ifndef DAC_H
#define DAC_H

#include "stm32l1xx.h"


void TWO_DAC_Init(void);

void DAC_VREG_M_Set(uint16_t val);
void DAC_VREG_R_Set(uint16_t val);

#endif