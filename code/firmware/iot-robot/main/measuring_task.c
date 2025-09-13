#include <string.h>
#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"

#include "measuring_services.h"
#include "measuring_state.h"


static const char *TAG = "Measuring Task";


//
// ---------------------------------------------------------------------------------------------------------
// Measuring
// ---------------------------------------------------------------------------------------------------------
//
void measuring_task(void *pvParameters) {

    // dht11
    int16_t temperature = 0;
    int16_t humidity = 0;
    
    //bmp280
    float temp2 , hum2, pressure;
    
    // photoresistor
    int light_reading = 0;
    int voltage = 0;
    int ligh_level = 0;

    if(measuring_services_init() != MEASURING_INITIALIZATION_SUCCESS)
        return;

    while(1) {

        if (measuring_service_get_temperature_and_humidity( &humidity, &temperature) == MEASURING_READING_SUCCESS) {
            measuring_state_set_humidity(humidity/10);
            measuring_state_set_temperature(temperature/ 10);
        } else {
            ESP_LOGE(TAG,"Could not read data from sensor\n");
        }

        if (measuring_service_get_pressure( &pressure,  &temp2, &hum2) != MEASURING_READING_SUCCESS) {
            ESP_LOGI(TAG, "Temperature/pressure reading failed\n");
        } else {

            measuring_state_set_pressure(pressure/100);
        }    

        if (measuring_service_get_light_level( &light_reading, &voltage , &ligh_level) != MEASURING_READING_SUCCESS) {
            ESP_LOGI(TAG, "Light reading failed\n");
        } else {
            measuring_state_set_light((float)ligh_level);
        }    

        ESP_LOGI(TAG, "BMP280_Pressure: %.2f Pa, BMP280_Temperature: %.2f C, DHT11_Temperature: %d C, DHT11_Humidity: %d %%, Light: %d %%"
            , pressure, temp2, temperature , humidity , ligh_level);

        measuring_state_t state = measuring_state_get();
        
        ESP_LOGI(TAG, "Pressure: %.2f Pa, Temperature: %.2f C, Humidity: %.2f %%, Light: %.2f %%"
            , state.pressure, state.temperature, state.humidity , state.light);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
