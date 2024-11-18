#include <stdio.h>
#include "mqtt_tcp.h"

#define TEST_MQTT

void app_main(void)
{
  #ifdef TEST_MQTT
    mqtt_main();
  #endif

  #ifdef TEST_I2C
    // do sth
  #endif

}