#include <Arduino.h>

#include "system.h"
#include "cmd.h"
#include "coral_stepper.h"
#include "servo_control.h"

// Core #1: I2C comm
// Core #2: stepper control

void setup() {
  pin_init();
  comm_init();

  if (Coral.HAS_SERVO) {
    servo_init();
  }

  if (SERIAL_DEBUG) {
    Serial.begin(115200);
    while (!Serial) {
      sleep_ms(10);
    }
  }
}

void setup1() {
  ;
}

void loop() {
  handle_data();
}

void loop1() {
  control_motion();
}






