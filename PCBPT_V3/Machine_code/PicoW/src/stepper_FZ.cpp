#include <AccelStepper.h>

#include "stepper_FZ.h"
#include "comm.h"
#include "system.h"

float motion_A = 0.0;
float motion_B = 0.0;
float motion_X = 0.0;
float motion_Y = 0.0;

AccelStepper stepper1(AccelStepper::DRIVER, D15, D14); // 
AccelStepper stepper2(AccelStepper::DRIVER, D11, D10); //
AccelStepper stepper3(AccelStepper::DRIVER, D7, D6);   //
AccelStepper stepper4(AccelStepper::DRIVER, D13, D12); // 
AccelStepper stepper5(AccelStepper::DRIVER, D9, D8);   //
AccelStepper stepper6(AccelStepper::DRIVER, D5, D4);   //
AccelStepper stepper7(AccelStepper::DRIVER, D3, D2);   // 
AccelStepper stepper8(AccelStepper::DRIVER, D1, D0);   // 

void stepper_init(uint8_t speed_option) {
  if (speed_option == MOTION_SPEED) {
    stepper1.setMaxSpeed(1000);
    stepper1.setAcceleration(2000);
    stepper1.setCurrentPosition(MOTOR_1_HOME_POS);

    stepper2.setMaxSpeed(1000);
    stepper2.setAcceleration(2000);
    stepper2.setCurrentPosition(MOTOR_2_HOME_POS);

    stepper3.setMaxSpeed(3000);
    stepper3.setAcceleration(6000);
    stepper3.setCurrentPosition(MOTOR_3_HOME_POS);

    stepper4.setMaxSpeed(1000);
    stepper4.setAcceleration(2000);
    stepper4.setCurrentPosition(MOTOR_4_HOME_POS);

    stepper5.setMaxSpeed(1000);
    stepper5.setAcceleration(2000);
    stepper5.setCurrentPosition(MOTOR_5_HOME_POS);

    stepper6.setMaxSpeed(1000);
    stepper6.setAcceleration(2000);
    stepper6.setCurrentPosition(MOTOR_6_HOME_POS);

    // stepper7.setMaxSpeed(2000);
    // stepper7.setAcceleration(6000);
    // stepper1.setCurrentPosition(MOTOR_7_HOME_POS);

    // stepper8.setMaxSpeed(2000);
    // stepper8.setAcceleration(6000);
    // stepper1.setCurrentPosition(MOTOR_8_HOME_POS);
  }
  else {
    stepper1.setMaxSpeed(500);
    stepper1.setAcceleration(1000);
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
    // stepper7.setMaxSpeed(500);
    // stepper7.setAcceleration(1000);
    // stepper8.setMaxSpeed(500);
    // stepper8.setAcceleration(1000);
  }
}

void home_stepper(AccelStepper *stepper, int pos, uint8_t limit_sw_pin, long home_pos) {
  stepper->move(pos);
  while (digitalRead(limit_sw_pin)) {
    stepper->run();
  }
  stepper->setCurrentPosition(home_pos);
}

void home_machine(void) {
  stepper_init(HOME_SPEED);

  home_stepper(&stepper3, -50000, LIMIT_SW_3_PIN, MOTOR_3_HOME_POS);
  home_stepper(&stepper1, -50000, LIMIT_SW_1_PIN, MOTOR_1_HOME_POS);
  home_stepper(&stepper2, -50000, LIMIT_SW_2_PIN, MOTOR_2_HOME_POS);

  // home_stepper(&stepper4, -50000, LIMIT_SW_4_PIN, MOTOR_4_HOME_POS);
  // home_stepper(&stepper5, -50000, LIMIT_SW_5_PIN, MOTOR_5_HOME_POS);
  // home_stepper(&stepper6, -50000, LIMIT_SW_6_PIN, MOTOR_6_HOME_POS);

  stepper_init(MOTION_SPEED);
}

void start_motion(void) {

  stepper3.moveTo(MOTOR_3_PROBE_INI_POS);

  while(stepper3.distanceToGo()) {
    stepper3.run();
  }

  if (recv_CMD.cmd_type == MANUCAL_CTRL_CMD) {
    if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS)) {
      long pos_A = (recv_CMD.A_position_f)/stepper_res_1;
      stepper1.move(pos_A);
    }
    if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS)) {
      long pos_B = (recv_CMD.B_position_f)/stepper_res_1;
      stepper2.move(pos_B);
    }

    while(stepper1.distanceToGo() || stepper2.distanceToGo()) {
      stepper1.run();
      stepper2.run();
    }
  }

  else {
    if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS)) {
      long pos_A = (recv_CMD.A_position_f + LASER_BIAS_1)/stepper_res_1;
      stepper1.moveTo(pos_A);
    }
    if (recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS)) {
      long pos_B = (recv_CMD.B_position_f + LASER_BIAS_2)/stepper_res_1;
      stepper2.moveTo(pos_B);
    }

    while(stepper1.distanceToGo() || stepper2.distanceToGo()) {
      stepper1.run();
      stepper2.run();
    }
  }

  if (recv_CMD.cmd_type == PROBE_CMD) {
    stepper3.moveTo(MOTOR_3_PROBE_POS);

    while(stepper3.distanceToGo()) {
      stepper3.run();
    }
  }
}




