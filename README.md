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
The device is coded in C, and makes/receives phone calls by communicating with the GSM module through the UART interface. A key matrix is used to input the numbers and Adafruit libraries are used to control the OLED Display. Different states of the phone have been defined, in which different output is displayed on the OLED.

## Features:
- Making and receiving phone calls

## BOM:
| Product Name                         | Product Link                                                                                    | Unit Price | Amount | Total  | Remarks                        |
|--------------------------------------|-------------------------------------------------------------------------------------------------|------------|--------|--------|--------------------------------|
| PCB                                  | https://jlcpcb.com/                                                                             | $2.00      | 1      | $11.47 |                                |
| SIM800L GPRS GSM Module              | https://roboticsdna.in/product/sim800l-module/                                                  | $3.10      | 1      | $4.04  | Delivery costs and taxes apply |
| 3.7V 1500 mAh LiPo Battery           | https://robu.in/product/1500mah-pcm-protected-micro-li-po-battery                               | $2.96      | 1      | $2.96  |                                |
| Tactile Push Button Switch           | https://robu.in/product/6x6x5-tactile-push-button-switch                                        | $0.014     | 12     | $0.28  | Only in packs of 10            |
| Microphone                           | https://robu.in/product/microphone-97mm-pack-of-3                                               | $0.20      | 1      | $0.58  | Only in packs of 3             |
| TP4056 Li-ion battery charging board | https://robu.in/product/tp4056-1a-lipo-battery-charging-board-micro-usb-with-current-protection | $0.12      | 1      | $0.12  |                                |
| 28mm Trumpet Speaker                 | https://robu.in/product/0-5w-8ohm-trumpet-speaker-diameter-28mm                                 | $0.30      | 1      | $0.30  |                                |
| SPST Rocker Switch                   | https://robu.in/product/spst-rocker-switch-pack-of-3                                            | $0.15      | 1      | $0.15  |                                |
| ESP32-C3 SUPERMINI                   | https://robu.in/product/esp32-c3-development-board-with-soldering                               | $2.59      | 1      | $2.59  |                                |
| 0.91 inch I2C OLED Display           | https://roboticsdna.in/product/0-91-inch-iic-4-pin-oled-display-module-ssd1306-white/           | $1.66      | 1      | $2.60  | Delivery costs and taxes apply |
|                                      |                                                                                                 |            | TOTAL  | $25.09 |                                |


## Zine:

![zine](assets/zine.png)