//
// Created by l1534 on 9/14/2023.
//

#include "Arduino.h"
#include "system.h"

void debug_info(char * output) {
  Serial.println(output);
}

void debug_info(int output) {
  Serial.println(output);
}
