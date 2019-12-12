
#include <main.h>
#include <systick.h>

static __IO uint32_t sysTickCounter;


void delay (uint32_t delay) {
	sysTickCounter = delay;
	while (sysTickCounter != 0) {
	}
}

void SysTick_Handler (void) {
	if (sysTickCounter != 0x00) {
		sysTickCounter--;
	}
}


void systickInit (void) {
	while (SysTick_Config(SystemCoreClock / 1000000) != 0) {}
}
