#include "system_display.h" 
#include "image_128x64.h"


esp_err_t system_display_idle (void)
{
  drv_sh1106_clear_screen();
  bsp_timer_delay(10);

  // Start let's dht11 read data

  // Start button

  return ESP_OK;
}


esp_err_t system_display_boot (void)
{
  drv_sh1106_display_image(image_logo_uit);
  bsp_timer_delay(10);
}
