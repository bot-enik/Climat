#include "Messages.h"

// Sending messages(flags)
// to other FSMs, to control them.
// There are two types:
// Messages - regular for one user
// BroadcastMessages - for many users
// Flags: 0 - disable
//        1 - enable, but inactive(will be 2 at next iteration)
//        2 - enable and active
// ProcessMesages changes the flags of the message after iteration
// Also controls if there any active message

uint8_t Messages[MAX_MESSAGES];
uint8_t BroadcastMessages[MAX_MESSAGES];

uint8_t noMsgCounter, AnyActiveMessage;

void InitMessages(void) {

  uint8_t i;
  
  for (i=0; i<MAX_MESSAGES; i++)
    Messages[i] = 0;
  for (i=0; i<MAX_MESSAGES; i++)
    BroadcastMessages[i] = 0;
  
  noMsgCounter = 0;

}

/* ============================================================================
                    ##### PROCESS message functions #####
 ============================================================================*/

void ProcessMessages(void) {
  
  uint8_t i;
  uint8_t flag = 0;
  
  for (i=0; i<MAX_MESSAGES; i++) {
    if (Messages[i] >= 2) {
      Messages[i] = 0;
      flag = 1;
    }
    if (Messages[i] == 1) {
      Messages[i] = 2;
      flag = 1;
    }
  }
  for (i=0; i<MAX_MESSAGES; i++) {
    if (BroadcastMessages[i] == 2) {
      BroadcastMessages[i] = 0;
      flag = 1;
    }
    if (BroadcastMessages[i] == 1) {
      BroadcastMessages[i] = 2;
      flag = 1;
    }
  }
  
  // If there was any active messages then return 1
  if (flag) AnyActiveMessage = 1;
  else AnyActiveMessage = 0;
}

void ProcessWatchDog(void) {

  // If 10 times loop goes without any active messages
  // It seems that some messages was lost
  // If so, this function start Convertion again
  
  if (AnyActiveMessage == 0) {
    
    noMsgCounter++;
    if (noMsgCounter >= 10) {SendMessage(MSG_GV_BGN_FSM_ACTIVATE);}
    
  }
  else noMsgCounter = 0;
  
}

/* ============================================================================
                    ##### SEND  message functions #####
 ============================================================================*/

void SendMessage(uint8_t Msg) {

  if((Messages[Msg] == 0) || ((Messages[Msg] > 2))) Messages[Msg] = 1;
  
}

void SendBroadcastMessage(uint8_t Msg) {

  if(BroadcastMessages[Msg] == 0) BroadcastMessages[Msg] = 1;
  
}

/* ============================================================================
                    ##### GET  message functions #####
 ============================================================================*/

uint8_t GetMessage(uint8_t Msg) {

  if(Messages[Msg] == 2) {
  
    Messages[Msg] = 0;
    return 1;
    
  };
  
  return 0;
}

uint8_t GetBroadcastMessage(uint8_t Msg) {

  if(BroadcastMessages[Msg] == 2) return 1;
    
  return 0;
  
}


/* ============================================================================
                            ##### Debug functions #####
 ============================================================================*/

uint8_t ReadMessage(uint8_t Msg) {

  return Messages[Msg];

}

uint8_t ReadBroadcastMessage(uint8_t Msg) {

  return BroadcastMessages[Msg];

}

void ShowAllMessages(void) {

  for(uint8_t i=0; i<MAX_MESSAGES; i++) {
  
    if (Messages[i] != 0){
      printf("Msg = %d, val = %d\n", i, Messages[i] );
    }
    if (BroadcastMessages[i] != 0) {
      printf("BroadMsg = %d, value = %d\n", i, BroadcastMessages[i] );
    }
    
  }
  
}

/*
      sprintf(Send_Buf_MSG, " Message ");
      add_to_buf(Send_Buf_MSG,9);
      sprintf(Send_Buf_MSG, "%d",i);
      add_to_buf(Send_Buf_MSG,1);
      sprintf(Send_Buf_MSG, " value ");
      add_to_buf(Send_Buf_MSG,9);
      sprintf(Send_Buf_MSG, "%d",Messages[i]);
      add_to_buf(Send_Buf_MSG,1);

//*/









