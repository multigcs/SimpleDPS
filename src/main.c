
#include <main.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <adc.h>
#include <ili9163.h>

#define TYPE 1

__IO float volt_set = 3.3;
__IO float amp_set = 0.5;
__IO float volt_preset = 3.3;
__IO float amp_preset = 0.5;
__IO uint8_t power = 0;

__IO int8_t encoder_last = 0;
__IO int8_t encoder_val = 0;
__IO uint8_t encoder_sw = 0;

uint8_t setmode = 0;
uint8_t flash_update = 0;


#define BANK1_WRITE_START_ADDR  ((uint32_t)0x800cc00)
uint32_t fidnum = 0x3210ABC1;

void flash_read() {
    uint32_t Address = BANK1_WRITE_START_ADDR;
    if ((*(__IO uint32_t*) Address) == fidnum) {
        uint32_t i_volt_set = (*(__IO uint32_t*) (Address + 4));
        uint32_t i_amp_set = (*(__IO uint32_t*) (Address + 8));
        volt_set = i_volt_set / 10.0;
        amp_set = i_amp_set / 10.0;
        volt_preset = volt_set;
        amp_preset = amp_set;
    }
}

void flash_write() {
    uint32_t Address = BANK1_WRITE_START_ADDR;
    FLASH_UnlockBank1();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ErasePage(BANK1_WRITE_START_ADDR);
    FLASH_ProgramWord(Address, fidnum);
    uint32_t i_volt_set = volt_set * 10.0;
    uint32_t i_amp_set = amp_set * 10.0;
    FLASH_ProgramWord((Address + 4), i_volt_set);
    FLASH_ProgramWord((Address + 8), i_amp_set);
    FLASH_LockBank1();
}


// Fix Timer for Timeline-Moves and Encoder-Read
void TIM3_IRQHandler() {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		int8_t new = 0;
		int8_t diff;
		int8_t delta;
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0) {
			new = 3;
		}
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0) {
			new ^= 1;
		}
		diff = encoder_last - new;
		if (diff & 1) {
			encoder_last = new;
			delta = (diff & 2) - 1;
			if (delta > 0) {
				encoder_val++;
			} else if (delta < 0) {
				encoder_val--;
			}
		}

        // power button
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0) {
			power = 1 - power;
			while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0);
		}
        // upper button
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0) {
			setmode = 0;
		}
        // lower button
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0) {
			setmode = 1;
		}
        // encoder button
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0) {
            if (volt_set != volt_preset || amp_set != amp_preset) {
                flash_update = 1;
            }
			volt_set = volt_preset;
            amp_set = amp_preset;
		}
        // set button
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0) {
            if (volt_set != volt_preset || amp_set != amp_preset) {
                flash_update = 1;
            }
			volt_set = volt_preset;
            amp_set = amp_preset;
		}


    }
}

void timer3Init(void) {
	uint16_t PrescalerValue = 100;
	uint16_t Period = 600;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure; 
	timerInitStructure.TIM_Prescaler = PrescalerValue;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = Period;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


volatile uint8_t rx_data[25];
volatile uint8_t rx_n = 0;


void USART1_IRQHandler(void) {
	static uint8_t n = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		char c = USART_ReceiveData(USART1);
		if (c == '\n' || c == '\r') {
			if (rx_data[0] >= 'A' && rx_data[0] <= 'Z' && rx_data[rx_n - 1] == '#') {
				uint8_t vn = 0;
				uint16_t values[2];
				for (n = 1; n < rx_n; n++) {
					if (rx_data[n] == ';' && vn < 2) {
						values[vn++] = atoi(rx_data + n + 1);
					}
				}
				if (rx_data[0] == 'V' && rx_data[1] == ';' && vn == 1) {
					// echo "V;10000#" > /dev/rfcomm1 // in mV
					volt_set = (float)values[0] / 1000.0;
				} else if (rx_data[0] == 'A' && rx_data[1] == ';' && vn == 1) {
					// echo "A;500#" > /dev/rfcomm1 // in mA
					amp_set = (float)values[0] / 1000.0;
				} else if (rx_data[0] == 'P' && rx_data[1] == ';' && vn == 1) {
					// echo "P;1#" > /dev/rfcomm1
					power = (uint8_t)values[0];
				} else {
					printf("Wrong num values\r\n");
				}
			} else {
				printf("unknown cmd\r\n");
			}
			rx_n = 0;
		} else if (rx_n < 24) {
			rx_data[rx_n++] = c;
			rx_data[rx_n] = 0;
		}
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {   
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	}
}




int main(void) {
	char tmp_str[24];
	uint8_t update = 0;
	uint8_t stat = 0;

	systemInit();
	Adc1Init();

	serialInit();
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Analog-IN Pin(s)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// DAC Pin(s)
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PowerCTL
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);

	// Buttons
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Encoder
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	timer3Init();

	// DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 0xF;          
	TIM_TimeBaseStructure.TIM_Prescaler = 0xF;       
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	DAC_InitTypeDef            DAC_InitStructure;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);
	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	// Display
	ili9163Init();

    flash_read();
    setmode = 0;

	while (1) {

		// DACs
		if (power == 1) {
            if (TYPE == 1) {
                DAC_SetChannel1Data(DAC_Align_12b_R, (uint16_t)((volt_set - 1.7) / 0.01308));
                DAC_SetChannel2Data(DAC_Align_12b_R, (uint16_t)(amp_set * 615) + 250);
            } else {
                DAC_SetChannel1Data(DAC_Align_12b_R, (uint16_t)(volt_set / 0.01308));
                DAC_SetChannel2Data(DAC_Align_12b_R, (uint16_t)(amp_set * 615) + 250);
            }
			GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
		} else {
			DAC_SetChannel1Data(DAC_Align_12b_R, 0);
			DAC_SetChannel2Data(DAC_Align_12b_R, 0);
			GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
		}

		// ADCs
		uint16_t n = 0;
		uint32_t val1 = 0;
		uint32_t val2 = 0;
		uint32_t val3 = 0;
		for (n = 0; n < 180; n++) {
			val1 += Adc1Read(8);
			val2 += Adc1Read(9);
			val3 += Adc1Read(7);
		}
		float volt_in = (float)(val1 / 180) / 60.0;
		float volt_out = (float)(val2 / 180) / 77.0;
		float amp_out = ((float)(val3 / 180) - 39.0) / 590.0;
		if (amp_out < 0.0) {
			amp_out = 0.0;
		}


        if (flash_update == 1) {
            flash_update = 0;
            flash_write();
        }

		if (stat == 1) {
			ili9163Puts(1, 120, 1, BLUE, "*");
			stat = 0;
		} else {
			ili9163Puts(1, 120, 1, BLUE, " ");
			stat = 1;
		}


		// Display
        if (setmode == 0) {
            ili9163Puts(2, 79, 2, MAGENTA, " ");
            if (volt_preset != volt_set && stat == 0) {
                ili9163Puts(2, 19, 2, YELLOW, " ");
            } else {
                ili9163Puts(2, 19, 2, YELLOW, ">");
            }
            if (encoder_val != 0) {
                volt_preset += (float)encoder_val / 20;
                encoder_val = 0;
                update = 1;
            }
            amp_preset = amp_set;
        } else {
            ili9163Puts(2, 19, 2, YELLOW, " ");
            if (amp_preset != amp_set && stat == 0) {
                ili9163Puts(2, 79, 2, MAGENTA, " ");
            } else {
                ili9163Puts(2, 79, 2, MAGENTA, ">");
            }
            if (encoder_val != 0) {
                amp_preset += (float)encoder_val / 20;
                encoder_val = 0;
                update = 2;
            }
            volt_preset = volt_set;
        }
		if (update == 0) {
			update = 1;
			if (power == 1) {
				ili9163Puts(2, 1, 1, GREEN, "On ");
				sprintf(tmp_str, "  In=%02.2fV", volt_in);
				n = ili9163Lens(1, tmp_str);
				ili9163Puts(127 - n, 1, 1, GREEN, tmp_str);
				ili9163Line(0, 10, 127, 10, GREEN);
			} else {
				ili9163Puts(2, 1, 1, RED, "Off ");
				sprintf(tmp_str, "  In=%02.2fV", volt_in);
				n = ili9163Lens(1, tmp_str);
				ili9163Puts(127 - n, 1, 1, RED, tmp_str);
				ili9163Line(0, 10, 127, 10, RED);
			}
		} else if (update == 1) {
			update = 2;
			sprintf(tmp_str, "%02.2fV   ", volt_preset);
			ili9163Puts(20, 19, 2, YELLOW, tmp_str);
			sprintf(tmp_str, "  %02.2fV", volt_out);
			n = ili9163Lens(3, tmp_str);
			ili9163Puts(126 - n, 40, 3, YELLOW, tmp_str);
		} else {
			update = 0;
			sprintf(tmp_str, "%02.2fA   ", amp_preset);
			ili9163Puts(20, 79, 2, MAGENTA, tmp_str);
			sprintf(tmp_str, "  %02.2fA", amp_out);
			n = ili9163Lens(3, tmp_str);
			ili9163Puts(126 - n, 100, 3, MAGENTA, tmp_str);
		}



		printf("#%i", power);
		printf(";%02.2f", volt_in);
		printf(";%02.2f", volt_set);
		printf(";%02.2f", volt_out);
		printf(";%02.2f", amp_set);
		printf(";%02.2f", amp_out);
		printf("\r\n", amp_out);

	}

}





