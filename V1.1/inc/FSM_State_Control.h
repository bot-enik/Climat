









#ifndef FSM_STATE_CONTROL_H
#define FSM_STATE_CONTROL_H

#include "stm32l1xx.h"
#include "main.h"
#include "HDC_I2C.h"
#include "GPIO_PERIPH.h"
#include "Messages.h"
#include "Timers.h"

#define ACTIVE_SC      (0xFF)
#define DISABLE_SC     (0x00)
 
#define HEAT_HIGH_TEMP (0x03)
#define HEAT_LOW_TEMP  (0x33)

#define PWR_HIGH_TEMP  (0x12)
#define PWR_LOW_TEMP   (0x44)

void State_Control_Process_FSM(uint8_t *state_ptr, uint8_t *sc_past_state_ptr, uint8_t *error_type_p,
                               uint16_t revolutions_min, uint16_t revolutions_max,
                               uint32_t revolutions_fb, uint16_t *fansState,
                               struct forward_data_str_Temp *ptrTempHum );

#endif