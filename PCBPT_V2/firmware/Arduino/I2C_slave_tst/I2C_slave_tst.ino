#include <Wire.h>

uint8_t cmd = 0x00;

void setup() {
  Wire.begin(50);
  Serial.begin(115200);
  while(!Serial) delay(10);
  Serial.println("Setup done.");
}

void loop() {
  if (Wire.available()) {
    cmd = Wire.read();
    Serial.println(cmd);
  }
}


