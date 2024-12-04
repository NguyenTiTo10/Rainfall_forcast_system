# idf_component_register(SRCS "mqtt_tcp.c" "main.c"
#                     INCLUDE_DIRS ".")

idf_component_register(SRCS "main.c" "middle_mqtt.c" "mqtt_tcp/mqtt_tcp.c" 

                        "../lib/src/system/system_manager.c"      
                        "../lib/src/system/system_display.c"  
                        "../lib/src/system/system_data.c"

                        "../lib/src/middleware/font_image.c"

                        "../lib/src/driver/drv_sh1106.c"
                        "../lib/src/driver/drv_dht11.c"
                        "../lib/src/driver/drv_bUtton.c"

                        "../lib/src/bsp/bsp_i2c.c" 
                        "../lib/src/bsp/bsp_timer.c"
                        "../lib/src/bsp/bsp_gpio.c"


                    INCLUDE_DIRS "." "mqtt_tcp" 
                        "../lib/inc/system"

                        "../lib/inc/middleware"
                        
                        "../lib/inc/driver"
                        
                        "../lib/inc/bsp" )
