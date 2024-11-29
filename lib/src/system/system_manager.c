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
  // char date[] = "29-11-2024";
  // char time[] = "19:00";
  char date_time [] = "29.11.2024 - 19:00";
  char location [] = "QUANG BINH";
  char temp[] = "Temp";

  drv_sh1106_display_text_center(0, date_time);

  drv_sh1106_display_text_center(4, "Temp: ");
  drv_sh1106_display_text_center(5, "Humid: ");
  drv_sh1106_display_text_center(3, "Air press:");
  drv_sh1106_display_text_center(1, temp);
  // drv_sh1106_display_text_center(5, "ANOTHER LINE 5");

  drv_sh1106_display_text_center(7, location);
}


void system_oled_test_default ()
{
#define DISPLAY_TEXT
#ifdef DISPLAY_TEXT
    // Test displaying text on the OLED
    drv_sh1106_display_text(0, 0, "Temp: ");
    drv_sh1106_display_text(0, 1, "Humid: ");
    drv_sh1106_display_text(0, 2, "Air press:");
    drv_sh1106_display_text(0, 3, "ANOTHER LINE 3");
    drv_sh1106_display_text(0, 4, "ANOTHER LINE 4");
    // drv_sh1106_display_text(0, 5, "ANOTHER LINE 5");
    // drv_sh1106_display_text(0, 6, "ANOTHER LINE 6");
    // drv_sh1106_display_text(0, 7, "ANOTHER LINE 7");
    // drv_sh1106_display_text(0, 8, "ANOTHER LINE 8");


    bsp_delay (3000);

#else
    drv_sh1106_display_image(image_logo_uit);

    bsp_delay (3000);

    drv_sh1106_clear_screen();

    bsp_delay (3000);

    drv_sh1106_display_image(image_logo_ce);

    bsp_delay (3000);

#endif

    // drv_sh1106_clear_screen();

    // drv_sh1106_turn_off();
}



void system_manage_loop(void)
{
  system_manage_init();

// #define TEST_OLED_DEFAULT
#ifdef TEST_OLED_DEFAULT
  system_oled_test_default();
#else
  system_oled_test_gui();
#endif
}




