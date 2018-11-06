#include "FSM_State_Control.h"

uint8_t FSM_SC_state = DISABLE_SC;
uint8_t HEAT_HYST_state = HEAT_LOW_TEMP;
uint8_t PWR_HYST_state = PWR_LOW_TEMP;


char Send_Buf_SC[200];

void State_Control_Process_FSM(uint8_t *state_ptr, uint8_t *sc_past_state_ptr, uint8_t *error_type_p,
                               uint16_t revolutions_min, uint16_t revolutions_max,
                               uint32_t revolutions_fb, uint16_t *fansState,
                               struct forward_data_str_Temp *ptrTempHum) {
  
  float temp;
  uint8_t flag = 0;
  uint8_t sc_current_state = 0;

switch(FSM_SC_state)
{
    case ACTIVE_SC:
      
        // JUST Send the flag
        SendMessage(MSG_FBC_FSM_ACTIVATE);
        FSM_SC_state = DISABLE_SC;
        
    break;
    case DISABLE_SC:
      
      if(GetMessage(MSG_SC_FSM_ACTIVATE)) {
              
          FSM_SC_state = ACTIVE_SC;
          
          sc_current_state = *state_ptr;
          // Dont touch the GPIOs every loop, only when statement was changed 
          /*sc_current_state = *state_ptr;
          if (*sc_past_state_ptr != sc_current_state) {
            flag = 1;
            printf("1\n");
          }
          else {
            flag = 0;
          }*/
          flag = 1;
          
          
          // Fans control
          temp = ptrTempHum->temp_Itog;

          Fans_Control(sc_current_state, revolutions_min, revolutions_max, 
                       temp, revolutions_fb, fansState); 
          
          
          // Power, heaters and fans control
          if(PBS_MODE) {
              if (sc_current_state == EMERGENCY) 
              {
                  Power_Control(OFF);
                  Heater1_Control(OFF);
                  Heater2_Control(OFF);
              }
              else 
              {
                
                // Hysteresis for Heater
                switch (HEAT_HYST_state)
                {
                  case (HEAT_LOW_TEMP):
                    if (temp <= (TEMP_PWR_ON))
                    {
                        Heater1_Control(ON);
                        Heater2_Control(ON);
                        FanR_Set(ON, 1200);
                        FanM_Set(ON, 1200);
                        fansState[4] = 1;
                        fansState[5] = 1;
                        fansState[6] = 1;
                        fansState[3] = 2440; 
                    }
                    else 
                    {
                        HEAT_HYST_state = HEAT_HIGH_TEMP;
                    }
                  break;
                  case (HEAT_HIGH_TEMP):
                    if (temp >=  TEMP_PWR_OFF) 
                    {
                        Heater1_Control(OFF);
                        Heater2_Control(OFF);
                    }
                    else 
                    {
                        HEAT_HYST_state = HEAT_LOW_TEMP;
                    }
                  break;
                  default:
                      HEAT_HYST_state = HEAT_LOW_TEMP;
                  break;
                }
                
                // Hysteresis for Power
                switch (PWR_HYST_state)
                {
                  case (PWR_LOW_TEMP):
                    if (temp <= (TEMP_PWR_ON))
                    { 
                        Power_Control(OFF);
                    }
                    else 
                    {
                        PWR_HYST_state = PWR_HIGH_TEMP;
                    }
                  break;
                  case (PWR_HIGH_TEMP):
                    if (temp >=  TEMP_PWR_OFF) 
                    {
                        Power_Control(ON);
                    }
                    else 
                    {
                        PWR_HYST_state = PWR_LOW_TEMP;
                    }
                  break;
                  default:
                      PWR_HYST_state = PWR_LOW_TEMP;
                  break;
                }
  
              }
          }// PBS
          else {
              Power_Control(OFF);
              Heater1_Control(OFF);
              Heater2_Control(OFF);
          }
          
          flag = 0; 
       }// if(GetMessage)
      
    break;
    default:
      // DEFAULT 
      FSM_SC_state = DISABLE_SC;
    break;
} //  

}