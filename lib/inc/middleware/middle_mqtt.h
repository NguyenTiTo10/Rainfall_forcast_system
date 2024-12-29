#ifndef MQTT_H
#define MQTT_H


#include "bsp_mqtt_tcp.h"


typedef enum
{
  TIME_UPDATE,
  HATINH_RAIN_UPDATE,
  QUANGBINH_RAIN_UPDATE,
  QUANGTRI_RAIN_UPDATE,
  UNKNOWN_UPDATE
} middle_mqtt_update_state_t;

void middle_mqtt_init();

void middle_mqtt_send_sensor_data ();

void middle_mqtt_get_time_update ();

void middle_mqtt_get_rainfall_update ();

bool middle_mqtt_get_data ();



void mqtt_test(void);

#endif