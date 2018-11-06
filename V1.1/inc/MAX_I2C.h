/**
  ******************************************************************************
  * @file    MAX_I2C.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the MAX31725-31726 peripheral:
  *            + Read the temperature 
  *            
  *            
  *              
**/

#ifndef MAX_I2C_h
#define MAX_I2C_h

#include "stm32l1xx.h"
#include "I2C.h"
#include "main.h"


// MAX registers
#define TEMP_REG_MAX 0x00

// MAX interfaces
float I2CReadTempMAXAcc(uint8_t address, I2C_TypeDef* I2Cx);
int16_t I2CReadTempMAX(uint8_t address, I2C_TypeDef* I2Cx);

#endif