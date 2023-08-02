#include <PacketSerial.h>

#include "cmd.h"
#include "system.h"

#define RECV_DATA_LEN  5

volatile uint8_t cmd           = 0x00;
volatile uint8_t data_recv_cnt = 0;
volatile bool    got_data      = false;

float move2pos               = 0.0;

volatile float target_pos    = 0.0;
volatile float move_steps    = 0.0;
volatile float distance2move = 0.0;

uint8_t encoded_cmd[RECV_DATA_LEN] = {0x00};
uint8_t decoded_cmd[RECV_DATA_LEN] = {0x00};

COBS cobs;

void comm_init(void) {
  Wire1.begin(I2C_ADDRESS);
}

void handle_data(void) {
  if (!Coral_homed) {
    Coral_stepper.home(HOME_POS);
  }
  if (Wire1.available() > 0) {
    cmd = Wire1.read();
    if (cmd == 0) {
      data_recv_cnt = 0;
      cobs.decode(encoded_cmd, RECV_DATA_LEN, decoded_cmd);
      memcpy(&move2pos, decoded_cmd, RECV_DATA_LEN-1);
      target_pos = move2pos;

      if (SERIAL_DEBUG) {
        Serial.print("move2pos = ");
        Serial.println(target_pos);
      }

      got_data = true;

      /*
      if (target_pos <= HOME_POS) {
        Coral_stepper.home(HOME_POS);
      }
      distance2move = target_pos - Coral_stepper.current_pos;
      if (distance2move > 0) {
        // set direction;
      }
      else {
        distance2move = -1 * distance2move;
        // set direction;
      }
      move_steps = distance2move * COE_D_TO_STEPS;
      Coral_stepper.run(round(move_steps));

      if (SERIAL_DEBUG) {
        Serial.print("move_steps = ");
        Serial.println(round(move_steps));
      }
      */

    }
    else {
      encoded_cmd[data_recv_cnt] = cmd;
      data_recv_cnt ++;
    }
  }
}

void control_motion(void) {
  if (got_data) {
    got_data = false;

    if (target_pos <= HOME_POS) {
      Coral_stepper.home(HOME_POS);
    }

    distance2move = target_pos - Coral_stepper.current_pos;

    if (SERIAL_DEBUG) {
      Serial.print("distance2move = ");
      Serial.println(round(distance2move));
    }

    // distance2move = target_pos;

    /*
    if (SERIAL_DEBUG) {
      Serial.print("distance2move = ");
      Serial.println(round(distance2move));
    }

    if (distance2move > 0) {
      // set direction;
    }
    else {
      distance2move = -1 * distance2move;
      // set direction;
    }
    move_steps = distance2move * COE_D_TO_STEPS;
    Coral_stepper.run(round(move_steps));
    */
    Coral_stepper.run(distance2move);

    if (SERIAL_DEBUG) {
      Serial.print("move_steps = ");
      Serial.println(round(move_steps));
    }

    Coral_stepper.set_current_pos(target_pos);
  }
}
