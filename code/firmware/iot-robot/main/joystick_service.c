#include <string.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "esp_log.h"
#include "joystick_service.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "adc_service.h"


// // GPIO 34 (ADC1) - VRX
// static adc_channel_t channel_x =  ADC1_CHANNEL_6;      
// static adc_unit_t unit_x = ADC_UNIT_1;               
// static adc_atten_t atten_x = ADC_ATTEN_DB_11;        

// // GPIO 35 (ADC1) - VRY
// static adc_channel_t channel_y = ADC1_CHANNEL_7;      
// static adc_unit_t unit_y = ADC_UNIT_1;               
// static adc_atten_t atten_y = ADC_ATTEN_DB_11;        

// static esp_adc_cal_characteristics_t adc1_chars_x;
// static esp_adc_cal_characteristics_t adc1_chars_y;

// static const adc_bits_width_t WIDTH = ADC_WIDTH_BIT_11;


void joystick_initialize(){

    // usando ADC 1
    // adc1_config_width(WIDTH);
    // ESP_ERROR_CHECK(adc1_config_channel_atten(channel_x, atten_x));
    // esp_adc_cal_characterize(unit_x, atten_x, WIDTH, 0, &adc1_chars_x);
    // ESP_ERROR_CHECK(adc1_config_channel_atten(channel_y, atten_y));
    // esp_adc_cal_characterize(unit_y, atten_y, WIDTH, 0, &adc1_chars_y);
}

void joystick_get_reading(int * read_x , int * read_y){

    int adc_reading_x =  0;
    int adc_reading_y =  0;

    adc_service_joystick_read(&adc_reading_x , &adc_reading_y);

    // int adc_reading_x =  adc1_get_raw((adc1_channel_t)channel_x);
    // int adc_reading_y =  adc1_get_raw((adc1_channel_t)channel_y);

    *read_x = adc_reading_x / 900;
    *read_y = adc_reading_y / 900;

    joystick_transform(read_x , read_y);
}


//
// private functions
//

void joystick_transform(int * x , int * y){
    if(*x == 0)
        *x = JOYSTICK_READING_MIN;
    else if(*x == 1)
        *x = JOYSTICK_READING_NEUTRAL;
    else if(*x == 2)
        *x = JOYSTICK_READING_MAX;

    if(*y == 2)
        *y = JOYSTICK_READING_MAX;
    else if(*y == 1)
        *y = JOYSTICK_READING_NEUTRAL;
    else if( *y==0)
        *y = JOYSTICK_READING_MIN;
}