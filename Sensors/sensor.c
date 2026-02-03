#include <reg51.h>

// Port Definitions for ADC0804
sbit ADC_RD = P2 ^ 0;   // Read signal (RD)
sbit ADC_WR = P2 ^ 1;   // Write signal (WR)
sbit ADC_INTR = P2 ^ 2; // Interrupt signal (INTR - INT0)
sbit ADC_CS = P3 ^ 5;   // Chip Select

#define ADC_DATA P1 // ADC data lines connected to Port 1

// Function Prototypes
void ADC_init(void);
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
unsigned char ADC_read(void);
float convert_to_temperature(unsigned char adc_value);
void UART_init(void);
void UART_send_char(unsigned char ch);
void UART_send_string(unsigned char *str);

// Delay function (microseconds)
void delay_us(unsigned int us)
{
    unsigned int i;
    for (i = 0; i < us; i++)
        ;
}

// Delay function (milliseconds)
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++)
            ;
}

// Initialize ADC0804
void ADC_init(void)
{
    P1 = 0xFF; // Configure Port 1 as input
    ADC_RD = 1;
    ADC_WR = 1;
    ADC_CS = 0; // Enable chip select
    delay_ms(10);
}

// Read ADC value
unsigned char ADC_read(void)
{
    unsigned char adc_value;
    unsigned int timeout = 0;

    // Start conversion
    ADC_WR = 0; // Write pulse to start conversion
    delay_us(10);
    ADC_WR = 1;
    delay_us(10); // Wait for WR pulse to settle

    // Wait for conversion to complete (INTR goes LOW when conversion is done)
    // ADC0804 conversion time is typically 100us
    while (ADC_INTR == 1 && timeout < 1000)
    {
        delay_us(1);
        timeout++;
    }

    // Read the result
    ADC_RD = 0; // Read pulse
    delay_us(10);
    adc_value = ADC_DATA; // Read data from ADC
    ADC_RD = 1;

    return adc_value;
}

// Convert ADC value to temperature (for LM35 or similar)
// LM35 output: 10mV per °C
// ADC0804: Vref = 5V, 8-bit resolution
// Formula: Temperature = (ADC_value * 5 * 100) / 256
float convert_to_temperature(unsigned char adc_value)
{
    float voltage, temperature;

    // Convert ADC value to voltage (0-5V for 0-255)
    voltage = (adc_value * 5.0) / 255.0;

    // For LM35 sensor: 10mV per °C
    temperature = voltage * 100.0;

    return temperature;
}

// UART Initialization (for debugging/displaying values)
void UART_init(void)
{
    TMOD = 0x20; // Timer 1: Mode 2 (8-bit auto-reload)
    TH1 = 0xFD;  // Load timer value for 9600 baud at 11.0592 MHz
    SCON = 0x50; // Mode 1: 8-bit UART, receiver enabled
    TR1 = 1;     // Start Timer 1
    TI = 1;      // Set TI flag for first transmission
    // ES = 1;     // Serial interrupt disabled (not using interrupts)
    // EA = 1;     // Global interrupt disabled (not using interrupts)
}

// Send character via UART
void UART_send_char(unsigned char ch)
{
    SBUF = ch;
    while (!TI)
        ;
    TI = 0;
}

// Send string via UART
void UART_send_string(unsigned char *str)
{
    while (*str)
    {
        UART_send_char(*str++);
    }
}

// Convert integer to string
void int_to_string(unsigned int num, unsigned char *str)
{
    unsigned char i = 0, temp = num;

    if (num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (temp > 0)
    {
        i++;
        temp /= 10;
    }

    temp = i;
    while (num > 0)
    {
        str[--i] = (num % 10) + '0';
        num /= 10;
    }
    str[temp] = '\0';
}

#include <string.h>

void serial_init()
{
    TMOD = 0x20; // Timer1, mode 2 (8-bit auto-reload)
    TH1 = 0xFD;  // 9600 baud rate at 11.0592 MHz
    SCON = 0x50; // Mode 1 (8-bit UART), REN enabled
    TR1 = 1;     // Start Timer1
}

void send_char(char ch)
{
    SBUF = ch; // Load character to buffer
    while (TI == 0)
        ;   // Wait until transmission complete
    TI = 0; // Clear transmit interrupt flag
}

void send_string(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        send_char(str[i]);
    }
}


// Main function
void main(void)
{
    unsigned char adc_value;
    float temperature;
    unsigned char temp_str[10];

    UART_init();
    ADC_init();

    serial_init();
    send_string("Hello from 8051!\r\n");

    UART_send_string("ADC0804 Temperature Reader\r\n");
    UART_send_string("Starting...\r\n");

    while (1)
    {
        // Read ADC value
        adc_value = ADC_read();

        // Convert to temperature
        temperature = convert_to_temperature(adc_value);

        // Display results
        UART_send_string("Temp: ");

        // Send temperature (integer part)
        int_to_string((unsigned int)temperature, temp_str);
        UART_send_string((unsigned char *)temp_str);
        UART_send_string(" C\r\n");

        delay_ms(1000); // 1 second delay between readings
    }
}