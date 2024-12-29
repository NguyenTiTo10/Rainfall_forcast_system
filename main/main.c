#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"

#include "system_manager.h"

#include "middle_mqtt.h"


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

static const char *TAG = "Button";

static esp_err_t    i2c_master_init    (void);
static esp_err_t    config_gpio        (void);
static esp_err_t    config_isr_gpio    (void);
static esp_err_t    app_init           (void);



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


static esp_err_t config_gpio (void)
{
    gpio_config_t io_conf = 
    {
        .mode = GPIO_MODE_INPUT,               // Set as input
        .pull_up_en = GPIO_PULLUP_DISABLE,     // Disable pull-up
        .pull_down_en = GPIO_PULLDOWN_ENABLE,  // Enable pull-down
        .intr_type = GPIO_INTR_POSEDGE         // Trigger interrupt on rising edge
    };
    

    // Configuring GPIO32
    io_conf.pin_bit_mask = (1ULL << MAIN_BTN_GPIO);  // Select GPIO 32
    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) return err;

    // Configuring GPIO25
    io_conf.pin_bit_mask = (1ULL << LEFT_BTN_GPIO);  // Select GPIO 25
    err = gpio_config(&io_conf);
    if (err != ESP_OK) 
        return err;

    // Configuring GPIO26
    io_conf.pin_bit_mask = (1ULL << MID_BTN_GPIO);  // Select GPIO 26
    err = gpio_config(&io_conf);
    if (err != ESP_OK) 
        return err;

    // Configuring GPIO27
    io_conf.pin_bit_mask = (1ULL << RIGHT_BTN_GPIO);  // Select GPIO 27
    err = gpio_config(&io_conf);
        return err;
}


static esp_err_t config_isr_gpio (void)
{
    esp_err_t ret;

    // Install GPIO ISR service with default flags
    ret = gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    if (ret != ESP_OK)
        return ret;

    // Hook ISR handler for the button GPIO
    gpio_isr_handler_add(MAIN_BTN_GPIO, bsp_gpio_isr_handler, NULL);
    gpio_isr_handler_add(LEFT_BTN_GPIO, bsp_gpio_isr_handler, NULL);
    gpio_isr_handler_add(MID_BTN_GPIO, bsp_gpio_isr_handler, NULL);
    gpio_isr_handler_add(RIGHT_BTN_GPIO, bsp_gpio_isr_handler, NULL);


    ESP_LOGI(TAG, "Interrupt-based button detection initialized.");

    return ESP_OK;
}


esp_err_t app_init (void)
{
    esp_err_t ret;

    ret = i2c_master_init();                    // Initialize I2C 
    if (ret != ESP_OK) 
    {
        printf("Failed to initialize I2C.\n");
        return ret;
    }
        
    
    ret = config_gpio();                       // Initialize GPIO 
    if (ret != ESP_OK)
    {
        printf("GPIO error.\n");
        return ret;
    }
    
    ret = config_isr_gpio();                   // Initialize GPIO ISR
    if (ret != ESP_OK)
    {
        printf("ISR GPIO error.\n");
        return ret;
    }

    return ESP_OK;
}


void app_main(void) 
{
    app_init();                     // Init (I2C), Button (GPIO and ISR GPIO)

    middle_mqtt_init();                           // Init MQTT         

    system_manage_init();           // Init driver OLED, driver DHT11

    while (1)
    {
        system_manage_loop();  
        bsp_timer_delay(10);
    }


}
