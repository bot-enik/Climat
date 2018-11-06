#include "I2C.h"

uint32_t I2C_Timeout = 0;

uint16_t povtor_count = 0;

uint8_t I2C1_Good = 1, I2C2_Good = 1, I2C1_Good_HDC = 1, I2C1_Good_MAX = 1;

uint8_t now_param = 0;

/* ============================================================================
                            ##### Initialize #####
 ============================================================================*/

void I2CInit(void) {

  I2C1_Init();
  I2C2_Init();
 
}


void I2C1_Init(void) {
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);  
    
    GPIO_InitTypeDef  GPIOInitStruct;
    GPIOInitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIOInitStruct.GPIO_OType = GPIO_OType_OD;
    GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIOInitStruct);
    
    I2C_InitTypeDef I2CInitStruct;
    I2CInitStruct.I2C_ClockSpeed = 100000;
    I2CInitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2CInitStruct.I2C_Mode = I2C_Mode_I2C;
    I2CInitStruct.I2C_OwnAddress1 = 0x11; //random
    I2CInitStruct.I2C_Ack = I2C_Ack_Disable;
    I2CInitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    
    I2C_Init(I2C1, &I2CInitStruct);
    
    I2C_Cmd(I2C1, ENABLE);    
}

void I2C2_Init(void) {
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);
 
    GPIO_InitTypeDef  GPIOInitStruct;
    GPIOInitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIOInitStruct.GPIO_OType = GPIO_OType_OD;
    GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIOInitStruct);

    I2C_InitTypeDef I2CInitStruct;
    I2CInitStruct.I2C_ClockSpeed = 100000;
    I2CInitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2CInitStruct.I2C_Mode = I2C_Mode_I2C;
    I2CInitStruct.I2C_OwnAddress1 = 0x11; //random
    I2CInitStruct.I2C_Ack = I2C_Ack_Disable;
    I2CInitStruct.I2C_DutyCycle = I2C_DutyCycle_2;

    I2C_Init(I2C2, &I2CInitStruct);  

    I2C_Cmd(I2C2, ENABLE);

}

/* ============================================================================
                            ##### Core functions #####
 ============================================================================*/

void I2CStart(uint8_t address, uint8_t direction, uint8_t ack, I2C_TypeDef* I2Cx) {
 
    if(I2C_State(I2Cx) == 1) 
    {
      I2C_AcknowledgeConfig(I2Cx, ENABLE);
      I2C_GenerateSTART(I2Cx, ENABLE);
      I2C_Timeout = MEDIUM_TIMEOUT;
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
        if((I2C_Timeout--) == 0) {
          I2C_Report(I2Cx);
          break;
        }
      }
      I2C_Send7bitAddress(I2Cx, address, direction);   
    }
}

void I2CStop(I2C_TypeDef* I2Cx) {
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

uint8_t I2CReadAck(I2C_TypeDef* I2Cx) {
  
    uint8_t data = 0;
    if(I2C_State(I2Cx) == 1) 
    {
      I2C_AcknowledgeConfig(I2Cx, ENABLE);
      I2C_Timeout = MEDIUM_TIMEOUT;
      while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) ){
        if((I2C_Timeout--) == 0) {
          I2C_Report(I2Cx);
          break;
        }
      }
      data = I2C_ReceiveData(I2Cx);
    }
    return data;
    
}

uint8_t I2CReadNAck(I2C_TypeDef* I2Cx) {
  
    uint8_t data = 0;
    if(I2C_State(I2Cx) == 1) 
    {
      I2C_AcknowledgeConfig(I2Cx, DISABLE);
      I2C_Timeout = MEDIUM_TIMEOUT;
      while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) ){
        if((I2C_Timeout--) == 0) {
          I2C_Report(I2Cx);
          break;
        }
      }
      data = I2C_ReceiveData(I2Cx);
      I2C_GenerateSTOP(I2Cx, ENABLE);
    }
    return data;
    
}

void I2CWriteData(uint8_t data, I2C_TypeDef* I2Cx) {
  
    if(I2C_State(I2Cx) == 1) 
    { 
      I2C_SendData(I2Cx, data);
      I2C_Timeout = MEDIUM_TIMEOUT;
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
        if((I2C_Timeout--) == 0) {
          I2C_Report(I2Cx);
          break;
        }
      }
    }
}



/* ============================================================================
                            ##### Support functions #####
 ============================================================================*/



void I2C_Report(I2C_TypeDef* I2Cx) {

  if(I2Cx == I2C1) {
    I2C1_Good = 0;
    if(now_param == MAX_I2C1) {I2C1_Good_MAX = 0;}
    if(now_param == HDC_I2C1) {I2C1_Good_HDC = 0;}
    //printf("1\n");
  }
  if(I2Cx == I2C2) {
    I2C2_Good = 0; 
    //printf("2\n");
  }
  
}

void Reload_I2C(void) {
  
  if (povtor_count >= RELOAD_COUNT) {  
    
    povtor_count = 0;
    
    if(I2C1_Good == 0) {
      I2C_DeInit(I2C1);
      I2C1_Init();
      I2C1_Good = 1;
      I2C1_Good_MAX = 1;
      I2C1_Good_HDC = 1;
      
    }
    if(I2C2_Good == 0) {
      I2C_DeInit(I2C2);
      I2C2_Init();
      I2C2_Good = 1;
    }
  }
  else {povtor_count++;}
}

uint8_t I2C_State(I2C_TypeDef* I2Cx) {

  if(I2Cx == I2C1) {
    if(now_param == MAX_I2C1) {return I2C1_Good_MAX;}
    if(now_param == HDC_I2C1) {return I2C1_Good_HDC;}
  }
  if(I2Cx == I2C2) {
    return I2C2_Good;
  } 
  return 1;
  
}

void Set_Sensor(uint8_t sensor) {

  now_param = sensor;

}

uint8_t I2C_State_Sensor(uint8_t sens) {

  if(sens == MAX_I2C1) {return I2C1_Good_MAX;}
  if(sens == HDC_I2C1) {return I2C1_Good_HDC;}
  if(sens == MAX_I2C2) {return I2C2_Good;}
  else {return 0;}
  
}







