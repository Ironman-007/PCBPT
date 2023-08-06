#pragma once

#include <Arduino.h>

#include "Servo.h"

#define SERVO_PIN D29

#define PROBE_POS   140
#define UNPROBE_POS 10

extern void servo_init(void);
extern void probe(int value);

extern Servo coral_servo;

// #define 