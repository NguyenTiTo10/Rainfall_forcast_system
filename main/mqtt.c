#include "mqtt.h"
#include "bsp_timer.h"

#define TOPIC "Test"


void mqtt_test (void)
{
  mqtt_main();

  bsp_mqtt_client_subscribe(TOPIC);
  bsp_mqtt_client_publish(TOPIC, "Hello world");

  while (1)
  {
    if (bsp_mqtt_get_flag())
    {
      event_data_recieve_t data_event = get_event_data();
      printf("TOPIC=%.s\r\n", data_event.topic);
      printf("DATA=%.s\r\n", data_event.data);
    }
    bsp_mqtt_set_flag(false);
  }
  
  
}