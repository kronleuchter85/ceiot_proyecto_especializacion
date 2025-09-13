
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "display_service.h"
#include "measuring_state.h"

//
// ---------------------------------------------------------------------------------------------------------
// Display
// ---------------------------------------------------------------------------------------------------------
//
void display_task(void * args){

    display_service_init();

    char first_line[20];
    char second_line[20];

    while (true) {

        measuring_state_t state = measuring_state_get();

        sprintf(first_line, "Temp: %.1f C", state.temperature);
        sprintf(second_line, "Hume: %.1f %%", state.humidity);

        display_service_print(first_line , second_line);
        vTaskDelay(3000 / portTICK_PERIOD_MS);

        sprintf(first_line, "Pres: %.1f hPa", state.pressure);
        sprintf(second_line, "Lumi: %.1f %%", state.light);
        
        display_service_print(first_line , second_line);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}