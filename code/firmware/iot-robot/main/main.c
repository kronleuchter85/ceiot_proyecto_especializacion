#include <string.h>
#include <stdio.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

// #include "motors_service.h"
// #include "joystick_service.h"
// #include "robot_position_state.h"
// #include "measuring_state.h"

// UDP server headers
#include <sys/param.h>
#include "esp_wifi.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
// #include "lwip/err.h"
// #include "lwip/sockets.h"
// #include "lwip/sys.h"
// #include <lwip/netdb.h>
#include <bmp280.h>
#include "adc_service.h"

#include "wifi_service.h"


#include <sys/time.h>
#include "esp_sntp.h"

// #include "display_task.h"
#include "measuring_task.h"
#include "udp_server_task.h"
#include "mqtt_task.h"
#include "motors_task.h"

static const char *TAG = "IoT Robot";



//
// ---------------------------------------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------------------------------------
//
void app_main(void){

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_netif_init());
    
    // ---------------------------------------------------------------------------------------------------------
    // Wifi AP init
    // ---------------------------------------------------------------------------------------------------------
    // ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    // wifi_service_init_access_point();

    ESP_ERROR_CHECK(example_connect());

    // ---------------------------------------------------------------------------------------------------------
    // I2C init & ADC
    // ---------------------------------------------------------------------------------------------------------
    ESP_ERROR_CHECK(i2cdev_init());
    adc_service_initialize();


    // ---------------------------------------------------------------------------------------------------------
    // SNTP
    // ---------------------------------------------------------------------------------------------------------
    ESP_LOGI(TAG, "Inicializando SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");  // Puedes usar otros servidores
    sntp_init();

    // ---------------------------------------------------------------------------------------------------------
    // task del motor
    // ---------------------------------------------------------------------------------------------------------
    xTaskCreate(motors_task, "motors_task", 4096, (void*)true , 5, NULL);

    // ---------------------------------------------------------------------------------------------------------
    // task del joystick
    // ---------------------------------------------------------------------------------------------------------
    // xTaskCreate(joystick_task, "joystick_task", 4096, (void*)true , 5, NULL);

    // ---------------------------------------------------------------------------------------------------------
    // task UDP server
    // ---------------------------------------------------------------------------------------------------------
    xTaskCreate(udp_server_task, "udp_server", 4096, (void*)true , 5, NULL);
    
    // ---------------------------------------------------------------------------------------------------------
    // measuring task
    // ---------------------------------------------------------------------------------------------------------
    xTaskCreate(measuring_task, "measuring_task", 4096, (void*)true , 5, NULL);
    

    // ---------------------------------------------------------------------------------------------------------
    // display task
    // ---------------------------------------------------------------------------------------------------------
    // xTaskCreate(display_task, "display_task", 4096, (void*)true , 5, NULL);


    // ---------------------------------------------------------------------------------------------------------
    // MQTT task
    // ---------------------------------------------------------------------------------------------------------
    xTaskCreate(mqtt_service_task, "mqtt_service_task", 4096, (void*)true , 5, NULL);
    
}
