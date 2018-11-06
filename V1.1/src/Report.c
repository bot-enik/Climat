#include "Report.h"

uint8_t Report_State, flag=0, transmit, change_count = 0;
int8_t  byte_previous = 0;
uint32_t Delay_To_Report,transmit_delay;

void Send_Report(uint8_t cur_statement, uint16_t *fans,
                 uint8_t error_type_rep,
                 struct forward_data_str_ADC *ptrADC, 
                 struct forward_data_str_Temp *ptrTempHum) {

  int8_t byte = 0;
  uint8_t gpio_type_rep = 0;
  uint16_t FanM1_voltage, FanM2_voltage, FanR_voltage, Calc_voltage;
  
      switch (Report_State)
      {
        case ON:
          
           Delay_To_Report = MainTimerSet(1000);  
           Report_State = OFF;
          
        break;
        case OFF:
          
          gpio_type_rep = 0;
          if ((uint8_t)GPIO_ReadInputDataBit(GPIOB, PWRGD1) == 1) {
              gpio_type_rep |= (0x1U << 0);
          }
          if ((uint8_t)GPIO_ReadInputDataBit(GPIOB, PWRGD2) == 1) {
              gpio_type_rep |= (0x1U << 2);
          }
          if ((uint8_t)GPIO_ReadInputDataBit(GPIOC, PWRGD3) == 1) {
              gpio_type_rep |= (0x1U << 4);
          }
          
          if(!MainTimerIsExpired(Delay_To_Report)) break;
          Report_State = ON;
          
          FanM1_voltage = ptrADC->fanm1_VLT;
          FanM2_voltage = ptrADC->fanm2_VLT;
          FanR_voltage  = ptrADC->fanR_VLT;
          Calc_voltage  = fans[3];
          
          // First two bits
          // 
          switch(cur_statement) 
          {
            case EMERGENCY:           // msb 11 lsb
              byte |= (0x1U << 6);
              byte |= (0x1U << 7);
            break;
            case HEATING_AND_DRYING:  // 10
              byte = 0;
              byte |= (0x1U << 7);
            break;
            case NORMAL:              // 00
              byte = 0;
            break;                 
          }
          // If two or more fans is dead at the beginning of the test, then
          // set 01 to first two bits
          if(fans[7] == 1) {
            byte = 0;
            byte |= (0x1U << 6);
          }
          
          
          // Did we set the fans on?
          // EN signals
          if (fans[4] == 1) {
              byte |= (0x1U << 0);
          }
          if (fans[5] == 1) {
              byte |= (0x1U << 2);
          }
          if (fans[6] == 1) {
              byte |= (0x1U << 4);
          }
          
          // Check voltages + rotating
          // AlARM signal
          if ((((Calc_voltage - FanM1_voltage) > 400) || ((Calc_voltage - FanM1_voltage) < -400))
              && (fans[4] == 1)) {
              byte |= (0x1U << 1);
          }
          if ((((Calc_voltage - FanM2_voltage) > 400) || ((Calc_voltage - FanM2_voltage) < -400))
              && (fans[5] == 1)) {
              byte |= (0x1U << 3);
          }          
          if ((((Calc_voltage - FanR_voltage) > 400) || ((Calc_voltage - FanR_voltage) < -400))
              && (fans[6] == 1)) {
              byte |= (0x1U << 5);
          }
          // If fan doesnt work
          if (fans[0] == 1) {
              byte |= (0x1U << 1);
          }
          if (fans[1] == 1) {
              byte |= (0x1U << 3);
          }
          if (fans[2] == 1) {
              byte |= (0x1U << 5);
          }
          
          // byte:
          // STT - state FxAL - fan alarm FxEN - fan enable
          // |STT|STT|FRAL|FREN|F2AL|F2EN|F1AL|F1EN|
          //  MSB                              LSB
          
          // if value of byte changes wait some iterations
          if (byte != byte_previous)
          {
            change_count++;
            if (change_count >= 3) {
              change_count = 0;
            }
            else {
              byte = byte_previous;
            }
          }
          byte_previous = byte;
          
          transmit_delay = MainTimerSet(40);
          flag = 0;
          transmit = BYTE_TRANSMIT;
          while(!((MainTimerIsExpired(transmit_delay)) || (flag)) ) {
           
            switch (transmit)
            {
              case BYTE_TRANSMIT:
                if(!USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
                {
                  transmit = TEMP_1_TRANSMIT;
                  USART_SendData(USART1, byte);
                }
              break;
              case TEMP_1_TRANSMIT:
                if(!USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
                {
                  transmit = TEMP_2_TRANSMIT;
                  Send_Temp(ptrTempHum->temp_HDC);
                }
              break;
              case TEMP_2_TRANSMIT:
                if(!USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
                {
                  transmit = TEMP_3_TRANSMIT;
                  Send_Temp(ptrTempHum->temp_MAX1);
                }
              break;
              case TEMP_3_TRANSMIT:
                if(!USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
                {
                  transmit = HUM_TRANSMIT;    
                  Send_Temp(ptrTempHum->temp_MAX2);
                  flag = 1;
                  transmit = TRANSMIT_END;   
                }
              break;
              default:
                transmit = TRANSMIT_END;
                flag = 1;
              break;
            }  
          }
    
          //printf("byte = %x\n", byte); 
          byte = 0;     
      }
}

void Send_Temp(float temp) {
  
  int8_t byte = 0;
  
  // Convert temp from float to byte
  byte = (int8_t)temp;
  
  USART_SendData(USART1, byte);
  // printf("%d\n", byte);
  
}

