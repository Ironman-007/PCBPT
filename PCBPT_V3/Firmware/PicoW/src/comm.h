//
// Created by l1534 on 9/13/2023.
//

#ifndef __COMM_H
#define __COMM_H

#include "cmd.h"

#define DATA_FOR_A 0
#define DATA_FOR_B 1
#define DATA_FOR_X 2
#define DATA_FOR_Y 3
#define DATA_FOR_NONE 3

extern bool probe_cmd;

extern uint8_t cmd_in[100];

extern void comm_init(void);
extern void comm_update(void);
//extern void comm_handle_cmd(uint8_t * cmd);

#endif //__COMM_H
