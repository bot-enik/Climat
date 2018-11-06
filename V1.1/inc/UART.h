/**
  ******************************************************************************
  * @file    UART.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the UART peripheral:
  *          + Initialize USART
  *          + Transmit function for simple transmit (blocking other processes)
  *          + Transmit function using interrupts (destroying previous message if it not transmit yet)
  *          + Transmit function using DMA (destroying previous message if it not transmit yet)
  *          + Transmit function using DMA (send all messages)
  *            
  *              
**/


#ifndef UART_H
#define UART_H

#include "stm32l1xx.h"
#include "main.h"

// Init functions
void USART_Init_1(void);
void DMA_USART_Init(void);

// Transmit function for simple transmit
void USART1_Send_Data (int16_t data_USART);
void SendStrToPC(char *str_ptr);
void SendAnyDataToPC(char *str_ptr, uint16_t count);

// Transmit function using interrupts
void USART1_IRQHandler(void);
void SendToPC_IT(char *str_ptr);

// Transmit function using DMA
void DMA_printf(char *str_ptr, uint8_t n, uint8_t count);

// Transmit function using DMA (good one)
void DMA1_Channel4_IRQHandler(void);
void send_buffer(void);
void add_to_buf(char *str_ptr, uint8_t numb);

#endif