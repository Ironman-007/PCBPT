#include <PacketSerial.h>

#include "cmd.h"

#define RECV_DATA_LEN  5
#define COE_D_TO_STEPS 40 // needs to be calibrated

volatile uint8_t cmd = 0x00;
volatile bool got_data;
volatile uint8_t data_recv_cnt = 0;

float    distance2move = 0.0;
uint64_t move_steps    = 0;

uint8_t encoded_cmd[RECV_DATA_LEN] = {0x00};
uint8_t decoded_cmd[RECV_DATA_LEN] = {0x00};

COBS cobs;

void comm_init(void) {
  Wire1.begin(50);
}

void handle_data(void) {
  if (Wire1.available() > 0) {
    cmd = Wire1.read();
    if (cmd == 0) {
      data_recv_cnt = 0;
      cobs.decode(encoded_cmd, RECV_DATA_LEN, decoded_cmd);
      memcpy(&distance2move, decoded_cmd, RECV_DATA_LEN-1);
      Serial.print("distance2move = "); Serial.println(distance2move);
      // TODO:
      //   - Record current position and calculate how much distance to move
      move_steps = distance2move * COE_D_TO_STEPS;
      Coral_stepper.run(move_steps)
    }
    else {
      encoded_cmd[data_recv_cnt] = cmd;
      data_recv_cnt ++;
    }
  }
}


