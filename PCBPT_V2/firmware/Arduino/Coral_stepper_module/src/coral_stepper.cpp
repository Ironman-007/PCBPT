#include <Arduino.h>

#include "coral_stepper.h"

volatile bool output_flag     = false;
volatile bool output_done     = false;

volatile bool pulse_flag      = false;
volatile bool toggle_stat     = false;

coral_stepper::coral_stepper(uint8_t resolution, uint32_t interval) {
  this -> microstepping_res = resolution;
  this -> current_pos       = 0.0;
  this -> next_pos          = 0.0;
  this -> acceleration      = 0;
  this -> steps2move        = 0;
  this -> step_interval     = interval;

  digitalWrite(MS1_PIN, (this -> microstepping_res & MS1_MASK)? HIGH:LOW);
  digitalWrite(MS2_PIN, (this -> microstepping_res & MS2_MASK)? HIGH:LOW);
  digitalWrite(MS3_PIN, (this -> microstepping_res & MS3_MASK)? HIGH:LOW);
}

coral_stepper::~coral_stepper() {
  ;
}

void coral_stepper::set_resolution(uint8_t resolution) {
  this -> microstepping_res = resolution;

  digitalWrite(MS1_PIN, (this -> microstepping_res & MS1_MASK)? HIGH:LOW);
  digitalWrite(MS2_PIN, (this -> microstepping_res & MS2_MASK)? HIGH:LOW);
  digitalWrite(MS3_PIN, (this -> microstepping_res & MS3_MASK)? HIGH:LOW);
}

void coral_stepper::set_accel(int acc) {
  this -> acceleration = acc;
}

void coral_stepper::set_current_pos(float pos) {
  this -> current_pos = pos;
}

void coral_stepper::run(uint64_t steps) {
  uint64_t _steps2move   = 2 * steps;
  uint64_t _steps2move_i = 0;
  uint32_t _interval     = this->step_interval;

  for (_steps2move_i = 0; _steps2move_i < _steps2move; _steps2move_i ++) {
    toggle_stat = !toggle_stat;
    digitalWrite(STEP_PIN, toggle_stat);
    delayMicroseconds(_interval);
  }
}

coral_stepper Coral_stepper(MICROSTEPPING_1_8, 1000);

