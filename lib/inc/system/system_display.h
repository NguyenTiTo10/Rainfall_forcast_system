#ifndef SYSTEM_DISPLAY_H
#define SYSTEM_DISPLAY_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"

#include "bsp_timer.h"
#include "bsp_i2c.h"

#include "drv_sh1106.h"

esp_err_t system_display_idle (void);

esp_err_t system_display_boot (void);


#endif