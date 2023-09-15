//
// Created by l1534 on 9/13/2023.
//

#ifndef __STEPPER_FZ_H
#define __STEPPER_FZ_H

#include "Arduino.h"

extern float motion_A;
extern float motion_B;
extern float motion_X;
extern float motion_Y;

//class stepper_FZ {
//public:
//    stepper_FZ(uint step_pin, uint dir_pin, uint limit_pin, );
//private:
//
//};
//
//extern stepper_FZ motor_A;
//extern stepper_FZ motor_X;
//extern stepper_FZ motor_Y;
//
//extern stepper_FZ motor_B;
//extern stepper_FZ motor_P;
//extern stepper_FZ motor_Q;

class stepper_FZ {
public:
  stepper_FZ(uint8_t pin_dir, uint8_t pin_step,
             uint8_t pin_m0, uint8_t pin_m1, uint8_t pin_m2);
  ~stepper_FZ();

private:
};

#endif //PICOW_STEPPER_FZ_H
