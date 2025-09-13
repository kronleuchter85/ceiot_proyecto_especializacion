
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#include "utils.h"


RegionBox region_types[4] = {
    {-55.0, -38.0, -73.0, -63.0}, // ID = 0
    {-55.0, -38.0, -63.0, -53.0}, // ID = 1
    {-38.0, -21.0, -73.0, -63.0}, // ID = 2
    {-38.0, -21.0, -63.0, -53.0}  // ID = 3
};



uint8_t C[NUM_DEVICES] = {
    3, 2, 3, 1, 3, 3, 3, 1, 0, 0,
    1, 1, 3, 2, 1, 3, 2, 3, 0, 0,
    0, 1, 1, 2, 1, 2, 1, 3, 0, 0,
    2, 2, 3, 3, 0, 2, 1, 2, 3, 0,
    3, 0, 2, 1, 1, 0, 1, 2, 1, 1
};

const char deviceIds[NUM_DEVICES][STR_LEN] = {
    "12ad-dao23-ux23",
    "4b9f-xmc85-qw21",
    "7c2e-jkp94-nz17",
    "0d5g-pol56-tr90",
    "9a1s-bvc33-rt45",
    "8z7x-mnb12-kj34",
    "6y3t-qwe76-as12",
    "3h9u-wer65-pl09",
    "5j2k-lpo98-xy21",
    "1m0n-zxc45-vb67",
    "0q9w-hgf32-ty88",
    "2e4r-plm78-ju56",
    "7u8i-okn54-lp43",
    "9o0p-bnm87-xz21",
    "3q2w-hjk11-vm90",
    "4r5t-zxc34-qp78",
    "6y7u-vbn12-kl34",
    "8i9o-jkl56-op23",
    "1a2s-dfg44-wq12",
    "3z4x-qwe67-rt56",
    "5c6v-poi90-mn78",
    "7b8n-lkj23-vc45",
    "9m0l-xsw12-pl67",
    "2d3f-vbn78-oi34",
    "4g5h-mko56-qw89",
    "6j7k-zxc21-as34",
    "8l9p-wer65-ty90",
    "0q1w-asd43-vb12",
    "2e3r-fgh87-jk45",
    "4t5y-plm90-xz34",
    "6u7i-olk65-wq23",
    "8o9p-vbn11-rt78",
    "0a2s-dfg44-pl90",
    "3z4x-qwe67-as21",
    "5c6v-poi90-mn45",
    "7b8n-lkj23-vc67",
    "9m0l-xsw12-qw34",
    "2d3f-vbn78-oi12",
    "4g5h-mko56-jk67",
    "6j7k-zxc21-wq45",
    "8l9p-wer65-ty34",
    "0q1w-asd43-vb78",
    "2e3r-fgh87-jk90",
    "4t5y-plm90-xz12",
    "6u7i-olk65-as67",
    "8o9p-vbn11-rt45",
    "0a2s-dfg44-wq89",
    "3z4x-qwe67-vb34",
    "5c6v-poi90-pl78",
    "7b8n-lkj23-oi12",
    "9m0l-xsw12-jk56",
    "2d3f-vbn78-ty23"
};


// 
// coordenadas
// 

float generar_latitud_arg(float LAT_MAX , float LAT_MIN) {
    return ((float)rand() / RAND_MAX) * (LAT_MAX - LAT_MIN) + LAT_MIN;
}

float generar_longitud_arg(float LON_MAX, float LON_MIN) {
    return ((float)rand() / RAND_MAX) * (LON_MAX - LON_MIN) + LON_MIN;
}

float random_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX;
    return min + scale * (max - min);
}

// Temperatura en °C (ejemplo: -10 a 50)
float generate_temperature() {
    return random_float(-10.0, 50.0);
}

// Humedad relativa en % (0 a 100%)
float generate_humidity() {
    return random_float(0.0, 100.0);
}

// Presión atmosférica en hPa (ejemplo: 950 a 1050 hPa)
float generate_pressure() {
    return random_float(950.0, 1050.0);
}

// Luminosidad en porcentaje (0 a 100%)
float generate_luminosity() {
    return random_float(0.0, 100.0);
}


void get_date_str(char * date_str , int len){
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    strftime(date_str,len,"%Y-%m-%d", info);
}


void get_time_str(char * times_str , int len){
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    strftime(times_str,len,"%H:%M:%S", info);
}

void get_timestamp(char * timestamp){
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    strftime(timestamp,25,"%Y-%m-%d %H:%M:%S", info);
}


char * getDeviceByIndex(int index){
    return deviceIds[index];
}

int getZoneIdByDeviceIndex(int deviceIndex){
    return C[deviceIndex];
}


RegionBox getRegionBoxByZoneId(int zoneId){
    return region_types[zoneId];
}