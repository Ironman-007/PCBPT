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
}

void comm_handle_cmd(const uint8_t * cmd, int len) {
  recv_CMD.init_cmd();

  if (cmd[0] == 'P') recv_CMD.set_cmd_TYPE(PROBE_CMD);
  if (cmd[0] == 'C') recv_CMD.set_cmd_TYPE(CALI_CMD);
  if (cmd[0] == 'T') recv_CMD.set_cmd_TYPE(MANUCAL_CTRL_CMD);
  if (cmd[0] == 'S') recv_CMD.set_cmd_TYPE(SINGLE_PROBE_CMD);
  if (cmd[0] == 'H') recv_CMD.set_cmd_TYPE(HOME_CMD);

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

// IDLE_CMD,
// PROBE_CMD,
// CALI_CMD,
// MANUCAL_CTRL_CMD,
// HOME_CMD

void start_motion(CMD_TYPE cmdtype) {
  // start_motion_L();
  // start_motion_R();

  if (cmdtype == HOME_CMD) {
    home_machine();
  }
  if (cmdtype == CALI_CMD) {
    // probe(UNPROBE, 1, 1);

    // start_motion_X(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS), recv_CMD.A_position_f,
    //                recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_X_POS), recv_CMD.X_position_f,
    //                MOVETO);

    // start_motion_Y(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS), recv_CMD.B_position_f,
    //                recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_Y_POS), recv_CMD.Y_position_f,
    //                MOVETO);

    start_motion_L();
    start_motion_R();
  }
  if (cmdtype == PROBE_CMD) {
    probe(UNPROBE, 1, 1);

    start_motion_X(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS), recv_CMD.A_position_f - X_SAFETY,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_X_POS), recv_CMD.X_position_f + X_SAFETY,
                   MOVETO);

    start_motion_Y(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS), recv_CMD.B_position_f,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_Y_POS), recv_CMD.Y_position_f,
                   MOVETO);

    start_motion_X(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS), recv_CMD.A_position_f,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_X_POS), recv_CMD.X_position_f,
                   MOVETO);

    probe(PROBE, 1, 1);
  }
  if (cmdtype == SINGLE_PROBE_CMD) {
    probe(UNPROBE, 1, 1);

    start_motion_X(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS), recv_CMD.A_position_f,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_X_POS), recv_CMD.X_position_f + X_SAFETY_SINGLE,
                   MOVETO);

    start_motion_Y(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS), recv_CMD.B_position_f,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_Y_POS), recv_CMD.Y_position_f,
                   MOVETO);

    start_motion_X(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS), recv_CMD.A_position_f,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_X_POS), recv_CMD.X_position_f,
                   MOVETO);

    probe(PROBE, 1, 0);
  }
  if (cmdtype == MANUCAL_CTRL_CMD) {
    probe(UNPROBE, 1, 1);
    start_motion_X(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_A_POS), recv_CMD.A_position_f,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_X_POS), recv_CMD.X_position_f,
                   MOVE);

    start_motion_Y(recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_B_POS), recv_CMD.B_position_f,
                   recv_CMD.check_REG(recv_CMD.CMD_REG, CMD_Y_POS), recv_CMD.Y_position_f,
                   MOVE);
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
      recv_CMD.get_distance();
      start_motion(recv_CMD.cmd_type);

      i = 0;
    }
  }
}
