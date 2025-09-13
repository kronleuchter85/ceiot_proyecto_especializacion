


#ifndef UTILS_H
#define UTILS_H

#define NUM_DEVICES 50
#define STR_LEN 16



typedef struct {
    float lat_min;
    float lat_max;
    float lon_min;
    float lon_max;
} RegionBox;

float generar_latitud_arg(float LAT_MAX , float LAT_MIN) ;
float generar_longitud_arg(float LON_MAX, float LON_MIN) ;
float random_float(float min, float max) ;
float generate_temperature() ;
float generate_humidity() ;
float generate_pressure() ;
float generate_luminosity() ;

void obtain_time(void);
void get_date_str(char * date_str , int len);
void get_time_str(char * times_str , int len);
void get_timestamp(char * timestamp);



char * getDeviceByIndex(int index);

int getZoneIdByDeviceIndex(int deviceIndex);

RegionBox getRegionBoxByZoneId(int zoneId);

#endif