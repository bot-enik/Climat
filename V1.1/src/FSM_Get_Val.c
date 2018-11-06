#include "FSM_Get_Val.h"

volatile uint8_t FSM_GV_state = DISABLE_GV;

uint16_t zero_counter = 0, period_previous = 0;

uint32_t Delay_For_Conversion;

char Send_Buf_GV[127];

void Get_Val_Process_FSM(struct forward_data_str_Temp *ptrTempHum,
                         struct forward_data_str_ADC *ptrADC,
                         struct forward_data_str_Speed *ptrSpeed,
                         uint8_t *fan_to_measure, uint16_t *fans_states) {

uint16_t period_prom;                           
float TM1,TM2,TH;


switch(FSM_GV_state)
  {
    case ACTIVE_GV:
      
        SendMessage(MSG_Analyzis_FSM_ACTIVATE);
        Reload_I2C();
        FSM_GV_state = DISABLE_GV;
        //printf("A\n");
        //printf("%d\n", FSM_GV_state);
        
    break;
    case DISABLE_GV:
      
      if(GetMessage(MSG_GV_BGN_FSM_ACTIVATE)) {
         
          FSM_GV_state = READ_VAL_GV;
          // Start Conversions and read "slow" I2C
          /// EXTI_SENM1_Init();
          ptrTempHum-> temp_MAX1 = I2CReadTempMAXAcc(MAX1_ADDR, I2C1);          
          I2CStartConversionTempHumidityHDC (HDC_ADDR, I2C1);
          Delay_For_Conversion = MainTimerSet(DELAY_FOR_READING_MS);
          ADC_Start_4Ch_Inj_Conv();
          ptrTempHum-> temp_MAX2 = I2CReadTempMAXAcc(MAX2_ADDR, I2C2);
          
          if (I2C_State_Sensor(MAX_I2C1) == 0) {ptrTempHum-> temp_MAX1 = -127;}
          if (I2C_State_Sensor(MAX_I2C2) == 0) {ptrTempHum-> temp_MAX2 = -127;}
      }  
      
    break;
    case READ_VAL_GV:
      if(!MainTimerIsExpired(Delay_For_Conversion)) break;
      
        FSM_GV_state = ACTIVE_GV;    
        // Get fan speed
        
        period_prom = Get_Period();
        //printf("%d\n",period_prom);
        // Filtering zeros and enormous values
        if ((period_prom == 0) || (period_prom > 64000)){
          if (zero_counter <= 3){
            zero_counter++;
            period_prom = period_previous;
          }
          else {
            period_previous = 0;
          }
        }
        else {
          zero_counter = 0;
          period_previous = period_prom;
        }
        
        if (fans_states[0] == 0)
        {
            setFanToMeasure(FANM1);
        }
        else if (fans_states[1] == 0) 
        {
            setFanToMeasure(FANM2);
        }
        else
        {
            setFanToMeasure(FANR);
        }
        blocking_Delay(70);
        ptrSpeed->fanM1_Speed = Get_Pulse_Value() / PULSE_PER_REVOLUTION;
        // printf("%d\n\n",period_prom);
        //ptrSpeed->fanM1_Speed = (SEC/(PULSE_PER_REVOLUTION*period_prom));
        //if(ptrSpeed->fanM1_Speed){ printf("%d\n\n",ptrSpeed->fanM1_Speed);}
                                 
        
        ///EXTI_DeInit();
        // Reading converted values
        I2CReadTempHumidityHDC(HDC_ADDR, I2C1, ptrTempHum);
        if (I2C_State_Sensor(HDC_I2C1) == 0) {ptrTempHum->temp_HDC = -127;}
        
        ADC_Read_4Ch(ptrADC);  
        
        // Finding temp for calculating  
        TM1 = ptrTempHum-> temp_MAX1;
        TM2 = ptrTempHum-> temp_MAX2;
        TH = ptrTempHum-> temp_HDC;  

         
        // Finding highest temp 
        if((TM1 == -127) && (TM2 == -127) && (TH == -127)) {
          ptrTempHum->temp_Itog = 63; // if all I2C dont work
        }
        else {
          if ( TM1 >= TM2 ){
            if( TM1 >= TH ){
                ptrTempHum->temp_Itog = TM1;
            }
            else {
                ptrTempHum->temp_Itog = TH;
            }
          }
          else {
            if( TM2 >= TH ){
                ptrTempHum->temp_Itog = TM2;
            }
            else {
                ptrTempHum->temp_Itog = TH;
            }
          }
        }
          if (I2C_State_Sensor(MAX_I2C1) == 0) {ptrTempHum-> temp_MAX1 = ptrTempHum->temp_Itog;}
          if (I2C_State_Sensor(MAX_I2C2) == 0) {ptrTempHum-> temp_MAX2 = ptrTempHum->temp_Itog;}
          if (I2C_State_Sensor(HDC_I2C1) == 0) {ptrTempHum-> temp_HDC = ptrTempHum->temp_Itog;}
        
        
        // Test
        /*
        printf("bat_VLT = %d \n",ptrADC-> power_in_VLT );
        printf("fanm2_VLT %d \n",ptrADC-> fanR_VLT);
        printf("Temp =  %f \n", ptrTempHum->temp_HDC);
        printf("Hum =  %f \n",ptrTempHum->humidity_HDC);
        printf("PpS =  %d \n",ptrSpeed->fanM1_Speed);
        */
         
    break;
    default:
      
      // DEFAULT
      FSM_GV_state = DISABLE_GV;
      
    break;
  }             

}  