#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <stdio.h>

#include "drv_sh1106.h"
#include "drv_dht11.h"
#include "drv_button.h"

#include "system_display.h"


typedef enum
{
  IDLE,                             
  BOOT_STATE,                       
  HA_TINH_SCREEN_SENSOR,            
  HA_TINH_SCREEN_RAIN,
  QUANG_BINH_SCREEN_SENSOR,            
  QUANG_BINH_SCREEN_RAIN,
  QUANG_TRI_SCREEN_SENSOR,            
  QUANG_TRI_SCREEN_RAIN,
  OFFLINE_STATE
} system_main_state_t;

typedef enum
{
  UPDATE_TIME,
  UPDATE_SENSOR_DATA,
  UPDATE_RAIN,
} system_update_state_t;




void system_manage_init (void);

void system_manage_loop (void);


#endif