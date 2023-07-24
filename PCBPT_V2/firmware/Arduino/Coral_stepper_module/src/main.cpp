#include <Arduino.h>

#include "system.h"
#include "cmd.h"
#include "coral_stepper.h"

void setup() {
  pin_init();
  comm_init();
  Serial.begin(115200);
  while (!Serial) {sleep_ms(10);}
  // Coral_stepper.set_resolution(MICROSTEPPING_1_8);
  // stepper_init(MICROSTEPPING_1_8);
}

// void setup1() {
  // Serial.begin(115200);
  // while (!Serial) {sleep_ms(10);}
// }

void loop() {
  handle_data();
  // output_PWM(100);
}

// void loop1() {
//   ;
// }






