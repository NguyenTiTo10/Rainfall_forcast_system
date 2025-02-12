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

void middle_mqtt_request_update_rain (void);

bool middle_mqtt_get_data ();

void middle_mqtt_extract_time (void);

void middle_mqtt_extract_rain (void);

middle_mqtt_update_state_t middle_mqtt_detect_update_type ();

char *middle_mqtt_get_time (void);

char *middle_mqtt_get_rainfall_line_1 (void);

char *middle_mqtt_get_rainfall_line_2 (void);

char *middle_mqtt_get_rainfall_line_3 (void);

void middle_mqtt_send_sensor_data (float temp, float humid, float pressure);


#endif