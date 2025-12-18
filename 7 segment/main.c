#include<reg51.h>

void delay(){
	int i = 0;
	for(i = 0; i < 30000; i++);
}

int digits[] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F, // 9
  0x77, // A
  0x7C, // b (lowercase)
  0x3D, // C
  0x5E, // d (lowercase)
  0x79, // E
  0x71  // F
};
int i = 0;
int j = 0;

void main(void){
	while(1){
		P3 = digits[i];
		P2 = digits[j];
		j = (j+1)%10;
		if(j == 0 ){
			i = (i+1) % 10;
		}
		
		delay();
	}
}