#include "system_data.h"


sys_data_sensor_update_t data_sensor_update;

static char temp_str  [30];           // Buffer to hold the temperature string
static char humid_str [30];           // Buffer to hold the humidity string
static char press_str [30];



void system_data_start_update_sensor (void)
{
  drv_dht11_start_read();                     
    
  data_sensor_update.humid = drv_dht11_get_humid();   

  drv_bmp180_start_read();     
  data_sensor_update.pressure = drv_bmp180_get_press(); 
  data_sensor_update.temp  = drv_bmp180_get_temp();

  // Add test code send to flespi.io

  return;         
}


char *system_data_get_update_temp (void)
{
  snprintf(temp_str , sizeof(temp_str),   "Temp   : %.1f C         ", data_sensor_update.temp);

  printf("%s\n", temp_str);

  return temp_str;
}


char *system_data_get_update_humid (void)
{
  snprintf(humid_str, sizeof(humid_str),  "Humid  : %.1f %%        ", data_sensor_update.humid);

  printf("%s\n", humid_str);

  return humid_str;
}


char *system_data_get_update_press (void)
{
  snprintf(press_str, sizeof(press_str),  "Presure: %.1f hPa       ", data_sensor_update.pressure);

  printf("%s\n\n", press_str);

  return press_str;
}
