#include <Arduino.h>

#include "system.h"

SerialUSB SerialDebug;

static uint8_t PINS2INIT[PIN_NUM]      = {RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, STEP_PIN, DIR_PIN};
static int     PINS2INIT_MODE[PIN_NUM] = {OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT};
static int     PINS2INIT_STAT[PIN_NUM] = {LED_OFF, LED_OFF, LED_OFF, LOW, LOW};

static int i = 0;

void pin_init(void) {
  for (i=0; i< PIN_NUM; i++) {
    pinMode(PINS2INIT[i], PINS2INIT_MODE[i]);
    digitalWrite(PINS2INIT[i], PINS2INIT_STAT[i]);
  }
}

void TURN_ON_LED(bool R=false, bool G=false, bool B=false) {
  digitalWrite(RGB_R_PIN, (R)? LED_ON:LED_OFF);
  digitalWrite(RGB_G_PIN, (G)? LED_ON:LED_OFF);
  digitalWrite(RGB_B_PIN, (B)? LED_ON:LED_OFF);
}
