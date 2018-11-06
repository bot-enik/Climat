#include "MAX_I2C.h"

uint32_t I2C_Timeout_MAX = 0;

/* ============================================================================
                            ##### MAX interfaces #####
 ============================================================================*/

float I2CReadTempMAXAcc(uint8_t address, I2C_TypeDef* I2Cx) {
    // Accuracy 0.00390625 C
    // Can change to lower if needed
    // But return type becomes int16_t 
    uint16_t data = 0;
    float temp = 0;

    if(I2Cx == I2C1) {Set_Sensor(MAX_I2C1);}
    if(I2Cx == I2C2) {Set_Sensor(MAX_I2C2);}
    
    if(I2C_State(I2Cx) == 1) 
    {
      
        I2CStart(address, I2CTransmitter, I2CAckEnable, I2Cx);
        I2C_Timeout_MAX = MEDIUM_TIMEOUT;
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
          if((I2C_Timeout_MAX--) == 0) {
            I2C_Report(I2Cx);
            break;
          }
        } 
        
        I2CWriteData(TEMP_REG_MAX, I2Cx);
        I2CStart(address, I2CReciever, I2CAckDisable, I2Cx);
        I2C_Timeout_MAX = MEDIUM_TIMEOUT;
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
          if((I2C_Timeout_MAX--) == 0) {
            I2C_Report(I2Cx);
            break;
          }
        }
        
        data |= (I2CReadAck(I2Cx) << 8);
        //printf("data 1 = %d \n",data);
        data |= I2CReadNAck(I2Cx);
        //printf("data 2 = %d \n",data);
        
          
        if ((data & (1<<15))) {
          temp = (float)(32768 - data)*0.00390625;
          //printf("tempNegative  = %f \n",temp);
        }
        else {
          temp = (float)data*0.00390625; // Max accuracy 
          //printf("tempPositive  = %f \n",temp);
        }
    }   
    return temp;
}

int16_t I2CReadTempMAX(uint8_t address, I2C_TypeDef* I2Cx){
    uint16_t data = 0;
    int16_t temp = 0;

    if(I2Cx == I2C1) {Set_Sensor(MAX_I2C1);}
    if(I2Cx == I2C2) {Set_Sensor(MAX_I2C2);}    
    
    if(I2C_State(I2Cx) == 1) 
    {
       
      I2CStart(address, I2CTransmitter, I2CAckEnable, I2Cx);
      I2C_Timeout_MAX = MEDIUM_TIMEOUT;
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if((I2C_Timeout_MAX--) == 0) {
          I2C_Report(I2Cx);
          break;
        }
      }  
      
      I2CWriteData(TEMP_REG_MAX, I2Cx);
      I2CStart(address, I2CReciever, I2CAckDisable, I2Cx);
      I2C_Timeout_MAX = MEDIUM_TIMEOUT;
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
        if((I2C_Timeout_MAX--) == 0) {
          I2C_Report(I2Cx);
          break;
        }
      }
      
      data |= (I2CReadAck(I2Cx) << 8);
      printf("data 1 = %d \n",data);
      data |= I2CReadNAck(I2Cx);
      printf("data 2 = %d \n",data);
      
        
      if ((data & (1<<15))) {
        temp = ((32768 - data) >> 8);
        printf("tempNegative  = %d \n",temp);
      }
      else {
        temp = (data >> 8); //Accuracy 1 C
        printf("tempPositive  = %d \n",temp);
      }
    }
    return temp;
}
