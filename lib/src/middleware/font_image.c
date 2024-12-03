#include "font_image.h"

void mid_font_display_logo_uit  (void)
{
  drv_sh1106_clear_screen ();

  drv_sh1106_display_image (image_logo_uit);
}

void mid_font_display_logo_ce   (void);