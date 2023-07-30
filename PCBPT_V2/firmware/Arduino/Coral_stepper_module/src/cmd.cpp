#include "cmd.h"

volatile uint8_t cmd = 0x00;

volatile bool got_data;

void comm_init(void) {
  Wire1.begin(50);
}

void handle_data(void) {
  if (Wire1.available()) {
    cmd = Wire1.read();
    Serial.println(cmd);
    // Coral_stepper.run((uint64_t)cmd);
  }
}


