/* nanoDRONE 

   This Prototype code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#define PWM_0_OUT_IO_NUM   12
#define PWM_1_OUT_IO_NUM   13
#define PWM_2_OUT_IO_NUM   14
#define PWM_3_OUT_IO_NUM   15

#define GPIO_OUTPUT_IO_1    16

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_1))

#define EXAMPLE_ESP_WIFI_SSID      "nanoDRONE.0.0.1"
#define EXAMPLE_ESP_WIFI_PASS      "123456789"
#define EXAMPLE_MAX_STA_CONN       1

// pwm pin number
const uint32_t pin_num[4] = {
    PWM_0_OUT_IO_NUM,
    PWM_1_OUT_IO_NUM,
    PWM_2_OUT_IO_NUM,
    PWM_3_OUT_IO_NUM
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


