#include "system_manager.h"
#include "image_128x64.h"

// static void system_oled_test_default ();
static void system_oled_test_screen_1();

static system_main_state_t current_state = IDLE;
static system_main_state_t next_state;


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




static void system_oled_test_default ()
{
  drv_sh1106_display_image(image_logo_uit);

  bsp_timer_delay (3000);

  drv_sh1106_clear_screen();

  bsp_timer_delay (3000);

  drv_sh1106_display_image(image_logo_ce);

  bsp_timer_delay (3000);

  drv_sh1106_clear_screen();

  drv_sh1106_turn_off();
}


static void system_oled_test_screen_1()
{
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


}


static void system_test_dht11 (void)
{
    uint32_t count = 0;
    float temp = 0.0f, humid = 0.0f;  // Initialize variables to default values


    while(1)
    {
      
      if(drv_dht11_start_read())
      {  
        humid = drv_dht11_get_humid();
        temp = drv_dht11_get_temp();
        count += 1;
      }

      printf("Count: %lu\n", count);
      printf("[Temperature]> %.2f  \n",temp);
      printf("[Humidity]> %.2f \n \n",humid);

      bsp_timer_delay(2000);
    } 
}


static void system_manage_test_button (void)
{
  drv_btn_update_state();
}


static void system_manage_test (void)
{
// #define TEST_OLED_DEFAULT
// #define TEST_OLED_SCREEN_1
// #define TEST_DHT11
#define TEST_BUTTON

#ifdef TEST_OLED_DEFAULT
  system_oled_test_default();
#elif defined(TEST_OLED_SCREEN_1)
  system_oled_test_screen_1();
#elif defined(TEST_DHT11)
  system_test_dht11();
#elif defined(TEST_BUTTON)
  system_manage_test_button();
#endif
}


void system_manage_loop(void)
{
  current_state = system_manager_get_state();

  switch (current_state)
  {
    case IDLE:
      system_display_idle ();
      break;

    case BOOT_STATE:
      system_display_boot ();
      break;

    default:
      break;
  }

  return;
}



system_main_state_t system_manager_get_state ()
{
  next_state = current_state;

  switch (current_state)
  {
    case BOOT_STATE:
      if (drv_btn_detect_press() == MAIN_BTN_PRESSED)
        next_state = ONLINE_STATE;
      break;

    default:
      break;
  }

  return next_state;
}



