#include <reg51.h>

sbit pin = P2^0;

void pwm(int n);
void delay();

void main(void){
	unsigned int i;
	while(1){
		delay();

		for(i = 1; i <= 255; i++){
			pwm(i);
		}
	}
}

void pwm(int n){
	unsigned int i;
	for(i = 1; i <= n*100; i++)
		pin = 1;
	for(; i <= 25500; i++)
		pin = 0;
}

void delay(){
	unsigned int i;
	for(i = 1; i <= 65000; i++);
}
