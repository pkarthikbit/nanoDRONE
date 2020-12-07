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
static const char *TAG = "nanoDRONE";
static httpd_handle_t server = NULL;
static uint32_t dtyCyl_u32[4];

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

/***************************************
*** setup softap
***************************************/
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) 
    {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) 
    {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap()
{
    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

/***************************************
*** http server
***************************************/
/* An HTTP GET handler */
esp_err_t power_get_handler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) 
    {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
          
            dtyCyl_u32[0] = dtyCyl_u32[0] + 100;

            if(dtyCyl_u32[0] >=1000)
            {
                dtyCyl_u32[0] = 0;
            }
            
            // channel0, 1 output hight level.
            // channel2, 3 output low level.
            //pwm_stop(0x3);
            //gpio_set_level(GPIO_OUTPUT_IO_1, st_LED);
        }
        free(buf);
    }

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    return ESP_OK;
}

httpd_uri_t power = {
    .uri       = "/power",
    .method    = HTTP_GET,
    .handler   = power_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "power_success"
};

/* generic functions */
httpd_handle_t start_webserver(void)
{   
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) 
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &power);
        return server;
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
        return NULL;
    }
}

/***************************************
*** PWM Configuration
***************************************/
void pwm_config()
{
    pwm_init(PWM_PERIOD, duties_init, 4, pin_num);
    pwm_set_phases(phase_init);
    pwm_start();
}

void pwm_run()
{
    pwm_set_duty(0, dtyCyl_u32[0]);
    
    int16_t count = 0;

    while (1) {
        if (count == 20) {
            // channel0, 1 output hight level.
            // channel2, 3 output low level.
            pwm_stop(0x3);
            ESP_LOGI(TAG, "PWM stop\n");
        } else if (count == 30) {
            pwm_start();
            ESP_LOGI(TAG, "PWM re-start\n");
            count = 0;
        }

        count++;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

/***************************************
*** Main function
***************************************/
void app_main()
{
    /***************************************
    *** Init functions
    ***************************************/
    {
        ESP_ERROR_CHECK(nvs_flash_init());
        ESP_ERROR_CHECK(esp_netif_init());

        /* Configure the pins */
        pin_config();

        /* setup SoftAp */
        wifi_init_softap();
        
        /* http_server */
        server = start_webserver();

        /* PWM config */
        pwm_config();
    }
   
    /***************************************
    *** Loop functions
    ***************************************/
    while(1)
    {
        pwm_run();
    }
}
