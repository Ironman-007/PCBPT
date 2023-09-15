//
// Created by l1534 on 9/14/2023.
//

#include "Arduino.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cmd.h"

using namespace std;

cmd recv_CMD = cmd();

cmd::cmd() {
  this -> cmd_type = IDLE_CMD;
  this -> CMD_REG = 0x00;

  memset(A_position, 0, sizeof(A_position));
  memset(B_position, 0, sizeof(B_position));
  memset(X_position, 0, sizeof(X_position));
  memset(Y_position, 0, sizeof(Y_position));

  A_position_f = 0.0;
  B_position_f = 0.0;
  X_position_f = 0.0;
  Y_position_f = 0.0;
}

void cmd::init_cmd() {
  this -> cmd_type = IDLE_CMD;
  this -> CMD_REG = 0x00;

  memset(A_position, 0, sizeof(A_position));
  memset(B_position, 0, sizeof(B_position));
  memset(X_position, 0, sizeof(X_position));
  memset(Y_position, 0, sizeof(Y_position));

  A_position_f = 0.0;
  B_position_f = 0.0;
  X_position_f = 0.0;
  Y_position_f = 0.0;
}

void cmd::set_cmd_TYPE(CMD_TYPE type) {
  this -> cmd_type = type;
}

void cmd::set_cmd_REG(uint8_t pos, uint8_t data) {
  this -> CMD_REG &= ~(0x01 << pos); // clear the bit
  this -> CMD_REG |=  (data << pos); // set a bit
}

void cmd::tst_function() {
  int print_i = 0;

//  for (print_i = 0; A_position[print_i] > 0; print_i++) {
//    position_string += A_position[print_i];
//  }

  A_position_f = std::atof((char *)A_position);
  B_position_f = std::atof((char *)B_position);
  X_position_f = std::atof((char *)X_position);
  Y_position_f = std::atof((char *)Y_position);

  Serial.printf("A_position_f=%f\n", A_position_f);
  Serial.printf("B_position_f=%f\n", B_position_f);
  Serial.printf("X_position_f=%f\n", X_position_f);
  Serial.printf("Y_position_f=%f\n", Y_position_f);

//  for (print_i = 0; print_i < 10; print_i++) {
//    Serial.printf("B_position[%d]=%c\n", print_i, B_position[print_i]);
//  }
//  for (print_i = 0; print_i < 10; print_i++) {
//    Serial.printf("X_position[%d]=%c\n", print_i, X_position[print_i]);
//  }
//  for (print_i = 0; print_i < 10; print_i++) {
//    Serial.printf("Y_position[%d]=%c\n", print_i, Y_position[print_i]);
//  }

}



