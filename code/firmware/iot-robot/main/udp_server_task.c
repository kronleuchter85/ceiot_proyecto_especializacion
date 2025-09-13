#include <string.h>
#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#include "measuring_state.h"
#include "robot_position_state.h"

static const char *TAG = "UDP Server Task";

// UDP server conf
#define PORT 3333

static const char *payload = "Temp: 24.5 - Hum: 44 - Luz: 66 - Pres: 720";
static const char *payload2 = "%0.1f %0.1f %0.1f %0.1f";


//
// ---------------------------------------------------------------------------------------------------------
// UDP Server
// ---------------------------------------------------------------------------------------------------------
//
void udp_server_task(void * pvParameters){
    bool logging = (bool)pvParameters;

    char body[256];

    char rx_buffer[128];
    char addr_str[128];
    int addr_family = AF_INET;
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;

    while (1) {

        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");


        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10; 
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket bound, port %d", PORT);

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(source_addr);


        while (1) {
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                // break;
            }
            // Data received
            else {

                // procesamos el comando recibido

                if (source_addr.ss_family == PF_INET) {
                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
                } 

                rx_buffer[len] = 0;
                ESP_LOGI(TAG, "Received %d bytes from %s:%d - [%s]", len, addr_str , PORT , rx_buffer);
                
                //
                // actualizamos el estado del robot
                //
                robot_position_t action = robot_position_state_get_action_by_name(rx_buffer);
                robot_position_state_update(action);

            }

            measuring_state_t state = measuring_state_get();

            sprintf(body, payload2, state.temperature  , state.humidity , state.light , state.pressure );
            ESP_LOGI(TAG, "Sending status %s", body);

            int err = sendto(sock, body, strlen(body), 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }

            vTaskDelay(100 / portTICK_PERIOD_MS);

        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

