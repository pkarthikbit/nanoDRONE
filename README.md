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

