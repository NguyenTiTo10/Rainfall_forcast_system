#include "bsp_mqtt_tcp.h"

#define CONFIG_BROKER_URL      "mqtt://mqtt.flespi.io"
#define CONFIG_WIFI_SSID       "Redmi 12"      
#define CONFIG_WIFI_PASSWORD   "toanmanh@123456"   
#define PORT_ADDRESS           1883
#define USERNAME               "KBuTL4GcQIdeStibgS2YOd6YTJq1AydfcAde7ERrlOx1hJGaJjgPgAGe4GMqNVqc"
#define TOPIC                  "RAINFALL_FORCAST_SYSTEM"

#define MAX_RECONNECT_ATTEMPTS 5 // Maximum number of reconnection attempts

esp_mqtt_event_handle_t         event;
esp_mqtt_client_handle_t        client;
event_data_recieve_t            ret_data;

bool                            mqtt_wifi_connect_flag  =   false;  
bool                            mqtt_event_data_flag    =   false;

int                             msg_id;

static const char *TAG = "mqtt_example";

static EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0

int reconnect_attempts = 0; // Counter for Wi-Fi reconnection attempts

void set_mqtt_event_data(void);
bool bsp_mqtt_get_data_flag(void);
bool bsp_mqtt_get_wifi_flag(void);
void bsp_mqtt_set_data_flag(bool status);
void bsp_mqtt_set_wifi_flag(bool status);
int bsp_mqtt_client_subscribe(const char *topic);
int bsp_mqtt_client_publish(const char *topic, const char *data);

static void log_error_if_nonzero(const char *message, int error_code) 
{
    if (error_code != 0) 
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) 
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    event = event_data;
    client = event->client;

    switch ((esp_mqtt_event_id_t)event_id) 
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            bsp_mqtt_client_subscribe(TOPIC);
            bsp_mqtt_set_wifi_flag(true);                  // Set the flag to true on MQTT connection
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            bsp_mqtt_set_wifi_flag(false);                  // Set the flag to true on MQTT connection
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            bsp_mqtt_set_data_flag(true);
            set_mqtt_event_data();
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) 
            {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }

            bsp_mqtt_set_wifi_flag(false);                  // Set the flag to true on MQTT connection

            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

void set_mqtt_event_data(void) 
{
    ret_data.topic_length = event->topic_len;
    ret_data.topic = event->topic;

    ret_data.data_length = event->data_len;
    ret_data.data = event->data;
}

event_data_recieve_t get_ret_event_data(void) 
{
    return ret_data;
}

bool bsp_mqtt_get_data_flag(void) 
{
    return mqtt_event_data_flag;
}

void bsp_mqtt_set_data_flag(bool status) 
{
    mqtt_event_data_flag = status;
}

bool bsp_mqtt_get_wifi_flag(void) 
{
    return mqtt_wifi_connect_flag;
}

void bsp_mqtt_set_wifi_flag(bool status) 
{
    mqtt_wifi_connect_flag = status;
}

int bsp_mqtt_client_subscribe(const char *topic) 
{
    msg_id = esp_mqtt_client_subscribe(client, topic, 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
    return msg_id;
}

int bsp_mqtt_client_publish(const char *topic, const char *data) 
{
    msg_id = esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
    return msg_id;
}

static void mqtt_app_start(void) 
{
    esp_mqtt_client_config_t mqtt_cfg = 
    {
        .broker.address.uri = CONFIG_BROKER_URL,
        .broker.address.port = PORT_ADDRESS,
        .credentials.username = USERNAME,
        .credentials.authentication.password = "",
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) 
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        if (reconnect_attempts < MAX_RECONNECT_ATTEMPTS) 
        {
            reconnect_attempts++;
            ESP_LOGW(TAG, "Wi-Fi disconnected, retrying... Attempt %d of %d", reconnect_attempts, MAX_RECONNECT_ATTEMPTS);
            esp_wifi_connect();
        } 
        else 
        {
            ESP_LOGE(TAG, "Max Wi-Fi reconnection attempts reached. Stopping retries.");
        }
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        reconnect_attempts = 0; // Reset attempts on successful connection
    }
}

#define WIFI_CONNECT_TIMEOUT_MS  15000 // 15 seconds timeout

static void wifi_init(void) 
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = 
    {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Wi-Fi initialization completed. Waiting for connection...");

    // Wait for connection or timeout
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT,
                                           pdFALSE,
                                           pdTRUE,
                                           pdMS_TO_TICKS(WIFI_CONNECT_TIMEOUT_MS));

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to AP");
    } else {
        ESP_LOGE(TAG, "Failed to connect to AP within timeout");
        esp_wifi_stop(); // Stop Wi-Fi to clean up
        ESP_LOGI(TAG, "Wi-Fi stopped due to timeout");
        // Optionally return an error code or handle failure
    }
}


void bsp_mqtt_start(void) 
{
    reconnect_attempts = 0;         // Reset reconnection attempts counter
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();                    // Initialize Wi-Fi and wait for connection
    mqtt_app_start();               // Start MQTT client after Wi-Fi is connected
}

void bsp_mqtt_stop(void) 
{
    if (client != NULL) 
    {
        ESP_LOGI(TAG, "Stopping MQTT client...");
        esp_mqtt_client_stop(client);
        esp_mqtt_client_destroy(client);
        client = NULL;
    }
}

