#include "softap.h"
#include "tcp_server.h"
void app_main(){
    initializeAccessPoint();
    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
}
