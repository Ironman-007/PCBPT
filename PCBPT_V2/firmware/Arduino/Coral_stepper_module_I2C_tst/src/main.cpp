#include <Wire.h>

volatile uint8_t cmd      = 0x00;

void setup() {
  Wire1.begin(50);
  Serial.begin(115200);
  while(!Serial) delay(10);
  Serial.println("Setup done.");
}

void loop() {
  if (Wire1.available()) {
    cmd = Wire1.read();
    Serial.println(cmd);
  }
}


