#include "mqtt.h"
#include "bsp_timer.h"

#define     TOPIC               "RAINFALL_FORCAST_SYSTEM"


void mqtt_test (void)
{
  mqtt_main();

  bsp_mqtt_client_publish (TOPIC, "Request_data");

  bsp_mqtt_client_publish (TOPIC, "35,40.9,15.0");

  while (1)
  {
    if (bsp_mqtt_get_data_flag())
    {
      event_data_recieve_t topic_data = get_ret_event_data();

      printf("TOPIC=%.*s\r\n", topic_data.topic_length, topic_data.topic);
      printf("DATA=%.*s\r\n", topic_data.data_length, topic_data.data);

      printf("Received data AAAAAAAAAAAAAAAAAAAAAAAAAA\n");
      bsp_mqtt_set_data_flag(false);
    }

    bsp_timer_delay(10);
  }
  
}