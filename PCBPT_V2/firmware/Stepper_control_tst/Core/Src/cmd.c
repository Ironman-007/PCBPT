#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "cmd.h"
#include "stepper_control.h"
#include "debug_FZ.h"

UART_HandleTypeDef huart1;

uint8_t CMD_recv_buffer[CMD_BUFFER_LEN] = {0};
uint8_t CMD_recv[CMD_BUFFER_LEN] = {0};

static int i                    = 0;
static int data_start_index     = 0;
static int data_stop_index      = 0;

static uint32_t     step_number  = 0;
static unsigned int rotation_dir = 0;

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void) {
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler();
  }
}

// void Send_data_back(uint8_t * data2beback, uint16_t data_len, uint32_t timeout) {
//   HAL_UART_Transmit(&huart1, data2beback, data_len, timeout);
// }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  memcpy(CMD_recv, CMD_recv_buffer, CMD_BUFFER_LEN);
  HAL_UART_Receive_DMA(&huart1, CMD_recv_buffer, CMD_BUFFER_LEN);
}

void DMA_init(void) {
  HAL_GPIO_WritePin(GPIOA, PIN_RS495_RE_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, PIN_RS485_DE_Pin, GPIO_PIN_RESET);

  HAL_UART_Receive_DMA(&huart1, CMD_recv_buffer, CMD_BUFFER_LEN);
}

void Parse_CMD(uint8_t* cmd_recv) {
  step_number      = 0;
  data_start_index = 0;
  data_stop_index  = 0;

  if (cmd_recv[0] == 'P') {
    flash_led_once (50);

    for (i = 1; i < CMD_BUFFER_LEN; i ++) {
      if (cmd_recv[i] == ME_ID) {
        data_start_index = i + 1;
      }
      else if (cmd_recv[i] > '9') {
        data_stop_index = i;
      }
    }

    for (i = data_start_index; i < data_stop_index; i ++) {
      if (cmd_recv[i] == '+') {
        rotation_dir = CLKDIR;
      }
      else if (cmd_recv[i] == '-') {
        rotation_dir = COUNTERCLKDIR;
      }
      else if (cmd_recv[i] >= '0' && cmd_recv[i] <= '9') {
          step_number = step_number * 10 + (uint32_t)(cmd_recv[i] - '0');
      }
    }

    move_steps(rotation_dir, step_number);

    memset(cmd_recv, 0, CMD_BUFFER_LEN);
  }

  // flash_led_once (50);
}


