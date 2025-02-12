#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include "string.h"

#include "drv_bmp180.h"
#include "drv_dht11.h"
#include "drv_button.h"

#include "middle_mqtt.h"

typedef struct 
{
  float temp;
  float humid;
  float pressure;
} sys_data_sensor_update_t;

void system_data_start_update_sensor  (void);

void system_data_send_data_sensor     (void);  


char *system_data_get_update_temp     (void);
char *system_data_get_update_humid    (void);
char *system_data_get_update_press    (void);

#endif