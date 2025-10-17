/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <bmp280.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "display_service.h"
#include "joystick_service.h"
#include "robot_position_state.h"
#include "measuring_state.h"

#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "example";

static const char * MESSAGE_FORWARD = "MOVING_FORWARD";
static const char * MESSAGE_BACKWARD = "MOVING_BACKWARD";
static const char * MESSAGE_LEFT = "ROTATE_LEFT";
static const char * MESSAGE_RIGHT = "ROTATE_RIGHT";
static const char * MESSAGE_STOP = "STOP";



//
// ---------------------------------------------------------------------------------------------------------
// Display
// ---------------------------------------------------------------------------------------------------------
//
static void display_task(void * args){

    display_service_init();

    char first_line[20];
    char second_line[20];

    while (true) {

        measuring_state_t state = measuring_state_get();

        sprintf(first_line, "Temp: %.1f C", state.temperature);
        sprintf(second_line, "Hume: %.1f %%", state.humidity);
        // sprintf(first_line, "Temp: %.1f C", 21.1);
        // sprintf(second_line, "Hume: %.1f %%", 25.5);

        display_service_print(first_line , second_line);
        vTaskDelay(3000 / portTICK_RATE_MS);

        sprintf(first_line, "Pres: %.1f hPa", state.pressure);
        sprintf(second_line, "Lumi: %.1f %%", state.light);
        // sprintf(first_line, "Pres: %.1f hPa", 1000.0);
        // sprintf(second_line, "Lumi: %.1f %%", 50.0);
        
        display_service_print(first_line , second_line);
        vTaskDelay(3000 / portTICK_RATE_MS);
    }
}


//
// ---------------------------------------------------------------------------------------------------------
// UDP Task
// ---------------------------------------------------------------------------------------------------------
//
static void udp_client_task(void *pvParameters){
    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);


        struct sockaddr_storage source_addr;
        socklen_t socklen = sizeof(source_addr);

        while (1) {

            char * payload = NULL;
            int i;
            int err;
            int times = 1000;

            if(robot_position_state_get() == MOVING_FORWARD){
                payload = MESSAGE_FORWARD;
            }else if (robot_position_state_get() == MOVING_BACKWARD){
                payload = MESSAGE_BACKWARD;
            }else if (robot_position_state_get() == ROTATE_LEFT){
                payload = MESSAGE_LEFT;
            }else if (robot_position_state_get() == ROTATE_RIGHT){
                payload = MESSAGE_RIGHT;
            }else if (robot_position_state_get() == REST){
                payload = MESSAGE_STOP;
            }

            //
            // enviando commandos
            //
            err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }

            //
            // recibiendo status del robot
            //
            
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
            }
            else {
                rx_buffer[len] = 0; 
                ESP_LOGI(TAG, "Received %d bytes from %s: [%s]", len, host_ip , rx_buffer);

                char * val = NULL;
                
                val = strtok(rx_buffer, " ");
                ESP_LOGI(TAG, "Parsed values [%s]", val);
                float temperature = atof(val);

                val = strtok(NULL, " ");
                ESP_LOGI(TAG, "Parsed values [%s]", val);
                float humidity = atof(val);
                
                val = strtok(NULL, " ");
                ESP_LOGI(TAG, "Parsed values [%s]", val);
                float light = atof(val);
                
                val = strtok(NULL, " ");
                ESP_LOGI(TAG, "Parsed values [%s]", val);
                float pressure = atof(val);

                measuring_state_set_temperature(temperature);        
                measuring_state_set_humidity(humidity);        
                measuring_state_set_light(light);        
                measuring_state_set_pressure(pressure);        

                // if (strncmp(rx_buffer, "OK: ", 4) == 0) {
                //     ESP_LOGI(TAG, "Received expected message, reconnecting");
                //     break;
                // }
            }


            ESP_LOGI(TAG, "Mensaje enviado [%s]", payload);
            vTaskDelay(5 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}


//
// ---------------------------------------------------------------------------------------------------------
// Joystick Task
// ---------------------------------------------------------------------------------------------------------
//
static void joystick_task(void * args){

    joystick_initialize();

    int reading_x ;
    int reading_y ;

    while (1) {

        joystick_get_reading(&reading_x , &reading_y);

        robot_position_t action = robot_position_state_get_action_by_coordinates(reading_x , reading_y);

        robot_position_state_update(action);

        ESP_LOGI("POC Joystick - Reading", " (%d , %d) ", reading_x , reading_y);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}



void app_main(void){

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    // I2C init
    //
    ESP_ERROR_CHECK(i2cdev_init());
    
    //
    // task del joystick
    //
    xTaskCreate(joystick_task, "joystick_task", 4096, NULL, 5, NULL);

    //
    // task del udp_server
    //
    xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);


    //
    // display task
    //
    xTaskCreate(display_task, "display_task", 4096, (void*)true , 5, NULL);
}
