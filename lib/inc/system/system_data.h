#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include "drv_bmp180.h"
#include "drv_dht11.h"
#include "drv_button.h"

typedef struct 
{
  float temp;
  float humid;
  float pressure;
} sys_data_sensor_update_t;


#endif