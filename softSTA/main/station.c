#include "station.h"
//=========================================//
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
static EventGroupHandle_t s_wifi_event_group;
//=========================================//
static const char *TAG = "wifi station";
static int s_retry_num = 0;

/**
 * @brief RTOS event handler
 * @param event_base can be WIFI_EVENT, IP_EVENT or ETH_EVENT
 * @param event_id depends on event_base
 * @param event_data
 * @retval None
*/
static void event_handler(
                        void* arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data){
    //WIFI_EVENT {START}
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        esp_wifi_connect();
    }
    //WIFI_EVENT {STA_DISCONNECTED}
    else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        //----------------------------------------------------------
        else{xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);}
        //----------------------------------------------------------
        ESP_LOGI(TAG,"connect to the AP fail");
    }
    //IP_EVENT {GOT_IP}
    else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        //---------------------------------------------------------
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        //---------------------------------------------------------
    }
}

void wifi_init_sta(void){
    //
    s_wifi_event_group = xEventGroupCreate();
    //
    tcpip_adapter_init();
    //
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    //
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    //
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS
        },
    };

    /* Setting a password implies station will connect to all security modes including WEP/WPA.
    * However these modes are deprecated and not advisable to be used. Incase your Access point
    * doesn't support WPA2, these mode can be enabled by commenting below line */

    if (strlen((char *)wifi_config.sta.password)) {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    vEventGroupDelete(s_wifi_event_group);
}

void keepSTAconnected_task(void *pvParameters){
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);
    while(1){
            if (bits & WIFI_CONNECTED_BIT) {
                //connected do nothing
            }else if (bits & WIFI_FAIL_BIT) {
                wifi_init_sta();//try to connect
            }else{
                wifi_init_sta();//ESP_LOGE(TAG, "UNEXPECTED EVENT");
            }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}