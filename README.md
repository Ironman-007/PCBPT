# PCBPT (PCB Probe tester)

This is a system that can help directly connect the schematic of the PCB design with test equipments (oscilloscope. etc,). When doing PCB debug, we would need to use our hands to hold the oscilloscope probes, and our hands can not be that precise and table. Sometimes our hands are not enough, and we need to go from schematic and PCB layout back and forth to find a proper pad for the some signal and place a probe on it.

The PCBPT is connect the Sch with the oscilloscope. The user can choose a signal they want to check on the Sch, and the system will choose a proper pad and place a probe on the pad. Then the user can just check the output from the oscilloscope. All the user needs to do is selecting signals from Sch and check the output.

## PCBPT_V1.0

The first version of PCBPT is here:
https://www.media.mit.edu/projects/pcb-probe-tester/overview/

It's very rough. The machine design is not that perfect. The motor I used is 28BYJ-48 (https://www.amazon.com/ELEGOO-28BYJ-48-ULN2003-Stepper-Arduino/dp/B01CP18J4A). It's very slow. The software is not that good, either. I have a seperate GUI for the signal choosing. However, what I want is choose the signal from the EDA, or make a GUI with the Sch and choose the signal from the SCH.

## PCBPT_V2.0
The second version is under developed now.

The new machine is looked like this: ![alt text](/images/PCBPT_3dmodel_1.PNG)

I really love the color Prusa!!! :)

I'm using better stepper motor: [#1204 (SY20STH30-0604A)](https://www.pololu.com/product/1204) and [#1205 (SY28STH32-0674A)](https://www.pololu.com/product/1205).

There are lots of things left to do:
- the LM8UU linear bearing bought from Amazon are really not that good. They loose. I'm trying to make the space between 2 bearings a little bigger, so when the bearings are installed, the shafts are pushing the bearings out. This may make the bearings tighter and mitigate the loosing.
