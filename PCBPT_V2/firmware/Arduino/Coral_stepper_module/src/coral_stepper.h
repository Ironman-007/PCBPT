#pragma once

#include "system.h"

#define MICROSTEPPING_1_1    0x00
#define MICROSTEPPING_1_2    0x01
#define MICROSTEPPING_1_4    0x02
#define MICROSTEPPING_1_8    0x03
#define MICROSTEPPING_1_16   0x07

#define MS1_MASK             0x01
#define MS2_MASK             0x02
#define MS3_MASK             0x04

#define MS1_PIN              D7
#define MS2_PIN              D8
#define MS3_PIN              D9

extern volatile bool output_flag;
extern volatile bool output_done;

extern volatile uint32_t  pulse_i;

extern volatile bool pulse_flag;
extern volatile bool toggle_stat;

#define LED_TOGGLE_INTERVAL_MS  1000L
#define SIMPLE_TIMER_MS         2000L
#define TIMER_INTERVAL_MS       1L

extern volatile bool pulse_flag;

class coral_stepper {
private:

public:
  coral_stepper(uint8_t resolution);
  ~coral_stepper();

  uint8_t  microstepping_res;
  float    current_pos;  // current position
  float    next_pos;
  int      acceleration;
  uint32_t steps2move;
  uint32_t step_interval;

  void set_resolution(uint8_t resolution);
  void set_accel(int acc);
  void set_current_pos(float pos);
  void run(uint64_t steps); // move how many steps
};


extern coral_stepper Coral_stepper;


