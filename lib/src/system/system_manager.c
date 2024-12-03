#include "system_manager.h"


static system_main_state_t current_state = IDLE;


esp_err_t system_manage_init (void)
{
  esp_err_t ret;

  ret = drv_sh1106_init();              // Init driver oled 
  if (ret != ESP_OK)
  {
    printf("Oled SH1106 ERORR.\n");
    return ret;
  }

  ret = drv_dht11_init();               // Init driver dht11
  if (ret != ESP_OK)
  {
    printf("DHT11 Initialization failed!");
    return ret;
  }

  // Init MQTT


  return ESP_OK;
}


// static void system_test_dht11 (void)
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


// static void system_manage_test (void)
// {
// // #define TEST_OLED_DEFAULT
// // #define TEST_OLED_SCREEN_1
// // #define TEST_DHT11
// #define TEST_BUTTON

// #ifdef TEST_OLED_DEFAULT
//   system_oled_test_default();
// #elif defined(TEST_OLED_SCREEN_1)
//   system_oled_test_screen_1();
// #elif defined(TEST_DHT11)
//   system_test_dht11();
// #elif defined(TEST_BUTTON)
//   system_manage_test_button();
// #endif
// }



system_main_state_t system_manage_update_state ()
{
  system_main_state_t next_state = current_state;

  switch (current_state)
  {
    case IDLE:
      // check if the data update is complete (dht11, mqtt, location)
      next_state = BOOT_STATE;
      break;

    case BOOT_STATE:
      if (drv_btn_detect_press() == MAIN_BTN_PRESSED)
        next_state = ONLINE_STATE;
      break;

    case ONLINE_STATE:
      if (drv_btn_detect_press() == MAIN_BTN_PRESSED)
        next_state = OFFLINE_STATE;
      break;

    case OFFLINE_STATE:
      if (drv_btn_detect_press() == MAIN_BTN_PRESSED)
        next_state = ONLINE_STATE;
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

    case ONLINE_STATE:
      system_display_online ();
      break;

    case OFFLINE_STATE:
      system_display_offline ();

    default:
      break;
  }

  return;
}


