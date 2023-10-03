// MultiStepper.pde
// -*- mode: C++ -*-
//
// Shows how to multiple simultaneous steppers
// Runs one stepper forwards and backwards, accelerating and decelerating
// at the limits. Runs other steppers at the same time
//
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

AccelStepper stepper3(AccelStepper::DRIVER, D15, D14);

void setup() {  
  stepper3.setMaxSpeed(2000);
  stepper3.setAcceleration(6000);
  stepper3.moveTo(500); 
}

void loop() {
  // Change direction at the limits
  if (stepper3.distanceToGo() == 0) {
    delay(1000);
	  stepper3.moveTo(-stepper3.currentPosition());
  }

  stepper3.run();
}
