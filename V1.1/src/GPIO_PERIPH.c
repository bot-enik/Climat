#include "GPIO_PERIPH.h"

// Direct logic in gpio pins
// Means that ON = HIGH state

/* ============================================================================
                            ##### LED Control #####
 ============================================================================*/
void NORM_LED(uint8_t led) {

  // Turn on the led
  if (led == ON) {
      GPIO_SetBits(GPIOH, LED0);
      GPIO_SetBits(GPIOC, IO0_LEDG);
  }
  // Turn off the led
  else {
      GPIO_ResetBits(GPIOH, LED0);
      GPIO_ResetBits(GPIOC, IO0_LEDG);
  }  
  
}

void EMERG_LED(uint8_t led) {

  // Turn on the led
  if (led == ON) {
      GPIO_SetBits(GPIOH, LED1);
      GPIO_SetBits(GPIOC, IO1_LEDR);
  }
  // Turn off the led
  else {
      GPIO_ResetBits(GPIOH, LED1);
      GPIO_ResetBits(GPIOC, IO1_LEDR);
  }  
  
}

void GREEN_LED (uint8_t ledG) {
    NORM_LED(ledG);
}

void RED_LED (uint8_t ledR) {
    EMERG_LED(ledR);
}

void ORANGE_LED (uint8_t ledO) {
    NORM_LED(ledO);
    EMERG_LED(ledO);
}

/* ============================================================================
                       ##### Power and heater control #####
 ============================================================================*/

void Power_Control(uint8_t pwr) {

  // Turn on the power
  if (pwr == ON) {
      GPIO_SetBits(GPIOA, PWR1_EN);
  }
  // Turn off the power
  else {
      GPIO_ResetBits(GPIOA, PWR1_EN);
  }
  
}

void Heater1_Control(uint8_t pwr) {

  // Turn on the heater
  if (pwr == ON) {
      GPIO_SetBits(GPIOA, PWR2_EN);
  }
  // Turn off the heater
  else {
      GPIO_ResetBits(GPIOA, PWR2_EN);
  }
  
}

void Heater2_Control(uint8_t pwr) {

  // Turn on the heater
  if (pwr == ON) {
      GPIO_SetBits(GPIOA, PWR3_EN);
  }
  // Turn off the heater
  else {
      GPIO_ResetBits(GPIOA, PWR3_EN);
  }
  
}

/* ============================================================================
                           ##### FAN Control #####
 ============================================================================*/

void FanR_Set(uint8_t state, int16_t setVlt) {

  // Must be in range 0 to 4090
  if(setVlt < 0) {setVlt = 0;}
  if(setVlt > 4090) {setVlt = 4090;}
  
  if (state == OFF) {
      GPIO_ResetBits(GPIOB, FANR_EN);
  }
  else if (state == MAX) {
      GPIO_SetBits(GPIOB, FANR_EN);
      // Control function
      DAC_VREG_R_Set(0);
  } 
  else {
      GPIO_SetBits(GPIOB, FANR_EN);
      // Control function
      DAC_VREG_R_Set(setVlt);
      
  }

}

void FanM_Set(uint8_t state, int16_t setVlt) {
  
  // Must be in range 0 to 4090
  if(setVlt < 0) {setVlt = 0;}
  if(setVlt > 4090) {setVlt = 4090;}
  
  if (state == OFF) {
      GPIO_ResetBits(GPIOB, FANM1_EN);
      GPIO_ResetBits(GPIOA, FANM2_EN);  
  }
  else if (state == MAX) {
      GPIO_SetBits(GPIOB, FANM1_EN);
      GPIO_SetBits(GPIOA, FANM2_EN);
      // Control function
      DAC_VREG_M_Set(0);
  } 
  else {
      GPIO_SetBits(GPIOB, FANM1_EN);
      GPIO_SetBits(GPIOA, FANM2_EN);
      // Control function
      DAC_VREG_M_Set(setVlt);
  }

}
