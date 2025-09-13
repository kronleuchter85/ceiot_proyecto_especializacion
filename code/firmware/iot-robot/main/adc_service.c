#include "esp_adc_cal.h"
#include "driver/adc.h"

//Default vref para el ADC1 - fotoresistor
#define DEFAULT_VREF    1100        


//
// ADC1 - fotoresistor
//
// ADC1:GPIO36
static adc_channel_t channel = ADC_CHANNEL_0;      
static adc_unit_t unit = ADC_UNIT_1;               
// Full scale 0-3.9V, precision range 150mV-2450mV
static adc_atten_t atten = ADC_ATTEN_DB_11;        
static esp_adc_cal_characteristics_t adc_chars;
static esp_adc_cal_value_t val_type;

//
// joystick
//
// GPIO 34 (ADC1) - VRX
static adc_channel_t channel_x =  ADC1_CHANNEL_6;      
static adc_unit_t unit_x = ADC_UNIT_1;               
static adc_atten_t atten_x = ADC_ATTEN_DB_11;        

// GPIO 35 (ADC1) - VRY
static adc_channel_t channel_y = ADC1_CHANNEL_7;      
static adc_unit_t unit_y = ADC_UNIT_1;               
static adc_atten_t atten_y = ADC_ATTEN_DB_11;        


static adc_bits_width_t WIDTH = ADC_WIDTH_BIT_11;

static esp_adc_cal_characteristics_t adc1_chars_x;
static esp_adc_cal_characteristics_t adc1_chars_y;

void adc_service_initialize(){

    //
    // joystick
    //
    adc1_config_width(WIDTH);
    ESP_ERROR_CHECK(adc1_config_channel_atten(channel_x, atten_x));
    esp_adc_cal_characterize(unit_x, atten_x, WIDTH, 0, &adc1_chars_x);
    ESP_ERROR_CHECK(adc1_config_channel_atten(channel_y, atten_y));
    esp_adc_cal_characterize(unit_y, atten_y, WIDTH, 0, &adc1_chars_y);

    //
    // inicializacion del ADC1 para el fotoresistor
    //
    // adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten((adc1_channel_t)channel, atten);
    esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

}

void adc_service_joystick_read(int * adc_reading_x , int * adc_reading_y){
    * adc_reading_x =  adc1_get_raw((adc1_channel_t)channel_x);
    * adc_reading_y =  adc1_get_raw((adc1_channel_t)channel_y);
}

void adc_service_light_read(int * reading , int * voltage){
    *reading = adc1_get_raw((adc1_channel_t)channel);
    *voltage = esp_adc_cal_raw_to_voltage(*reading, &adc_chars);
}