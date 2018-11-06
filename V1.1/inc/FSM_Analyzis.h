#ifndef FSM_ANALYZIS_H
#define FSM_ANALYZIS_H

#include "stm32l1xx.h"
#include "main.h"
#include "HDC_I2C.h"
#include "Messages.h"
#include "Timers.h"


#define ACTIVE_Analyzis      (0xFF)
#define DISABLE_Analyzis     (0x00)

#define A                    (0x77)
#define B                    (0x44)

void Analyzis_Process_FSM ( struct forward_data_str_Temp *Temp_ptr,
                            uint8_t *past_state_p, uint8_t *state_count_p,
                            uint8_t *saved_state_p, uint8_t *feedback_state_p,
                            uint8_t *current_state_p, uint8_t *error_type_p);

#endif