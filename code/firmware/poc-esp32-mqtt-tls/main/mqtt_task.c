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


#include "utils.h"
#include "mqtt_task.h"

#include <sys/time.h>
#include "esp_sntp.h"




#define BROKER_URI "mqtts://a2jlrwil23uep5-ats.iot.eu-west-2.amazonaws.com:8883"



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



void mqtt_service_task(void * pvParameters)
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
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);


    float pressure = 0.0, temperature= 0.0, humidity= 0.0;


    while(1){

        printf("Publishing message\n");

        char body[200];

        char timestamp[25];
        memset(timestamp , '\0' , sizeof(timestamp));
        get_timestamp(timestamp);

        char date_str[12];
        memset(date_str , '\0' , sizeof(date_str));
        get_date_str(date_str, sizeof(date_str));

        char time_str[12];
        memset(time_str , '\0' , sizeof(time_str));
        get_time_str(time_str , sizeof(time_str));

        for (int i = 0; i < NUM_DEVICES; i++) {

            char * deviceId = getDeviceByIndex(i);
            int zoneId = getZoneIdByDeviceIndex(i);

            printf("%s\n", deviceId);

            RegionBox region = getRegionBoxByZoneId(zoneId);

            float latitude = generar_latitud_arg(region.lat_max , region.lat_min) ;
            float longitude = generar_longitud_arg(region.lon_max , region.lon_min);

            //
            // sending Temperature
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, getDeviceByIndex(i) , "Temperature" , generate_temperature()  , latitude, longitude, date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);
            
            //
            // sending Humidity
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, getDeviceByIndex(i) , "Humidity" , generate_humidity()   , latitude, longitude, date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);

            //
            // sending Pressure
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, getDeviceByIndex(i) , "Pressure" , generate_pressure()  , latitude, longitude , date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);
            
            //
            // sending Humidity
            //
            memset(body , '\0' , sizeof(body));
            sprintf(body, BODY, getDeviceByIndex(i) , "Luminosity" , generate_luminosity()  , latitude, longitude , date_str , time_str);
            esp_mqtt_client_publish(client, "readings", body, strlen(body), 1, 0);


        }
       
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
