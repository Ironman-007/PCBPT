#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     4

#include <Arduino.h>

#include "system.h"
#include "cmd.h"

#include "stepper_control.h"
#include "RPi_Pico_TimerInterrupt.h"
#include "RPi_Pico_ISR_Timer.h"

RPI_PICO_Timer     ITimer1(0);
RPI_PICO_ISR_Timer ISR_timer;

bool TimerHandler(struct repeating_timer *t) {
  (void) t;
  ISR_timer.run();
  return true;
}

void timer_ISR_init_FZ(void) {
  ITimer1.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler);
  ISR_timer.setInterval(1L, output_pulses);
}

void setup() {
  pin_init();
  timer_ISR_init_FZ();
}

void setup1() {
  SerialDebug.begin(115200);
  while (!SerialDebug) {sleep_ms(10);}
}

void loop() {
  output_PWM(4);
}

void loop1() {
  cmd = SerialDebug.read();
  if (cmd == 's') {
    toggle_stat = true;
    SerialDebug.println("output_flag = true;");
    output_flag = true;
  }
  if (output_done) {
    output_done = false;
    SerialDebug.println("Done");
  }
}


