/**
 * @file tcp_server.h
*/
#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_
    #include <string.h>
    #include <sys/param.h>

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "esp_system.h"
    #include "esp_log.h"
    #include "esp_netif.h"
    #include "esp_event.h"
    #include "protocol_examples_common.h"
    #include "nvs.h"
    #include "nvs_flash.h"

    #include "lwip/err.h"
    #include "lwip/sockets.h"
    #include "lwip/sys.h"
    #include <lwip/netdb.h>

    //#define PORT CONFIG_EXAMPLE_PORT
    #define PORT 3333

    void tcp_server_task(void *pvParameters);
#endif