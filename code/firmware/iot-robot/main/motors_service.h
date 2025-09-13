//
// del tutorial: https://esp32tutorials.com/esp32-dc-motor-l289n-esp-idf/
//


#ifndef MOTORS_SERVICE_H
#define MOTORS_SERVICE_H

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

void motors_initialize(mcpwm_unit_t unit_to_use , mcpwm_timer_t timer_to_use , int gpio_pin_A , int gpio_pin_b);

void motors_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle);

void motors_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle);

void motors_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

#endif