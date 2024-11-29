#include "system_manager.h"
#include "image_128x64.h"

static void system_oled_test_default ();
static void system_oled_test_gui();


void system_manage_init (void)
{
    // Init the oled 
  if (drv_sh1106_init() == ESP_OK)
    printf("Oled SH1106 initialized successfully.\n");
  drv_sh1106_clear_screen();
}

void system_manage_loop(void)
{
  system_manage_init();

#ifdef TEST_OLED_DEFAULT
  system_oled_test_default();
#else
  system_oled_test_gui();
#endif
}



static void system_oled_test_default ()
{


#define DISPLAY_TEXT
#ifdef DISPLAY_TEXT
    // Test displaying text on the OLED
    drv_sh1106_write_string(0, 0, "Temp: ");
    drv_sh1106_write_string(0, 1, "Humid: ");
    drv_sh1106_write_string(0, 2, "Air press:");
    drv_sh1106_write_string(0, 3, "ANOTHER LINE 3");
    drv_sh1106_write_string(0, 4, "ANOTHER LINE 4");
    drv_sh1106_write_string(0, 5, "ANOTHER LINE 5");
    drv_sh1106_write_string(0, 6, "ANOTHER LINE 6");
    drv_sh1106_write_string(0, 7, "ANOTHER LINE 7");

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