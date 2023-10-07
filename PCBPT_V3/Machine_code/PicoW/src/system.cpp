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

void pin_init(void) {
  pinMode(LIMIT_SW_1_PIN, INPUT);
  pinMode(LIMIT_SW_2_PIN, INPUT);
  pinMode(LIMIT_SW_3_PIN, INPUT);
  pinMode(LIMIT_SW_4_PIN, INPUT);
  pinMode(LIMIT_SW_5_PIN, INPUT);
  pinMode(LIMIT_SW_6_PIN, INPUT);
}