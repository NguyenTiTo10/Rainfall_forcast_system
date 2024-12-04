#include "system_display.h" 
#include "esp_log.h"

char date_time [] = "29.11.2024 - 19:00";
char location_1[] = "QUANG BINH";
char location_2[] = "HO CHI MINH";
char temp     [] = "Temp    : 30.6 C ";
char humid    [] = "Humid   : 80.0 % ";
char air_press[] = "Pressure: 20.0 Pa";

char ifs  [] = "IFS  : 35.6  43.2  30.0";
char tito [] = "Tito : 35.6  43.2  30.0";
char vrain[] = "Vrain: 35.6  43.2  30.0";


esp_err_t system_display_idle (void)
{
  drv_sh1106_turn_on();

  drv_sh1106_clear_screen();

  ESP_LOGI("Init state", "Completed");

  // Start let's dht11 read data

  // Start button

  return ESP_OK;
}


esp_err_t system_display_boot (void)
{
  mid_font_display_logo_uit();

  drv_sh1106_delay_screen (500);

  drv_sh1106_clear_screen();

  ESP_LOGI("Boot state", "Completed");

  return ESP_OK;
}


esp_err_t system_display_screen_1   (void)       // Could be some parameter later, this is just the code test
{
  drv_sh1106_draw_border_top ();

  drv_sh1106_display_time(date_time);
  drv_sh1106_display_location (location_1);

  drv_sh1106_display_text(0, 3, temp);
  drv_sh1106_display_text(0, 5, humid);
  drv_sh1106_display_text(0, 7, air_press);

  return ESP_OK;
}


esp_err_t system_display_screen_2   (void)       // Could be some parameter later, this is just the code test
{
  drv_sh1106_draw_border_top ();

  drv_sh1106_display_time(date_time);
  drv_sh1106_display_location (location_1);

  drv_sh1106_display_text(0, 3, ifs);
  drv_sh1106_display_text(0, 5, tito);
  drv_sh1106_display_text(0, 7, vrain);

  return ESP_OK;
}


esp_err_t system_display_offline  (void)
{
  mid_font_display_logo_ce ();

  drv_sh1106_delay_screen (500);

  drv_sh1106_turn_off ();

  return ESP_OK;
}   
