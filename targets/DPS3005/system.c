
#include <main.h>
#include <stdio.h>
#include <systick.h>
#include <serial.h>


void systemInit(void) {
	systickInit();

	// Disable JTAG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	while (1);
}
#endif
