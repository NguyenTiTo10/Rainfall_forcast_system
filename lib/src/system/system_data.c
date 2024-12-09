#include "system_data.h"


char humid_str[30];           // Buffer to hold the humidity string
char press_str[30];

void system_data_start_update_sensor (void)
{
  drv_dht11_start_read();                     
    
  float humid = drv_dht11_get_humid();   

  drv_bmp180_start_read();     
  float press = drv_bmp180_get_press(); 
  float temp  = drv_bmp180_get_temp();         

  // Print the formatted strings
  printf("%s\n", humid_str);
  printf("%s\n\n", press_str);

}


char *system_data_update_temp (void)
{
  char temp_str[30];            // Buffer to hold the temperature string

  snprintf(temp_str , sizeof(temp_str),   "Temp   : %.1f C         ", temp);

  printf("%s\n", temp_str);

  return temp_str;
}

// Format strings
  snprintf(humid_str, sizeof(humid_str),  "Humid  : %.1f %%        ", humid);
  snprintf(press_str, sizeof(press_str),  "Presure: %.1f hPa       ", press);