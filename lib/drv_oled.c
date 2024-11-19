// File: main/sh1106_driver.c

#include "drv_oled.h"


#define TEST_FONT_DEFAULT

#ifdef TEST_FONT_DEFAULT
    #include "font.h"
#elif
    #include "font8x8_basic.h"
#endif

#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000 // Fast mode
#define I2C_MASTER_TX_BUF_LEN 0
#define I2C_MASTER_RX_BUF_LEN 0
#define SH1106_ADDR 0x3C // Default I2C address for the OLED

// SH1106 Command Definitions
#define SH1106_DISPLAY_OFF 0xAE
#define SH1106_DISPLAY_ON 0xAF
#define SH1106_SET_CONTRAST 0x81
#define SH1106_NORMAL_DISPLAY 0xA6
#define SH1106_SET_PAGE_ADDR 0xB0

// I2C communication initialization
void i2c_master_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21, // Adjust for your setup
        .scl_io_num = GPIO_NUM_22, // Adjust for your setup
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_LEN, I2C_MASTER_TX_BUF_LEN, 0));
}

// Send a command to SH1106
void sh1106_send_command(uint8_t command)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SH1106_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true); // Co = 0, D/C# = 0 for command
    i2c_master_write_byte(cmd, command, true);
    i2c_master_stop(cmd);
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

// Send data (pixels) to SH1106
void sh1106_send_data(uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SH1106_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x40, true); // Co = 0, D/C# = 1 for data
    i2c_master_write(cmd, data, len, true);
    i2c_master_stop(cmd);
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

// Clear the display buffer
void sh1106_clear_display()
{
    uint8_t clear_data[OLED_WIDTH] = {0x00};
    for (uint8_t page = 0; page < (OLED_HEIGHT / 8); page++)
    {
        sh1106_send_command(SH1106_SET_PAGE_ADDR + page); // Set page address
        sh1106_send_command(0x00);                        // Set lower column address
        sh1106_send_command(0x10);                        // Set higher column address
        sh1106_send_data(clear_data, OLED_WIDTH);
    }
}

// Display text function (using the 5x7 font)
void sh1106_display_text(const char *text, uint8_t page)
{
    uint8_t buffer[OLED_WIDTH] = {0};
    size_t text_len = strlen(text);

    for (size_t i = 0; i < text_len && i < OLED_WIDTH / 6; i++)
    {
        for (uint8_t j = 0; j < 5; j++)
        {
            buffer[i * 6 + j] = font_format[text[i] - 32][j]; // Map character to font data
        }
        buffer[i * 6 + 5] = 0x00; // Add space between characters
    }

    sh1106_send_command(SH1106_SET_PAGE_ADDR + page);
    sh1106_send_command(0x00);
    sh1106_send_command(0x10);
    sh1106_send_data(buffer, OLED_WIDTH);
}

// Display image function
void sh1106_display_image(const uint8_t *image_data, size_t size)
{
    for (uint8_t page = 0; page < (OLED_HEIGHT / 8); page++)
    {
        sh1106_send_command(SH1106_SET_PAGE_ADDR + page);
        sh1106_send_command(0x00);
        sh1106_send_command(0x10);
        sh1106_send_data(image_data + (page * OLED_WIDTH), OLED_WIDTH);
    }
}

// Initialization function for SH1106
void sh1106_init()
{
    i2c_master_init();
    sh1106_send_command(SH1106_DISPLAY_OFF);
    sh1106_send_command(SH1106_SET_CONTRAST);
    sh1106_send_command(0x7F); // Contrast level
    sh1106_send_command(SH1106_NORMAL_DISPLAY);
    sh1106_clear_display(); // Start with a clear screen
    sh1106_send_command(SH1106_DISPLAY_ON);
}

// Main task to test the SH1106 OLED display
void oled_i2c_test()
{
    // Initialize the display
    sh1106_init();

    // Display text on the first page (top of the screen)
    sh1106_display_text("Hello, OLED!", 0);

    // Delay to view the text
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // Clear the display after viewing
    sh1106_clear_display();
}