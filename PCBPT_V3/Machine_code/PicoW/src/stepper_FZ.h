//
// Created by l1534 on 9/13/2023.
//

#ifndef __STEPPER_FZ_H
#define __STEPPER_FZ_H

#include "Arduino.h"

#define HOME_SPEED   0
#define MOTION_SPEED 1

#define stepper_res_1 0.1 // how much the linear motion is in one step

#define MOTOR_1_HOME_POS 0
#define MOTOR_2_HOME_POS 0
#define MOTOR_3_HOME_POS 0
#define MOTOR_4_HOME_POS 0
#define MOTOR_5_HOME_POS 0
#define MOTOR_6_HOME_POS 0

#define MOTOR_3_PROBE_POS     2700
#define MOTOR_3_PROBE_INI_POS 1500

#define MOTOR_6_PROBE_POS     2750
#define MOTOR_6_PROBE_INI_POS 1500

#define LASER_BIAS_1 19
#define LASER_BIAS_2 9

#define LASER_BIAS_3 125
#define LASER_BIAS_4 13.7
// #define LASER_BIAS_3 127.5
// #define LASER_BIAS_4 11.7

#define UNPROBE 0
#define PROBE   1

extern float motion_A;
extern float motion_B;
extern float motion_X;
extern float motion_Y;

#define X_SAFETY        3
#define X_SAFETY_SINGLE 100

#define MOVE   0
#define MOVETO 1

extern void stepper_init(uint8_t speed_option);

extern void start_motion_L(void);
extern void start_motion_R(void);

extern void home_machine(void);
extern void home_stepper(void);

extern void start_motion_X(uint8_t left, float left_pos, uint8_t right, float right_pos, uint8_t move_or_move_To);
extern void start_motion_Y(uint8_t left, float left_pos, uint8_t right, float right_pos, uint8_t move_or_move_To);
extern void probe(uint8_t probe_action, uint8_t left, uint8_t right);

#endif //PICOW_STEPPER_FZ_H
