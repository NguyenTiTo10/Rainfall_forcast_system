#include "system_manager.h"
#include "image_128x64.h"

// static void system_oled_test_default ();
static void system_oled_test_gui();


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

static void system_oled_test_gui()
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


void system_oled_test_default ()
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

static system_state_t sys_state_check = IDLE;

void system_manage_loop(void)
{
  system_manage_init();

// #define TEST_OLED_DEFAULT
#ifdef TEST_OLED_DEFAULT
  system_oled_test_default();
#else
  system_oled_test_gui();
#endif

  while (1)
  {
    switch (sys_state_check)
    {
      case IDLE:
        
        break;
      
      default:
        break;
    }
    bsp_delay(50);
  }
}




