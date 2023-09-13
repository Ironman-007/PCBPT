//
// Created by Fangzheng on 9/13/2023.
//

#include "Arduino.h"
#include "comm.h"
#include "stepper_FZ.h"

bool probe_cmd = false;

uint8_t cmd_in[100] = {0};

static int i = 0;

void comm_init(void) {
  Serial.begin(115200);
}

static void comm_handle_cmd(uint8_t * cmd) {
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
}

void comm_update(void) {
  if (Serial.available()) {
    uint8_t reacv_c = Serial.read();
    if (reacv_c != '\n') {
      cmd_in[i] = reacv_c;
      i++;
    }
    else {
      i = 0;
      comm_handle_cmd(cmd_in);
    }
  }
}
