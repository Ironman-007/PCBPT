#pragma once

#include <Arduino.h>

#include "Servo.h"

#define SERVO_PIN D29

#define SERVO_PROBE_ANGLE   10
#define SERVO_UNPROBE_ANGLE 160

extern void servo_init(void);
extern void probe(int value);

extern Servo coral_servo;

// #define 