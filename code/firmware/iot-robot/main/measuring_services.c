#include "measuring_services.h"
#include <string.h>
#include <stdio.h>
#include "dht.h"
#include "bmp280.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "adc_service.h"

#include "esp_log.h"

#define DHT_GPIO 4
#define BMP_SDA_GPIO 18
#define BMP_SCL_GPIO 19

#define PHOTORESISTOR_MIN_READING 0.0
#define PHOTORESISTOR_MAX_READING 2050.0

static const char *TAG = "temp_collector";


//
// DHT11 consts
//

static const gpio_num_t dht_gpio = DHT_GPIO;
static const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;

//
// BMP280
//
bmp280_params_t params;
bmp280_t dev;

//
// ADC1 - fotoresistor
//

// static adc_channel_t channel = ADC_CHANNEL_0;      // ADC1:GPIO36, ADC2:GPIO4
// static adc_unit_t unit = ADC_UNIT_1;               // ADC2
// static adc_atten_t atten = ADC_ATTEN_DB_11;        // Full scale 0-3.9V, precision range 150mV-2450mV
// static esp_adc_cal_characteristics_t adc_chars;
// static esp_adc_cal_value_t val_type;

t_measuring_status measuring_services_init(void){


    // I2C init
    //
    // ESP_ERROR_CHECK(i2cdev_init());
    

    //
    // inicializacion del bmp280
    //

    if(bmp280_init_default_params(&params) != ESP_OK){
        return MEASURING_INITIALIZATION_ERROR;
    }
    memset(&dev, 0, sizeof(bmp280_t));
    if(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_0, 0, BMP_SDA_GPIO, BMP_SCL_GPIO) != ESP_OK ){
        return MEASURING_INITIALIZATION_ERROR;
    }
    if(bmp280_init(&dev, &params)!= ESP_OK){
        return MEASURING_INITIALIZATION_ERROR;
    }


    //
    // inicializacion del ADC1 para el fotoresistor
    //
    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten((adc1_channel_t)channel, atten);
    // esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);


    return MEASURING_INITIALIZATION_SUCCESS;
}


t_measuring_status measuring_service_get_temperature_and_humidity(int16_t * h, int16_t * t){

    int16_t temperature = 0;
    int16_t humidity = 0;

    if(dht_read_data(sensor_type, dht_gpio, &humidity, &temperature) == ESP_OK){
        *h = humidity;
        *t = temperature;

        return MEASURING_READING_SUCCESS;
    }else{
        *h = -1;
        *t = -1;

        return MEASURING_READING_ERROR;
    }
}

t_measuring_status measuring_service_get_pressure( float * p,float * t,float * h){

    float pressure =0, temperature =0, humidity =0;

    if(bmp280_read_float(&dev, &temperature, &pressure, &humidity) == ESP_OK){
        *h = humidity;
        *t = temperature;
        *p = pressure;
        return MEASURING_READING_SUCCESS;
    }else{
        *h = -1;
        *t = -1;
        *p = -1;
        return MEASURING_READING_ERROR;
    }
}

t_measuring_status measuring_service_get_light_level(int * reading , int * voltage , int * ligh_level){

    adc_service_light_read(reading , voltage);


    float level = (PHOTORESISTOR_MAX_READING - (float)*reading) / PHOTORESISTOR_MAX_READING;

    *ligh_level = level*100;

    // int adc_reading = adc1_get_raw((adc1_channel_t)channel);
    // uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars);
    // *reading = adc_reading;
    // double vol = *voltage / 1000.0f;
    // double Rt = 10 * vol / (3.3 - vol); //calculate resistance value of thermistor
    // double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); //calculate temperature (Kelvin)
    // double tempC = tempK - 273.15;     //calculate temperature (Celsius)

    ESP_LOGI(TAG, "Resistance: %d kOhm , Voltage: %d mV \n" , *reading, *voltage  );

    return MEASURING_READING_SUCCESS;
}