#ifndef __MISC__
#define __MISC__
#include "NUC100Series.h"

void Set_output_mode(int gpcNum);
void Toggle_LED(int gpcNum);
void SysInit(void);
void TMR0_EnableInt(void);
void Enable_IRQ0_NVIC(void);
void ClearIntFlag_TIMER0(void);
void Init_Timer0(void);
void Init_GPB15(void);
#endif
