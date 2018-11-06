#ifndef FSM_GET_VAL_H
#define FSM_GET_VAL_H

#include "stm32l1xx.h"
#include "main.h"
#include "Fan_Speed.h"
#include "HDC_I2C.h"
#include "ADC.h"
#include "Messages.h"
#include "Timers.h"

#include "Report.h"


#define ACTIVE_GV      (0xF1)
#define READ_VAL_GV    (0x70)
#define DISABLE_GV     (0x10)

void Get_Val_Process_FSM(struct forward_data_str_Temp *ptrTempHum,
                         struct forward_data_str_ADC *ptrADC,
                         struct forward_data_str_Speed *ptrSpeed,
                         uint8_t *fan_to_measure, uint16_t *fans_states);

#endif