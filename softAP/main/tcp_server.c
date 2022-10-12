#include "tcp_server.h"
#define CONFIG_EXAMPLE_IPV4

static const char *TAG = "example";

//static void tcp_server_task(void *pvParameters){
void tcp_server_task(void *pvParameters){
    char rx_buffer[128];
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    struct sockaddr_in destAddr;
    //ANY=255.255.255.255 = 0xFF.FF.FF.FF (accept all)
    destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    destAddr.sin_family = AF_INET;
    //u16_t PORT
    destAddr.sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);
    //create a socket
    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);        
    //if (listen_sock < 0) {ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);break;}
    if (listen_sock < 0) {ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);}
    ESP_LOGI(TAG, "Socket created");
    //bind address to the socket
    int err = bind(listen_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
    //if (err != 0) {ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);break;}
    if (err != 0) {ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);}    
    ESP_LOGI(TAG, "Socket binded");//log the info

    err = listen(listen_sock, 1);
    //if (err != 0) {ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);break;}
    if (err != 0) {ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);}
    ESP_LOGI(TAG, "Socket listening");

    struct sockaddr_in sourceAddr;
    uint addrLen = sizeof(sourceAddr);
    int sock = accept(listen_sock, (struct sockaddr *)&sourceAddr, &addrLen);
    //if (sock < 0) {ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);break;}
    if (sock < 0) {ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);}
    ESP_LOGI(TAG, "Socket accepted");

    while (1) {
        // receive data
        int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        // Error occured during receiving
        if (len < 0) {ESP_LOGE(TAG, "recv failed: errno %d", errno);break;}
        // Connection closed
        else if (len == 0) {ESP_LOGI(TAG, "Connection closed");break;}
        // Data received
        else {
            inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
            rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
            ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
            ESP_LOGI(TAG, "%s", rx_buffer);
            // send back receied data
            int err = send(sock, rx_buffer, len, 0);
            if (err < 0) {ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);break;}
        }
         vTaskDelay(200/portTICK_PERIOD_MS);
    }
    //===========================================================
    if (sock != -1) {
        ESP_LOGE(TAG, "Shutting down socket and restarting...");
        shutdown(sock, 0);
        close(sock);
    }
    vTaskDelete(NULL);
    //===========================================================
}
