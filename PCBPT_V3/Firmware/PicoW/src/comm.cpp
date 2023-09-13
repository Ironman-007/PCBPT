//
// Created by Fangzheng on 9/13/2023.
//

#include "Arduino.h"
#include "comm.h"
#include "stepper_FZ.h"

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
}

void which_data(uint8_t data_flag) {
  if (data_flag == DATA_FOR_A) {
    bool A_flag = true;
    bool B_flag = false;
    bool X_flag = false;
    bool Y_flag = false;
  }
  if (data_flag == DATA_FOR_B) {
    bool A_flag = false;
    bool B_flag = true;
    bool X_flag = false;
    bool Y_flag = false;
  }
  if (data_flag == DATA_FOR_X) {
    bool A_flag = false;
    bool B_flag = false;
    bool X_flag = true;
    bool Y_flag = false;
  }
  if (data_flag == DATA_FOR_Y) {
    bool A_flag = false;
    bool B_flag = false;
    bool X_flag = false;
    bool Y_flag = true;
  }
  if (data_flag == DATA_FOR_NONE) {
    bool A_flag = false;
    bool B_flag = false;
    bool X_flag = false;
    bool Y_flag = false;
  }
}

void comm_handle_cmd(uint8_t * cmd, int len) {
  if (cmd[0] == 'P') {
    probe_cmd = true;
  }
  if (cmd[0] == 'C') {
    probe_cmd = false;
  }
  if (cmd[0] == 'H') {
//    TODO:
//    Home the machine:
//      - Lift all probes first
//      - Home X;
//      - Home Y;
    ;
  }

  int ii = 0;

  for (ii=1; ii<len; ii++) {
    int iii = 0;

    if (cmd[ii] == 'A') {
      which_data(DATA_FOR_A);
      iii = 0;
    }
    if (cmd[ii] == 'B') {
      which_data(DATA_FOR_B);
      iii = 0;
    }
    if (cmd[ii] == 'X') {
      which_data(DATA_FOR_X);
      iii = 0;
    }
    if (cmd[ii] == 'Y') {
      which_data(DATA_FOR_Y);
      iii = 0;
    }
    if (cmd[ii] == '\n') {
    // TODO: cleanup all buffer
    }

    if (A_flag) A_coo_c[iii++] = cmd[ii];
    if (B_flag) B_coo_c[iii++] = cmd[ii];
    if (X_flag) X_coo_c[iii++] = cmd[ii];
    if (Y_flag) Y_coo_c[iii++] = cmd[ii];
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
      i = 0;
    }
  }
}
