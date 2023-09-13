//
// Created by Fangzheng on 9/5/2023.
//

#include <stdio.h>

#include "Arduino.h"
#include "pico/stdlib.h"

#include "hardware/address_mapped.h"
#include "hardware/platform_defs.h"
#include "hardware/uart.h"

#include "hardware/structs/uart.h"
#include "hardware/resets.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#include "pico/assert.h"
#include "pico.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "comm.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void setup() {
  comm_init();

  Serial.printf("ADC Example, measuring GPIO26\n");
  adc_init();
  adc_gpio_init(26);
  adc_select_input(0);
}

void loop() {
  // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
  const float conversion_factor = 3.3f / (1 << 12);
  uint16_t result = adc_read();
  Serial.printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
  sleep_ms(500);
};
