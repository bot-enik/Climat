#include "FSM_FB_Control.h"

uint8_t FSM_FBC_state = DISABLE_FBC;

void Feedback_Conrol_Process_FSM(struct forward_data_str_ADC *ADC_ptr,
                                 uint8_t *feedback_state_p, uint8_t *curent_state_p,
                                 uint8_t *error_type_p) {
 
  
  switch(FSM_FBC_state)
  {
    case ACTIVE_FBC:
      
        // JUST Send the flag
        SendMessage(MSG_GV_BGN_FSM_ACTIVATE);
        FSM_FBC_state = DISABLE_FBC;
        //printf("J\n");
        if(ReadMessage(MSG_GV_BGN_FSM_ACTIVATE))
        //printf("%d\n",ReadMessage(MSG_GV_BGN_FSM_ACTIVATE));
        
    break;
    case DISABLE_FBC:
      
      if(GetMessage(MSG_FBC_FSM_ACTIVATE)) {
        FSM_FBC_state = ACTIVE_FBC;
        
          //printf("I\n");
          // Delay because inertion of the system
          blocking_Delay(INERTION_DELAY);
          // Controls the input voltage
          // Controls the GPIOs
          // Controls the fans voltage
          
          uint8_t PWRGD_1, PWRGD_2, PWRGD_3;
          uint16_t FANM1_VLT, FANM2_VLT, FANR_VLT,IN_VLT;
          PWRGD_1 = (uint8_t)GPIO_ReadInputDataBit(GPIOB, PWRGD1);  
          PWRGD_2 = (uint8_t)GPIO_ReadInputDataBit(GPIOB, PWRGD2);
          PWRGD_3 = (uint8_t)GPIO_ReadInputDataBit(GPIOC, PWRGD3); 
          
          FANM1_VLT = ADC_ptr->fanm1_VLT;
          FANM2_VLT = ADC_ptr->fanm2_VLT;
          FANR_VLT  = ADC_ptr->fanR_VLT;
          IN_VLT    = ADC_ptr->power_in_VLT;
          
          // Input voltage must not exceed tresholds
          // Fans voltage must not exceed up tresholds
          if ((IN_VLT  >= INPUT_VOLTAGE_LOW_TR) && 
              (IN_VLT  <=  INPUT_VOLTAGE_UP_TR) &&
              (FANM1_VLT <= FANS_VOLTAGE_UP_TR) &&
              (FANM2_VLT <= FANS_VOLTAGE_UP_TR) &&
              (FANR_VLT  <= FANS_VOLTAGE_UP_TR)) {
            
              // NORMAL STATE
              if(*curent_state_p == NORMAL){    
              //printf("NORMAL \n");
              
                // Power check
                // DIRECT logic
                if ((PWRGD_1 != Bit_RESET) && 
                    (PWRGD_2 == Bit_RESET) && 
                    (PWRGD_3 == Bit_RESET))
                { *feedback_state_p = NORMAL; }
                else 
                { *feedback_state_p = EMERGENCY;
                  *error_type_p = GPIO_ERROR;
                  //printf("PWRFB ERR \n");
                } 
                
              }
              // HEATING_AND_DRYING STATE
              else if(*curent_state_p == HEATING_AND_DRYING){
              //printf("HAndDry \n");
              
                // Power check
                // DIRECT logic
                if ((PWRGD_1 == Bit_RESET) && 
                    (PWRGD_2 != Bit_RESET) && 
                    (PWRGD_3 != Bit_RESET)) {
         
                   // Fans voltage low treshold check,
                   
                      if ((FANM1_VLT > WORKING_FAN_VLT_LOW_TR) &&
                       (FANM2_VLT > WORKING_FAN_VLT_LOW_TR) &&
                       (FANR_VLT  > WORKING_FAN_VLT_LOW_TR))
                     { *feedback_state_p = HEATING_AND_DRYING; }
                   else 
                     { *feedback_state_p = EMERGENCY; 
                       *error_type_p = FAN_VLTG_ERROR_LOW_TR;
                       //printf("FANVLT ERR \n");
                     }
                    
                 }  
               
                else 
                { *feedback_state_p = EMERGENCY; 
                  *error_type_p = GPIO_ERROR;
                  //printf("PWRFB ERR \n");
                }
                
              }
              else // EMERGENCY STATE
              { 
                ///
              }
          }
          else // Power check
          { 
            *feedback_state_p = EMERGENCY;
            *error_type_p = VLTG_ERROR;
            //printf("PWR_IN ERROR");
          }
      }
    break;
    default:
      
      // DEFAULT
      FSM_FBC_state = DISABLE_FBC;
      
    break;
 
  }
}