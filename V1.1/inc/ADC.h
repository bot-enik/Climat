/**
  ******************************************************************************
  * @file    ADC.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the ADC peripheral:
  *            + 4 channel ADC init and control
  *              
**/

#ifndef ADC_H
#define ADC_H

#include "stm32l1xx.h"
#include "main.h"

// Initialize data struct for ADC values
typedef struct forward_data_str_ADC
{
      uint16_t power_in_VLT, fanR_VLT, fanm1_VLT, fanm2_VLT;
      
}data_str_ADC;


// Initialize
void FOUR_CH_ADC_Init(void);
//void ONE_CH_ADC_Init(void);

// Core function 
void ADC_Start_4Ch_Inj_Conv(void);
void ADC_Read_4Ch(data_str_ADC *data_ptr);

#endif