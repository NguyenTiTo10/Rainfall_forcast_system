#include "bsp_mqtt_tcp.h"

#define     CONFIG_BROKER_URL   "mqtt://mqtt.flespi.io"
#define     PORT_ADDRESS        1883
#define     USERNAME            "KBuTL4GcQIdeStibgS2YOd6YTJq1AydfcAde7ERrlOx1hJGaJjgPgAGe4GMqNVqc" 
#define     TOPIC               "RAINFALL_FORCAST_SYSTEM"


esp_mqtt_event_handle_t     event;

esp_mqtt_client_handle_t    client;

event_data_recieve_t        ret_data;

bool                        mqtt_event_data_flag = false;

int                         msg_id;


static const char *TAG = "mqtt_example";

void set_mqtt_event_data        (void);
bool bsp_mqtt_get_data_flag     (void);
void bsp_mqtt_set_data_flag     (bool status);
int  bsp_mqtt_client_subscribe (const char *topic);
int  bsp_mqtt_client_publish   (const char *topic, const char *data);


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

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
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
            set_mqtt_event_data ();

            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) 
            {
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



void set_mqtt_event_data (void)
{
    ret_data.topic_length = event->topic_len;
    ret_data.topic = event->topic;

    ret_data.data_length = event->data_len;
    ret_data.data = event->data;

    return;
}


event_data_recieve_t get_ret_event_data (void)
{
    return ret_data;
}


bool bsp_mqtt_get_data_flag (void)
{
    return mqtt_event_data_flag;
}


void bsp_mqtt_set_data_flag (bool status)
{
    mqtt_event_data_flag = status;
    return;
}


int bsp_mqtt_client_subscribe (const char *topic)
{
    msg_id = esp_mqtt_client_subscribe(client, topic, 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    return msg_id;
}


int bsp_mqtt_client_publish   (const char *topic, const char *data)
{
    msg_id = esp_mqtt_client_publish(client, topic, data, 0, 0, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
    return msg_id;
}



static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = 
    {
        .broker.address.uri                   = CONFIG_BROKER_URL,
        .broker.address.port                  = PORT_ADDRESS,
        .credentials.username                 = USERNAME,
        .credentials.authentication.password  = "",
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}


void esp_log_connect (void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("mqtt_example", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("transport", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);
}


void mqtt_main(void)
{
    // esp_log_connect();

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
