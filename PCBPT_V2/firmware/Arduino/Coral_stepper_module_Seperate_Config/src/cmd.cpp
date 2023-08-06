#include <PacketSerial.h>

#include "cmd.h"
#include "system.h"
#include "servo_control.h"

#define RECV_DATA_LEN  5

volatile uint8_t cmd           = 0x00;
volatile uint8_t data_recv_cnt = 0;
volatile bool    got_data      = false;

Coral_state coral_state;

float move2pos               = 0.0;

volatile float target_pos    = 0.0;
volatile float move_steps    = 0.0;
volatile float distance2move = 0.0;

uint8_t encoded_cmd[RECV_DATA_LEN] = {0x00};
uint8_t decoded_cmd[RECV_DATA_LEN] = {0x00};

COBS cobs;

void set_coral_state(uint8_t done, float current, float target) {
  coral_state.motion_done = done;
  coral_state.currentpos  = current;
  coral_state.targetpos   = target;
}

void set_coral_state_done(uint8_t done) {
  if (SERIAL_DEBUG) {
    Serial.print("set_coral_state_done");
    Serial.println(done);
  }

  coral_state.motion_done = done;
}

void set_coral_state_currentpos(float current) {
  if (SERIAL_DEBUG) {
    Serial.print("set_coral_state_currentpos");
    Serial.println(current);
  }

  coral_state.currentpos  = current;
}

void set_coral_state_targetpos(float target) {
  if (SERIAL_DEBUG) {
    Serial.print("set_coral_state_targetpos");
    Serial.println(target);
  }

  coral_state.targetpos   = target;
}

void report_state(void) {
  if (SERIAL_DEBUG) {
    Serial.println("Report state:");
    Serial.println(coral_state.motion_done);
    Serial.println(coral_state.currentpos);
    Serial.println(coral_state.targetpos);
  }

  Wire1.write((byte *)&coral_state, sizeof(coral_state));
}

void comm_init(void) {
  Wire1.begin(Coral.I2C_ADDR);
  Wire1.onRequest(report_state);

  set_coral_state(0, 0.0, 0.0);
}

void handle_data(void) {
  if (!stepper_homed) {
    Coral_stepper.home();
    Coral_stepper.set_current_pos(Coral.HOME_POS);
    set_coral_state_currentpos(Coral.HOME_POS);
    stepper_homed = true;
  }

  else {
    if (Wire1.available() > 0) {
      cmd = Wire1.read();
      if (cmd == 0) {
        data_recv_cnt = 0;
        cobs.decode(encoded_cmd, RECV_DATA_LEN, decoded_cmd);
        memcpy(&move2pos, decoded_cmd, RECV_DATA_LEN-1);
        target_pos = move2pos;

        set_coral_state_targetpos(target_pos);

        if (SERIAL_DEBUG) {
          Serial.print("move2pos = ");
          Serial.println(target_pos);
        }
        got_data = true;
      }
      else {
        encoded_cmd[data_recv_cnt] = cmd;
        data_recv_cnt ++;
      }
    }
  }
}

void control_motion(void) {
  if (got_data) {
    got_data = false;

    if (!Coral.HAS_SERVO) {
      if (Coral.M_control) {
        if (target_pos >= Coral.HOME_POS) {
          Coral_stepper.home();
          Coral_stepper.set_current_pos(Coral.HOME_POS);
          set_coral_state_currentpos(Coral.HOME_POS);
        }
        else {
          distance2move = target_pos - Coral_stepper.current_pos;

          if (SERIAL_DEBUG) {
            Serial.print("distance2move = ");
            Serial.println(round(distance2move));
          }

          Coral_stepper.run(distance2move);

          if (SERIAL_DEBUG) {
            Serial.print("move_steps = ");
            Serial.println(round(move_steps));
          }

          Coral_stepper.set_current_pos(target_pos);
          set_coral_state_currentpos(target_pos);
        }
      }
      else {
        if (target_pos <= Coral.HOME_POS) {
          Coral_stepper.home();
          Coral_stepper.set_current_pos(Coral.HOME_POS);
          set_coral_state_currentpos(Coral.HOME_POS);
        }
        else {
          distance2move = target_pos - Coral_stepper.current_pos;

          if (SERIAL_DEBUG) {
            Serial.print("distance2move = ");
            Serial.println(round(distance2move));
          }

          Coral_stepper.run(distance2move);

          if (SERIAL_DEBUG) {
            Serial.print("move_steps = ");
            Serial.println(round(move_steps));
          }

          Coral_stepper.set_current_pos(target_pos);
          set_coral_state_currentpos(target_pos);
        }
      }
    }
    else {
      if (SERIAL_DEBUG) {
        Serial.print("Probing");
        Serial.println(target_pos);
      }

      if (target_pos > 0) probe(PROBE_POS);
      else                probe(UNPROBE_POS);
    }
  }
}
