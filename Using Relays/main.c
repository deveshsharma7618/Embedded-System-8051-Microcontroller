#include<reg51.h>
sbit switch_pin = P3^1;
sbit relay_pin = P3^0;

void main(void){
	while(1){
		if(!switch_pin){
				relay_pin = 1;
		}else{
				relay_pin = 0;
		}
}
}