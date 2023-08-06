#include <Arduino.h>

#include "coral_stepper.h"
#include "cmd.h"
#include "servo_control.h"

volatile bool output_flag   = false;
volatile bool output_done   = false;

volatile bool pulse_flag    = false;
volatile bool toggle_stat   = false;

volatile bool stepper_homed = false;

coral_stepper::coral_stepper(uint8_t resolution, uint32_t interval) {
  this -> microstepping_res = resolution;
  this -> current_pos       = Coral.HOME_POS;
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

void coral_stepper::run(float distance) {
  set_coral_state_done(MOVING);

  float distance_to_move     = distance;

  uint64_t steps_to_move     = 0;

  if (distance_to_move > 0) {
    // digitalWrite(DIR_PIN, Coral.DIR2HOME);
    digitalWrite(DIR_PIN, Coral.DIR2BOARD);
  }
  else {
    distance_to_move = -1 * distance_to_move;
    // digitalWrite(DIR_PIN, Coral.DIR2BOARD);
    digitalWrite(DIR_PIN, Coral.DIR2HOME);
  }

  steps_to_move = floor(distance_to_move/COE_D_TO_STEPS_1_2);

  if (SERIAL_DEBUG) {
    Serial.print("steps_to_move_1_1 = ");
    Serial.println(steps_to_move);
  }

  this -> set_resolution(MICROSTEPPING_1_2);

  uint64_t _steps2move   = 2 * steps_to_move;
  uint64_t _steps2move_i = 0;
  uint32_t _interval     = this->step_interval;

  for (_steps2move_i = 0; _steps2move_i < _steps2move; _steps2move_i ++) {
    toggle_stat = !toggle_stat;
    digitalWrite(STEP_PIN, toggle_stat);
    delayMicroseconds(_interval);
  }

  steps_to_move = round((distance_to_move - steps_to_move*COE_D_TO_STEPS_1_2)/COE_D_TO_STEPS_1_8);

  if (SERIAL_DEBUG) {
    Serial.print("steps_to_move_1_8 = ");
    Serial.println(steps_to_move);
  }

  this -> set_resolution(MICROSTEPPING_1_8);

  _steps2move   = 2 * steps_to_move;
  _steps2move_i = 0;

  for (_steps2move_i = 0; _steps2move_i < _steps2move; _steps2move_i ++) {
    toggle_stat = !toggle_stat;
    digitalWrite(STEP_PIN, toggle_stat);
    delayMicroseconds(_interval);
  }
  set_coral_state_done(MOTION_DONE);
}

void coral_stepper::home(void) {
  set_coral_state_done(MOVING);

  if (Coral.HAS_SERVO) {
    probe(UNPROBE_POS); // Lift up the probe
    if (SERIAL_DEBUG) Serial.println("probe(UNPROBE_POS)");
  }

  uint32_t _interval = this->step_interval;

  digitalWrite(DIR_PIN, Coral.DIR2HOME);

  while(!digitalRead(LIMIT_SW_0)) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(_interval);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(_interval);
  }

  if (Coral.IS_ROT) {
    this -> set_resolution(MICROSTEPPING_1_8);
    digitalWrite(DIR_PIN, Coral.DIR2BOARD);

    uint64_t _steps2move   = Coral.RIGHT_ANGLE_STEPS;
    uint64_t _steps2move_i = 0;

    for (_steps2move_i = 0; _steps2move_i < _steps2move; _steps2move_i ++) {
      toggle_stat = !toggle_stat;
      digitalWrite(STEP_PIN, toggle_stat);
      delayMicroseconds(_interval);
    }
  }
  set_coral_state_done(MOTION_DONE);
}

coral_stepper Coral_stepper(MICROSTEPPING_1_4, 1000);



