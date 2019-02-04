#include "UART.h"

char Send_Buffer[127];

char Send_Buffer1[127];
char Send_Buffer2[127];

uint8_t flag_buf = 0;
uint8_t transmit_was_end = 1;
uint8_t refresh = 1;
uint16_t pointer1 = 0;
uint16_t pointer2 = 0;

uint16_t Send_Counter = 0;

/* ============================================================================
                            ##### Init func #####
 ============================================================================*/
void USART_Init_1(void) {
  
      // GPIO INIT
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
           
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
      
      GPIO_Init(GPIOA, &GPIO_InitStruct);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
      
      // USART
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      
      USART_InitTypeDef USART_InitStruct;
      USART_InitStruct.USART_BaudRate = 9600;
      USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
      USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
      USART_InitStruct.USART_Parity = USART_Parity_No; 
      USART_InitStruct.USART_StopBits = USART_StopBits_1; 
      USART_InitStruct.USART_WordLength = USART_WordLength_8b;
      
      USART_Init(USART1, &USART_InitStruct);
      
      // Interrupts
      // Global IT enable
      NVIC_EnableIRQ(USART1_IRQn);
      // Also need to enable interrupts for transmitting(in start transmit func)
      // Like this: USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
      
      
      USART_Cmd(USART1, ENABLE);
 
}

/* ============================================================================
                      ##### Simpliest transmit functions #####
 ============================================================================*/
//
// This Functions is really slow! Like blocking_delay
//

void USART1_Send_Data (int16_t data_USART) {

  USART_SendData(USART1, data_USART);
  
}

  // Before use this functions
  // (In main) Declare array: Str
  // use sprintf(Str, "TEXT") to convert
  // text into Str array (ASCII) and then call:
  // SendStrToPC(Str);

  // Remember - name of array == pointer to its first element

void SendStrToPC(char *str_ptr) {
  
  uint16_t i = 0;
  
  while(str_ptr[i] != 0) {
    USART_SendData(USART1, str_ptr[i]);
    // wait because UART is slow
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
    i++;
  } 
  
}

void SendAnyDataToPC(char *str_ptr, uint16_t count) {

  // count - number of symbols to send
  uint16_t i = 0;
  
  while(i < count) {
    USART_SendData(USART1, str_ptr[i]);
    // wait because UART is slow
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
    i++;
  } 
  

}


/* ============================================================================
                             ##### DMA #####
 ============================================================================*/

void DMA_USART_Init(void) {

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_InitTypeDef DMA_InitStruct;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)Send_Buffer;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;  
  DMA_InitStruct.DMA_BufferSize = sizeof(Send_Buffer); //
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; // DMA_Mode_Circular or DMA_Mode_Normal
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  
  DMA_Init(DMA1_Channel4, &DMA_InitStruct);
 
  // Global interrupts DMA1
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
  
  //DMA_Cmd(DMA1_Channel4, ENABLE); // if DMA_Mode_Circular
  
  // DMA waits for the USART(because USART is slow)
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
  
}

// How this function works:
// In main create            char Send_Buf[100];
// Then write to Send_Buf    sprintf(Send_Buf, "TEXT");
// At last                   DMA_printf(Send_Buf , 1 or 0);

void DMA_printf(char *str_ptr, uint8_t n, uint8_t count) {

  // Sizeof dont work
  uint16_t buf_size = count;
  
  for (uint8_t i=0; i<buf_size; i++){
    Send_Buffer[i] = str_ptr[i];
  }
  
  // Perenos stroki
  if (n == 1) {
    
    Send_Buffer[buf_size] = 0x0A; 
    DMA_Cmd(DMA1_Channel4, DISABLE);  
    DMA1_Channel4->CNDTR = (buf_size+1);
    DMA_Cmd(DMA1_Channel4, ENABLE);
  }
  else {

    DMA_Cmd(DMA1_Channel4, DISABLE);  
    DMA1_Channel4->CNDTR = (buf_size);
    DMA_Cmd(DMA1_Channel4, ENABLE);    
    
  }
  
}





void DMA1_Channel4_IRQHandler(void) {

  if(DMA_GetITStatus(DMA1_IT_TC4) == SET) {
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    // OBrabot4ik okon4aniya pereda4i
    transmit_was_end = 1; 
    refresh = 1;
    
  }
}

void add_to_buf(char *str_ptr, uint8_t numb) {

  uint16_t buf_size = numb;
 
  uint16_t pointer_save = 0;
  uint8_t i = 0;
  // The fist buf is free
  if(flag_buf) {
    pointer_save = pointer1 + buf_size;
    Send_Buffer1[pointer1] = str_ptr[i];
    while(pointer1 <= pointer_save){
        pointer1++;
        i++;
        Send_Buffer1[pointer1] = str_ptr[i];
    }
  }
  else {
    pointer_save = pointer2 + buf_size;
    Send_Buffer2[pointer2] = str_ptr[i];
    while(pointer2 <= pointer_save){
        pointer2++;
        i++;
        Send_Buffer2[pointer2] = str_ptr[i];
    }
    
  }
  
  if(refresh) {
    refresh = 0;
    send_buffer();
    // pointer to buffer indexes
    if(flag_buf) {pointer1 = 0;}
    else {pointer2 = 0;}
    
    // Changing the buffers roles
    flag_buf = 1 - flag_buf;

  }
 
}

void send_buffer(void) {
  
uint16_t buf_size = 0;
  
if (transmit_was_end) {
   if((flag_buf == 0) && (pointer2 > 0)) {  
      buf_size = pointer2; 
      for (uint8_t i=0; i<buf_size; i++){
          Send_Buffer[i] = Send_Buffer2[i];
      }    
       DMA_Cmd(DMA1_Channel4, DISABLE);  
       DMA1_Channel4->CNDTR = (buf_size);
       DMA_Cmd(DMA1_Channel4, ENABLE);  
   }
   else {
       if((flag_buf == 1) && (pointer1 > 0)) {
          buf_size = pointer1;
          for (uint8_t i=0; i<buf_size; i++){
              Send_Buffer[i] = Send_Buffer1[i];
          }  
          
           DMA_Cmd(DMA1_Channel4, DISABLE);  
           DMA1_Channel4->CNDTR = (buf_size);
           DMA_Cmd(DMA1_Channel4, ENABLE); 
       }
       else {}
   }
   
   transmit_was_end = 0;   
   
}

}

void test (uint8_t num) {

  pointer1 = num;
  flag_buf = 0;
  sprintf(Send_Buffer1, "Karim");
  
}


/* ============================================================================
                      ##### Transmit using interupts #####
 ============================================================================*/
//
// Transmit destroys previous one if it not finished yet
//

/*
void USART1_IRQHandler(void) {

  // This func calls at the end of receiving and transmitting
  // We should differ it
  
  // End of transmitting
  if(USART_GetITStatus(USART1, USART_IT_TXE) == SET) 
  {
    USART_ClearITPendingBit(USART1, USART_IT_TXE);
    if (Send_Buffer[Send_Counter] != 0) 
    {
      USART_SendData(USART1, Send_Buffer[Send_Counter]);
      Send_Counter++;
    } 
    else {
      Send_Counter = 0;
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    };
  }

}

void SendToPC_IT(char *str_ptr) {
  
  // We should create globall buffer for example (Send_Buffer) 
  // to send the data and global counter (Send_Counter)
  
  uint16_t buf_size = sizeof(str_ptr);
  
  for (uint8_t i=0; i<buf_size; i++){
    Send_Buffer[i] = str_ptr[i];
  }  
  
  // Initialize first transmit
  USART_SendData(USART1, Send_Buffer[Send_Counter]);
  Send_Counter++;
  
  // Interrupt by transmitting
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  
}

*/



