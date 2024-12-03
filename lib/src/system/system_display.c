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

  return ESP_OK;
}


esp_err_t system_display_online   (void)       // Could be some parameter later, this is just the code test
{
  drv_sh1106_clear_screen();
  bsp_timer_delay(10);
  
  char date_time [] = "29.11.2024 - 19:00";
  char location_1[] = "QUANG BINH";
  char location_2[] = "HO CHI MINH";

  drv_sh1106_draw_border_top(15, 0, 105, 18, 1);

  drv_sh1106_display_text_center(0, date_time);

  char temp[] = "Temp: 30.6 C ";
  char humid[] = "Humid: 80 %";
  char air_press[] = "Air press: 20 Pa";


  drv_sh1106_display_text(0, 3, temp);
  drv_sh1106_display_text(0, 5, humid);
  drv_sh1106_display_text(0, 7, air_press);

  
  drv_sh1106_display_text_center(1, location_1);
  bsp_timer_delay (3000);
  drv_sh1106_display_text_center(1, location_2);

  return ESP_OK;
}


esp_err_t system_display_offline  (void)
{
  drv_sh1106_clear_screen();
  bsp_timer_delay(10);

  drv_sh1106_display_image(image_logo_ce);
  bsp_timer_delay(10);

  return ESP_OK;
}   
