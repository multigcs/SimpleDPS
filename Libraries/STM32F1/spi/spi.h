

#define SPI_RX_MAX 1024

extern uint8_t spiRxBuff[SPI_RX_MAX];

void spiWrite(uint8_t *data, uint16_t len);
void spiInit(void);

