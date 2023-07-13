#pragma once

#include <Arduino.h>

#include "system.h"
// #include "stepper_control.h"

/*
 * if the module also controls a servo, define this.
 * if not, comment this
*/
#define _W_SERVO_

extern SerialUSB SerialDebug;

#define PIN_NUM 5

#define LED_ON  LOW
#define LED_OFF HIGH

// RGB LED pins
#define RGB_R_PIN 17
#define RGB_G_PIN 16
#define RGB_B_PIN 25

// Stepper pins
#define STEP_PIN  D0
#define DIR_PIN   D1

extern void pin_init(void);

extern void TURN_ON_LED(bool R, bool G, bool B);
