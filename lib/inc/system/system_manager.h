#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <stdio.h>

// #include"esp_err.h"
#include "bsp_timer.h"

#include "drv_bmp180.h"
#include "drv_sh1106.h"
#include "drv_dht11.h"
#include "drv_button.h"

#include "middle_mqtt.h"

#include "system_data.h"
#include "system_display.h"


typedef enum
{
  IDLE,                             
  BOOT_STATE,   
  CURRENT_LOCATION_SCREEN_SENSOR,                    
  HA_TINH_SCREEN_RAIN,        
  QUANG_BINH_SCREEN_RAIN,
  QUANG_TRI_SCREEN_RAIN,
  OFFLINE_STATE
} system_main_state_t;






void system_manage_init (void);

void system_manage_loop (void);


#endif