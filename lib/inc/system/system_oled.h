#ifndef SYSTEM_OLED_H
#define SYSTEM_OLED_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"

#include "bsp_timer.h"
#include "bsp_i2c.h"

#include "drv_sh1106.h"

esp_err_t system_display_idle (void);

#endif