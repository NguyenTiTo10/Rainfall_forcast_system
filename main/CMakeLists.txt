# idf_component_register(SRCS "mqtt_tcp.c" "main.c"
#                     INCLUDE_DIRS ".")

idf_component_register(SRCS "main.c" "mqtt_tcp/mqtt_tcp.c" "../lib/drv_oled.c"
                    INCLUDE_DIRS "." "mqtt_tcp" "../lib")
