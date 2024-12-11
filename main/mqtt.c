#include "mqtt.h"
#include "bsp_timer.h"
#include "system_manager.h"

#define     TOPIC               "RAINFALL_FORCAST_SYSTEM"
#define     REQUEST_UPDATE      "START"


event_data_recieve_t  recieved_data;

middle_mqtt_update_state_t update_state;

void middle_mqtt_init()
{
  mqtt_main();

  bsp_mqtt_client_publish (TOPIC, REQUEST_UPDATE);
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


middle_mqtt_update_state_t middle_mqtt_detect_update_type ()
{
  char update_type[4];

  // Split the string
  strncpy(update_type, recieved_data.data, 3); // Copy the first 3 characters
  update_type[3] = '\0';         // Null-terminate the string

  printf ("First part: %s\n", update_type);

  if (strcmp(update_type, "100") == 0) 
  {
      printf("Update time\n");
      return TIME_UPDATE;        
  } 
  else if (strcmp(update_type, "110") == 0) 
  {
      printf("Update rain Ha Tinh\n");
      return HATINH_RAIN_UPDATE;  
  } 
  else if (strcmp(update_type, "111") == 0) 
  {
      printf("Update rain Quang Binh\n");
      return QUANGBINH_RAIN_UPDATE;  
  }
  else if (strcmp(update_type, "112") == 0) 
  {
      printf("Update rain Quang Binh\n");
      return QUANGBINH_RAIN_UPDATE;  
  }
}


void mqtt_test (void)
{
  middle_mqtt_init();

  bsp_mqtt_client_publish (TOPIC, "35,40.9,15.0");

  while (1)
  {
    if (middle_mqtt_get_data())
    {
      middle_mqtt_detect_update_type();
      
      bsp_timer_delay(10);
    }

    bsp_timer_delay(10);
  }
  
}