#include <reg51.h>
#include <string.h>
#define lcd_width 20
#define lcd_height 4
sbit LED = P1 ^ 0; // Define LED at P1.0
int cursor_pos = 0;

unsigned int timer_count = 0; // Counter for 1 second

void Timer0_Init(void);
void lcd_send_byte(unsigned char byte_val);
void lcd_command(unsigned char cmd);
void lcd_goto(unsigned char row, unsigned char col);
void send_lcd(unsigned char *s);
void clear_screen();

void delay_ms(unsigned int ms);

// Timer 0 ISR - Toggles P1^0 every 1 second
void Timer0_ISR(void) interrupt 1
{
    TH0 = 0xFC; // Reload for ~1ms delay @ 11.0592MHz
    TL0 = 0x66;

    timer_count++;
    if (timer_count >= 1000) // 1000ms = 1 second
    {
        timer_count = 0;
        LED = ~LED; // Toggle LED every 1 second
    }
}

void main(void)
{
    delay_ms(500); // Wait for LCD to power up

    // Initialize UART for 9600 baud at 11.0592 MHz crystal
    SCON = 0x50;      // 8-bit UART, enable receiver
    TMOD = 0x20;      // Timer 1 in mode 2 (auto-reload)
    TH1 = TL1 = 0xFD; // 9600 baud
    TR1 = 1;          // Start timer 1
    // Initialize LCD
    clear_screen();
    Timer0_Init();

    // Display on serial LCD

    while (1)
    {
        lcd_goto(0, 0);
        send_lcd("Intrupts in C");

        lcd_goto(1, 0);
        send_lcd("Using 8051 Timer0");

        lcd_goto(2, 0);
        send_lcd("Devesh Sharma");

        delay_ms(4000);
        clear_screen();

    }
}

void Timer0_Init(void)
{
    TMOD &= 0xF0; // Clear Timer 0 bits
    TMOD |= 0x01; // Timer 0, Mode 1 (16-bit timer)
    TH0 = 0xFC;   // Load initial value for ~1ms delay @ 11.0592MHz
    TL0 = 0x66;
    EA = 1;  // Enable global interrupts
    ET0 = 1; // Enable Timer 0 interrupt
    TR0 = 1; // Start Timer 0
}

void lcd_send_byte(unsigned char byte_val)
{
    SBUF = byte_val;
    while (TI == 0)
        ;
    TI = 0;
    delay_ms(1);
}

void lcd_command(unsigned char cmd)
{
    lcd_send_byte(0xFE); // Command prefix
    lcd_send_byte(cmd);
    delay_ms(2);
}

void lcd_goto(unsigned char row, unsigned char col)
{
    unsigned char pos;
    if (row == 0)
        pos = 0x00 + col;
    else if (row == 1)
        pos = 0x40 + col;
    else if (row == 2)
        pos = 0x14 + col;
    else
        pos = 0x54 + col;

    lcd_command(0x80 | pos);
    cursor_pos = row * lcd_width + col;
}

// Send string to LCD
void send_lcd(unsigned char *s)
{
    int i = 0;
    int j = 1;
    while (*s != '\0')
    {
        lcd_send_byte(*s);
        i++;
        s++;
        cursor_pos++;
        if (i >= lcd_width - 1)
        {
            lcd_goto(j, 0);
            j++;
            i = 0;
        }
    }
}

// Clear entire LCD screen
void clear_screen()
{
    lcd_command(0x01); // Clear display command
    delay_ms(2);
    cursor_pos = 0;
}
// Delay function in milliseconds
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++)
            ; // Approx 1ms @ 11.0592MHz
}