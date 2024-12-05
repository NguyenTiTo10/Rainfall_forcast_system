#include "system_display.h" 
#include "esp_log.h"


static void system_display_data_to_screen (system_data_display_t data)
{
  drv_sh1106_display_time(data.time);
  drv_sh1106_display_location (data.location);
  drv_sh1106_display_text(0, 3, data.text_line_1);
  drv_sh1106_display_text(0, 5, data.text_line_2); 
  drv_sh1106_display_text(0, 7, data.text_line_3);

  return;
}


void system_display_idle (void)
{
  drv_sh1106_turn_on();

  drv_sh1106_clear_screen();

  ESP_LOGI("Init state", "Completed");

  // Start let's dht11 read data

  // Start button

  return;
}


void system_display_boot (void)
{
  mid_font_display_logo_uit();

  drv_sh1106_delay_screen (500);

  drv_sh1106_clear_screen();

  drv_sh1106_draw_border_top ();

  ESP_LOGI("Boot state", "Completed");

  return;
}


void system_display_screen_sensor    (system_data_display_t data)
{
  // System check valid data for data sensor

  system_display_data_to_screen (data);
}     


void system_display_screen_rain      (system_data_display_t data)
{
  // System check valid data for data rainfall amount

  system_display_data_to_screen (data);
} 


void system_display_offline  (void)
{
  mid_font_display_logo_ce ();

  drv_sh1106_delay_screen (500);

  drv_sh1106_turn_off ();

  ESP_LOGI("OFFLINE STATE", "Completed");

  return;
}   
