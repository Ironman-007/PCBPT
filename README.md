# PCBPT (PCB Probe tester)

This is a system that can help directly connect the schematic of the PCB design with test equipments (oscilloscope. etc,). When doing PCB debug, we would need to use our hands to hold the oscilloscope probes, and our hands can not be that precise and table. Sometimes our hands are not enough, and we need to go from schematic and PCB layout back and forth to find a proper pad for the some signal and place a probe on it.

The PCBPT is connect the Sch with the oscilloscope. The user can choose a signal they want to check on the Sch, and the system will choose a proper pad and place a probe on the pad. Then the user can just check the output from the oscilloscope. All the user needs to do is selecting signals from Sch and check the output.

## PCBPT_V1.0

The first version of PCBPT is here:
https://www.media.mit.edu/projects/pcb-probe-tester/overview/

It's very rough. The machine design is not that perfect. The motor I used is '28BYJ-48 (https://www.amazon.com/ELEGOO-28BYJ-48-ULN2003-Stepper-Arduino/dp/B01CP18J4A)'.
