#include <stdbool.h>
#include <math.h>

#include "stepper_control.h"
#include "debug_FZ.h"

static unsigned int ms1 = 0;
static unsigned int ms2 = 0;
static unsigned int ms3 = 0;

static volatile int i = 0;

volatile bool int_flag = false;

volatile bool start_moving = false;

static volatile uint64_t stp2move = 0;

static double real_time_speed = SPEED_INIT;
static double acc   = 0.0;
static double acc_t = 0.0;

void stepper_init(uint8_t misro_stepping_confg) {
  ms1 = (misro_stepping_confg)      & 0x01;
  ms2 = (misro_stepping_confg >> 1) & 0x01;
  ms3 = (misro_stepping_confg >> 2) & 0x01;

  HAL_GPIO_WritePin(GPIOA, PIN_MS1_Pin, ms1);
  HAL_GPIO_WritePin(GPIOA, PIN_MS2_Pin, ms2);
  HAL_GPIO_WritePin(GPIOA, PIN_MS3_Pin, ms3);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (start_moving) {
    int_flag = true;
    HAL_GPIO_TogglePin(GPIOA, PIN_STEP_Pin);
    i++;
  }
}

void calc_speed(void) {
  if (i < acc_t) {
    if (int_flag == true) {
      int_flag = false;
      __HAL_TIM_SET_AUTORELOAD(&htim2, lround(real_time_speed));
      real_time_speed -= acc;
    }
  }
  else if (i > (stp2move - acc_t)) {
    if (int_flag == true) {
      int_flag = false;
      __HAL_TIM_SET_AUTORELOAD(&htim2, lround(real_time_speed));
      real_time_speed += acc;
    }
  }
  else {
    real_time_speed = SPEED_MAX;
    __HAL_TIM_SET_AUTORELOAD(&htim2, lround(real_time_speed));
  }
}

void move_steps(unsigned int direction, uint32_t steps) {
  HAL_GPIO_WritePin(GPIOA, PIN_STEP_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, PIN_DIR_Pin, direction);

  stp2move = 2*steps;
  i        = 0;

  acc_t = (ACC_REGION * stp2move);
  acc   = ((double)(SPEED_INIT - SPEED_MAX))/acc_t;
 
  while(i < stp2move) {
    start_moving = true;
    calc_speed();
  }
  start_moving = false;
  flash_led_once(50);
}




