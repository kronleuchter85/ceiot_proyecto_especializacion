#include "robot_position_state.h"
#include "joystick_service.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"


static const char *TAG = "POC Joystick - Reading";

//
// ---------------------------------------------------------------------------------------------------------
// Joystick
// ---------------------------------------------------------------------------------------------------------
//
static void joystick_task(void * args){

    // joystick_initialize();

    int reading_x ;
    int reading_y ;

    while (1) {

        joystick_get_reading(&reading_x , &reading_y);

        robot_position_t action = robot_position_state_get_action_by_coordinates(reading_x , reading_y);

        robot_position_state_update(action);

        ESP_LOGI(TAG, " (%d , %d) ", reading_x , reading_y);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
