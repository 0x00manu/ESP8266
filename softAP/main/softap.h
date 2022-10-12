#ifndef SOFTAP_H_
#define SOFTAP_H_
    #include <string.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "esp_system.h"
    #include "esp_wifi.h"
    #include "esp_event.h"
    #include "esp_log.h"
    #include "nvs_flash.h"

    #include "lwip/err.h"
    #include "lwip/sys.h"

    /* The examples use WiFi configuration that you can set via project configuration menu.

    If you'd rather not, just change the below entries to strings with
    the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
    */
    #define mySSID                      "SingularLabs"
    #define myPassword                    "8285676991"
    //#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
    //#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
    #define EXAMPLE_ESP_WIFI_SSID      mySSID
    #define EXAMPLE_ESP_WIFI_PASS      myPassword
    #define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN
    void initializeAccessPoint(void);
#endif