#include<reg51.h>
sbit in1 = P2^0;
sbit in2 = P2^1;
sbit in3 = P2^2;
sbit in4 = P2^3;

void main(void){
	in1 = 0;
	in2 = 1;
	
	in3  = 1;
	in4 = 0;
}