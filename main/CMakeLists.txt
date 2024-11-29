# idf_component_register(SRCS "mqtt_tcp.c" "main.c"
#                     INCLUDE_DIRS ".")

idf_component_register(SRCS "main.c" "mqtt_tcp/mqtt_tcp.c" 
                        "../lib/inc/system/"

                        "../lib/inc/middleware/"

                        "../lib/inc/driver/drv_sh1106.c"

                        "../lib/inc/bsp/bsp_i2c.h" "../lib/inc/bsp/bsp_timer.h"


                    INCLUDE_DIRS "." "mqtt_tcp" 
                        "../lib/inc/system"

                        "../lib/inc/middleware"
                        
                        "../lib/inc/driver"
                        
                        "../lib/inc/bsp" )
