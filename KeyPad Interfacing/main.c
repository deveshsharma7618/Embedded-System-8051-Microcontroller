#include<reg51.h>

sbit r1 = P1^3;
sbit r2 = P1 ^ 4;
sbit r3 = P1 ^ 5;
sbit r4 = P1 ^ 6;
sbit c1 = P1 ^ 0;
sbit c2 = P1 ^ 1;
sbit c3 = P1 ^ 2;
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

void show(int digit){
	P2 = digits[digit];
}

void main(void){
	while(1){
		c1 = c2 = c3 = 1;
		r1 = r2 = r3 = r4 = 0;
		if(c1 == 0){
			r1 = r2 = r3 = r4 = 1;
			c1 = c2 =c3 = 0;
			if(r1 == 0){
				show(1);
			}else if(r2 == 0){
				show(4);
			}else if(r3 == 0){
				show(7);
			}else if(r4 == 0){
				
			}
		}else if(c2 == 0){
			r1 = r2 = r3 = r4 = 1;
			c1 = c2 =c3 = 0;
			if(r1 == 0){
				show(2);
			}else if(r2 == 0){
				show(5);
			}else if(r3 == 0){
				show(8);
			}else if(r4 == 0){
				show(0);
			}
		}else if(c3 == 0){
			r1 = r2 = r3 = r4 = 1;
			c1 = c2 =c3 = 0;
			if(r1 == 0){
				show(3);
			}else if(r2 == 0){
				show(6);
			}else if(r3 == 0){
				show(9);
			}else if(r4 == 0){
				
			}
		}
	}
}