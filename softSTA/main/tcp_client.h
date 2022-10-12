/**
 * @file station.h
 */
#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_
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
	//use CONFIG_EXAMPLE_IPV4_ADDR in make menuconfig
	//192.168.4.1->((u32_t)0xC0A80401UL)
	#define HOST_IP_ADDR ((const char*)"192.168.4.1")
	//use CONFIG_EXAMPLE_PORT in make menuconfig
	#define PORT ((u16_t)3333)

	void tcp_client_task(void *pvParameters);
#endif