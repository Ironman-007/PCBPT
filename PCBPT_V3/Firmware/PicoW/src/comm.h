//
// Created by l1534 on 9/13/2023.
//

#ifndef __COMM_H
#define __COMM_H

#ifdef __cplusplus
extern "C"
{
#endif

extern bool probe_cmd;

extern uint8_t cmd_in[100];

extern void comm_init(void);
extern void comm_update(void);
//extern void comm_handle_cmd(uint8_t * cmd);

#ifdef __cplusplus
}
#endif

#endif //__COMM_H
