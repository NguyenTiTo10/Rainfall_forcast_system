#include "drv_button.h"

#define TAG "Button ISR"          // Define a tag for logging
#define DEBOUNCE_DELAY_MS 50     // Debounce delay in milliseconds

static  uint32_t           last_debounce_time = 0;               // Last debounce time
static  uint32_t           current_time;
static  uint32_t           elasped_time;

static  uint8_t             count = 1;


drv_btn_type_t drv_btn_detect_press (void)
{
  if (!bsp_get_isr_flag())
    return NON_BTN_PRESSED;

    // Count the time to check whether debounce or not
  current_time = bsp_timer_get_time() / 1000;
  elasped_time = current_time - last_debounce_time;
  if (elasped_time <= DEBOUNCE_DELAY_MS)
    return NON_BTN_PRESSED;

  bsp_set_isr_flag (false);
  last_debounce_time = current_time;

  if (bsp_gpio_read_pin(MAIN_BTN_GPIO) == 1)
    return MAIN_BTN_PRESSED;
  
  if (bsp_gpio_read_pin(LEFT_BTN_GPIO) == 1)
    return LEFT_BTN_PRESSED;

  if (bsp_gpio_read_pin(MID_BTN_GPIO) == 1)
    return MID_BTN_PRESSED;

  if (bsp_gpio_read_pin(RIGHT_BTN_GPIO) == 1)
    return RIGHT_BTN_PRESSED;

  else
    return -1;
}



void drv_btn_update_state (void)
{
  drv_btn_type_t btn_state = drv_btn_detect_press();

  // Logging the button press
  switch (btn_state) 
  {
    case MAIN_BTN_PRESSED:
      ESP_EARLY_LOGI(TAG, "Count: %d - Main Button Pressed!", count);
      break;
    case LEFT_BTN_PRESSED:
      ESP_EARLY_LOGI(TAG, "Count: %d - Left Button Pressed!", count);
      break;
    case MID_BTN_PRESSED:
      ESP_EARLY_LOGI(TAG, "Count: %d - Mid Button Pressed!", count);
      break;
    case RIGHT_BTN_PRESSED:
      ESP_EARLY_LOGI(TAG, "Count: %d - Right Button Pressed!", count);
      break;
    default:
      count --;
      break;
  }
  
  count ++;
}


  
