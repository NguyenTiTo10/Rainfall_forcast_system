#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"

#include "system_manager.h"


// I2C configuration
#define I2C_MASTER_SCL_IO       22              
#define I2C_MASTER_SDA_IO       21              
#define I2C_MASTER_NUM          I2C_NUM_0       
#define I2C_MASTER_FREQ_HZ      400000     

// GPIO Button
#define     MAIN_BTN_GPIO       GPIO_NUM_32   // Use GPIO0 for the button
#define     LEFT_BTN_GPIO       GPIO_NUM_25
#define     MID_BTN_GPIO        GPIO_NUM_26
#define     RIGHT_BTN_GPIO      GPIO_NUM_27

static esp_err_t i2c_master_init(void);

static esp_err_t i2c_master_init(void) 
{
    i2c_config_t conf = 
    {
        .mode = I2C_MODE_MASTER,      
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);                                      
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);          
}

void app_main(void) 
{
    // Initialize I2C and the SH1106 OLED display
    if (i2c_master_init() == ESP_OK) 
        printf("I2C initialized successfully.\n");
    else 
        printf("Failed to initialize I2C.\n");

    system_manage_loop();    

#ifdef TEST_MQTT
  mqtt_main();
#endif

}
