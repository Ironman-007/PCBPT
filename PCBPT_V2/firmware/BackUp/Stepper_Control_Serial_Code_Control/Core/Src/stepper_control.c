#include "stepper_control.h"
#include "debug_FZ.h"

static unsigned int ms1 = 0;
static unsigned int ms2 = 0;
static unsigned int ms3 = 0;

static int i = 0;

static unsigned int step_delay_time = 0;

void stepper_init(uint8_t misro_stepping_confg) {
  ms1 = (misro_stepping_confg)      & 0x01;
  ms2 = (misro_stepping_confg >> 1) & 0x01;
  ms3 = (misro_stepping_confg >> 2) & 0x01;

  HAL_GPIO_WritePin(GPIOA, PIN_MS1_Pin, ms1);
  HAL_GPIO_WritePin(GPIOA, PIN_MS2_Pin, ms2);
  HAL_GPIO_WritePin(GPIOA, PIN_MS3_Pin, ms3);
}

void move_steps(unsigned int direction, uint32_t steps, uint32_t pwm_frequency) {
  HAL_GPIO_WritePin(GPIOA, PIN_DIR_Pin, direction);

  step_delay_time = (unsigned int)(1000/pwm_frequency/2);

  for (i=0; i<steps; i++) {
    HAL_GPIO_WritePin(GPIOA, PIN_STEP_Pin, GPIO_PIN_SET);
    HAL_Delay(step_delay_time);
    HAL_GPIO_WritePin(GPIOA, PIN_STEP_Pin, GPIO_PIN_RESET);
    HAL_Delay(step_delay_time);
  }

  flash_led_once (50);
}