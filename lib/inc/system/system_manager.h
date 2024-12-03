#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"

#include "bsp_timer.h"
#include "bsp_i2c.h"

#include "drv_sh1106.h"
#include "drv_dht11.h"
#include "drv_button.h"


typedef enum
{
  IDLE,             // Collect data sensor, mqtt, oled, ...
  BOOT_STATE,       // Display logo
  ONLINE_STATE,
  OFFLINE_STATE
} system_main_state_t;

typedef enum
{
  UPDATE_TIME,
  UPDATE_LOCATION,
  UPDATE_SENSOR_DATA,
  UPDATE_RAIN,
  UPDATE_SCREEN_MODE
} system_update_state_t;

esp_err_t system_manage_init (void);
void system_manage_loop (void);

#endif