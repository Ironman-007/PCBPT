#include <Arduino.h>

#include "system.h"
#include "cmd.h"
#include "coral_stepper.h"

void setup() {
  pin_init();
  comm_init();
  Serial.begin(115200);
  while (!Serial) {sleep_ms(10);}
}

// void setup1() {
//   ;
// }

void loop() {
  handle_data();
}

// void loop1() {
//   ;
// }






