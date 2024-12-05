#include "system_display.h" 
#include "esp_log.h"


void system_display_idle (void)
{
  drv_sh1106_turn_on();

  drv_sh1106_clear_screen();

  ESP_LOGI("Init state", "Completed");

  // Start let's dht11 read data

  // Start button

  return ESP_OK;
}


void system_display_boot (void)
{
  mid_font_display_logo_uit();

  drv_sh1106_delay_screen (500);

  drv_sh1106_clear_screen();

  drv_sh1106_draw_border_top ();

  ESP_LOGI("Boot state", "Completed");

  return ESP_OK;
}

void system_display_screen_1   (system_data_display_t data)       
{        
  drv_sh1106_display_time(data.time);
  drv_sh1106_display_location (data.location);
  drv_sh1106_display_text(0, 3, data.text_line_1);
  drv_sh1106_display_text(0, 5, data.text_line_2); 
  drv_sh1106_display_text(0, 7, data.text_line_3);
  // ------------------Display------------------//
  return ESP_OK;
}


void system_display_screen_2   (system_data_display_t data)       // Could be some parameter later, this is just the code test
{
  drv_sh1106_display_time(data.time);
  drv_sh1106_display_location (data.location);
  drv_sh1106_display_text(0, 3, data.text_line_1);
  drv_sh1106_display_text(0, 5, data.text_line_2); 
  drv_sh1106_display_text(0, 7, data.text_line_3);
  return ESP_OK;
}


void system_display_offline  (void)
{
  mid_font_display_logo_ce ();

  drv_sh1106_delay_screen (500);

  drv_sh1106_turn_off ();

  return ESP_OK;
}   
