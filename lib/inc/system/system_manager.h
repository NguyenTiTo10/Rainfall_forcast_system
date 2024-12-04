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

#include "system_display.h"


typedef enum
{
  IDLE,                             // Collect data sensor, mqtt, oled, ...
  BOOT_STATE,                       // Display logo
  ONLINE_SCREEN_1_STATE,            // 2 display screen: dht11 and rainfall amount
  ONLINE_SCREEN_2_STATE,
  OFFLINE_STATE
} system_main_state_t;

typedef enum
{
  UPDATE_TIME,
  UPDATE_LOCATION,
  UPDATE_SENSOR_DATA,
  UPDATE_RAIN,
} system_update_state_t;

typedef struct 
{
  char time[10];
  char location [10];
  char text_line_1 [10];
  char text_line_2 [10];
  char text_line_3 [10];
} system_data_display_t;


esp_err_t system_manage_init (void);

void system_manage_loop (void);


#endif