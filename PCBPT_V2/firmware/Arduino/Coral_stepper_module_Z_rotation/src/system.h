#pragma once

#include <Arduino.h>

#define CORAL_RORATION 0

#define CORAL_NUM 1

// #include "stepper_control.h"

/*
 * if the module also controls a servo, define this.
 * if not, comment this
*/
// #define _W_SERVO_
// #define HOME_POS    -20

#define SERIAL_DEBUG false

#define I2C_ADDRESS  0x36

#define LED_ON  LOW
#define LED_OFF HIGH

// RGB LED pins
#define RGB_R_PIN D17
#define RGB_G_PIN D16
#define RGB_B_PIN D25

// Stepper pins
#define STEP_PIN  D26
#define DIR_PIN   D27

extern volatile bool Coral_homed;

extern void pin_init(void);

extern void TURN_ON_LED(bool R, bool G, bool B);
