#include <AccelStepper.h>

#include "stepper_FZ.h"

float motion_A = 0.0;
float motion_B = 0.0;
float motion_X = 0.0;
float motion_Y = 0.0;

AccelStepper stepper1(AccelStepper::DRIVER, D3, D2);
AccelStepper stepper2(AccelStepper::DRIVER, D1, D0);
AccelStepper stepper3(AccelStepper::DRIVER, D5, D4);
AccelStepper stepper4(AccelStepper::DRIVER, D7, D6);
AccelStepper stepper5(AccelStepper::DRIVER, D9, D8);
AccelStepper stepper6(AccelStepper::DRIVER, D11, D10);
AccelStepper stepper7(AccelStepper::DRIVER, D15, D14);
AccelStepper stepper8(AccelStepper::DRIVER, D13, D12);

void stepper_init(void) {
  stepper1.setMaxSpeed(150.0);
  stepper1.setAcceleration(200.0);
  stepper2.setMaxSpeed(150.0);
  stepper2.setAcceleration(200.0);
  stepper3.setMaxSpeed(150.0);
  stepper3.setAcceleration(200.0);
  stepper4.setMaxSpeed(150.0);
  stepper4.setAcceleration(200.0);
  stepper5.setMaxSpeed(150.0);
  stepper5.setAcceleration(200.0);
  stepper6.setMaxSpeed(150.0);
  stepper6.setAcceleration(200.0);
  stepper7.setMaxSpeed(150.0);
  stepper7.setAcceleration(200.0);
  stepper8.setMaxSpeed(150.0);
  stepper8.setAcceleration(200.0);
}