#include "stepper_control.h"

static unsigned int ms1 = 0;
static unsigned int ms2 = 0;
static unsigned int ms3 = 0;

void stepper_init(uint8_t misro_stepping_confg) {
  ms1 = (misro_stepping_confg)      & 0x01;
  ms2 = (misro_stepping_confg >> 1) & 0x01;
  ms3 = (misro_stepping_confg >> 2) & 0x01;

  HAL_GPIO_WritePin(GPIOA, PIN_MS1_Pin, ms1);
  HAL_GPIO_WritePin(GPIOA, PIN_MS2_Pin, ms2);
  HAL_GPIO_WritePin(GPIOA, PIN_MS3_Pin, ms3);

  // HAL_GPIO_WritePin(GPIOA, PIN_MS1_Pin|PIN_MS2_Pin|PIN_MS3_Pin, GPIO_PIN_RESET);
}