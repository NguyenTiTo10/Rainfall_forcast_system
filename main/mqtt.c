#include "mqtt.h"
#include "bsp_timer.h"
#include "system_manager.h"

#define     TOPIC               "RAINFALL_FORCAST_SYSTEM"
#define     REQUEST_UPDATE      "START"


event_data_recieve_t  recieved_data;

system_update_state_t update_state;

void middle_mqtt_init()
{
  mqtt_main();

  bsp_mqtt_client_publish (TOPIC, REQUEST_UPDATE);
}

void middle_mqtt_request_update ()
{
  
}

bool middle_mqtt_get_data ()
{
  if (bsp_mqtt_get_data_flag())
  {
    recieved_data = get_ret_event_data();

    printf("TOPIC=%.*s\r\n", recieved_data.topic_length, recieved_data.topic);
    printf("DATA=%.*s\r\n", recieved_data.data_length, recieved_data.data);
    printf("DATA LENGTH=%d\r\n", recieved_data.data_length);

    bsp_mqtt_set_data_flag(false);

  }
  else
    return false;
    
  return true;
}

system_update_state_t


void mqtt_test (void)
{
  middle_mqtt_init();

  bsp_mqtt_client_publish (TOPIC, "35,40.9,15.0");

  while (1)
  {
    if (middle_mqtt_get_data())
    {
      bsp_timer_delay(10);
    }

    bsp_timer_delay(10);
  }
  
}