#ifndef MQTT_TCP_H
#define MQTT_TCP_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
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

#include "esp_log.h"
#include "mqtt_client.h"


typedef struct 
{
  char topic [100];
  char data [100];

} event_data_recieve_t;


void mqtt_main (void);
event_data_recieve_t get_event_data (esp_mqtt_event_handle_t event); 




#endif // MQTT_TCP_H
