#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"
#include "bsp_timer.h"
#include "bsp_i2c.h"

#include "drv_sh1106.h"


typedef enum
{
  IDLE,
  BOOT_STATE,
  ONLINE_STATE,
  OFFLINE_STATE
} system_state_t;


void system_manage_loop (void);

#endif