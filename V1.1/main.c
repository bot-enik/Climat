#include "main.h"

data_str_Temp data_Temp;
data_str_ADC data_ADC;
data_str_Speed data_Speed;

data_str_Temp *data_ptr_Temp = &data_Temp;
data_str_ADC *data_ptr_ADC = &data_ADC;
data_str_Speed *data_ptr_Speed = &data_Speed;
 
uint8_t current_state, past_state, state_count, error_type,
        saved_state, feedback_state;

uint8_t *current_state_ptr, *past_state_ptr, *state_count_ptr, 
        *saved_state_ptr, *feedback_state_ptr, *error_type_ptr;

uint16_t rev_min, rev_max, rev_test, rev_calc;

uint32_t SpeedCountDelay = 0;
uint32_t *SpeedCountDelay_ptr = &SpeedCountDelay;

uint32_t period_pulse;

/*--------------------------------------------------------------*/
// IF you get ERROR here, then you should define MODE correctly (main.h)
uint8_t ERROR_CHECK_MODE_DEFINE[PBS_MODE^ATS_MODE];
/*--------------------------------------------------------------*/

// Value for the elements of array:
// workingFans[0-2] - fan alert (1)               (SpeedCounter.c)
// workingFans[3] - approximate value on fan ADC  (Fan_control.c)
// workingFans[4-6] - do we set the fan to work   (Fan_control.c)
// 0,4 - FANM1 1,5 - FANM2 2,6 - FANR
// 7 - (1) means that 2 or more fans are dead 
uint16_t workingFans[] = {0,0,0,0,0,0,0,0};

uint8_t good_Fan = FANM1;
uint8_t *good_Fan_ptr = &good_Fan;

// For "GUI"
#define WAIT (0x00)
#define SHOW (0xFF)
uint8_t Show_State = 0;
uint32_t Delay_To_Show = 0;

char Send_Buf[100];

void main(void) {
  
  data_Temp.humidity_HDC = 10;
  data_Temp.temp_HDC  = 0;
  data_Temp.temp_MAX1 = 0;
  data_Temp.temp_MAX2 = 0;
  
  data_ADC.power_in_VLT = 0;
  data_ADC.fanR_VLT  = 0;
  data_ADC.fanm1_VLT = 0;
  data_ADC.fanm2_VLT = 0;
  
  data_Speed.fanM1_Speed = 0;
  data_Speed.fanM2_Speed = 0;
  data_Speed.fanR_Speed  = 0;
  
  if(PBS_MODE) {
    current_state  = HEATING_AND_DRYING;
    past_state     = HEATING_AND_DRYING;
    saved_state    = HEATING_AND_DRYING;
    feedback_state = HEATING_AND_DRYING;  
  }
  else {
    current_state  = NORMAL;
    past_state     = NORMAL;
    saved_state    = NORMAL;
    feedback_state = NORMAL;   
  }
  
  error_type  = NO_ERROR;
  state_count = 0;
  
  current_state_ptr  = &current_state;
  past_state_ptr     = &past_state;
  state_count_ptr    = &state_count;
  saved_state_ptr    = &saved_state;
  feedback_state_ptr = &feedback_state;
  error_type_ptr     = &error_type;
  
  Full_Init();
  
  Fan_Test();
  
  uint8_t gpio_type_rep = 0;
  
  while(1){ 

      Get_Val_Process_FSM(data_ptr_Temp, data_ptr_ADC, data_ptr_Speed, good_Fan_ptr, workingFans);
      
      Analyzis_Process_FSM(data_ptr_Temp, past_state_ptr, state_count_ptr,
                           saved_state_ptr, feedback_state_ptr, current_state_ptr,
                           error_type_ptr);

      Speed_Count_FSM(data_ptr_ADC, workingFans, rev_min, rev_max, good_Fan_ptr,
                      feedback_state_ptr, current_state_ptr, error_type_ptr, data_ptr_Speed);
      
      if(PBS_MODE) {
        Led_Control(current_state, *error_type_ptr, workingFans, data_ptr_Temp->temp_Itog);
      }
      
      State_Control_Process_FSM(current_state_ptr, past_state_ptr, error_type_ptr, 
                                rev_min, rev_max, (data_ptr_Speed->fanM1_Speed),
                                workingFans, data_ptr_Temp);  
    
      if(GPIO_FEEDBACK_ON) {
        Feedback_Conrol_Process_FSM(data_ptr_ADC, feedback_state_ptr,
                                  current_state_ptr, error_type_ptr);
      }
      
      if(ATS_MODE) {
        Send_Report(current_state, workingFans, error_type, data_ptr_ADC, data_ptr_Temp);
      }
      
      ProcessMessages();
      
      ProcessWatchDog();
      
      if(DEBUG_ON) {
      switch (Show_State)
      {
        case WAIT:
          
          Show_State = SHOW;
          Delay_To_Show = MainTimerSet(2000);  
          
        break;
        case SHOW:
          
          if(!MainTimerIsExpired(Delay_To_Show)) break;
          
          Show_State = WAIT;
          
          sprintf(Send_Buf, "\n temperature = ");
          add_to_buf(Send_Buf,15);
          sprintf(Send_Buf, "%f",data_ptr_Temp->temp_Itog);
          add_to_buf(Send_Buf,3);
          
          sprintf(Send_Buf, "\n temperaMAX1 = ");
          add_to_buf(Send_Buf,15);
          sprintf(Send_Buf, "%f",data_ptr_Temp->temp_MAX1);
          add_to_buf(Send_Buf,3);
          
          sprintf(Send_Buf, "\n temperaMAX2 = ");
          add_to_buf(Send_Buf,15);
          sprintf(Send_Buf, "%f",data_ptr_Temp->temp_MAX2);
          add_to_buf(Send_Buf,3);
          
          sprintf(Send_Buf, "\n temperaHDC1 = ");
          add_to_buf(Send_Buf,15);
          sprintf(Send_Buf, "%f",data_ptr_Temp->temp_HDC);
          add_to_buf(Send_Buf,3);
          
          sprintf(Send_Buf, "\n humidity = ");
          add_to_buf(Send_Buf,13);
          sprintf(Send_Buf, "%f",data_ptr_Temp->humidity_HDC);
          add_to_buf(Send_Buf,3);
          

          sprintf(Send_Buf, "\n fan speed = ");
          add_to_buf(Send_Buf,15);
          sprintf(Send_Buf, "%d",data_ptr_Speed->fanM1_Speed);
          add_to_buf(Send_Buf,1);
 
          sprintf(Send_Buf, "\n statement = ");
          add_to_buf(Send_Buf,14);
          
          switch (current_state)
          {
            case HEATING_AND_DRYING:
              sprintf(Send_Buf, "H_a_D\n");
            break;
            case NORMAL:
              sprintf(Send_Buf, "Norma\n");
            break;
            case EMERGENCY:
              sprintf(Send_Buf, "Emerg\n");
            break;
          }
          add_to_buf(Send_Buf,6);
          
          
          sprintf(Send_Buf, " error type = ");
          add_to_buf(Send_Buf,14);
          sprintf(Send_Buf, "%d",error_type);
          add_to_buf(Send_Buf,1);
          sprintf(Send_Buf,"\n");
          add_to_buf(Send_Buf,1);
          sprintf(Send_Buf,"\n");
          add_to_buf(Send_Buf,1);
          sprintf(Send_Buf,"\n");
          add_to_buf(Send_Buf,1);
          
          break;      
      }
      }
      // */
      
  }
}



/* ============================================================================
                            ##### Support functions #####
 ============================================================================*/

void Full_Init(void) {

  Clock_Setup();
  MainTimerInit();
  InitMessages();
  
  /// EXTI_SENM1_Full_Init_FanSpeed(); This func or TIM_Pulses_INIT();
  setFanToMeasure(FANM1);
  All_GPIO_Init();
  I2CInit();
  FOUR_CH_ADC_Init();
  TWO_DAC_Init();

  USART_Init_1();
  DMA_USART_Init();
  
  I2CHDCInit(HDC_ADDR, I2C1);  
  
  SendMessage(MSG_GV_BGN_FSM_ACTIVATE);
  
  Power_Control(OFF);
  Heater1_Control(OFF);
  Heater2_Control(OFF);
  
}


void Clock_Setup(void) {
      
    // Using Internal Hight speed oscillator
    RCC_HSEConfig(RCC_HSE_OFF); 
    RCC_HSICmd(ENABLE);   
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
      
    // Test
    /*
    // If want to measure the clock
    //RCC_ClocksTypeDef Clock;
    //RCC_ClocksTypeDef *Clock_ptr = &Clock;
    
    //RCC_GetClocksFreq(Clock_ptr);
    //printf("SYSCLK = %d \n", Clock_ptr->SYSCLK_Frequency);
    //printf("HCLK = %d \n", Clock_ptr->HCLK_Frequency);
    //printf("PCLK1 = %d \n", Clock_ptr->PCLK1_Frequency);
    //printf("PCLK2 = %d \n", Clock_ptr->PCLK2_Frequency);
    
    //printf("%x \n", RCC_GetSYSCLKSource());
    */
}


void Fan_Test(void) {
  
  uint16_t bad_fan = 0;
  RED_LED(ON);
  GREEN_LED(ON);
  
  for(uint8_t flag = 0; flag <3; flag++) {
    
        switch(flag)
        {
          case 0:
            setFanToMeasure(FANM1);
          break;
          case 1:
            setFanToMeasure(FANM2);
            blocking_Delay(1000);
            
          break;
          case 2:
            setFanToMeasure(FANR);
            blocking_Delay(1000);
            
          break;
          default:
          break;
        }
    
      // Turn on fan and count pulses
      FanM_Set(MAX, 0);
      FanR_Set(MAX, 0);
      blocking_Delay(100);  
      
      // REVOLUTIONS (minimum voltage on fans)
      FanM_Set(ON, 4000);
      FanR_Set(ON, 4000);
      blocking_Delay(4000);
      
      
      RED_LED(OFF);
      GREEN_LED(ON);
      
      rev_min = Get_Pulse_Value() / PULSE_PER_REVOLUTION;
      
      // REVOLUTIONS (maximum voltage on fans)
      FanM_Set(MAX, 0);
      FanR_Set(MAX, 0);
      
      blocking_Delay(4000);
      
      
      rev_max = Get_Pulse_Value() / PULSE_PER_REVOLUTION;
      
      // Check the counted values
      if (((rev_max - MAX_REV_PER_SEC) < 5) && ((rev_max - MAX_REV_PER_SEC) > -5)) {
        if (((rev_min - MIN_REV_PER_SEC) < 5) && ((rev_min - MIN_REV_PER_SEC) > -5)) {
          
        switch(flag)
        {
          case 0:
            good_Fan = FANM1;
          break;
          case 1:
            good_Fan = FANM2;
          break;
          case 2:
            good_Fan = FANR;
          break;
          default:
          break;
        }
          
        }
        else {bad_fan++;}
      }
      else {bad_fan++;}

      
      
      
      // If 2 or more fans is dead, then set this flag
      if(bad_fan >= 2) {
          workingFans[7] = 1;
      }
      
      // If all fans dont work, then set default values
      if (bad_fan >= 3) {
        rev_min = MIN_REV_PER_SEC;
        rev_max = MAX_REV_PER_SEC;
        
        workingFans[7] = 1;
        // NO fans
        good_Fan = FANS_OFF;
        current_state = MOST_FANSOFF;
      } 
  }
  setFanToMeasure(good_Fan);
  //printf("max = %d\n",(rev_max));
  //printf("min = %d\n",(rev_min)); 
  RED_LED(OFF);
  GREEN_LED(OFF);
  
  
}









