
#ifndef MEASURING_STATE_H
#define MEASURING_STATE_H

typedef struct{

    float temperature;
    float humidity;
    float pressure;
    float light;

} measuring_state_t;

measuring_state_t measuring_state_get();

void measuring_state_set_temperature(float temp);
void measuring_state_set_humidity(float humidity);
void measuring_state_set_pressure(float press);
void measuring_state_set_light(float light);


#endif