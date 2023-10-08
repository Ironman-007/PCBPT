#include <AccelStepper.h>

#include "stepper_FZ.h"
#include "comm.h"
#include "system.h"

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

<<<<<<< HEAD
void stepper_init(uint8_t speed_option) {
  if (speed_option == MOTION_SPEED) {
    stepper1.setMaxSpeed(2000);
    stepper1.setAcceleration(6000);
    stepper1.setCurrentPosition(0);
    stepper2.setMaxSpeed(2000);
    stepper2.setAcceleration(6000);
    stepper3.setMaxSpeed(2000);
    stepper3.setAcceleration(6000);
    stepper4.setMaxSpeed(2000);
    stepper4.setAcceleration(6000);
    stepper5.setMaxSpeed(2000);
    stepper5.setAcceleration(6000);
    stepper6.setMaxSpeed(2000);
    stepper6.setAcceleration(6000);
    stepper7.setMaxSpeed(2000);
    stepper7.setAcceleration(6000);
    stepper7.setCurrentPosition(0);
    stepper8.setMaxSpeed(2000);
    stepper8.setAcceleration(6000);
  }
  else {
    stepper1.setMaxSpeed(500);
    stepper1.setAcceleration(1000);
    stepper1.setCurrentPosition(0);
    stepper2.setMaxSpeed(500);
    stepper2.setAcceleration(1000);
    stepper3.setMaxSpeed(500);
    stepper3.setAcceleration(1000);
    stepper4.setMaxSpeed(500);
    stepper4.setAcceleration(1000);
    stepper5.setMaxSpeed(500);
    stepper5.setAcceleration(1000);
    stepper6.setMaxSpeed(500);
    stepper6.setAcceleration(1000);
    stepper7.setMaxSpeed(500);
    stepper7.setAcceleration(1000);
    stepper7.setCurrentPosition(0);
    stepper8.setMaxSpeed(500);
    stepper8.setAcceleration(1000);
  }
}

void home_stepper(void) {
  stepper7.move(-50000);
  while (digitalRead(LIMIT_SW_1_PIN)) {
    stepper7.run();
  }
   stepper7.setCurrentPosition(0);
}

void home_machine(void) {
  stepper_init(HOME_SPEED);
  home_stepper();
  stepper_init(MOTION_SPEED);
=======
void stepper_init(void) {
  stepper1.setMaxSpeed(2000);
  stepper1.setAcceleration(6000);
  stepper1.setCurrentPosition(0);
  stepper2.setMaxSpeed(2000);
  stepper2.setAcceleration(6000);
  stepper3.setMaxSpeed(2000);
  stepper3.setAcceleration(6000);
  stepper4.setMaxSpeed(2000);
  stepper4.setAcceleration(6000);
  stepper5.setMaxSpeed(2000);
  stepper5.setAcceleration(6000);
  stepper6.setMaxSpeed(2000);
  stepper6.setAcceleration(6000);
  stepper7.setMaxSpeed(2000);
  stepper7.setAcceleration(6000);
  stepper7.setCurrentPosition(0);
  stepper8.setMaxSpeed(2000);
  stepper8.setAcceleration(6000);
>>>>>>> 14ecc97add2df48317fe627889a14f7b62b5eadb
}

void start_motion(void) {
  if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS)) {
<<<<<<< HEAD
    long pos_A = (recv_CMD.A_position_f)/stepper_res_1;
    stepper7.moveTo(pos_A);
  }
  if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS)) {
    long pos_B = (recv_CMD.B_position_f)/stepper_res_1;
    stepper1.moveTo(pos_B);
=======
    // Serial.println("A move");
    long pos_A = (recv_CMD.A_position_f)/stepper_res_1;
    // Serial.println(pos_A);
    stepper7.moveTo(pos_A);
  }
  if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS)) {
    stepper1.moveTo((recv_CMD.B_position_f)/stepper_res_1);
    stepper1.run();
>>>>>>> 14ecc97add2df48317fe627889a14f7b62b5eadb
  }

  while(stepper7.distanceToGo() || stepper1.distanceToGo()) {
    stepper1.run();
    stepper7.run();
  }

  // if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_X_POS)) {
  //   stepper3.moveTo((recv_CMD.X_position_f)/stepper_res_1);
  //   stepper3.run();
  // }
  // if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_Y_POS)) {
  //   stepper4.moveTo((recv_CMD.Y_position_f)/stepper_res_1);
  //   stepper4.run();
  // }
}




