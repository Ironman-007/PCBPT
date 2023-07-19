#include <Arduino.h>

#include "stepper_control.h"

volatile bool output_flag = false;
volatile bool output_done = false;

volatile bool pulse_flag  = false;
volatile bool toggle_stat = false;

volatile uint32_t pulse_i = 0;

static volatile bool int_flag = false;

/*
void output_pulses(void) {
  int_flag = true;
}

void output_PWM(uint32_t steps) {
  while (pulse_i < 2*steps) {
    if (int_flag) {
      int_flag = false;
      digitalWrite(STEP_PIN, toggle_stat);
      toggle_stat = !toggle_stat;

      pulse_i ++;
    }
  }
  output_flag = false;
  output_done = true;
  output_done = true;
  pulse_i     = 0;
}
*/

void stepper_init(uint8_t resolution) {
  digitalWrite(MS1_PIN, (resolution & MS1_MASK)? HIGH:LOW);
  digitalWrite(MS2_PIN, (resolution & MS2_MASK)? HIGH:LOW);
  digitalWrite(MS3_PIN, (resolution & MS3_MASK)? HIGH:LOW);
}

void output_pulses(void) {
  if (output_flag) {
    digitalWrite(STEP_PIN, toggle_stat);
    toggle_stat = !toggle_stat;

    pulse_i ++;
  }
}

void output_PWM(uint32_t steps) {
  if (pulse_i == steps*2) {
    output_flag = false;
    output_done = true;
    pulse_i     = 0;
  }
}

