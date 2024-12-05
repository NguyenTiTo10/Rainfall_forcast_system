#include "mqtt.h"
#include "bsp_timer.h"

void mqtt_test (void)
{
  mqtt_main();

  while (1)
  {
    if (bsp_mqtt_get_data_flag())
    {
      printf("Received data AAAAAAAAAAAAAAAAAAAAAAAAAA\n");
      bsp_mqtt_set_data_flag(false);
    }

    bsp_timer_delay(10);
  }
  
}