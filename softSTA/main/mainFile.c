#include "station.h"
#include "tcp_client.h"
void app_main(){
    ESP_ERROR_CHECK(nvs_flash_init());
    //===============================//
    ESP_LOGI("btech.manu@gmail.com", "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    //===============================//
    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
    //===============================//
}
