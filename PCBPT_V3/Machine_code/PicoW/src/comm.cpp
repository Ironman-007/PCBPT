//
// Created by Fangzheng on 9/13/2023.
//

#include "Arduino.h"
#include "comm.h"
#include "stepper_FZ.h"
#include "system.h"

bool probe_cmd = false;

bool A_flag = false;
bool B_flag = false;
bool X_flag = false;
bool Y_flag = false;

uint8_t cmd_in[100] = {0};

uint8_t A_coo_c[10] = {0};
uint8_t B_coo_c[10] = {0};
uint8_t X_coo_c[10] = {0};
uint8_t Y_coo_c[10] = {0};

int i = 0;

void comm_init(void) {
  Serial.begin(115200);
  while(!Serial) {delay(10);}
  // Serial.println("PCBPT connected");
}

void comm_handle_cmd(const uint8_t * cmd, int len) {
  recv_CMD.init_cmd();

  if (cmd[0] == 'P') recv_CMD.set_cmd_TYPE(PROBE_CMD);
  if (cmd[0] == 'C') recv_CMD.set_cmd_TYPE(CALI_CMD);
  if (cmd[0] == 'T') recv_CMD.set_cmd_TYPE(MANUCAL_CTRL_CMD);
  if (cmd[0] == 'H') {
    recv_CMD.set_cmd_TYPE(HOME_CMD);
    home_machine();
    //  TODO: Home the machine:
    //    - Lift all probes first
    //    - Home X;
    //    - Home Y;
    //    - Stepper1/2/3/4/5/6/7/8.setCurrentposition(HOME);

    recv_CMD.init_cmd();
    //    homing_machine();
  }

  int cmd_i = 1;
  int distance_data_i;

  while (cmd_i < len) {
    if (cmd[cmd_i] == 'A') {
      recv_CMD.set_cmd_REG(CMD_A_POS, 0x01);
      cmd_i++;

      for (distance_data_i = 0; cmd[cmd_i] < 'A' && cmd_i < len;) {
        recv_CMD.A_position[distance_data_i] = cmd[cmd_i];
        distance_data_i++;
        cmd_i++;
      }
    }
    if (cmd[cmd_i] == 'B') {
      recv_CMD.set_cmd_REG(CMD_B_POS, 0x01);
      cmd_i++;

      for (distance_data_i = 0; cmd[cmd_i] < 'A' && cmd_i < len;) {
        recv_CMD.B_position[distance_data_i] = cmd[cmd_i];
        distance_data_i++;
        cmd_i++;
      }
    }
    if (cmd[cmd_i] == 'X') {
      recv_CMD.set_cmd_REG(CMD_X_POS, 0x01);
      cmd_i++;

      for (distance_data_i = 0; cmd[cmd_i] < 'A' && cmd_i < len;) {
        recv_CMD.X_position[distance_data_i] = cmd[cmd_i];
        distance_data_i++;
        cmd_i++;
      }
    }
    if (cmd[cmd_i] == 'Y') {
      recv_CMD.set_cmd_REG(CMD_Y_POS, 0x01);
      cmd_i++;

      for (distance_data_i = 0; cmd[cmd_i] < 'A' && cmd_i < len;) {
        recv_CMD.Y_position[distance_data_i] = cmd[cmd_i];
        distance_data_i++;
        cmd_i++;
      }
    }
  }
}

void comm_update(void) {
  if (Serial.available()) {
    uint8_t reacv_c = Serial.read();
    if (reacv_c != '\n') {
      cmd_in[i] = reacv_c;
      i++;
    }
    else {
      comm_handle_cmd(cmd_in, i);
      recv_CMD.tst_function();
      // start_motion_L();
      start_motion_R();
      // TODO: run command.
      i = 0;
    }
  }
}
