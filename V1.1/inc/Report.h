/**
  ******************************************************************************
  * @file    Report.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to send the report via USART
  *            
  *            
  *              
**/


#ifndef REPORT_H
#define REPORT_H

#include "stm32l1xx.h"
#include "main.h"
#include "UART.h"

#define BYTE_TRANSMIT       (0x00)
#define TEMP_1_TRANSMIT     (0xF0)
#define TEMP_2_TRANSMIT     (0x0F)
#define TEMP_3_TRANSMIT     (0xFF)
#define HUM_TRANSMIT        (0x75)
#define ERROR_TYPE_TRANSMIT (0x34)
#define GPIO_TRANSMIT       (0x54)
#define TRANSMIT_END        (0x67)



void Send_Report(uint8_t cur_statement, uint16_t *fans,
                 uint8_t error_type_rep,
                 struct forward_data_str_ADC *ptrADC, 
                 struct forward_data_str_Temp *ptrTempHum);

void Send_Temp(float temp);





#endif