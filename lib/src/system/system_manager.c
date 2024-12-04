#include "system_manager.h"
#include "esp_log.h"


static system_main_state_t current_state = IDLE;
// static system_main_state_t next_state    = IDLE;

static uint8_t task_handle = 0;

static void system_manage_idle      (void);
static void system_manage_boot      (void);
static void system_manage_screen_1  (void);
static void system_manage_screen_2  (void);
static void system_manage_offline   (void);


esp_err_t system_manage_init (void)
{
  esp_err_t ret;

  ret = drv_sh1106_init();              // Init driver oled 
  if (ret != ESP_OK)
  {
    printf("Oled SH1106 ERORR.\n");
    return ret;
  }

  uint8_t err = drv_dht11_init();               // Init driver dht11
  if (!err)
    printf("DHT11 Initialization failed! \n");
  
  // Init MQTT


  return ESP_OK;
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
      next_state = ONLINE_SCREEN_1_STATE;
      break;

    case ONLINE_SCREEN_1_STATE:
      if (btn_pressed == MAIN_BTN_PRESSED)
          next_state = OFFLINE_STATE;

      if (btn_pressed == MID_BTN_PRESSED)
          next_state = ONLINE_SCREEN_2_STATE;

      break;

    case ONLINE_SCREEN_2_STATE:
      if (btn_pressed == MAIN_BTN_PRESSED)
          next_state = OFFLINE_STATE;

      if (btn_pressed == MID_BTN_PRESSED)
          next_state = ONLINE_SCREEN_1_STATE;

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

void system_manage_loop(void)
{
  system_main_state_t next_state = system_manage_update_state();
  current_state = next_state;

  switch (current_state)
  {
    case IDLE:
      system_manage_idle ();
      break;

    case BOOT_STATE:
      system_manage_boot ();
      break;

    case ONLINE_SCREEN_1_STATE:
      system_manage_screen_1 ();
      break;

    case ONLINE_SCREEN_2_STATE:
      system_manage_screen_2();
      break;

    case OFFLINE_STATE:
      system_manage_offline ();
      break;

    default:
      break;
  }

  return;
}


void system_manage_idle (void)
{
  system_display_idle ();
}


void system_manage_boot (void)
{
  system_display_boot ();

  // Send request to server 

  // Take data from server
}


void system_manage_screen_1 (void)
{
  // Check once again whether the essential data is complete

  // Take from system_update_data

  // Send to system_display

  system_data_display_t data_scree_1 = 
  {
    .time = "29.11.2024 - 19:00",
    .location = "HO CHI MINH",
    .text_line_1 = "Temp    : 30.6 C ",
    .text_line_2 = "Humid   : 80.0 % ",
    .text_line_3 = "Pressure: 20.0 Pa"
  };

  system_display_screen_1 (data_scree_1);
}


void system_manage_screen_2 (void)
{
  // Check once again whether the essential data is complete

  // Take from system_update_data

  // Send to system_display
  system_data_display_t data_screen_2 = 
  {
    .time = "29.11.2024 - 19:00",
    .location = "HO CHI MINH",
    .text_line_1 = "IFS  : 35.6  43.2  30.0",
    .text_line_2 = "Tito : 35.6  43.2  30.0",
    .text_line_3 = "Vrain: 35.6  43.2  30.0",
  };

  system_display_screen_2(data_screen_2);
}


void system_manage_offline (void)
{
  system_display_offline ();
}