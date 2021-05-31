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
esp_err_t data_get_handler(httpd_req_t *req)
{
    char*  buf_char;
    int32_t buf_len, buf_int;
    const char* resp_str;      //return value

    //Default value
    resp_str = "0x7F10";

    /* Read URL query string length and allocate memory for length */
    buf_len = httpd_req_get_url_query_len(req);
    buf_char = malloc(buf_len);

    if ((buf_len == 4) &&
        (httpd_req_get_url_query_str(req, buf_char, buf_len) == ESP_OK))
    {
        buf_int = atoi(buf_char);
        ESP_LOGI(TAG, "value received => %d", buf_int);



        // channel0, 1 output hight level.
        // channel2, 3 output low level.
        pwm_set_duty(0, 100);
        pwm_set_duty(1, 100);
        pwm_set_duty(2, 100);
        pwm_set_duty(3, 100);

        pwm_start();

        resp_str = "0x0000";      
        free(buf_char);
    }
    else
    {
        resp_str = "0x7F13";
    }
        
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    httpd_resp_send(req, resp_str, strlen(resp_str));

    return ESP_OK;
}

httpd_uri_t data = {
    .uri       = "/data",
    .method    = HTTP_GET,
    .handler   = data_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "0x7F10"
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
        httpd_register_uri_handler(server, &data);
        return server;
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
        return NULL;
    }
}

/***************************************
*** Configure pins
***************************************/
void nanoDRONE_pin_config()
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
*** PWM Configuration
***************************************/
void nanoDRONE_pwm_config()
{
    pwm_init(PWM_PERIOD, duties, 4, pin_num);
    pwm_set_phases(phase);
    pwm_start();
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
        nanoDRONE_pin_config();

        /* setup SoftAp */
        wifi_init_softap();
        
        /* http_server */
        server = start_webserver();

        /* PWM config */
        nanoDRONE_pwm_config();

        //LED off
        gpio_set_level(GPIO_OUTPUT_IO_1, true);
    }
   
    /***************************************
    *** Loop functions
    ***************************************/
    //while(1){}
}
