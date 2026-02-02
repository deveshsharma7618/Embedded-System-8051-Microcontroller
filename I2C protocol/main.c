#include <reg51.h>

// Pin definitions
sbit SDA = P2^0;  // Serial Data Line
sbit SCL = P2^1;  // Serial Clock Line

// I2C delay for timing
void i2c_delay(void) {
    unsigned char i;
    for(i = 0; i < 10; i++);
}

// I2C Start Condition
// SDA goes LOW while SCL is HIGH
void i2c_start(void) {
    SDA = 1;
    SCL = 1;
    i2c_delay();
    SDA = 0;  // START condition
    i2c_delay();
    SCL = 0;
    i2c_delay();
}

// I2C Stop Condition
// SDA goes HIGH while SCL is HIGH
void i2c_stop(void) {
    SCL = 0;
    SDA = 0;
    i2c_delay();
    SCL = 1;
    i2c_delay();
    SDA = 1;  // STOP condition
    i2c_delay();
}

// I2C Acknowledge
void i2c_ack(void) {
    SCL = 0;
    SDA = 0;  // Pull SDA low
    i2c_delay();
    SCL = 1;
    i2c_delay();
    SCL = 0;
    i2c_delay();
    SDA = 1;
}

// I2C No Acknowledge
void i2c_nack(void) {
    SCL = 0;
    SDA = 1;  // Leave SDA high
    i2c_delay();
    SCL = 1;
    i2c_delay();
    SCL = 0;
    i2c_delay();
}

// Send a byte over I2C
unsigned char i2c_send_byte(unsigned char byte) {
    unsigned char i, ack;
    
    for(i = 0; i < 8; i++) {
        SCL = 0;
        i2c_delay();
        
        if(byte & 0x80)
            SDA = 1;  // Send bit 1
        else
            SDA = 0;  // Send bit 0
        
        i2c_delay();
        SCL = 1;  // Clock pulse
        i2c_delay();
        byte <<= 1;
    }
    
    // Get ACK bit
    SCL = 0;
    SDA = 1;  // Release SDA
    i2c_delay();
    SCL = 1;
    i2c_delay();
    
    if(SDA == 0)
        ack = 1;  // ACK received
    else
        ack = 0;  // NACK received
    
    SCL = 0;
    i2c_delay();
    
    return ack;
}

// Receive a byte over I2C
unsigned char i2c_receive_byte(unsigned char ack_flag) {
    unsigned char i, byte = 0x00;
    
    SDA = 1;  // Release SDA
    
    for(i = 0; i < 8; i++) {
        SCL = 0;
        i2c_delay();
        SCL = 1;
        i2c_delay();
        
        byte <<= 1;
        byte |= SDA;
        
        i2c_delay();
    }
    
    SCL = 0;
    i2c_delay();
    
    if(ack_flag)
        i2c_ack();
    else
        i2c_nack();
    
    return byte;
}

// OLED12864I2C I2C Address (typically 0x78 or 0x7A)
#define OLED_ADDR 0x78

// OLED command codes
#define OLED_CMD_SET_CONTRAST 0x81
#define OLED_CMD_ENTIRE_DISPLAY_ON 0xA4
#define OLED_CMD_NORMAL_DISPLAY 0xA6
#define OLED_CMD_DISPLAY_OFF 0xAE
#define OLED_CMD_DISPLAY_ON 0xAF
#define OLED_CMD_SET_PAGE 0xB0
#define OLED_CMD_LOWER_COLUMN 0x00
#define OLED_CMD_UPPER_COLUMN 0x10
#define OLED_CMD_START_LINE 0x40

// Write command to OLED
void oled_write_command(unsigned char cmd) {
    i2c_start();
    i2c_send_byte(OLED_ADDR);  // OLED address + write bit
    i2c_send_byte(0x00);        // Command byte (0x00 = command, 0x40 = data)
    i2c_send_byte(cmd);         // Command
    i2c_stop();
}

// Write data to OLED
void oled_write_data(unsigned char val) {
    i2c_start();
    i2c_send_byte(OLED_ADDR);   // OLED address + write bit
    i2c_send_byte(0x40);        // Data byte (0x40 = data)
    i2c_send_byte(val);        // Data
    i2c_stop();
}

// Initialize OLED display
void oled_init(void) {
    // Turn off display
    oled_write_command(OLED_CMD_DISPLAY_OFF);
    
    // Set display clock divide ratio
    oled_write_command(0xD5);
    oled_write_command(0x80);
    
    // Set multiplex ratio
    oled_write_command(0xA8);
    oled_write_command(0x3F);
    
    // Set display offset
    oled_write_command(0xD3);
    oled_write_command(0x00);
    
    // Set start line
    oled_write_command(OLED_CMD_START_LINE);
    
    // Set charge pump
    oled_write_command(0x8D);
    oled_write_command(0x14);
    
    // Set memory addressing mode
    oled_write_command(0x20);
    oled_write_command(0x02);
    
    // Set segment re-map
    oled_write_command(0xA1);
    
    // Set COM output scan direction
    oled_write_command(0xC8);
    
    // Set COM pins hardware configuration
    oled_write_command(0xDA);
    oled_write_command(0x12);
    
    // Set contrast
    oled_write_command(OLED_CMD_SET_CONTRAST);
    oled_write_command(0x7F);
    
    // Set pre-charge period
    oled_write_command(0xD9);
    oled_write_command(0xF1);
    
    // Set VCOMH deselect level
    oled_write_command(0xDB);
    oled_write_command(0x40);
    
    // Set entire display on
    oled_write_command(OLED_CMD_ENTIRE_DISPLAY_ON);
    
    // Set normal display
    oled_write_command(OLED_CMD_NORMAL_DISPLAY);
    
    // Turn on display
    oled_write_command(OLED_CMD_DISPLAY_ON);
}

// Clear OLED display
void oled_clear(void) {
    unsigned char page, col;
    
    for(page = 0; page < 8; page++) {
        oled_write_command(0xB0 + page);  // Set page address
        oled_write_command(0x00);          // Set lower column address
        oled_write_command(0x10);          // Set upper column address
        
        for(col = 0; col < 128; col++) {
            oled_write_data(0x00);  // Write blank data
        }
    }
}

// Display a simple pattern (for testing)
void oled_test_pattern(void) {
    unsigned char page, col;
    
    for(page = 0; page < 8; page++) {
        oled_write_command(0xB0 + page);  // Set page address
        oled_write_command(0x00);          // Set lower column address
        oled_write_command(0x10);          // Set upper column address
        
        for(col = 0; col < 128; col++) {
            if(page % 2 == 0)
                oled_write_data(0xFF);  // Alternating pattern
            else
                oled_write_data(0x00);
        }
    }
}

// Delay function (milliseconds)
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 121; j++);  // Adjust for your clock frequency
}

// 5x8 ASCII font data (simplified, character 0x20 to 0x7E)
const unsigned char code font_5x8[95][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x03, 0x00, 0x03, 0x00}, // "
    {0x14, 0x3E, 0x14, 0x3E, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x63, 0x13, 0x08, 0x64, 0x63}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x2A, 0x1C, 0x7F, 0x1C, 0x2A}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // Backslash
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    {0x7C, 0x04, 0x18, 0x04, 0x7C}, // m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // }
    {0x10, 0x08, 0x08, 0x10, 0x08}, // ~
};

// Set cursor to specific page and column
void oled_set_cursor(unsigned char page, unsigned char col) {
    oled_write_command(0xB0 + (page & 0x07));  // Set page (0-7)
    oled_write_command(0x00 + (col & 0x0F));   // Set lower column
    oled_write_command(0x10 + ((col >> 4) & 0x0F));  // Set upper column
}

// Write a single character at current cursor position
void oled_write_char(unsigned char c) {
    unsigned char i;
    
    if(c < 0x20 || c > 0x7E)
        c = 0x20;  // Default to space for invalid characters
    
    c -= 0x20;  // Offset for font array
    
    for(i = 0; i < 5; i++) {
        oled_write_data(font_5x8[c][i]);
    }
    oled_write_data(0x00);  // Space between characters
}

// Write string at specific page and column
void oled_write_string(unsigned char page, unsigned char col, const char* str) {
    oled_set_cursor(page, col);
    
    while(*str) {
        oled_write_char(*str++);
    }
}

// Main program
void main(void) {
    // Initialize P2 as open-drain for I2C
    P2 = 0xFF;  // Set all P2 pins high initially
    
    // Initialize OLED
    delay_ms(100);  // Wait for OLED to be ready
    oled_init();
    
    delay_ms(100);
    
    // Clear display
    oled_clear();
    delay_ms(100);
    
    // Display text at different positions
    oled_write_string(0, 0, "Hello, Devesh!");
    oled_write_string(1, 0, "Welcome to I2C");
    oled_write_string(2, 0, "How are you?");
    oled_write_string(3, 0, "I am fine.");
    oled_write_string(4, 0, "What are you doing?");
    oled_write_string(5, 0, "I am coding code!");
    oled_write_string(6, 0, "Where are you?");
    oled_write_string(7, 0, "At home.");
    // Main loop
    while(1) {
        delay_ms(1000);
    }
}
