/**
 * @file station.h
 */
#ifndef STATION_H_
#define STATION_H_
	#include <string.h>
	#include "freertos/FreeRTOS.h"
	#include "freertos/task.h"
	#include "freertos/event_groups.h"
	#include "esp_system.h"
	#include "esp_log.h"
	#include "esp_netif.h"
	#include "esp_event.h"
	#include "esp_wifi.h"
	#include "nvs.h"
	#include "nvs_flash.h"

	#include "lwip/err.h"
	#include "lwip/sys.h"

	/* The examples use WiFi configuration that you can set via project configuration menu

	   If you'd rather not, just change the below entries to strings with
	   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
	*/
	#define EXAMPLE_ESP_WIFI_SSID      "SingularLabs"//CONFIG_ESP_WIFI_SSID
	#define EXAMPLE_ESP_WIFI_PASS      "8285676991"//CONFIG_ESP_WIFI_PASSWORD
	#define EXAMPLE_ESP_MAXIMUM_RETRY  10//CONFIG_ESP_MAXIMUM_RETRY
	void wifi_init_sta(void);
#endif