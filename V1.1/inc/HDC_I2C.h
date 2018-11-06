/**
  ******************************************************************************
  * @file    HDC_I2C.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the HDC1080 peripheral:
  *            + Read the temperature and humidity 
  *            
  *            
  *              
**/

#ifndef HDC_I2C_h
#define HDC_I2C_h

#include "stm32l1xx.h"
#include "I2C.h"
#include "main.h"
#include "Timers.h"

// HDC registers
#define TEMP_REG_HDC          0x00
#define HUMIDITY_REG_HDC      0x01
#define CONF_REG_HDC          0x02

// Low accuracy(temp 11b, humi 8b) temp included
#define MSB_HDC_Settings_temp 0x16
#define LSB_HDC_Settings_temp 0x00

#define HUM_ONLY              0x00

// 

// Initializing data structure for temp and humidity
// struct forward_data_str_Temp;

typedef struct forward_data_str_Temp
{
      float temp_HDC, humidity_HDC, temp_MAX1, temp_MAX2, temp_Itog;
}data_str_Temp;


// HDC interfaces
void I2CHDCInit(uint8_t address, I2C_TypeDef* I2Cx);
void I2CStartConversionTempHumidityHDC (uint8_t address, I2C_TypeDef* I2Cx);
void I2CReadTempHumidityHDC(uint8_t address, I2C_TypeDef* I2Cx, data_str_Temp *ptr);
void readHum (uint8_t address, I2C_TypeDef* I2Cx, data_str_Temp *ptrHDC);


#endif