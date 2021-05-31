# nanoDRONE
Create a Drone with ESP8266 RTOS

Setup the build environment in Linux Mint:
1. Clone ESP8266_RTOS_SDK
2. Clone xtensa-lx106-elf
3. Under /home/[user]/.profile, have the below lines

export IDF_PATH=~/[folder path]/esp8266RTOS/ESP8266_RTOS_SDK
export PATH="$HOME/[folder path]/esp8266RTOS/xtensa-lx106-elf/bin:$PATH"

Below are the commands:
1. make menuconfig
2. make all
3. make flash
4. make flash monitor

Controller request format:
Cmdline --> curl http://192.168.4.1/data?xxxx
Browser --> http://192.168.4.1/data?xxxx

where,
xxxx is a 2byte bit mapped data

bit0 = Power On/ Off
bit1 = North
bit2 = East
bit3 = West
bit4 = South
bit5 = North East
bit6 = North West
bit7 = South East
bit8 = South West
bit9 = Up
bit10 = Down
bit11 = ClockWise rotation
bit12 = Counter ClockWise rotation
bit13 = <reserved>
bit14 = <reserved>
bit15 = <reserved>

Drone Response Code:
0x0000 = Positive response
0x7F10 = Negative response - General reject
0x7F13 = Negative response - Request Length incorrect
0x7F31 = Negative response - Request out of range
