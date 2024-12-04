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


// void system_test_dht11 (void)
// {
//     uint32_t count = 0;
//     float temp = 0.0f, humid = 0.0f;  // Initialize variables to default values


//     while(1)
//     {
      
//       if(drv_dht11_start_read())
//       {  
//         humid = drv_dht11_get_humid();
//         temp = drv_dht11_get_temp();
//         count += 1;
//       }

//       printf("Count: %lu\n", count);
//       printf("[Temperature]> %.2f  \n",temp);
//       printf("[Humidity]> %.2f \n \n",humid);

//       bsp_timer_delay(2000);
//     } 
// }


// static void system_manage_test_button (void)
// {
//   drv_btn_update_state();
// }




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

  if (current_state != next_state)
    current_state = next_state;
  else if (task_handle != 0)
    task_handle = task_handle -1;
  else
    return;

  switch (current_state)
  {
    case IDLE:
      system_display_idle ();
      break;

    case BOOT_STATE:
      system_display_boot ();
      break;

    case ONLINE_SCREEN_1_STATE:
      system_display_screen_1 ();
      break;

    case ONLINE_SCREEN_2_STATE:
      system_display_screen_2();
      break;

    case OFFLINE_STATE:
      system_display_offline ();
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
}

