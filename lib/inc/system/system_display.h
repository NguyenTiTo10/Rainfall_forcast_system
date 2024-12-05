#ifndef SYSTEM_DISPLAY_H
#define SYSTEM_DISPLAY_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"

#include "bsp_timer.h"

#include "drv_sh1106.h"

#include "font_image.h"

#include "system_manager.h"

typedef struct 
{
  char *time;
  char *location;
  char *text_line_1;
  char *text_line_2;
  char *text_line_3;
} system_data_display_t;



void system_display_idle             (void);

void system_display_boot             (void);

void system_display_screen_sensor    (system_data_display_t data);       

void system_display_screen_rain      (system_data_display_t data);       

void system_display_offline          (void);  


#endif