#pragma once

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MICROSTEP_1_1  0
#define MICROSTEP_1_2  1
#define MICROSTEP_1_4  2
#define MICROSTEP_1_8  3
#define MICROSTEP_1_16 7

extern int MS[3]; // MS3, MS2, MS1 - microstepping control

extern void stepper_init(uint8_t misro_stepping_confg);

extern void move_steps(uint8_t steps, uint32_t delay_time);

#ifdef __cplusplus
}
#endif