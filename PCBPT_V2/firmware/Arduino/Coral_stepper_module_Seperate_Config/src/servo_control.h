#pragma once

#include <Arduino.h>

#include "Servo.h"

#define SERVO_PIN D29

#define PROBE_POS   100
#define UNPROBE_POS 0

extern void servo_init(void);
extern void probe(int value);

extern Servo coral_servo;

// #define 