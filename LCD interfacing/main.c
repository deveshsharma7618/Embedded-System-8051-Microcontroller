#include <reg51.h>

#define lcd_data P3

sbit rs=P2^0;
sbit rw=P2^1;
sbit en=P2^2;

void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay();

void lcd_init()
{
    cmd(0x38);
    cmd(0x0e);
    cmd(0x01);
    cmd(0x06);
    cmd(0x0c);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    lcd_data=a;
    rs=0;
    rw=0;
    en=1;
    lcd_delay();
    en=0;
}

void dat(unsigned char b)
{
    lcd_data=b;
    rs=1;
    rw=0;
    en=1;
    lcd_delay();
    en=0;
}

void show(unsigned char *s)
{
    while(*s) {
        dat(*s++);
    }
}

void lcd_delay()
{
    unsigned int lcd_delay;
    for(lcd_delay=0;lcd_delay<=6000;lcd_delay++);
}

int main()
{
    unsigned int j;
    lcd_init();
    while(1) {
        cmd(0x80);
        show("   Welcome   ");
        cmd(0xc0);
        show(" Devesh Sharma");

        for(j=0; j<30000; j++);
        cmd(0x01);
        for(j=0; j<30000; j++);
    }
}