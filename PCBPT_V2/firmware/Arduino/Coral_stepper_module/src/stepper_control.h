#pragma once

#include "system.h"

// #define TIMER_INTERRUPT_DEBUG         1
// #define _TIMERINTERRUPT_LOGLEVEL_     4

// #include "RPi_Pico_TimerInterrupt.h"
// #include "RPi_Pico_ISR_Timer.h"

// extern RPI_PICO_Timer     ITimer1;
// extern RPI_PICO_ISR_Timer ISR_timer;

extern volatile bool output_flag;
extern volatile bool output_done;

extern volatile uint32_t  pulse_i;

extern volatile bool pulse_flag;
extern volatile bool toggle_stat;

#define LED_TOGGLE_INTERVAL_MS  1000L
#define SIMPLE_TIMER_MS         2000L
#define TIMER_INTERVAL_MS       1L

extern volatile bool pulse_flag;

// extern void timer_ISR_init_FZ(void);
extern void output_pulses(void);
extern void output_PWM(uint32_t steps);
