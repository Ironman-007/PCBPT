#include "debug_FZ.h"

void flash_led_once(uint32_t period) {
  HAL_GPIO_WritePin(PIN_LED_GPIO_Port, PIN_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(period);
  HAL_GPIO_WritePin(PIN_LED_GPIO_Port, PIN_LED_Pin, GPIO_PIN_RESET);
}
