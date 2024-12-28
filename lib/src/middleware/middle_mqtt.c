#include "middle_mqtt.h"
#include "bsp_timer.h"
#include "system_manager.h"

#define     MAIN_TOPIC                  "RAINFALL_FORCAST_SYSTEM"

#define     REQUEST_UPDATE_MESSAGE      "START"


event_data_recieve_t  recieved_data;

middle_mqtt_update_state_t update_state;

char *update_time;
char *update_rainfall_line_1;
char *update_rainfall_line_2;
char *update_rainfall_line_3;

void middle_mqtt_init()
{
  bsp_mqtt_start();

  bsp_mqtt_client_publish (MAIN_TOPIC, REQUEST_UPDATE_MESSAGE);

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

  strncpy(update_type, recieved_data.data, 3);  

  update_type[3] = '\0';                        

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
    printf("Update rain Quang Tri\n");
    return QUANGTRI_RAIN_UPDATE;  
  }

  else
    return UNKNOWN_UPDATE;
}


void middle_mqtt_extract_rain_data (void)
{
  char result[3][6]; // To store formatted numbers
  char *token = strtok(recieved_data.data, "|");
  int count = 0;

  // Skip "111" and empty token after "||"
  while (token && strcmp(token, "111") == 0) 
  {
    token = strtok(NULL, "|");
  }

  // Extract first three numbers
  while (token && count < 3) 
  {
    if (strlen(token) == 3) 
      snprintf(result[count], sizeof(result[count]), " %s", token);
    else snprintf(result[count], sizeof(result[count]), "%s", token);
    count++;
    token = strtok(NULL, "|");
  }

  // Print results
  for (int i = 0; i < count; i++) printf("%s\n", result[i]);
  return;
}


void mqtt_test (void)
{
  middle_mqtt_init();

  while (1)
  {
    if (middle_mqtt_get_data())
    {
      middle_mqtt_detect_update_type();

      middle_mqtt_extract_rain_data();

      bsp_timer_delay(10);
    }

    bsp_timer_delay(10);
  }
  
}