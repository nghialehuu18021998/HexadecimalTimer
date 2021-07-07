#include <stdio.h>
#include "NUC100Series.h"
#include "misc.h"
#include "Seven_segment_LED.h"

#define ACTIVITY 5
#if ACTIVITY == 2
void TMR0_IRQHandler(void){
	Toggle_LED(12ul);
	ClearIntFlag_TIMER0();
}

int main(void)
{
	Set_output_mode(12ul);
	SysInit();
	Init_Timer0();
	while(1);
}
#elif ACTIVITY == 4

volatile uint32_t current_value = 0;
volatile uint32_t current_segment = 0;

int main(void)
{
	SysInit();
	Init_Timer0();
	while (1);
}

void TMR0_IRQHandler(void){
	CloseSevenSegment();
	ShowSevenSegment(current_segment, current_value);
 	current_value++;
	if (current_value > 15)
	{
		current_value = 0;
		current_segment++;
		if(current_segment > 3){
			current_segment = 0;
		}
	}
	ClearIntFlag_TIMER0();
}
#elif ACTIVITY == 5

volatile uint32_t current_value[4] = {0, 0, 0, 0};
volatile uint32_t current_segment = 0;
volatile uint8_t is_counting = 1;

void reset_timer(void) {
	uint32_t i;
	for (i = 0; i < 4; i++) {
		current_value[i] = 0;
	}
	current_segment = 0;
}

void display_segments(void) {
	uint32_t i;
	for (i = 0; i < 4; i++) {
		CloseSevenSegment();
		ShowSevenSegment(i, current_value[i]);
	}
}

int main(void) {
	
	SysInit();
	Set_output_mode(12ul);
	Init_GPB15();
	Init_Timer0();
	while(1) {
		display_segments();
	}
}

void TMR0_IRQHandler(void){
	if(is_counting) {
		current_value[0]++;
		//Toggle_LED(12ul);
		if (current_value[0] >15)	{
			current_value[0] = 0;
			current_value[1]++;
			//Toggle_LED(12ul);
			if (current_value[1] >15)	{
				current_value[1] = 0;
				current_value[2]++;
				//Toggle_LED(12ul);
				if (current_value[2] >15)	{
					current_value[2] = 0;
					current_value[3]++;
					//Toggle_LED(12ul);
					if (current_value[3] >15)	{
						reset_timer();
					}
				}
			}
		}
	}
	
	ClearIntFlag_TIMER0();
}

void EINT1_IRQHandler(void){
	while (!(PB->PIN & 1 << 15)){
		is_counting = 0;
		display_segments();
	}
	is_counting = 1;
	//clear interrupt flag
	PB->ISRC |= (1ul << 15);
}
#endif
