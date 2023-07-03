#include "cmd.h"

UART_HandleTypeDef huart1;
uint8_t * CMD_recv;

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
 **/
void MX_USART1_UART_Init(void) {
  huart1.Instance                    = USART1;
  huart1.Init.BaudRate               = 115200;
  huart1.Init.WordLength             = UART_WORDLENGTH_8B;
  huart1.Init.StopBits               = UART_STOPBITS_1;
  huart1.Init.Parity                 = UART_PARITY_NONE;
  huart1.Init.Mode                   = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  HAL_UART_Receive_IT(&huart1, CMD_recv, 4); 
}


