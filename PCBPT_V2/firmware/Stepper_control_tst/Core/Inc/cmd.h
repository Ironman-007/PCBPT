#ifndef __CMD_H
#define __CMD_H

#include "main.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef  htim2;

extern void MX_USART1_UART_Init(void);

#define CMD_BUFFER_LEN 20

#define ME_ID          'X'

extern uint8_t CMD_recv_buffer[CMD_BUFFER_LEN]; // Bytes received from USART;
extern uint8_t CMD_recv[CMD_BUFFER_LEN]; // Bytes received from USART;

extern void DMA_init(void);
extern void Parse_CMD(uint8_t* cmd_recv);

extern volatile bool int_flag;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
