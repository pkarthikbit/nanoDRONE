/* nanoDRONE 

   This Prototype code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "nanoDRONE_inf.h"
#include "nanoDRONE_priv.h"

/***************************************
*** Static variable definition
***************************************/
static const char *TAG = "main";
static int cnt = 0;

/***************************************
*** Configure pins
***************************************/
void pin_config()
{
    /* pin 16 LED */
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO15/16
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}


void blinkLED_infinit()
{
    ESP_LOGI(TAG, "cnt: %d\n", cnt++);
    vTaskDelay(1000 / portTICK_RATE_MS);
    gpio_set_level(GPIO_OUTPUT_IO_1, cnt % 2);
}

void app_main()
{
    /***************************************
    *** Init functions
    ***************************************/
    {
        /* Configure the pins */
        pin_config();
    }
   
    /***************************************
    *** Loop functions
    ***************************************/
    while(1)
    {
        /* blink LED infinitely */
        blinkLED_infinit();
    }
}
