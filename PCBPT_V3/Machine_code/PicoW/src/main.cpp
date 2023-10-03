//
// Created by Fangzheng on 9/5/2023.
//

#include <stdio.h>
#include <string>
#include <iostream>

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
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "stepper_FZ.h"
#include "comm.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void setup() {
  comm_init();
  stepper_init();
}

void loop() {
  comm_update();
};
