#include "HDC_I2C.h"

uint32_t I2C_Timeout_HDC = 0;

/* ============================================================================
                            ##### HDC interfaces #####
 ============================================================================*/

void I2CHDCInit(uint8_t address, I2C_TypeDef* I2Cx) {

    Set_Sensor(HDC_I2C1);
    if(I2C_State(I2Cx) == 1) 
    {  
      I2CStart(address, I2CTransmitter, I2CAckEnable, I2Cx);
      I2C_Timeout_HDC = MEDIUM_TIMEOUT;
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if((I2C_Timeout_HDC--) == 0) {
              I2C_Report(I2Cx);
              printf("error\n");
              break;
            }
      }    
      I2CWriteData(CONF_REG_HDC, I2Cx);
      I2CWriteData(MSB_HDC_Settings_temp, I2Cx);
      I2CWriteData(LSB_HDC_Settings_temp, I2Cx);
      I2CStop(I2Cx);
    }
}

void I2CStartConversionTempHumidityHDC (uint8_t address, I2C_TypeDef* I2Cx) {
    
    Set_Sensor(HDC_I2C1);
    //if(I2C_State(I2Cx) == 1) 
    //{
      // Triggering the measurement
      I2CStart(address, I2CTransmitter, I2CAckEnable, I2Cx);
      I2C_Timeout_HDC = MEDIUM_TIMEOUT;
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if((I2C_Timeout_HDC--) == 0) {
              I2C_Report(I2Cx);
              break;
            }
      }    
      I2CWriteData(TEMP_REG_HDC, I2Cx);
      
      // Must wait for convertion
    //}
}

void I2CReadTempHumidityHDC(uint8_t address, I2C_TypeDef* I2Cx, data_str_Temp *ptrHDC) {
    
    uint16_t temp_data = 0;
    uint16_t humidity_data = 0; 
    
    Set_Sensor(HDC_I2C1);
    if(I2C_State(I2Cx) == 1) 
    {
      // Start reading temp and humidity
      I2CStart(address, I2CReciever, I2CAckDisable, I2Cx);
      I2C_Timeout_HDC = MEDIUM_TIMEOUT;
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
      {
        if((I2C_Timeout_HDC--) == 0) {
              I2C_Report(I2Cx);
              break;
            }
      }    
      
      temp_data |= (I2CReadAck(I2Cx) << 8);
      temp_data |= I2CReadAck(I2Cx);
      humidity_data |= (I2CReadAck(I2Cx) << 8);
      //printf("h1 = %x \n",humidity_data);
      humidity_data |= I2CReadNAck(I2Cx);
      //printf("h2 = %x \n",humidity_data);
      
      // Control
      /*
      printf("temp_data 1 = %d \n",temp_data);
      printf("temp_data 2 = %d \n",temp_data);
      printf("humidity_data 1 = %d \n",humidity_data);
      printf("humidity_data 2 = %d \n",humidity_data);
      */
    }
    // Convert values
    //ptrHDC->temp_HDC = (((temp_data >> 2)*165));
    ptrHDC->temp_HDC = (((float)temp_data / 65536)*165 - 40); // Celcius
    ptrHDC->humidity_HDC = ((float)humidity_data / 65536)*100; // Percents
    
}

