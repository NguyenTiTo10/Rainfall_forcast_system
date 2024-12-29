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


void middle_mqtt_extract_time (void)
{
  char *token = strtok(recieved_data.data, "|");

  if (token != NULL)
    token = strtok(NULL, "|");

  char buffer_time[50] = "";

  if (token != NULL) 
    strcpy(buffer_time, token); 


  uint8_t length = strlen(buffer_time);

  printf("%s\n", buffer_time);
  printf("Length of the new string: %d\n", length);

  
  return;
}

void middle_mqtt_extract_rain (void)
{
  char result[3][6];
  char *token = strtok(recieved_data.data, "|");
  int count = 0;

  while (token && count < 9) 
  {
    token = strtok(NULL, "|");

    snprintf(result[count], sizeof(result[count]), "%s", token);
    count++;
  }

  // Tạo chuỗi định dạng đầu ra
  char buffer_line_1[50] = "IFS  : ";
  char buffer_line_2[50] = "Tito : ";
  char buffer_line_3[50] = "Vrain: ";


  for (int i = 0; i < 3; i++) 
  {
    if (strlen(result[i]) == 3)
      strcat(buffer_line_1, " ");

    strcat(buffer_line_1, result[i]);

    if (i < 2)
      strcat(buffer_line_1, "  ");
  }

  for (int i = 0; i < 3; i++) 
  {
    if (strlen(result[i]) == 3)
      strcat(buffer_line_2, " ");

    strcat(buffer_line_2, result[i]);

    if (i < 2)
      strcat(buffer_line_2, "  ");
  }

  for (int i = 0; i < 3; i++) 
  {
    if (strlen(result[i]) == 3)
      strcat(buffer_line_3, " ");

    strcat(buffer_line_3, result[i]);

    if (i < 2)
      strcat(buffer_line_3, "  ");
  }

  int length;

  length = strlen(buffer_line_1);
  printf("%s\n", buffer_line_1); 
  printf("Length of the new string: %d\n", length); 

  length = strlen(buffer_line_2);
  printf("%s\n", buffer_line_2); 
  printf("Length of the new string: %d\n", length); 

  length = strlen(buffer_line_3);
  printf("%s\n", buffer_line_3); 
  printf("Length of the new string: %d\n", length); 

  return;
}


void middle_mqtt_extract_data()
{
  update_state = middle_mqtt_detect_update_type();

  switch (update_state)
  {
    case TIME_UPDATE:
      middle_mqtt_extract_time();
      break;

    case HATINH_RAIN_UPDATE:
      middle_mqtt_extract_rain();
      break;

    case QUANGBINH_RAIN_UPDATE:
      middle_mqtt_extract_rain();
      break;

    case QUANGTRI_RAIN_UPDATE:
      middle_mqtt_extract_rain();
      break;
    
    default:
      break;
  }

  return;
}

void mqtt_test (void)
{
  middle_mqtt_init();

  while (1)
  {
    if (middle_mqtt_get_data())
    {

      middle_mqtt_extract_data();

      bsp_timer_delay(10);
    }

    bsp_timer_delay(10);
  }
  
}