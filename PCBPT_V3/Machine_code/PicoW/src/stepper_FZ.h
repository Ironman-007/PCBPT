//
// Created by l1534 on 9/13/2023.
//

#ifndef __STEPPER_FZ_H
#define __STEPPER_FZ_H

#include "Arduino.h"

#define HOME_SPEED   0
#define MOTION_SPEED 1

#define stepper_res_1 0.1 // how much the linear motion is in one step

extern float motion_A;
extern float motion_B;
extern float motion_X;
extern float motion_Y;

extern void stepper_init(uint8_t speed_option);

extern void start_motion(void);

extern void home_machine(void);
extern void home_stepper(void);

#endif //PICOW_STEPPER_FZ_H
