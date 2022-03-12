## Miley Embedded Software
This repository contains all embedded C code that is running on the Miley Programmable Entrance Announcer.

### SPI Module
The dsPIC's SPI module is responsible for communicating with an SD/MMC SD card. The [FatFS library](http://elm-chan.org/fsw/ff/00index_e.html) is being used to open and read files from the SD card on the dsPIC. Pins on the dsPIC have been configured as follows:
- CS = RB14
- SCLK = RB13
- SDI = RC14
- SDO = RC15 

### Building
This repo contains only source code. If you want to build and test this code you must first create a new MPLABX project targeted to the dsPIC33CK256MP506 and then import the code in this repo into your project hierarchy. 

### Authors:
- Justen Di Ruscio
- Vardaan Malhotra
- Zachary Demerson
- Ethan Garnier