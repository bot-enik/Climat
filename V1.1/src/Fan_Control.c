#include "Fan_Control.h"

uint8_t Hysteresis_state = HYST_LOW;
uint8_t local_state_fc = LOCAL_1;
uint8_t simple_controller_state = LOCAL_1;
uint8_t fanR_is_set = OFF;
uint8_t fanR_counter = 0;
uint16_t Vadc = 3800, rev_calculated, rev_real;
uint32_t Delay_For_Start,rev;

float Vapprox;

char Send_Buf_FC[100];

void Fans_Control(uint8_t current_state, 
                  uint16_t MIN_REV, uint16_t MAX_REV, 
                  float temp_FC, uint32_t rev_real, uint16_t *fans_statement) {                   

  switch (current_state)
  {
    // HEATING_AND_DRYING
    case HEATING_AND_DRYING:
         // Control of fans in HEATING_AND_DRYING: FSM_State_control.c
    break;
    
    // NORMAL or EMERGENCY state
    default:
      if(temp_FC <= TEMP_PWR_ON) {
        
        /*
         FanR_Set(ON, 1200);
         FanM_Set(ON, 1200);
         fans_statement[4] = 1;
         fans_statement[5] = 1;
         fans_statement[6] = 1;
         fans_statement[3] = 2440; 
        */
      }
      else {  // temp_FC > TEMP_PWR_ON
        // Using Hysteresis
        switch (Hysteresis_state)
        {
        case (HYST_LOW):
           if (temp_FC < TEMP_HYST_LOW_TR){
              FanR_Set(OFF, 0);
              FanM_Set(OFF, 0);
              fans_statement[4] = 0;
              fans_statement[5] = 0;
              fans_statement[6] = 0;
              fans_statement[3] = 0;
           }
           else if ((temp_FC >= TEMP_HYST_LOW_TR) && (temp_FC < TEMP_HYST_HIGHT_TR)){
             Hysteresis_state = HYST_PROM;
           }
           else {   
             Hysteresis_state = HYST_HIGH;               
           }
         break; // HYST LOW
         
         // When fans starts rotating, first FAN_START_DELAY ms
         // it will rotate at max speed
         case (HYST_PROM):
         switch (local_state_fc)
             {
               case LOCAL_1:           
                 // First FAN_START_DELAY ms after switched-off state
                 // rotating with MAX speed
                 FanM_Set(MAX, 0);
                 //printf("LC1");
                 // Check the fans, if something wrong, turn on reserve one
                 if ( ((fans_statement[0] == 1) && (fans_statement[4] == 1)) ||
                      ((fans_statement[1] == 1) && (fans_statement[5] == 1)) ){ 
                       // Small filter
                       if (fanR_counter >= 5){
                          FanR_Set(MAX, 0);
                          fans_statement[6] = 1;
                          fanR_counter = 6;
                       }
                       else {
                          fanR_counter++;
                       }  
                 }
                 else {
                       FanR_Set(OFF, 0);
                       fans_statement[6] = 0;
                       fanR_counter = 0;
                 }
                 
                 fans_statement[4] = 1;
                 fans_statement[5] = 1;
                 fans_statement[3] = 2900;
                 Delay_For_Start = MainTimerSet(FAN_START_DELAY);
                 local_state_fc = LOCAL_2;
               break;
               case LOCAL_2:
                 if(!MainTimerIsExpired(Delay_For_Start)) break;
                 Hysteresis_state = HYST_MEDIUM;
                 //printf("LC2");
                 local_state_fc = LOCAL_1;
               break;
               default:
                 local_state_fc = LOCAL_1;
               break;
             }
        break; // HYST_PROM
          
        case (HYST_MEDIUM):
           // Linear part of the graph
           if((temp_FC >= (TEMP_HYST_LOW_TR - TEMP_HYST_DIF)) && (temp_FC < TEMP_HYST_HIGHT_TR)) {
               
               //Calculating the revolutions
               rev = (uint32_t)((MIN_REV) + (temp_FC - (TEMP_HYST_LOW_TR - TEMP_HYST_DIF)) * 
                 ((MAX_REV - MIN_REV) / (TEMP_HYST_HIGHT_TR - TEMP_HYST_LOW_TR)));      
               
               //Simpliest control system
               if(rev_real < rev) { Vadc = Vadc - 10;}
               else if (rev_real > rev){ Vadc = Vadc + 10;}
               else {Vadc = Vadc;}
               
               if(Vadc <= 30 )
                 Vadc = 30;
               else if (Vadc > 4000)
                 Vadc = 4000;
                            
               
               FanM_Set(ON, Vadc);
               // Check the fans, if something wrong, turn on reserve one
               if ( ((fans_statement[0] == 1) && (fans_statement[4] == 1)) ||
                    ((fans_statement[1] == 1) && (fans_statement[5] == 1)) ){ 
                     // Small filter
                     if (fanR_counter >= 5){
                        FanR_Set(ON, Vadc);
                        fans_statement[6] = 1;
                        fanR_counter = 6;
                     }
                     else {
                        fanR_counter++;
                     }  
               }
               else {
                 FanR_Set(OFF, 0);
                 fans_statement[6] = 0;
                 fanR_counter = 0;
               }
               
               // Calculating approximate FANxx_VLT
               Vapprox = (-0.399*Vadc + 2923.0);
               
               fans_statement[4] = 1;
               fans_statement[5] = 1;
               
               fans_statement[3] = (uint16_t)Vapprox;
           
           }
           else if (temp_FC < (TEMP_HYST_LOW_TR - TEMP_HYST_DIF)) {
             Hysteresis_state = HYST_LOW;  
           }
           else {
             Hysteresis_state = HYST_HIGH;  
           }

         break; // HYST_MEDIUM
        
         case (HYST_HIGH):
           if (temp_FC >= (TEMP_HYST_HIGHT_TR - TEMP_HYST_DIF)){
               FanR_Set(MAX, 0);
               FanM_Set(MAX, 0);
               fans_statement[4] = 1;
               fans_statement[5] = 1;
               fans_statement[6] = 1;
               fans_statement[3] = 2900;
           }
           else if((temp_FC < (TEMP_HYST_HIGHT_TR - TEMP_HYST_DIF)) && (temp_FC > (TEMP_HYST_LOW_TR - TEMP_HYST_DIF))){
               //Calculating the revolutions
               rev = (uint32_t)((MIN_REV) + (temp_FC - (TEMP_HYST_LOW_TR - TEMP_HYST_DIF)) * 
                 ((MAX_REV - MIN_REV) / (TEMP_HYST_HIGHT_TR - TEMP_HYST_LOW_TR)));

               //Simpliest control system
               if(rev_real < rev) { Vadc = Vadc - 10;}
               else if (rev_real > rev){ Vadc = Vadc + 10;}
               else {Vadc = Vadc;}
               
               if(Vadc <= 30 )
                 Vadc = 30;
               else if (Vadc > 4000)
                 Vadc = 4000;
               
               FanM_Set(ON, Vadc);
               // Check the fans, if something wrong, turn on reserve one
               if ( ((fans_statement[0] == 1) && (fans_statement[4] == 1)) ||
                    ((fans_statement[1] == 1) && (fans_statement[5] == 1)) ){ 
                     // Small filter
                     if (fanR_counter >= 5){
                        FanR_Set(ON, Vadc);
                        fans_statement[6] = 1;
                        fanR_counter = 6;
                     }
                     else {
                        fanR_counter++;
                     }  
               }
               else {
                 FanR_Set(OFF, 0);
                 fans_statement[6] = 0;
                 fanR_counter = 0;
               }
               
               // Calculating approximate FANxx_VLT
               Vapprox = (-0.399*Vadc + 2923.0);
               
               fans_statement[4] = 1;
               fans_statement[5] = 1;
               fans_statement[3] = (uint16_t)Vapprox;               
            }
            else {
               Hysteresis_state = HYST_LOW;
            }
          break;
          default:
            //////////////////////////////////
          break;
        }
     break;
    }
  }
}
