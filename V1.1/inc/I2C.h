/**
  ******************************************************************************
  * @file    I2C.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the I2C peripheral:
  *            + I2C init
  *            + Core functions for reseive and transmit
  *              
**/

#ifndef I2C_H
#define I2C_H

#include "stm32l1xx.h"
#include "main.h"

// STM registers
#define I2CTransmitter       0
#define I2CReciever          1
#define I2CAckEnable         1
#define I2CAckDisable        0

#define LONG_TIMEOUT   (4000000)
#define MEDIUM_TIMEOUT (300000)

#define RELOAD_COUNT   (1000)

#define MAX_I2C1       (0xFF)
#define MAX_I2C2       (0x00)
#define HDC_I2C1       (0x0F)


// Initialize
void I2C1_Init(void);
void I2C2_Init(void);
void I2CInit(void);

// Core functions
void I2CStart(uint8_t address, uint8_t direction, uint8_t ack, I2C_TypeDef* I2Cx);
void I2CStop(I2C_TypeDef* I2Cx);
void I2CWriteData(uint8_t data, I2C_TypeDef* I2Cx);
uint8_t I2CReadAck(I2C_TypeDef* I2Cx);
uint8_t I2CReadNAck(I2C_TypeDef* I2Cx);

// Support functions
void I2C_Report(I2C_TypeDef* I2Cx);
void Reload_I2C(void);
uint8_t I2C_State(I2C_TypeDef* I2Cx);
void Set_Sensor(uint8_t sensor);
uint8_t I2C_State_Sensor(uint8_t sens);


#endif