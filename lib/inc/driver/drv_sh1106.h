// File: main/sh1106.h

#ifndef DRV_SH1106_H
#define DRV_SH1106_H

#include <stdint.h>
#include "string.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "bsp_i2c.h"
#include "bsp_timer.h"

esp_err_t drv_sh1106_init(void);

esp_err_t drv_sh1106_clear_screen(void);

esp_err_t drv_sh1106_display_text(uint8_t x, uint8_t y, const char *str);     // Could adjust later

esp_err_t drv_sh1106_display_time (const char *time);

esp_err_t drv_sh1106_display_location (const char *location);

esp_err_t drv_sh1106_display_image(const uint8_t *image);  

esp_err_t drv_sh1106_draw_border_top  (void);

esp_err_t drv_sh1106_turn_on(void);

esp_err_t drv_sh1106_turn_off(void);








#endif // SH1106_H
