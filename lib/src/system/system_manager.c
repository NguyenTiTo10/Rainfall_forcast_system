#include "system_manager.h"
#include "image_128x64.h"

// static void system_oled_test_default ();
static void system_oled_test_gui();


void system_manage_init (void)
{
    // Init the oled 
  if (drv_sh1106_init() == ESP_OK)
    printf("Oled SH1106 initialized successfully.\n");
  drv_sh1106_clear_screen();
}

static void system_oled_test_gui()
{
  
}


void system_oled_test_default ()
{
#define DISPLAY_TEXT
#ifdef DISPLAY_TEXT
    char date_time [] = "29.11.2024 - 19:00";
    char temp[] = "Temp: 30.6 C ";
    char humid[] = "Humid: 80 %";
    char air_press[] = "Air press: 20 Pa";
    char location [] = "QUANG BINH";


    drv_sh1106_display_text_center(0, date_time);

    drv_sh1106_display_text(0, 2, temp);
    drv_sh1106_display_text(0, 4, humid);
    drv_sh1106_display_text(0, 6, air_press);

    drv_sh1106_display_text_center(7, location);


    bsp_delay (3000);

#else
    drv_sh1106_display_image(image_logo_uit);

    bsp_delay (3000);

    drv_sh1106_clear_screen();

    bsp_delay (3000);

    drv_sh1106_display_image(image_logo_ce);

    bsp_delay (3000);

#endif

    drv_sh1106_clear_screen();

    drv_sh1106_turn_off();
}



void system_manage_loop(void)
{
  system_manage_init();

#define TEST_OLED_DEFAULT
#ifdef TEST_OLED_DEFAULT
  system_oled_test_default();
#else
  system_oled_test_gui();
#endif
}




