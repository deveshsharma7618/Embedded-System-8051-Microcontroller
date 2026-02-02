#include <reg51.h>

void serial_init()
{
    TMOD = 0x20;    // Timer1, mode 2
    TH1 = 0xFD;     // 9600 baud rate
    SCON = 0x50;    // Mode 1, REN enabled
    TR1 = 1;        // Start Timer1
}

void receive_char()
{
    while(RI == 0);
    P1 = SBUF;
		RI = 0;
}

void main()
{
    serial_init();

    while(1)
    {
        receive_char();
    }
}
