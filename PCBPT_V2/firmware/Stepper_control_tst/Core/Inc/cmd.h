#ifndef __CMD_H
#define __CMD_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart1;
extern void MX_USART1_UART_Init(void);

extern uint8_t * CMD_recv; // Bytes received from USART;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
