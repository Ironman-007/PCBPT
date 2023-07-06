#include <stdbool.h>

#include "stepper_control.h"
#include "debug_FZ.h"

static unsigned int ms1 = 0;
static unsigned int ms2 = 0;
static unsigned int ms3 = 0;

static int i = 0;

volatile bool int_flag = false;

volatile bool start_moving = false;

static volatile uint64_t stp2move = 0;

void stepper_init(uint8_t misro_stepping_confg) {
  ms1 = (misro_stepping_confg)      & 0x01;
  ms2 = (misro_stepping_confg >> 1) & 0x01;
  ms3 = (misro_stepping_confg >> 2) & 0x01;

  HAL_GPIO_WritePin(GPIOA, PIN_MS1_Pin, ms1);
  HAL_GPIO_WritePin(GPIOA, PIN_MS2_Pin, ms2);
  HAL_GPIO_WritePin(GPIOA, PIN_MS3_Pin, ms3);
}

/*
void move_steps(unsigned int direction, uint32_t steps) {
  HAL_GPIO_WritePin(GPIOA, PIN_DIR_Pin, direction);

  for (i=0; i<steps; i++) {
    HAL_GPIO_WritePin(GPIOA, PIN_STEP_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, PIN_STEP_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
  }

  flash_led_once (50);
}
*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (start_moving) {
    if (i < stp2move) {
      HAL_GPIO_TogglePin(GPIOA, PIN_STEP_Pin);
      i++;
    }
    else
      start_moving = false;
  }
  // int_flag = true;
}

void move_steps(unsigned int direction, uint32_t steps) {
  HAL_GPIO_WritePin(GPIOA, PIN_STEP_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, PIN_DIR_Pin, direction);

  stp2move = 2*steps;
  i = 0;

  start_moving = true;

  flash_led_once (50);
}



