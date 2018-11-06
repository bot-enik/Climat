#ifndef FSM_FB_CONTROL_H
#define FSM_FB_CONTROL_H

#include "stm32l1xx.h"
#include "main.h"
#include "ADC.h"
#include "Messages.h"
#include "Timers.h"

#define ACTIVE_FBC      (0xFF)
#define DISABLE_FBC     (0x00)

void Feedback_Conrol_Process_FSM(struct forward_data_str_ADC *ADC_ptr,
                                 uint8_t *feedback_state_p, uint8_t *curent_state_p,
                                 uint8_t *error_type_p);
#endif