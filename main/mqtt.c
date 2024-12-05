#include "mqtt.h"

void mqtt_test (void)
{
  mqtt_main();

  event_data_recieve_t data_event = get_event_data();

  printf("TOPIC=%.s\r\n", data_event.topic);
  printf("DATA=%.s\r\n", data_event.data);
}