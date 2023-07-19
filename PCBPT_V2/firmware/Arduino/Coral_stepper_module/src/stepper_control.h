#pragma once

#include "system.h"

// #define TIMER_INTERRUPT_DEBUG         1
// #define _TIMERINTERRUPT_LOGLEVEL_     4

// #include "RPi_Pico_TimerInterrupt.h"
// #include "RPi_Pico_ISR_Timer.h"

// extern RPI_PICO_Timer     ITimer1;
// extern RPI_PICO_ISR_Timer ISR_timer;

#define MICROSTEPPING_1_1    0x00
#define MICROSTEPPING_1_2    0x01
#define MICROSTEPPING_1_4    0x02
#define MICROSTEPPING_1_8    0x03
#define MICROSTEPPING_1_16   0x07

#define MS1_MASK             0x01
#define MS2_MASK             0x02
#define MS3_MASK             0x04

#define MS1_PIN              D7
#define MS2_PIN              D8
#define MS3_PIN              D9

extern volatile bool output_flag;
extern volatile bool output_done;

extern volatile uint32_t  pulse_i;

extern volatile bool pulse_flag;
extern volatile bool toggle_stat;

#define LED_TOGGLE_INTERVAL_MS  1000L
#define SIMPLE_TIMER_MS         2000L
#define TIMER_INTERVAL_MS       1L

extern volatile bool pulse_flag;

extern void stepper_init(uint8_t resolution);

// extern void timer_ISR_init_FZ(void);
extern void output_pulses(void);
extern void output_PWM(uint32_t steps);
