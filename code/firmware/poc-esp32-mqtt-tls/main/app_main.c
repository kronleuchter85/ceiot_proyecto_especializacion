/* MQTT Mutual Authentication Example */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
// #include "esp_sntp.h"
// #include "esp_netif_sntp.h"
#include "esp_log.h"
#include "mqtt_client.h"




#include <sys/time.h>
#include "esp_sntp.h"




// Set your local broker URI
#define BROKER_URI "mqtts://a2jlrwil23uep5-ats.iot.eu-west-2.amazonaws.com:8883"


#define LAT_MIN -55.0f
#define LAT_MAX -22.0f
#define LON_MIN -73.0f
#define LON_MAX -53.0f


#define NUM_DEVICES 50
#define STR_LEN 16


static const char *TAG = "MQTTS_EXAMPLE";

static char *BODY = "{\"deviceId\":\"%s\" , \"type\":\"%s\", \"value\":\"%0.2f\", \"geoLat\":\"%0.6f\",\"geoLong\":\"%0.6f\", \"date\":\"%s\" , \"time\":\"%s\"}";


extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_broker_CA_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_broker_CA_crt_end");


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}




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

float generar_latitud_arg() {
    return ((float)rand() / RAND_MAX) * (LAT_MAX - LAT_MIN) + LAT_MIN;
}

float generar_longitud_arg() {
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



// 
// time SNTP 
// 

void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Inicializando SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");  // Puedes usar otros servidores
    sntp_init();
}

void obtain_time(void)
{
    initialize_sntp();

    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2020 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Esperando sincronización NTP... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry < retry_count) {
        ESP_LOGI(TAG, "Hora sincronizada: %s", asctime(&timeinfo));
    } else {
        ESP_LOGW(TAG, "No se pudo sincronizar hora con NTP");
    }
}


/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        // ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
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

static void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .client_id = "test1",
        .uri = BROKER_URI,
        .client_cert_pem = (const char *)client_cert_pem_start,
        .client_key_pem = (const char *)client_key_pem_start,
        .cert_pem = (const char *)server_cert_pem_start,
    };

    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    // ESP_ERROR_CHECK(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_0, 0, SDA_GPIO, SCL_GPIO));
    // ESP_ERROR_CHECK(bmp280_init(&dev, &params));

    float pressure = 0.0, temperature= 0.0, humidity= 0.0;



    initialize_sntp();

    while(1){



        printf("Publishing message\n");

        char body[200];

        char timestamp[25];
        memset(timestamp , '\0' , sizeof(timestamp));
        get_timestamp(timestamp);



        char date_str[10];
        memset(date_str , '\0' , sizeof(date_str));
        get_date_str(date_str, sizeof(date_str));

        char time_str[10];
        memset(time_str , '\0' , sizeof(time_str));
        get_time_str(time_str , sizeof(time_str));




        for (int i = 0; i < NUM_DEVICES; i++) {

            printf("%s\n", deviceIds[i]);

            float latitude = generar_latitud_arg() ;
            float longitude = generar_longitud_arg();

            //
            // sending Temperature
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, deviceIds[i] , "Temperature" , generate_temperature()  , latitude, longitude, date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);
            
            //
            // sending Humidity
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, deviceIds[i] , "Humidity" , generate_humidity()   , latitude, longitude, date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);

            //
            // sending Pressure
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, deviceIds[i] , "Pressure" , generate_pressure()  , latitude, longitude , date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);
            
            //
            // sending Humidity
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, deviceIds[i] , "Luminosity" , generate_luminosity()  , latitude, longitude , date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);


        }


       
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{

    // esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    // esp_netif_sntp_init(&config);


    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */

    ESP_ERROR_CHECK(example_connect());

    mqtt_app_start();
}
