/**
  ******************************************************************************
  * @file    Messages.h
  * @author  Sayfullin K.R.
  * @version V1.0
  * @date    29-April-2018
  * @brief   This file provides functions to manage the following 
  *          functionalities of the VIRTUAL MESSAGES:
  *            
  *            
  *              
**/

#ifndef MESSAGES_H
#define MESSAGES_H

#include "main.h"

/* ----------------------------- Settings ----------------------------- */

#include "stm32l1xx.h"

#define MAX_MESSAGES (8)

#define MSG_GV_BGN_FSM_ACTIVATE    (1)
#define MSG_Analyzis_FSM_ACTIVATE  (2)
#define MSG_FBC_FSM_ACTIVATE       (3)
#define MSG_SC_FSM_ACTIVATE        (4)
#define MSG_GV_START_SPEED         (5)

/* -------------------------- End of settings -------------------------- */

void InitMessages(void);

void SendMessage(uint8_t Msg);
void SendBroadcastMessage(uint8_t Msg);

void ProcessMessages(void);
void ProcessWatchDog(void);

uint8_t GetMessage(uint8_t Msg);
uint8_t BroadcastMessage(uint8_t Msg);

uint8_t ReadMessage(uint8_t Msg);
uint8_t ReadBroadcastMessage(uint8_t Msg);
void ShowAllMessages(void);

#endif
