#include <stdio.h>
#include "mqtt_tcp.h"

// #define TEST_MQTT

#define TEST_OLED

void app_main(void)
{
  #ifdef TEST_MQTT
    mqtt_main();
  #endif

  #ifdef TEST_OLED
    oled_i2c_test ();
  #endif

}