#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdio.h>

/* ----------------------------- Libraries ----------------------------- */

// Standard Peripheral Library (SPL)
#include "stm32l1xx.h"

// Main drivers for periphery
#include "ADC.h"
#include "DAC.h"
#include "GPIO.h"
#include "I2C.h"
#include "UART.h"

// Control specific periphery
#include "GPIO_PERIPH.h"
#include "HDC_I2C.h" 
#include "MAX_I2C.h"

#include "Report.h"
#include "Fan_Speed.h"
#include "SpeedCounter.h"

// Higher level of control
#include "Fan_Control.h"
#include "LED_Control.h"

// Core functions for FSM system
#include "Messages.h"
#include "Timers.h"

// Finite state machines
#include "FSM_Get_Val.h"
#include "FSM_Analyzis.h"
#include "FSM_State_Control.h"
#include "FSM_FB_Control.h"


/* ----------------------------- Settings ----------------------------- */


           /**** SET '1' to ONE of modes, AND '0' to another ****/
// If you set each mode '1' or '0' there will be compile error!!!!!!!!!

// Heaters on, USART off. 
// Will define SAN_ACE_80 fan
#define PBS_MODE                 (1)

// Dont touch if dont know (must be 0)
#define GPIO_FEEDBACK_ON         (0)
#define DEBUG_ON                 (0)
// Look at the humidity values or not, to set the current state
#define HUMIDITY_USE             (0) 

// Heaters off, USART on.
// Will define SAN_ACE_92W fan
#define ATS_MODE                 (0)


              /**** Temperature and humidity constants ****/
// TR - treshold, HYST - hysteresis, HUM - humidity, INC - include, DIF - difference
// ALL VALUES MUST BE:
// TEMP_PWR_OFF < TEMP_PWR_ON < TEMP_HYST_LOW_TR < TEMP_HYST_HIGHT_TR < TEMP_UP_TR
#define TEMP_PWR_OFF             (1.0) //1
#define TEMP_PWR_ON              (5.0) //5
#define TEMP_HYST_LOW_TR         (40.0)
#define TEMP_HYST_HIGHT_TR       (50.0)
#define TEMP_UP_TR               (60.0)
#define TEMP_HYST_DIF            (5.0) // > 0
#define HUM_UP_TR                (84.0) 
#define HUM_UP_TR_INC_TEMP       (70.0)
#define TEMP_UP_TR_INC_HUM       (40.0)

// Allowable deviation of the temperature values from different sensors
#define MAX_TEMP_DEVIATION       (20.0)


                    /**** Define one of this fans: ****/
// If no one fan defined, program will set fan according to the mode

// #define  TEST_FAN
// #define  SAN_ACE_80
// #define  SAN_ACE_92W

#ifndef TEST_FAN
#ifndef SAN_ACE_80
#ifndef SAN_ACE_92W

#if (PBS_MODE == 1)
  #define  SAN_ACE_80
#endif

#if (ATS_MODE == 1)
  #define  SAN_ACE_92W
#endif

#endif
#endif
#endif

                       /**** Other settings  ****/
// Fans characteristics 
//(REV - revolutions)
#ifdef TEST_FAN
    #define MAX_REV_PER_MIN          (1500)
    #define PULSE_PER_REVOLUTION     (2)
    #define MAX_REV_PER_SEC          (25)
    #define MIN_REV_PER_SEC          (12)
#endif

#ifdef SAN_ACE_80
    #define MAX_REV_PER_MIN          (3000)
    #define PULSE_PER_REVOLUTION     (2)
    #define MAX_REV_PER_SEC          (50)
    #define MIN_REV_PER_SEC          (22)
#endif

#ifdef SAN_ACE_92W
    #define MAX_REV_PER_MIN          (2700)
    #define PULSE_PER_REVOLUTION     (2)
    #define MAX_REV_PER_SEC          (49)
    #define MIN_REV_PER_SEC          (21)
#endif

// When fan starts rotating first X [ms] it will be at MAX speed
#define FAN_START_DELAY          (2000)

// Delay for FB control [ms]
// (Because inertion of the system)
#define INERTION_DELAY           (50)

// Define how much the new state will not change current
// (end of FSM_Analyzis)
#define SENSITIVITY              (5)

// Delay time for waiting convertions of the periphery [ms]
// (HDC converts about 15 ms, SpeedCounter converts about 50 ms(when using EXTI))
#define DELAY_FOR_READING_MS     (50)

// Relative voltage on ADC (Input voltage and fans voltage)
#define INPUT_VOLTAGE_LOW_TR     (0)
#define INPUT_VOLTAGE_UP_TR      (0xFFFF)
#define WORKING_FAN_VLT_LOW_TR   (0)
#define FANS_VOLTAGE_UP_TR       (0xFFFF)

// Delay time for blinking leds [ms]
// (To indicate the state of the system)
#define HAD_BLINK_DELAY          (500)
#define N_BLINK_DELAY            (500)
#define TEMP_ERR_DELAY_1         (250)
#define TEMP_ERR_DELAY_2         (2000)
#define FAN_TEST_DELAY           (200)


/* -------------------------- End of settings -------------------------- */

// Define values for ON/OFF functions
#define ON                       (0x00)
#define OFF                      (0xFF)
#define MAX                      (0x0F) // fans when heating

// State names
#define HEATING_AND_DRYING       (0xF0)
#define NORMAL                   (0x0F)
#define EMERGENCY                (0xFF)
#define MOST_FANSOFF             (0x04)
#define FAN_TEST                 (0x63)

// Type of errors
#define NO_ERROR                 (0x00)

#define TEMP_ERROR               (0x10)
#define TEMP_EXCEED_TR           (0x11)  // MORE THAN UP TRESHOLD
#define TEMP_DEVIATION_ERROR     (0x12)  // BETWEEN SENSORS
#define HUM_INC_TEMP_ERROR       (0x13)  // TEMP + HUM > TEMP_UP_TR_INC_HUM

#define VLTG_ERROR               (0x21)  // INPUT VOLTAGE OUT OF TRESHOLD OR FAN VOLTAGE > UP TRESHOLD
#define FAN_VLTG_ERROR_LOW_TR    (0x22)  // FAN VOLTAGE < LOW TRESHOLD

#define GPIO_ERROR               (0x31)  // GPIO isnt set to needed value

#define PULSE_ERROR              (0x44)  //
#define ALERT                    (0x45)  // 

// Addresses of the I2C periphery
#define MAX1_ADDR                (0x90)
#define MAX2_ADDR                (0x90)
#define HDC_ADDR                 (0x80)

// Names of using GPIO pins(according to the scheme)
#define PWR1_EN                  (0x1U << 2)
#define PWR2_EN                  (0x1U << 1)
#define PWR3_EN                  (0x1U << 12)

#define PWRGD1                   (0x1U << 8)
#define PWRGD2                   (0x1U << 9)
#define PWRGD3                   (0x1U << 13)

#define FANM1_EN                 (0x1U << 5)
#define FANM2_EN                 (0x1U << 3)
#define FANR_EN                  (0x1U << 12)

#define LED0                     (0x1U)
#define LED1                     (0x1U << 1)

#define IO0_LEDG                 (0x1U << 14)
#define IO1_LEDR                 (0x1U << 15)


/* ----------------------------- Functions ----------------------------- */

void Clock_Setup(void);
void Full_Init(void);
void Fan_Test(void);


#endif