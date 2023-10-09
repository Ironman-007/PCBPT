//
// Created by Fangzheng on 9/14/2023.
//

#ifndef __CMD_H
#define __CMD_H

#include "stdint.h"

enum CMD_TYPE {
  IDLE_CMD,
  PROBE_CMD,
  CALI_CMD,
  MANUCAL_CTRL_CMD,
  HOME_CMD
};

//#define CMD_TYPE_POS  6
#define CMD_A_POS     6
#define CMD_B_POS     5
#define CMD_X_POS     4
#define CMD_Y_POS     3

#define CMD_TYPE_MASK 0x11 << CMD_TYPE_POS
//#define CMD_A_MASK    0x01 << CMD_A_POS
//#define CMD_B_MASK    0x01 << CMD_B_POS
//#define CMD_X_MASK    0x01 << CMD_X_POS
//#define CMD_Y_MASK    0x01 << CMD_Y_POS

class cmd {
public:
  cmd(void);
//  ~cmd(void);

  CMD_TYPE cmd_type;
  uint8_t  CMD_REG;

  void init_cmd(void);

  void set_cmd_TYPE(CMD_TYPE type);
  void set_cmd_REG(uint8_t pos, uint8_t data);
  uint8_t check_REG(uint8_t reg, uint8_t pos);

  uint8_t A_position[10];
  uint8_t B_position[10];
  uint8_t X_position[10];
  uint8_t Y_position[10];

  float A_position_f;
  float B_position_f;
  float X_position_f;
  float Y_position_f;

  void tst_function();

private:

};

extern cmd recv_CMD;

#endif //PICOW_CMD_H
