/* nanoDRONE 

   This Prototype code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#define PWM_D5_OUT_IO_NUM   14
#define PWM_D6_OUT_IO_NUM   12
#define PWM_D7_OUT_IO_NUM   13
#define PWM_D8_OUT_IO_NUM   15

/* Used to set the Motor rpm based on the client request */
#define MOTOR_D5    0
#define MOTOR_D6    1
#define MOTOR_D7    2
#define MOTOR_D8    3

#define GPIO_OUTPUT_IO_1    16

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_1))

#define EXAMPLE_ESP_WIFI_SSID      "nanoDRONE/0.0.1;0"
#define EXAMPLE_ESP_WIFI_PASS      "123456789"
#define EXAMPLE_MAX_STA_CONN       1

// pwm pin number
const uint32_t pin_num[4] = {
    PWM_D5_OUT_IO_NUM,
    PWM_D6_OUT_IO_NUM,
    PWM_D7_OUT_IO_NUM,
    PWM_D8_OUT_IO_NUM
};

// Frequency is 1/256*10^-6 Hz PWM, period is 256
#define PWM_PERIOD    256 //us

// duties table, real_duty = duties[x]/PERIOD
uint32_t duties[4] = {
    0, 0, 0, 0,
};

// phase table, delay = (phase[x]/360)*PERIOD
float phase[4] = {
    0, 0, 0, 0,
};

//Bit masks for Controller request
#define DRONE_POWER         1       //bit0 = Power On/ Off
#define DRONE_FLY_N         2       //bit1 = North
#define DRONE_FLY_E         4       //bit2 = East
#define DRONE_FLY_W         8       //bit3 = West
#define DRONE_FLY_S        16       //bit4 = South
#define DRONE_FLY_NE       32       //bit5 = North East
#define DRONE_FLY_NW       64       //bit6 = North West
#define DRONE_FLY_SE      128       //bit7 = South East
#define DRONE_FLY_SW      256       //bit8 = South West
#define DRONE_FLY_UP      512       //bit9 = Up
#define DRONE_FLY_DOWN   1024       //bit10 = Down
#define DRONE_FLY_CW     2048       //bit11 = ClockWise rotation
#define DRONE_FLY_CCW    4096       //bit12 = Counter ClockWise rotation
//<reserved>             8192       //bit13 = <reserved>
//<reserved>            16384       //bit14 = <reserved>
//<reserved>            32768       //bit15 = <reserved>
                                                     