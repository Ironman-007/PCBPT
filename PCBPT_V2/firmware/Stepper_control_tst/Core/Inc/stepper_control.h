#pragma once

#include "main.h"
#include "cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MICROSTEP_1_1  0
#define MICROSTEP_1_2  1
#define MICROSTEP_1_4  2
#define MICROSTEP_1_8  3
#define MICROSTEP_1_16 7

#define CLKDIR            GPIO_PIN_RESET
#define COUNTERCLKDIR     GPIO_PIN_SET

// delay time for different frequency
#define STEP_FREQ_10HZ    10
#define STEP_FREQ_50HZ    50
#define STEP_FREQ_100HZ   100
#define STEP_FREQ_200HZ   200
#define STEP_FREQ_500HZ   500

extern char Steps2move[CMD_BUFFER_LEN];

extern int MS[3]; // MS3, MS2, MS1 - microstepping control

extern void stepper_init(uint8_t misro_stepping_confg);

extern void move_steps(unsigned int direction, uint32_t steps);

#ifdef __cplusplus
}
#endif