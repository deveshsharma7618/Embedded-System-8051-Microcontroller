#include <reg51.h>

sbit led_pin = P2^0;

void delay_ms(unsigned int ms) {
    unsigned int i;
    unsigned int count;
    for (i = 0; i < ms; i++) {
        TMOD = 0x01; // Timer 0, Mode 1 (16-bit)
        TH0 = 0xFF;  // Load high byte (for ~1ms delay)
        TL0 = 0x08;  // Load low byte
        TR0 = 1;     // Start Timer 0

        while (TF0 == 0); // Wait for timer overflow

        TR0 = 0;    // Stop Timer 0
        TF0 = 0;    // Clear Timer 0 flag
    }
}

void main() {
     
    while (1) {
        led_pin = 1; 
        delay_ms(1000); 
        led_pin = 0; 
        delay_ms(1000); 
    }
}
