# PCBPT (PCB Probe tester)
## The PCBPT_V3 is the latest working version (the one I showed at UIST'23).

This is a system that can help directly connect the schematic of the PCB design with test equipment (oscilloscope. etc,). When doing PCB debugging, we would need to use our hands to hold the oscilloscope probes, and our hands can not be that precise and table. Sometimes our hands are not enough, and we need to go from schematic and PCB layout back and forth to find a proper pad for the signal and place a probe on it.

The PCBPT connects the Sch with the oscilloscope. The user can choose a signal they want to check on the Sch, and the system will choose a proper pad and place a probe on the pad. Then the user can just check the output from the oscilloscope. All the user needs to do is select signals from Sch and check the output.

## PCBPT_V1.0

The first version of PCBPT is here:
https://www.media.mit.edu/projects/pcb-probe-tester/overview/

It's very rough. The machine design is not that perfect. The motor I used is [28BYJ-48](https://www.amazon.com/ELEGOO-28BYJ-48-ULN2003-Stepper-Arduino/dp/B01CP18J4A). It's very slow. The software is not that good, either. I have a separate GUI for the signal choosing. However, what I want is to choose the signal from the EDA, or make a GUI with the Sch and choose the signal from the SCH.

## PCBPT_V2.0
### Mechanical
The new machine design (I really love the Prusa color!).

<p align="center">
  <img src="/Doc/Images/machine_v2.JPG" height="350" title="hover text">
</p>

### Electronics
The first version of the main control board is a modular design. Each module can control a stepper motor and a servo. Each module is an RP2040 XIAO + A4988 stepper driver. The host is an ESP32 module.

<p align="center">
  <img src="/Doc/Images/stepper_module.jpg" height="200" title="hover text">
  <img src="/Doc/Images/main_control_1.JPG" height="200" title="hover text">
</p>

All stepper control modules are connected with the ESP32 module with I2C. The Host runs CircuitPython and is responsible for handling commands from the GUI and sending commands to each stepper control module.
This design is unnecessarily complicated compared with the function of the machine.

The second version of the main control board is a single board. One RPi Pico W module + 6 A4988 driver modules.
The probes are in vertical motion, and each probe is a triangle PCB board with a pogo pin soldered on the edge.

<p align="center">
  <img src="/Doc/Images/main_v2.jpg" height="350" title="hover text">
  <img src="/Doc/Images/probe.JPG" height="350" title="hover text">
</p>

I use the PCBWay service for both the second version of the main control board and the triangle probe PCB board. The quality is great and the production is very fast.




