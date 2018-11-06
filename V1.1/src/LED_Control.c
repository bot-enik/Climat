#include "Led_Control.h"

static volatile uint32_t delay_time_ms, delay_time_1_emerg_ms, delay_time_2_emerg_ms;
uint8_t local_state = LS_First;
uint8_t lc_past_state = 12;

void Led_Control(uint8_t lc_current_state, uint16_t error_type_o, uint16_t *fansStatetment, float temperature) {
  
  // If there was change
  if (lc_past_state != lc_current_state) {
     lc_past_state = lc_current_state;
     delay_time_ms = MainTimerSet(0);
  }

  switch (lc_current_state)
  {
    // FAN test: green - orange - red - green - orange...
    case FAN_TEST:
      
/*      switch (local_state)
      {
        case LS_First:
          if(!MainTimerIsExpired(delay_time_ms)) break;
          RED_LED(OFF);
          GREEN_LED(ON);
          delay_time_ms = MainTimerSet(FAN_TEST_DELAY);
          local_state = LS_Second;
        break;
        case LS_Second:
          if(!MainTimerIsExpired(delay_time_ms)) break;
          ORANGE_LED(ON);
          delay_time_ms = MainTimerSet(FAN_TEST_DELAY);
          local_state = LS_Third;
        break;
        case LS_Third:
          if(!MainTimerIsExpired(delay_time_ms)) break;
          GREEN_LED(OFF);
          RED_LED(ON);
          delay_time_ms = MainTimerSet(FAN_TEST_DELAY);
          local_state = LS_First;
        break;
        default:
          local_state = LS_First;
        break;
      }
*/
    break;
    
    // NORMAL:  green on, red off (but if some fans is broken, then green-red-green...)
    // Also, if temp = TEMP_UP_TR - 5, then orange blinking
    case NORMAL:
      
    // Looking for broken fans
    if(fansStatetment[0]+fansStatetment[1]+fansStatetment[2] > 0) {
        switch (local_state)
        {
          case LS_First:
            if(!MainTimerIsExpired(delay_time_ms)) break;
            NORM_LED(OFF);
            EMERG_LED(ON);
            delay_time_ms = MainTimerSet(N_BLINK_DELAY);
            local_state = LS_Second;
          break;
          case LS_Second:
             if(!MainTimerIsExpired(delay_time_ms)) break;
             NORM_LED(ON);
            EMERG_LED(OFF);
            delay_time_ms = MainTimerSet(N_BLINK_DELAY);
            local_state = LS_First;
          break;
          default:
            local_state = LS_First;
          break;
        }      
        
      }
      else {
        if (temperature < (TEMP_UP_TR-5)) 
        {
            EMERG_LED(OFF);
            NORM_LED(ON);
        }
        else 
        {
            switch (local_state)
            {
              case LS_First:
                if(!MainTimerIsExpired(delay_time_ms)) break;
                ORANGE_LED(ON);
                delay_time_ms = MainTimerSet(N_BLINK_DELAY);
                local_state = LS_Second;
              break;
              case LS_Second:
                if(!MainTimerIsExpired(delay_time_ms)) break;
                ORANGE_LED(OFF);
                delay_time_ms = MainTimerSet(N_BLINK_DELAY);
                local_state = LS_First;
              break;
              default:
                local_state = LS_First;
              break;
            }
        }
      }
        
    break;    
    

    // HEATING_AND_DRYING - orange on (but if some fans is broken, then orange-red-orange...) 
    case HEATING_AND_DRYING:
      
      // Looking for broken fans
    if(fansStatetment[0]+fansStatetment[1]+fansStatetment[2] > 0) {
        switch (local_state)
        {
          case LS_First:
            if(!MainTimerIsExpired(delay_time_ms)) break;
            ORANGE_LED(ON);
            delay_time_ms = MainTimerSet(N_BLINK_DELAY);
            local_state = LS_Second;
          break;
          case LS_Second:
            if(!MainTimerIsExpired(delay_time_ms)) break;
            NORM_LED(OFF);
            EMERG_LED(ON);
            delay_time_ms = MainTimerSet(N_BLINK_DELAY);
            local_state = LS_First;
          break;
          default:
            local_state = LS_First;
          break;
        }      
        
      }
      else {
        ORANGE_LED(ON);
      }
    
    break;
    
    // EMERGENCY - blinking EMERG, NORM is off
    case EMERGENCY:
      
      
      NORM_LED(OFF);
      EMERG_LED(ON);
      
      /* This is complex indication (its working, but dont need now)

      if ((error_type_o == TEMP_EXCEED_TR) || 
          (error_type_o == TEMP_DEVIATION_ERROR) || 
            (error_type_o == HUM_INC_TEMP_ERROR) )
      {error_type_o = TEMP_ERROR;}
      
      if (error_type_o == FAN_VLTG_ERROR_LOW_TR)
      {error_type_o = VLTG_ERROR;}
      
      
      // Changing delay time according to the error
      switch (error_type_o) 
      {
        case TEMP_ERROR:
           delay_time_1_emerg_ms = TEMP_ERR_DELAY_1;
           delay_time_2_emerg_ms = TEMP_ERR_DELAY_1;
        break;
        case VLTG_ERROR:
           delay_time_1_emerg_ms = TEMP_ERR_DELAY_1;
           delay_time_2_emerg_ms = TEMP_ERR_DELAY_2;
        break;
        case GPIO_ERROR:
           delay_time_1_emerg_ms = TEMP_ERR_DELAY_2;
           delay_time_2_emerg_ms = TEMP_ERR_DELAY_2;
        break;
        default:

        break;
      }
      
      // Led control
      switch (local_state)
      {
        case LS_First:
          if(!MainTimerIsExpired(delay_time_ms)) break;
          NORM_LED(OFF);
          EMERG_LED(ON);
          delay_time_ms = MainTimerSet(delay_time_1_emerg_ms);
          local_state = LS_Second;
        break;
        case LS_Second:
          if(!MainTimerIsExpired(delay_time_ms)) break;
          NORM_LED(OFF);
          EMERG_LED(OFF);
          delay_time_ms = MainTimerSet(delay_time_2_emerg_ms);
          local_state = LS_First;             
        break;
        default:
          local_state = LS_First;
        break;
      }     
      
    */
      
      

    break; 
    
    // DEFAULT
    default:
        //
    break;
  }  
  
}