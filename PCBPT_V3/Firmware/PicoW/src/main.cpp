//
// Created by Fangzheng on 9/5/2023.
//
#include "Arduino.h"
#include "pico/stdlib.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void setup() {
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
}

void loop() {
  gpio_put(LED_PIN, 1);
  sleep_ms(500);
  gpio_put(LED_PIN, 0);
  sleep_ms(500);
};
