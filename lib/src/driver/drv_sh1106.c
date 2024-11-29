// File: main/sh1106_driver.c

#include "drv_sh1106.h"
#include "font_basic.h"


#define I2C_MASTER_NUM  I2C_NUM_0       // I2C port number
#define OLED_I2C_ADDR   0x3C            // SH1106 I2C address
#define COMMAND_MODE    0x00            // Control byte: Co = 0 (single byte), D/C# = 0 
                                        // Send command to oled
#define DATA_MODE       0x40            // Control byte: Co = 0, D/C# = 1 (data mode)
                                        // Represent pixel data to display

// OLED resolution macros
#define OLED_WIDTH 132          // OLED width in pixels
#define OLED_HEIGHT 64           // OLED height in pixels

// Font size macros
#define FONT_WIDTH   5
#define FONT_HEIGHT  7

static uint8_t screen_buffer[OLED_WIDTH * (OLED_HEIGHT / 8)];   // Frame buffer

static esp_err_t drv_sh1106_send_command(uint8_t command);
static esp_err_t drv_sh1106_write_data (uint8_t* data, size_t length);
static esp_err_t drv_sh1106_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
static esp_err_t drv_sh1106_update_screen(void);


static esp_err_t drv_sh1106_send_command(uint8_t command) 
{
    bool ret = false;                                     
    ret = bsp_i2c_write_mem((OLED_I2C_ADDR << 1) | I2C_MASTER_WRITE, COMMAND_MODE, &command, 1);
    return (ret == true) ? ESP_OK : ESP_FAIL;
}

static esp_err_t drv_sh1106_write_data (uint8_t* data, size_t length)
{
    bool ret = false;                                     
    ret = bsp_i2c_write_mem((OLED_I2C_ADDR << 1) | I2C_MASTER_WRITE, DATA_MODE, data, length);
    return (ret == true) ? ESP_OK : ESP_FAIL;
}

esp_err_t drv_sh1106_init(void) 
{
    if (!bsp_i2c_is_device_ready(OLED_I2C_ADDR))
        return ESP_ERR_INVALID_ARG;

    uint8_t init_cmds[] = 
    {
        0xAE,       // Display OFF
        0xD5, 0x80, // Set display clock divide ratio/oscillator frequency
        0xA8, 0x3F, // Set multiplex ratio to 1/64 duty
        0xD3, 0x00, // Set display offset
        0x40,       // Set display start line to 0
        0x8D, 0x14, // Enable charge pump
        0xA1,       // Set segment re-map (normal)
        0xC8,       // Set COM output scan direction (remapped)
        0xDA, 0x12, // Set COM pins hardware configuration
        0x81, 0x7F, // Set contrast control
        0xD9, 0xF1, // Set pre-charge period
        0xDB, 0x20, // Set VCOMH deselect level
        0xA4,       // Set entire display ON (resume to RAM content)
        0xA6,       // Set normal display (non-inverted)
        0xAF        // Display ON
    };
    for (int i = 0; i < sizeof(init_cmds); i++) 
    {
        bool ret = drv_sh1106_send_command(init_cmds[i]);
        if (ret != ESP_OK) 
            return ret; // Return the error code if a command fails
    }

    return ESP_OK;
}

esp_err_t drv_sh1106_clear_screen(void) 
{
    uint8_t empty_buffer[OLED_WIDTH] = {0x00}; // Predefined empty buffer for one page

    for (uint8_t page = 0; page < (OLED_HEIGHT / 8); page++) 
    {
        // Set page and column addresses once per page
        drv_sh1106_send_command(0xB0 + page);   // Set page address
        drv_sh1106_send_command(0x00);          // Set lower column address
        drv_sh1106_send_command(0x10);          // Set higher column address

        // Write a full empty buffer for this page
        drv_sh1106_write_data(empty_buffer, OLED_WIDTH);
    }

    return ESP_OK;
}

static esp_err_t drv_sh1106_write_char(uint8_t x, uint8_t y, char c) 
{
    if (x >= OLED_WIDTH || y >= (OLED_HEIGHT / FONT_HEIGHT)) 
        return ESP_ERR_INVALID_ARG;                         // Prevent out-of-bounds drawing

    uint8_t adjusted_x = x + 2;                             // Adjust by 2 to account for the SH1106 column offset

    drv_sh1106_send_command(0xB0 + y);                      // Set page address
    drv_sh1106_send_command(0x00 + (adjusted_x & 0x0F));    // Set lower column address
    drv_sh1106_send_command(0x10 + (adjusted_x >> 4));      // Set higher column address

    // Retrieve the font data for the character
#if FONT_WIDTH == 8
    const uint8_t *font_data = font8x8[(uint8_t)c];
#elif FONT_WIDTH == 5
    c = c - 32;
    const uint8_t *font_data = font5x7[(uint8_t)c];
#elif FONT_WIDTH == 4
    c = c - 32;
    const uint8_t *font_data = font4x6[(uint8_t)c];
#elif FONT_WIDTH == 3
    c = c - 32;
    const uint8_t *font_data = font3x5[(uint8_t)c];
#else
    #error "Unsupported FONT_WIDTH value"
#endif

    // Write the font data to the OLED using the updated function
    esp_err_t ret = drv_sh1106_write_data((uint8_t *)font_data, FONT_WIDTH);

    return ret;
}

esp_err_t drv_sh1106_display_text(uint8_t x, uint8_t y, const char *str) 
{
    if (!str) 
        return ESP_ERR_INVALID_ARG;      // Return error if input string is NULL

    uint8_t start_x = x + 2;            // Adjust start position for SH1106 offset
    while (*str) 
    {
        esp_err_t ret = drv_sh1106_write_char(start_x, y, *str++);
        if (ret != ESP_OK)
            return ret; 

        start_x += FONT_WIDTH; // Move to the next character position
        if (start_x >= OLED_WIDTH) 
        { 
            // Wrap to the next line if necessary
            start_x = 2; // Reset to adjusted start
            y++;
            if (y >= (OLED_HEIGHT / FONT_HEIGHT)) 
                return ESP_ERR_NO_MEM; 
        }
    }

    return ESP_OK; // Return success if all characters are written
}



esp_err_t drv_sh1106_display_text_center(uint8_t line, const char *str) 
{
    if (!str) 
        return ESP_ERR_INVALID_ARG; 

    uint8_t char_width = FONT_WIDTH;                            // Width of one character in pixels

    uint8_t text_width = strlen(str) * char_width;              // Calculate the pixel width of the text
    uint8_t start_x = (OLED_WIDTH - text_width) / 2;            // Calculate the starting x position to center the text

    uint8_t y = line;                                           // Calculate the y position based on the line number

    if ((OLED_WIDTH - text_width) <= 0) 
        start_x = 0;
    if (y >= (OLED_HEIGHT / FONT_HEIGHT))
        y = 0;

    while (*str) 
    {
        esp_err_t ret = drv_sh1106_write_char(start_x, y, *str++);
        if (ret != ESP_OK)
            return ret; 

        start_x += char_width;
    }

    return ESP_OK;
}

static esp_err_t drv_sh1106_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
        return ESP_ERR_INVALID_ARG; // Out of bounds

    uint16_t index = x + (y / 8) * OLED_WIDTH; // Calculate buffer index
    uint8_t bit = 1 << (y % 8);                // Determine the bit within the byte

    if (color)
        screen_buffer[index] |= bit;  // Set pixel (turn ON)
    else
        screen_buffer[index] &= ~bit; // Clear pixel (turn OFF)

    return ESP_OK;
}

static esp_err_t drv_sh1106_update_screen(void)
{
    for (uint8_t page = 0; page < (OLED_HEIGHT / 8); page++)
    {
        drv_sh1106_send_command(0xB0 + page); // Set page address
        drv_sh1106_send_command(0x00);        // Set lower column address
        drv_sh1106_send_command(0x10);        // Set higher column address

        uint8_t *page_buffer = &screen_buffer[page * OLED_WIDTH];
        esp_err_t ret = drv_sh1106_write_data(page_buffer, OLED_WIDTH);
        if (ret != ESP_OK) return ret;
    }
    return ESP_OK;
}

esp_err_t drv_sh1106_display_image(const uint8_t *image)
{
    for (uint8_t y = 0; y < OLED_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < OLED_WIDTH; x++)
        {
            uint16_t byte_index = x + (y / 8) * OLED_WIDTH;
            uint8_t bit = (image[byte_index] >> (y % 8)) & 0x01; // Extract the bit
            drv_sh1106_draw_pixel(x, y, bit);                   // Draw the pixel
        }
    }
    drv_sh1106_update_screen(); // Send the updated buffer to the display

    return ESP_OK;
}

esp_err_t drv_sh1106_turn_off(void)
{
    drv_sh1106_send_command(0xAE); // Display OFF
    drv_sh1106_send_command(0x8D); // Charge Pump Setting
    drv_sh1106_send_command(0x10); // Disable charge pump

    return ESP_OK;
}


// --------------------------DEVELOPING FUNCTION--------------------//

