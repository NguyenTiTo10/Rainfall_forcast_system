#include "middle_mqtt.h"
#include "bsp_timer.h"
#include "system_manager.h"

#define     MAIN_TOPIC                  "RAINFALL_FORCAST_SYSTEM"

#define     REQUEST_UPDATE_MESSAGE      "START"


event_data_recieve_t  recieved_data;

middle_mqtt_update_state_t update_state;

char buffer_data[100];

char update_time[30];
char update_rainfall_line_1[30]  =   "IFS  : ";
char update_rainfall_line_2[30]  =   "Tito : ";
char update_rainfall_line_3[30]  =   "Vrain: ";

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


void middle_mqtt_extract_rain (void)
{
  char delim[] = "|";
  char *token;

  // Skip the first token (the first number before double ||)
  token = strtok(buffer_data, delim);
  printf("First part: %s\n", token);


  // Create string IFS
  for (int i = 0; i < 3; i++)
  {
    token = strtok(NULL, delim);  

    if (strlen(token) == 3)
      strcat(update_rainfall_line_1, " ");

    strcat(update_rainfall_line_1, token);

    if (i < 2)
      strcat(update_rainfall_line_1, "  ");
  }


  // Create string Tito
  for (int i = 0; i < 3; i++)
  {
    token = strtok(NULL, delim);  

    if (strlen(token) == 3)
      strcat(update_rainfall_line_2, " ");

    strcat(update_rainfall_line_2, token);

    if (i < 2)
      strcat(update_rainfall_line_2, "  ");
  }


  // Create string Vrain
  for (int i = 0; i < 3; i++)
  {
    token = strtok(NULL, delim);  

    if (strlen(token) == 3)
      strcat(update_rainfall_line_3, " ");

    strcat(update_rainfall_line_3, token);

    if (i < 2)
      strcat(update_rainfall_line_3, "  ");
  }

  printf("%s\n", update_rainfall_line_1);
  printf("Size: %d\n", strlen(update_rainfall_line_1));

  printf("%s\n", update_rainfall_line_2);
  printf("Size: %d\n", strlen(update_rainfall_line_2));

  printf("%s\n", update_rainfall_line_3);
  printf("Size: %d\n", strlen(update_rainfall_line_3));

  return 0;
}

void middle_mqtt_extract_time (void)
{
  strcpy(update_time, recieved_data.data);

  printf("Time: %s\n", update_time);

  return 0;
}


void middle_mqtt_extract_data(void)
{
  update_state = middle_mqtt_detect_update_type();

  switch (update_state)
  {
    case TIME_UPDATE:
      middle_mqtt_extract_time_data();
      break;

    case HATINH_RAIN_UPDATE:
      middle_mqtt_extract_rain_data();
      break;

    case QUANGBINH_RAIN_UPDATE:
      middle_mqtt_extract_rain_data();
      break;

    case QUANGTRI_RAIN_UPDATE:
      middle_mqtt_extract_rain_data();
      break;

    default:
      break;
  }
}

void mqtt_test (void)
{
  middle_mqtt_init();

  while (1)
  {
    if (middle_mqtt_get_data())
    {
      strcpy(buffer_data, recieved_data.data);

      middle_mqtt_extract_data();
    }

    bsp_timer_delay(10);
  }
  
}