#include <reg51.h>

sbit rows[] = {P1^3, P1^4, P1^5, P1^6};
int rows_count = 4;
sbit columns[] = {P1^0, P1^1, P1^2};
int colums_count = 3;
int seven_segment_numbers[] = {
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

void delay(){
	unsigned int a = 0;
	while(a < 40000){
		a++;
}
	
void write_on_7_segment(int i){
	P2 = seven_segment_numbers[i];
}

void main(void){
	while(1){
		delay();
		
		// column detection
		int i = 0;
		for(int i = 0; i < rows_count; i++){
			rows[i] = 0;
		}
		
		int i = 0;
		for(int i = 0; i < colums_count; i++){
			columns[i] = 1;
		}
		
		if(columns[0] == 0){
			// row detection
			int i = 0;
			for(int i = 0; i < rows_count; i++){
				rows[i] = 1;
			}
		
			int i = 0;
			for(int i = 0; i < colums_count; i++){
				columns[i] = 0;
			}
			if(rows[0] == 0){
				write_on_7_segment(1);
			}
		}
		
		
		
	
	}
}