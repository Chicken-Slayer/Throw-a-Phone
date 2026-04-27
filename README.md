# Throw-a-Phone
Have you ever forgotten your phone at home, and just kept your SIM card along? Me too!
This device aims to solve this common problem faced by us in our daily lives.

In all seriousness, this is a device for people who need absolutely no distractions, or simply a cheap device that can be bought to simply make phone calls without any drama.

## How it works:
Inside the case, there is an ESP32 C3 SUPERMINI, which is compact and affordable - perfect for this use case. Basically, the device uses a 12 key matrix as input, which can be used to enter phone numbers and answer/end calls. The calls are handled using a SIM800L GSM module connected to an external antenna. Due to the hardware used, it can only support 2G networks, which are outdated and may not be available everywhere. 

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
The device is coded in C, and makes/receives phone calls by communicating with the GSM module through the UART interface. A key matrix is used to input the numbers and Adafruit libraries are used to control the OLED Display. Different states of the phone have been defined, in which different output is displayed on the OLED.

## Features:
- Making and receiving phone calls

## Replication Guide:
This project is quite easy to replicate hardware-wise, but the software part comes out to be more complicated.
1. Start off by wiring the charging board output to the rocker switch, and then the switch to the ESP32 and the GSM module. The GSM module cannot run on 3.3V supplied by ESP32, so it needs to be powered directly by the battery.
2. Solder the buttons and the OLED display on to the PCB, making sure that the diodes are in the right direction (COL2ROW).
3. Solder the ESP32 module to the PCB and connect the RX, TX and GPIO0 pins to the corresponding ones on the GSM modules.
4. Wire the speaker and the microphone directly to the GSM module and connect the PCB antenna (comes bundled with GSM module).
5. Connect the battery to the charging board power input. Make sure to check polarity.
6. Load up the firmware on to the ESP32 board, making sure to check the pin definitions in the code.
7. Secure the components in place, holding the PCB using the round insertions in the case, and hot gluing remaining components in their respective places. On the top part of the case, space has been given for inserting the microphone, speaker, and rocker switch. There are holes on the bottom part of the case to route the PCB antenna outside and connect the charging module to a USB cable. 

## BOM:
| Product Name                         | Amount |
|--------------------------------------|--------|
| PCB                                  | 1      |
| SIM800L GPRS GSM Module              | 1      |
| 3.7V 1500 mAh LiPo Battery           | 1      |
| Tactile Push Button Switch           | 12     |
| Microphone                           | 1      |
| TP4056 Li-ion battery charging board | 1      |
| 28mm Trumpet Speaker                 | 1      |
| SPST Rocker Switch                   | 1      |
| ESP32-C3 SUPERMINI                   | 1      |
| 0.91 inch I2C OLED Display           | 1      |

## Zine:

![zine](assets/zine.png)