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
  char time[19];
  char location [12];
  char text_line_1 [18];
  char text_line_2 [18];
  char text_line_3 [18];
} system_data_display_t;



esp_err_t system_display_idle       (void);

esp_err_t system_display_boot       (void);

esp_err_t system_display_screen_1   (system_data_display_t data);       // Could be some parameter later

esp_err_t system_display_screen_2   (system_data_display_t data);       // Could be some parameter later

esp_err_t system_display_offline    (void);  

esp_err_t system_display_test       (void);


#endif