#include "NUC100Series.h"
#include "Seven_segment_LED.h"

bin* decimal_to_binary(int decimal){
	static bin ip[4];
	for (int i = 0; i < 4; i++) {
		ip[i].x = (decimal & (1 << i)) >> i;
	}
	return ip;
}

void EnableSegment(uint32_t segment_no) {
	PE -> DOUT ^= 0xFF;
	PC -> DOUT |= (1<<(4+segment_no));
}

void CloseSegment(uint32_t segment_no) { 
	PC->DOUT &= ~(0x01 << (segment_no + 4));
}

void CloseSevenSegment(void) {
	uint32_t i = 0;
	for (i = 0; i < 4; i++){
		CloseSegment(i);
	}
}

void OpenSevenSegment(void) {
	uint32_t i = 0;
	//Set GPIO PC4 to 7 mode to OUTPUT
	
	PC -> PMD &= ~(0xFFul) << 8;
    PC -> PMD |= (0x55ul) << 8;
	//Set PC4-7 val to 0
	for (i = 4; i< 8; i++) {
		PC->DOUT &= ~(0x01 << i);
	}
	//Set PE0 to 7 mode to quasi
	PE -> PMD &= ~(0x10000);
    PE -> PMD |= (0x10000);
	// Set PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7 to 0
	for (i = 0; i < 8; i++){
		PE->DOUT &= ~((0x01) << i);
	}
}

void Delay_s(uint32_t count)
{
	uint32_t i;
	for (i = 0; i< count;i++)
	{
	}
}


void ShowSevenSegment(uint8_t no, uint8_t number) {
	short binA, binB, binC, binD;
	bin* binValue = decimal_to_binary(number);		// Binary value of 
	
	CloseSevenSegment();
	PE -> DOUT |= 0xFF;
	
	binA = (binValue+3)->x;
	binB = (binValue+2)->x;
	binC = (binValue+1)->x;
	binD = binValue->x;
	
	// [7] 	Y	= A'B'C' + A'BCD + ABC'D'				
	PE -> DOUT &= ~(((~binA & ~binB & ~binC) | 
											( ~binA &binB &binC &binD )|
													( binA &binB & ~binC & ~binD )) << 7);
	
	// [6] Y	= A'D + B'C'D + A'BC'				
	PE -> DOUT &= ~(((~binA & binD) | 		
										(~binB & ~binC & binD)|
												( ~binA &binB & ~binC )) << 6);

	// [5] Y = BCD + A'B'C'D + A'BC'D' + AB'CD'							
	PE -> DOUT &= ~(((binB & binC & binD)|				
									(~binA & ~binB & ~binC & binD)|
											(~binA & binB & ~binC & ~binD)|
												(binA & ~binB & binC & ~binD)) << 5);

	// [4] 	Y = BCD' + ACD + ABD' + A'BC'D					
	PE -> DOUT &= ~(((binB & binC & ~binD)|				
									(binA & binC & binD)|
											(binA & binB & ~binD)|
												(~binA & binB & ~binC & binD)) << 4);
	
	// [3] 	Y = A'B'C'D + A'BC'D' + AB'CD + ABC'D 					
	PE -> DOUT &= ~(((~binA & ~binB & ~binC & binD)|				
										(~binA & binB & ~binC & ~binD)|	
											(binA & ~binB & binC& binD)|
													(binA & binB & ~binC & binD)) << 3);

	// [2] 	Y = A'B'D + A'B'C + A'CD + ABC'D					
	PE -> DOUT &= ~(((~binA & ~binB & binD)|				
										(~binA & ~binB & binC)|				
											(~binA & binC & binD)|	
													(binA & binB & ~binC & binD)) << 2);
	// [1] 	Y = 1
	PE -> DOUT &= ~(1 << 1);
			
				// [0] 	C = ABD' + ABC + A'B'CD'									
	PE -> DOUT &= ~(((binA & binB & ~binD)|				
											(binA & binB & binC)|
												(~binA & ~binB & binC & ~binD)) << 0);
										
	EnableSegment(no);
}
