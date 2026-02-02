#include <reg51.h>

#define lcd P3

sbit rs   = P2^0;
sbit en   = P2^1;
sbit rd   = P2^2;
sbit wr   = P2^3;
sbit intr = P2^4;
sbit rw   = P2^7;

unsigned char temp;
unsigned int i;

/* Function declarations */
void delay(void);
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char *);
unsigned char adc_read(void);

/* MAIN PROGRAM */
void main()
{
    lcd_init();
    lcd_string("Temp : ");

    while(1)
    {
        temp = adc_read();      // Direct °C value

        lcd_cmd(0x87);          // Cursor after "Temp : "

        lcd_data((temp / 100) + '0');
        lcd_data(((temp / 10) % 10) + '0');
        lcd_data((temp % 10) + '0');

        lcd_data(223);          // Degree symbol
        lcd_data('C');

        delay();
    }
}

/* SIMPLE DELAY */
void delay(void)
{
    for(i = 0; i < 10000; i++);
}

/* LCD INITIALIZATION */
void lcd_init(void)
{
    lcd_cmd(0x38);   // 8-bit, 2 line
    lcd_cmd(0x0E);   // Display ON, cursor ON
    lcd_cmd(0x06);   // Auto increment
    lcd_cmd(0x01);   // Clear display
    lcd_cmd(0x80);   // Cursor at beginning
}

/* LCD COMMAND */
void lcd_cmd(unsigned char cmd)
{
    lcd = cmd;
    rs = 0;
    rw = 0;
    en = 1;
    delay();
    en = 0;
}

/* LCD DATA */
void lcd_data(unsigned char dat)
{
    lcd = dat;
    rs = 1;
    rw = 0;
    en = 1;
    delay();
    en = 0;
}

/* DISPLAY STRING */
void lcd_string(unsigned char *str)
{
    while(*str)
        lcd_data(*str++);
}

/* ADC0804 READ */
unsigned char adc_read(void)
{
    rd = 1;
    wr = 0;     // Start conversion
    wr = 1;

    while(intr); // Wait till conversion complete

    rd = 0;      // Enable output
    return P1;   // Read ADC data
}
