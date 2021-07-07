#include "misc.h"
#define PRIORITY_VAL 1
#define TCMPR_VAL 3000000
//#define TCMPR_VAL 187500 
//#define TCMPR_VAL 11718
//#define TCMPR_VAL 732

void TMR0_EnableInt(void) {
	TIMER0->TCSR |= (1ul << 29);
}

void Enable_IRQ0_NVIC(void)
{
	NVIC->ISER[0] = (1 << (8 & 0x1F));
}

void Set_output_mode(int gpcNum){
	PC->PMD &= ~(0b11 << gpcNum * 2);					// Change pin at gpioNum to OUTPUT mode, whose value should be from 12-15
	PC->PMD |= (0b01 << gpcNum * 2);
}

void Toggle_LED(int gpcNum){
	PC -> DOUT ^= (1 << gpcNum);				// Toggle the LED, whose value should be from 12-15
}

void SysInit(void) {
	uint32_t clkMask = 0x10;
	int32_t timeOutCount = 1200000;
	
	SYS_UnlockReg(); // Unlock protected registers
	// select Chip clock source & set clock source
	
	// Enable HXT external 12MHz crystal
	CLK->PWRCON |= (1ul << 0);

	//Wait till clock rdy
    while((CLK->CLKSTATUS & clkMask) != clkMask)
    {
        if(timeOutCount-- <= 0)
            break;
    }
	
	// Select timer clock sources
    CLK->CLKSEL1 &= ~(0b111 << 8);
	CLK->CLKSEL1 |= (0 << 8);

	//Enable TMR0
	//CLK->APBCLK &= 0;
	CLK->APBCLK |= (1 << 2);
	
	SYS_LockReg();  // Lock protected registers
}

void ClearIntFlag_TIMER0(void) {
	TIMER0->TISR |= 1ul;
}

void Init_Timer0(void){
	//Prescale 4
	TIMER0->TCSR &= ~(0xFF);
	TIMER0->TCSR |= 3;
	
	//Periodic mode
	TIMER0->TCSR &= ~(0b11 << 27);
	TIMER0->TCSR |= (1ul <<27);
	
	//Compare register value
	TIMER0->TCMPR &= ~(0xFFFFFF);
	TIMER0->TCMPR |= TCMPR_VAL;
	
	//Timer0 enable
    TMR0_EnableInt();
	
	//Enable Interrupt
    Enable_IRQ0_NVIC();
	
	//Start counting
	TIMER0->TCSR |= (1ul <<30);
	
	//Set priority to 1
	NVIC->IP[2] &= ~(PRIORITY_VAL << 6);
	NVIC->IP[2] |= (PRIORITY_VAL << 6);
}

void Init_GPB15(void){
	//GPIO Interrupt configuration. GPIO-B15 is the interrupt source
	PB->PMD &= (~(0x03ul << 30));
	PB->IMD &= (~(1ul << 15));
	PB->IEN |= (1ul << 15);
	
	//NVIC interrupt configuration for GPIO-B15 interrupt source
	NVIC->ISER[0] |= 1ul<<3;
	NVIC->IP[0] &= (~(3ul<<30));
}
