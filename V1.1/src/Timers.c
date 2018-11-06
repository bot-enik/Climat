#include "Timers.h"

// Creating virtual timers
// uses SWITCH and FSM 'ideology'

static volatile uint32_t TimeMs;

uint32_t Timers[MAX_TIMERS];

/* ============================================================================
                         ##### Core functions #####
 ============================================================================*/

void SysTick_Handler(void) {
  
  //SysTick_Handler()   SEE stm32l1xx_it.c
  TimeMs++;
   
  uint8_t i;
  for(i=0; i<MAX_TIMERS; i++) {
    Timers[i]++;
  }
  
} 

/* ============================================================================
                         ##### 'SWITCH' functions #####
 ============================================================================*/

void TimersInit(void) {
  
  uint8_t i;
  for(i=0; i<MAX_TIMERS; i++) {
    Timers[i] = 0;
  }
  // If dont use Main Timer uncomment this:
  // SysTick_Config(SystemCoreClock/1000); // Every 1 ms
}

uint32_t GetTimer(uint8_t Timer) {

  return Timers[Timer];

}

void ResetTimer(uint8_t Timer) {

  Timers[Timer] = 0;

}

/* ============================================================================
                         ##### 'FSM' functions #####
 ============================================================================*/

void MainTimerInit(void) {

  TimeMs = 0;
  SysTick_Config(SystemCoreClock/1000); // Every 1 ms
  
}


uint32_t GetTimeMs(void){

  return TimeMs;        
  
}

uint32_t MainTimerSet(uint32_t dop_time) {

  return TimeMs + dop_time; 
  
}

uint8_t MainTimerIsExpired(uint32_t Timer) {

  if((TimeMs - Timer) < (1UL << 31)) 
    return (Timer <= TimeMs);
  else
    return 0;
  
}

uint32_t MainTimerRemainingMs(uint32_t Timer){

  if ((TimeMs - Timer) < (1U << 31)) 
    return (TimeMs - Timer);
  else 
    return 0;

}
  
void blocking_Delay(uint32_t delay_time) {

  uint32_t time = MainTimerSet(delay_time);
  while(!(MainTimerIsExpired(time))){}
  
}

