#include "system_manager.h"
#include "esp_log.h"
#include <string.h>


#define DHT11_INTERVAL_US 3000000  // 3 seconds interval in microseconds

int64_t current_time;
int64_t last_time;  

static void system_manage_init_data_display(void);


system_data_display_t screen_location_sensor;
system_data_display_t screen_ha_tinh_rain;
system_data_display_t screen_quang_binh_rain;
system_data_display_t screen_quang_tri_rain;

static system_main_state_t current_state = IDLE;


static void system_manage_init_data_display()
{
  screen_location_sensor.location = "HO CHI MINH";

  screen_ha_tinh_rain.location      = "HA TINH";

  screen_quang_binh_rain.location   = "QUANG BINH";

  screen_quang_tri_rain.location    = "QUANG TRI";


  // Init the time display
  screen_location_sensor.time =   "27.12.2024 - 02:05";
  screen_ha_tinh_rain.time    =   "27.12.2024 - 02:05";
  screen_quang_binh_rain.time =   "27.12.2024 - 02:05";
  screen_quang_tri_rain.time  =   "27.12.2024 - 02:05";

  // Init the sensor display
  screen_location_sensor.text_line_1 = "Temp    : 99.9 C       ";
  screen_location_sensor.text_line_2 = "Humid   : 99.9 %       ";
  screen_location_sensor.text_line_3 = "Pressure: 99.9 Pa      ";
  
#define TEST_DISPLAY_RAIN


#ifdef TEST_DISPLAY_RAIN
  screen_ha_tinh_rain.text_line_1 = "IFS  :  0.2   1.3   0.7";
  screen_ha_tinh_rain.text_line_2 = "Tito :  0.3   0.9   0.5";
  screen_ha_tinh_rain.text_line_3 = "Vrain:  1.6   0.9   0.8";

  screen_quang_binh_rain.text_line_1 = "IFS  : 35.6  43.2  30.0";
  screen_quang_binh_rain.text_line_2 = "Tito : 35.6  43.2  30.0";
  screen_quang_binh_rain.text_line_3 = "Vrain: 35.6  43.2  30.0";

  screen_quang_tri_rain.text_line_1 = "IFS  : 35.6  43.2  30.0";
  screen_quang_tri_rain.text_line_2 = "Tito : 35.6  43.2  30.0";
  screen_quang_tri_rain.text_line_3 = "Vrain: 35.6  43.2  30.0";

#endif

  return;
}


void system_manage_init (void)
{
  if (drv_sh1106_init() != ESP_OK)
    printf("Oled SH1106 ERORR.\n");

  if (!drv_dht11_init())
    printf("DHT11 Initialization failed! \n");

  if (drv_bmp180_init() != DRV_BMP180_OK)
    printf("BMP180 Initialization failed! \n");
  
  system_manage_init_data_display();            // Init display data 

  // Init MQTT

  // Start Timer
  last_time = bsp_timer_get_time();  

  return;
}


system_main_state_t system_manage_update_state ()
{
  system_main_state_t next_state = current_state;
  drv_btn_type_t btn_pressed = drv_btn_detect_press(); // Store the result of button detection

  switch (current_state)
  {
    case IDLE:
      next_state = BOOT_STATE;
      break;

    case BOOT_STATE:
      next_state = CURRENT_LOCATION_SCREEN_SENSOR;
      break;

    case CURRENT_LOCATION_SCREEN_SENSOR:
      if (btn_pressed == MAIN_BTN_PRESSED)
        next_state = OFFLINE_STATE;

      if (btn_pressed == MID_BTN_PRESSED)
          next_state = HA_TINH_SCREEN_RAIN;

      break;

    case HA_TINH_SCREEN_RAIN:
      if (btn_pressed == MAIN_BTN_PRESSED)
          next_state = OFFLINE_STATE;

      if (btn_pressed == LEFT_BTN_PRESSED)
        next_state = QUANG_TRI_SCREEN_RAIN;

      if (btn_pressed == MID_BTN_PRESSED)
          next_state = CURRENT_LOCATION_SCREEN_SENSOR;
      
      if (btn_pressed == RIGHT_BTN_PRESSED)
          next_state = QUANG_BINH_SCREEN_RAIN;
      break;

    case QUANG_BINH_SCREEN_RAIN:
      if (btn_pressed == MAIN_BTN_PRESSED)
          next_state = OFFLINE_STATE;

      if (btn_pressed == LEFT_BTN_PRESSED)
        next_state = HA_TINH_SCREEN_RAIN;

      if (btn_pressed == MID_BTN_PRESSED)
          next_state = CURRENT_LOCATION_SCREEN_SENSOR;
      
      if (btn_pressed == RIGHT_BTN_PRESSED)
          next_state = QUANG_TRI_SCREEN_RAIN;
      break;

    case QUANG_TRI_SCREEN_RAIN:
      if (btn_pressed == MAIN_BTN_PRESSED)
          next_state = OFFLINE_STATE;

      if (btn_pressed == LEFT_BTN_PRESSED)
        next_state = QUANG_BINH_SCREEN_RAIN;

      if (btn_pressed == MID_BTN_PRESSED)
          next_state = CURRENT_LOCATION_SCREEN_SENSOR;
      
      if (btn_pressed == RIGHT_BTN_PRESSED)
          next_state = HA_TINH_SCREEN_RAIN;
      break;

    case OFFLINE_STATE:
      if (btn_pressed == MAIN_BTN_PRESSED)
        next_state = IDLE;
      break;

    default:
      break;
  }

  return next_state;
}


void system_manage_update_data (void)
{
  int64_t current_time = esp_timer_get_time();

  // Check if 3 seconds have passed
  if ((current_time - last_time) >= DHT11_INTERVAL_US)
  {
    last_time = current_time;   

    system_data_start_update_sensor();                

    screen_location_sensor.text_line_1 = system_data_get_update_temp();
    screen_location_sensor.text_line_2 = system_data_get_update_humid();
    screen_location_sensor.text_line_3 = system_data_get_update_press();
  }

  // Check if recive data from MQTT

  return;
}


void system_manage_loop(void)
{
  system_manage_update_data ();
  
  switch (current_state)
  {
    case IDLE:
      system_display_idle ();
      break;

    case BOOT_STATE:
      system_display_boot ();
      break;

    case CURRENT_LOCATION_SCREEN_SENSOR:
      system_display_screen_sensor(screen_location_sensor);
      break;

    case HA_TINH_SCREEN_RAIN:
      system_display_screen_rain  (screen_ha_tinh_rain);
      break;

    case QUANG_BINH_SCREEN_RAIN:
      system_display_screen_rain  (screen_quang_binh_rain);
      break;

    case QUANG_TRI_SCREEN_SENSOR:
      system_display_screen_sensor(screen_quang_tri_sensor);
      break;

    case QUANG_TRI_SCREEN_RAIN:
      system_display_screen_rain  (screen_quang_tri_rain);
      break;

    case OFFLINE_STATE:
      system_display_offline ();
      break;

    default:
      break;
  }

  system_main_state_t next_state = system_manage_update_state();
  current_state = next_state;

  return;
}