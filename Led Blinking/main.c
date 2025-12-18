#include<reg51.h>
sbit p0_led = P0^0;
sbit p2_led = P2^0;
sbit p2_blinking_led = P2^7;

void delay(){
	unsigned int i;
	for(i = 0; i < 20000; i++);
}

void main(void){
	while(1){
		p0_led = 0;
		p2_led = 1;
		p2_blinking_led = 1;
		delay();
		p0_led = 1;
		p2_blinking_led = 0;
		delay();
	}
}