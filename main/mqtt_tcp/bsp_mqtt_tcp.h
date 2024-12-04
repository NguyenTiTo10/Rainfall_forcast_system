#ifndef MQTT_TCP_H
#define MQTT_TCP_H

#include "esp_err.h"
#include "esp_log.h"
#include "mqtt_client.h"

void mqtt_main (void);

typedef struct 
{
  char topic [100];
  char data [100];

} event_data_recieve_t;

event_data_recieve_t get_event_data (void); 

#endif // MQTT_TCP_H
