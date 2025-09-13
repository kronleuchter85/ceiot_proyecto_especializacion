#include "measuring_state.h"

measuring_state_t measuring_state;

measuring_state_t measuring_state_get(){
    return measuring_state;
}

void measuring_state_set_temperature(float temp){
    measuring_state.temperature = temp;
}
void measuring_state_set_humidity(float humidity){
    measuring_state.humidity = humidity;
}
void measuring_state_set_pressure(float press){
    measuring_state.pressure = press;
}
void measuring_state_set_light(float light){
    measuring_state.light = light;
}