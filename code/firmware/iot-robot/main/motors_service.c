//
// del tutorial: https://esp32tutorials.com/esp32-dc-motor-l289n-esp-idf/
//

#include <stdio.h>
#include "motors_service.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "esp_attr.h"


void motors_initialize(mcpwm_unit_t unit_to_use , mcpwm_timer_t timer_to_use ,int gpio_pin_A , int gpio_pin_b){

  // 1. mcpwm gpio initialization
  printf("initializing mcpwm gpio...\n");
  if(unit_to_use == MCPWM_UNIT_0){
    mcpwm_gpio_init(unit_to_use, MCPWM0A, gpio_pin_A);
    mcpwm_gpio_init(unit_to_use, MCPWM0B, gpio_pin_b);
  }else if(unit_to_use ==MCPWM_UNIT_1){
    mcpwm_gpio_init(unit_to_use, MCPWM1A, gpio_pin_A);
    mcpwm_gpio_init(unit_to_use, MCPWM1B, gpio_pin_b);
  }

  // 2. initial mcpwm configuration
  printf("Configuring Initial Parameters of mcpwm...\n");
  mcpwm_config_t pwm_config;
  pwm_config.frequency = 1000; // frequency = 500Hz,
  pwm_config.cmpr_a = 0;       // duty cycle of PWMxA = 0
  pwm_config.cmpr_b = 0;       // duty cycle of PWMxb = 0
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(unit_to_use, timer_to_use, &pwm_config); // Configure PWM0A & PWM0B with above settings
}

void motors_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle){
  mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
  mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
  mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); // call this each time, if operator was previously in low/high state
}

void motors_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle){
  mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
  mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
  mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); // call this each time, if operator was previously in low/high state
}

void motors_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num){
  mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
  mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
}
