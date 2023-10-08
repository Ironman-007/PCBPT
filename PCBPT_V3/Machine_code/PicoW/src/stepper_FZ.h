//
// Created by l1534 on 9/13/2023.
//

#ifndef __STEPPER_FZ_H
#define __STEPPER_FZ_H

#include "Arduino.h"

<<<<<<< HEAD
#define HOME_SPEED   0
#define MOTION_SPEED 1

=======
>>>>>>> 14ecc97add2df48317fe627889a14f7b62b5eadb
#define stepper_res_1 0.1 // how much the linear motion is in one step

extern float motion_A;
extern float motion_B;
extern float motion_X;
extern float motion_Y;

<<<<<<< HEAD
extern void stepper_init(uint8_t speed_option);

extern void start_motion(void);

extern void home_machine(void);
extern void home_stepper(void);

=======
extern void stepper_init(void);

extern void start_motion(void);

>>>>>>> 14ecc97add2df48317fe627889a14f7b62b5eadb
#endif //PICOW_STEPPER_FZ_H
