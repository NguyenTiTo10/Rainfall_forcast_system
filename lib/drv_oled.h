// File: main/sh1106_driver.h

#ifndef SH1106_DRIVER_H
#define SH1106_DRIVER_H

#include "esp_log.h"

#include "driver/i2c.h"
#include "driver/gpio.h"

#include "esp_err.h"

#include "freertos/task.h"
#include "freertos/FreeRTOS.h"

#include <stdint.h>  
#include <stddef.h>  
#include <string.h>
#include "sdkconfig.h" 




// OLED display parameters
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// Function prototypes
void i2c_master_init(void);
void sh1106_init(void);
void sh1106_send_command(uint8_t command);
void sh1106_send_data(uint8_t *data, size_t len);
void sh1106_clear_display(void);
void sh1106_display_text(const char *text, uint8_t page);
void sh1106_display_image(const uint8_t *image_data, size_t size);

void oled_i2c_test (void);

#endif // SH1106_DRIVER_H
