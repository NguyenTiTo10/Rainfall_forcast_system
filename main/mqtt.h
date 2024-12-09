#ifndef MQTT_H
#define MQTT_H


#include "bsp_mqtt_tcp.h"


void middle_mqtt_init();

void middle_mqtt_send_sensor_data ();

void middle_mqtt_get_time_update ();

void middle_mqtt_get_rainfall_update ();


void mqtt_test(void);

#endif