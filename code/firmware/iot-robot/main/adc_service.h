
#ifndef ADC_SERVICE_H
#define ADC_SERVICE_H

void adc_service_initialize();
void adc_service_joystick_read(int * adc_reading_x , int * adc_reading_y);
void adc_service_light_read(int * reading , int * voltage);

#endif