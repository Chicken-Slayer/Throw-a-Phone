# Throw-a-Phone
Have you ever forgotten your phone at home, and just kept your SIM card along? Me too!
This device aims to solve this common problem faced by us in our daily lives.

In all seriousness, this is a device for people who need absolutely no distractions, or simply a cheap device that can be bought to simply make phone calls without any drama.

## How it works:
Inside the case, there is an ESP32 C3 SUPERMINI, which is compact and affordable - perfect for this use case. Basically, the device uses a 12 key matrix as input, which can be used to enter phone numbers and answer/end calls. The calls are handled using a SIM800L GSM module connected to an external antenna. Due to the hardware used, it can only support 2G networks, which are outdated and may not be available everywhere. However, this issue is easily fixed by swapping out the SIM800L module for a SIM6700X. However, that will require a few changes to the PCB (not too much timetaking, simply a 5-10 minute fix).

## PCB:
Here's the Schematic and the PCB:

![schematic](assets/schematic.png)

![pcb](assets/pcb.png)

## Enclosure:
The components are placed within a 3D printed enclosure. The PCB is held in place with round inserts set in place in both the PCB and the Enclosure. 

It has two separate pieces: 
- The top cover, featuring space for the keys, power switch, screen, and audio I/O
- The base, which has obtrusions designed to hold the PCB in place, as well as clearance for an external antenna and charging port.

The two pieces are secured using hot glue.

![model](assets/model.png)

## Firmware:
The device is coded in C, and makes/receives phone calls by communicating with the GSM module through the UART interface. 

## Features:
- Making and receiving phone calls

## BOM:
- 3.7V 1500mAh 1S LiPo Battery 
- SIM800L GPRS GSM Module Core Board Quad-band TTL Serial Port with the antenna
- ESP32-C3 SUPERMINI
- Battery
- SSD1306
- 12x push buttons
- Rocker Switch
- Microphone
- Speaker