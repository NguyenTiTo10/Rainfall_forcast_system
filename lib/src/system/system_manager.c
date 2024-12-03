#include "system_manager.h"
#include "image_128x64.h"

// static void system_oled_test_default ();
static void system_oled_test_screen_1();


void system_manage_init (void)
{
    // Init the oled 
  if (drv_sh1106_init() == ESP_OK)
    printf("Oled SH1106 initialized successfully.\n");
  else
    printf("Oled SH1106 ERORR.\n");

  // driver button init ();

  // driver dht11 init();

  // driver lm75 init();

  // driver bmp (air pressure) init();
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
  bsp_delay (3000);
  drv_sh1106_display_text_center(1, location_2);


}

static void system_oled_test_default ()
{
  drv_sh1106_display_image(image_logo_uit);

  bsp_delay (3000);

  drv_sh1106_clear_screen();

  bsp_delay (3000);

  drv_sh1106_display_image(image_logo_ce);

  bsp_delay (3000);

  drv_sh1106_clear_screen();

  drv_sh1106_turn_off();
}

static void system_test_dht11 (void)
{
  if (!drv_dht11_init())
    {
      ESP_LOGE("DHT11", "DHT11 Initialization failed!");
    }

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

      bsp_delay(2000);
    } 
}

static system_main_state_t sys_state_check = IDLE;

void system_manage_loop(void)
{
  system_manage_init();

// #define TEST_OLED_DEFAULT
#define TEST_DHT11

#ifdef TEST_OLED_DEFAULT
  system_oled_test_default();
#elif defined(TEST_OLED_SCREEN_1)
  system_oled_test_screen_1();
#elif defined(TEST_DHT11)
  system_test_dht11();
#endif

  // while (1)
  // {
  //   switch (sys_state_check)
  //   {
  //     case IDLE:
        
  //       break;
      
  //     default:
  //       break;
  //   }
  //   bsp_delay(50);
  // }
}


// static system_main_state_t system_state_next = IDLE;

// system_main_state_t system_manager_check_update ()
// {
//   switch (system_state_next)
//   {
//   case IDLE:
//     if (drv_detect_button_pressed() == MAIN_BUTTON)
//     {
//       system_state_next = BOOT_STATE;
//     }
//     break;
  
//   default:
//     break;
//   }
// }



