#pragma once

#include <Wire.h>

#include "coral_stepper.h"

extern volatile bool got_data;

extern volatile uint8_t cmd;

extern volatile uint8_t data_recv_cnt;

extern void comm_init(void);
extern void handle_data(void);
extern void control_motion(void);

