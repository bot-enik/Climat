#include "SpeedCounter.h"

uint8_t FSM_SpdCnt_state = START_SC;

uint16_t Pulses, Voltage, calculatedPulses;

uint8_t good_fan_counter = 0, pastError, counter = 0;

uint16_t fanm1_rev, fanm2_rev, fanr_rev;

uint32_t Delay_SC = 0, Delay_1 = 0, Delay_2 = 0, Delay_3 = 0;


void Speed_Count_FSM (struct forward_data_str_ADC *ADC_ptr,
                      uint16_t *fans_state, uint16_t minimum_rev,
                      uint16_t maximum_rev,
                      uint8_t *good_Fan_p, uint8_t *feedback_state_p, 
                      uint8_t *curent_state_p, uint8_t *error_type_p,
                      struct forward_data_str_Speed *ptrSpeeds) {
  
                        
  switch (FSM_SpdCnt_state)
  {
    case START_SC:
      
      FSM_SpdCnt_state = CHECK_FANM1;
      Delay_SC = MainTimerSet(300);
      setFanToMeasure(*good_Fan_p);
      
    break;
    case CHECK_FANM1:
      
      if(!MainTimerIsExpired(Delay_SC)) {break;}
      FSM_SpdCnt_state = CHECK_FANM2;
      fanm1_rev = 0;
      
      setFanToMeasure(FANM1);
      blocking_Delay(50);
 
      fanm1_rev = Get_Pulse_Value() / PULSE_PER_REVOLUTION;
      setFanToMeasure(*good_Fan_p);
      // Set the flag
      // printf("1 %d\n",fanm1_rev)
      
      if (fans_state[4] == 1) { // IS ON
          if((fanm1_rev > minimum_rev/2) && (fanm1_rev < maximum_rev*2)){
            fans_state[0] = 0;
          }
          else {fans_state[0] = 1;}
      }
      else { // OFF
          if(fanm1_rev < minimum_rev/2) {fans_state[0] = 0;}
          else {fans_state[0] = 1;}
      }
      Delay_1 = MainTimerSet(10);
      
      
    break;
    case CHECK_FANM2:
      
      if(!MainTimerIsExpired(Delay_1)) {break;}
      FSM_SpdCnt_state = CHECK_FANR;
      fanm2_rev = 0;
      
      setFanToMeasure(FANM2);
      blocking_Delay(50);
      
      fanm2_rev = Get_Pulse_Value() / PULSE_PER_REVOLUTION;
      setFanToMeasure(*good_Fan_p);
      // Set the flag
      // printf("2 %d\n",fanm2_rev);
      if (fans_state[5] == 1) { // IS ON
          if((fanm2_rev > minimum_rev/2) && (fanm2_rev < maximum_rev*2)){
            fans_state[1] = 0;
          }
          else {fans_state[1] = 1;}
      }
      else { // OFF
          if(fanm2_rev < minimum_rev/2) {fans_state[1] = 0;}
          else {fans_state[1] = 1;}
      }
      Delay_2 = MainTimerSet(10);
      
      
    break;
    case CHECK_FANR:
      
      if(!MainTimerIsExpired(Delay_2)) {break;}
      FSM_SpdCnt_state = CONCLUSION;
      fanr_rev  = 0;
      
      setFanToMeasure(FANR);
      blocking_Delay(50);
      
      fanr_rev = Get_Pulse_Value() / PULSE_PER_REVOLUTION;
      setFanToMeasure(*good_Fan_p);
      // Set the flag
      // printf("3 %d\n",fanr_rev);
      if (fans_state[6] == 1) { // IS ON
          if((fanr_rev > minimum_rev/2) && (fanr_rev < maximum_rev*2)){
            fans_state[2] = 0;
          }
          else {fans_state[2] = 1;}
      }
      else { // OFF
          if(fanr_rev < minimum_rev/2) {fans_state[2] = 0;}
          else {fans_state[2] = 1;}
      }
      Delay_3 = MainTimerSet(10);
      
    break;   
    case CONCLUSION:
      
      if(!MainTimerIsExpired(Delay_3)) {break;}
      FSM_SpdCnt_state = START_SC;
      // Select one of the working fans       
      if(good_fan_counter >= 3) {
        good_fan_counter = 0;
      }
      if(good_fan_counter == 0) {
        if (fans_state[0] == 1){*good_Fan_p = FANM1;}
        else {good_fan_counter++;}
      }
      if(good_fan_counter == 1) {
        if (fans_state[1] == 1){*good_Fan_p = FANM2;}
        else {good_fan_counter++;}
      }
      if(good_fan_counter == 2) {
        if (fans_state[2] == 1){*good_Fan_p = FANR;}
        else {good_fan_counter++;}
      }

      good_fan_counter++; 
      
      setFanToMeasure(*good_Fan_p);
      
      // If two or more fans is good, then set fans_state[7] to zero
      if((fans_state[0] + fans_state[1] + fans_state[2] <= 1) &&
         (fans_state[4] + fans_state[5] + fans_state[6] >= 2))
         {fans_state[7] = 0;}
      
      // printf("%d%d%d\n", fans_state[0],fans_state[1],fans_state[2]);
      // printf("%d\n",good_fan_counter);
      // printf("%d\n",*good_Fan_p);
      
    break;    
    default:
      FSM_SpdCnt_state = START_SC;
    break;       
  }
}




