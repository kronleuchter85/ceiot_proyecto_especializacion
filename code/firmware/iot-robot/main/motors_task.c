
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "motors_service.h"
#include "robot_position_state.h"

// 
// Motors configuration
// 

// left
#define GPIO_MCPWM0_A_OUT 26   
#define GPIO_MCPWM0_B_OUT 27   

// right
#define GPIO_MCPWM1_A_OUT 33   
#define GPIO_MCPWM1_B_OUT 25   


//
// ---------------------------------------------------------------------------------------------------------
// Motors
// ---------------------------------------------------------------------------------------------------------
//
void motors_task(void *arg) {

    float duty_cicle_counter = 40.0;

    motors_initialize(MCPWM_UNIT_0 , MCPWM_TIMER_0 , GPIO_MCPWM0_A_OUT , GPIO_MCPWM0_B_OUT);
    motors_initialize(MCPWM_UNIT_1 , MCPWM_TIMER_1 , GPIO_MCPWM1_A_OUT , GPIO_MCPWM1_B_OUT);
    
    bool at_rest = false;
    
    while (1){

        if(duty_cicle_counter > 60.0)
            duty_cicle_counter = 40.0;

        // printf("---------------------------------------------------\n");
        // printf("duty_cycle = %.2f\n" , duty_cicle_counter);
        // printf("---------------------------------------------------\n");
        
        robot_position_t state = robot_position_state_get();
        
        switch (state)
        {
            case MOVING_FORWARD:
                // printf("MOVING_FORWARD ...\n");
                motors_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, duty_cicle_counter);
                motors_forward(MCPWM_UNIT_1, MCPWM_TIMER_1, duty_cicle_counter);
                at_rest = false;
                break;
            case MOVING_BACKWARD:
                // printf("MOVING_BACKWARD ...\n");
                motors_backward(MCPWM_UNIT_0, MCPWM_TIMER_0, duty_cicle_counter);
                motors_backward(MCPWM_UNIT_1, MCPWM_TIMER_1, duty_cicle_counter);
                at_rest = false;
                break;
            case ROTATE_LEFT:
                // printf("ROTATE_LEFT ...\n");
                motors_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, duty_cicle_counter);
                motors_backward(MCPWM_UNIT_1, MCPWM_TIMER_1, duty_cicle_counter);
                at_rest = false;
                break;
            case ROTATE_RIGHT:
                // printf("ROTATE_RIGHT ...\n");
                motors_backward(MCPWM_UNIT_0, MCPWM_TIMER_0, duty_cicle_counter);
                motors_forward(MCPWM_UNIT_1, MCPWM_TIMER_1, duty_cicle_counter);
                at_rest = false;
                break;
            default:
                // printf("default ...\n");
                if(!at_rest){
                    motors_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
                    motors_stop(MCPWM_UNIT_1, MCPWM_TIMER_1);
                }
                at_rest = true;
                break;
        }

        // duty_cicle_counter = duty_cicle_counter + 10.0;

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
