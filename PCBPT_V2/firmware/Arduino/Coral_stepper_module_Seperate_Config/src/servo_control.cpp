#include "servo_control.h"

Servo coral_servo;

void servo_init(void) {
    coral_servo.attach(SERVO_PIN);
}

void probe(int value) {
    coral_servo.write(value);
}

