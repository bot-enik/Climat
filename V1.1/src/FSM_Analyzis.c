#include "FSM_Analyzis.h"

uint8_t FSM_Analyzis_state = DISABLE_Analyzis;
uint8_t a_local_state;
uint32_t delay_time_ms_a;

void Analyzis_Process_FSM ( struct forward_data_str_Temp *Temp_ptr,
                               uint8_t *past_state_p, uint8_t *state_count_p,
                               uint8_t *saved_state_p, uint8_t *feedback_state_p,
                               uint8_t *current_state_p, uint8_t *error_type_p){
                                 
  switch(FSM_Analyzis_state)
  {
    case ACTIVE_Analyzis:
      
        // JUST Send the flag
        SendMessage(MSG_SC_FSM_ACTIVATE);
        FSM_Analyzis_state = DISABLE_Analyzis;
        //printf("L\n");
        
    break;
    case DISABLE_Analyzis:
      
      if(GetMessage(MSG_Analyzis_FSM_ACTIVATE)) {
        FSM_Analyzis_state = ACTIVE_Analyzis;
        
        
          float temp_HDC  = Temp_ptr -> temp_HDC;
          float hum_HDC   = Temp_ptr -> humidity_HDC;
          float temp_MAX1 = Temp_ptr -> temp_MAX1;
          float temp_MAX2 = Temp_ptr -> temp_MAX2;
          float temp_itog = Temp_ptr -> temp_Itog;
          
          //printf("R\n");
          
          /*
          // If we get to the EMERGENCY state, then we cannot change this immediately
          // We should wait 10 min, and only then check the values from sensors
          // If they will be good looking, we will change the state
          // If no, we should wait again 10 min it will be(EMRERG_OUT_TRY)times
          // If values will be ok, then we get to the HEAITNG_AND_DRYING state
          if(*past_state_p == EMERGENCY) {
          
            switch (a_local_state)
            {
              case A:
                a_local_state = B;
                delay_time_ms_a = MainTimerSet(10*MIN);
              break;
              case B:
                if(!MainTimerIsExpired(delay_time_ms_a)) break;
                a_local_state = A;
              break;
            }
            
            if(a_local_state == A) break;
            
          }
          // Look some additional code at the end of this func
          */ 
            
            
            
          // Check feedback value
          if (*feedback_state_p == EMERGENCY)
            { *current_state_p = EMERGENCY; } 
          else {
              // Control of the values
              // Values must be in range of the real values
              // (from sensors datasheets)
              // Also they must be lower than MAX TEMP values
              if(
                 (((hum_HDC   > 99)  || (hum_HDC   <= 0)) && HUMIDITY_USE) ||
                 (temp_HDC  > TEMP_UP_TR)||(temp_HDC  < -128)  ||
                 (temp_MAX1 > TEMP_UP_TR)||(temp_MAX1 < -128)  ||
                 (temp_MAX2 > TEMP_UP_TR)||(temp_MAX2 < -128)
                ) 
              { *current_state_p = EMERGENCY;  *error_type_p = TEMP_EXCEED_TR;}  
              else {
                 // Temp values must be approximately equal
                 // Otherwise it is an error in some sensor
                 // So it will be EMERGENCY
                 if((temp_itog - temp_MAX1 > MAX_TEMP_DEVIATION) || 
                    (temp_itog - temp_HDC  > MAX_TEMP_DEVIATION) ||
                    (temp_itog - temp_MAX2 > MAX_TEMP_DEVIATION) 
                   )
                 { *current_state_p = EMERGENCY; *error_type_p = TEMP_DEVIATION_ERROR;}
                 else {
                   // Looking for values out of range
                   // If so, its time for HEATING_AND_DRYING
                   if((temp_HDC  < TEMP_PWR_ON) ||
                      (temp_MAX1 < TEMP_PWR_ON) ||
                      (temp_MAX2 < TEMP_PWR_ON) ||
                      ((hum_HDC > HUM_UP_TR) && HUMIDITY_USE)
                     )
                   { *current_state_p = HEATING_AND_DRYING; *error_type_p = NO_ERROR;}
                   else
                   // If all right then NORMAL *current_state_p
                   { *current_state_p = NORMAL; *error_type_p = NO_ERROR;}
                   
                   // But temp can be higher than TEMP_UP_TR_INC_HUM
                   // AND humidity is higher than HUM_UP_TR
                   // In is case it will be ENERGENCY
                   if (((hum_HDC > HUM_UP_TR_INC_TEMP) && ( (temp_HDC  > TEMP_UP_TR_INC_HUM) || 
                                                           (temp_MAX1 > TEMP_UP_TR_INC_HUM) ||
                                                           (temp_MAX2 > TEMP_UP_TR_INC_HUM) 
                       )) && HUMIDITY_USE)
                   { *current_state_p = EMERGENCY; *error_type_p = HUM_INC_TEMP_ERROR;}
                 }
              } 
          }
          //printf("state = %x \n", *current_state_p);
          
          // Counting up to SENSITIVITY to change the state
          // If value changes
          if(*current_state_p != (*past_state_p)) { 
            
            (*past_state_p) = *current_state_p;    
            (*state_count_p) = 0;
            *current_state_p = (*saved_state_p);
            
          }
          // No changes
          else {
            
            (*past_state_p) = *current_state_p;
            // Blocking overflow of the state_count
            if((*state_count_p) < (SENSITIVITY+1))  {(*state_count_p)++;}
            
            if ((*state_count_p) == SENSITIVITY) {
              
              (*saved_state_p) = *current_state_p;
              *current_state_p = *current_state_p; 
              
            }
            else {
              
              *current_state_p = (*saved_state_p);

            }  
          }
         
          //printf("Choose state = %x\n",*current_state_p);
      }
      
    break;
    default:
      
      // DEFAULT
      FSM_Analyzis_state = DISABLE_Analyzis;
      
    break;
  }  

}