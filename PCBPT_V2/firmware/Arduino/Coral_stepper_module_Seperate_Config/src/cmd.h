#pragma once

#include <Wire.h>

#include "coral_stepper.h"

#define MOTION_DONE 1
#define MOVING      0

extern volatile bool got_data;

extern volatile uint8_t cmd;

extern volatile uint8_t data_recv_cnt;

extern void comm_init(void);
extern void handle_data(void);
extern void control_motion(void);

struct Coral_state {
    volatile uint8_t motion_done;

    volatile float   currentpos;
    volatile float   targetpos;
};

extern Coral_state coral_state;

extern void set_coral_state(uint8_t done, float current, float target);
extern void set_coral_state_done(uint8_t done);
extern void set_coral_state_currentpos(float current);
extern void set_coral_state_targetpos(float target);

extern void report_state(void);