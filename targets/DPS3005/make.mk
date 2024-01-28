
BUILDDIR         := build/$(TARGET)
OPTLVL           := s
TOOLCHAIN_PREFIX := arm-none-eabi
OPENOCDCFG        = target/stm32f1x.cfg
AS                = $(TOOLCHAIN_PREFIX)-as
AR                = $(TOOLCHAIN_PREFIX)-ar
CC                = $(TOOLCHAIN_PREFIX)-gcc
LD                = $(TOOLCHAIN_PREFIX)-gcc
GDB               = $(TOOLCHAIN_PREFIX)-gdb
SIZE              = $(TOOLCHAIN_PREFIX)-size
OBJCOPY           = $(TOOLCHAIN_PREFIX)-objcopy
EXTLIBS          := Libraries
MISCLIB          := $(EXTLIBS)/STM32F1
CMSIS            := $(EXTLIBS)/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS
STD_PERIPH       := $(EXTLIBS)/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver

INCLUDE += -I$(STD_PERIPH)/inc
INCLUDE += -I$(MISCLIB)
INCLUDE += -I$(CMSIS)/CM3/DeviceSupport/ST/STM32F10x
INCLUDE += -I$(CMSIS)/CM3/CoreSupport
INCLUDE += -I$(CURDIR)/src
INCLUDE += -Itargets/$(TARGET)
INCLUDE += -I$(MISCLIB)/serial
INCLUDE += -I$(MISCLIB)/adc
INCLUDE += -I$(MISCLIB)/spi
INCLUDE += -I$(MISCLIB)/systick

STM_SRC = 
STM_SRC += $(STD_PERIPH)/src/misc.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_adc.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_bkp.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_can.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_cec.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_crc.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_dac.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_dbgmcu.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_dma.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_exti.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_flash.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_fsmc.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_gpio.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_iwdg.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_pwr.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_rcc.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_rtc.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_sdio.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_spi.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_tim.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_usart.c
STM_SRC += $(STD_PERIPH)/src/stm32f10x_wwdg.c
STM_SRC += $(MISCLIB)/system_stm32f10x.c
STM_SRC += targets/$(TARGET)/syscalls.c
STM_SRC += targets/$(TARGET)/system.c

STM_SRC += $(MISCLIB)/systick/systick.c
STM_SRC += $(MISCLIB)/serial/serial.c
STM_SRC += $(MISCLIB)/spi/spi.c
STM_SRC += $(MISCLIB)/adc/adc.c

MCUFLAGS  = -mcpu=cortex-m3 -mthumb -mlittle-endian
MCUFLAGS += -fsingle-precision-constant -Wall -finline-functions
MCUFLAGS += -Wdouble-promotion -std=c99 -fno-dwarf2-cfi-asm  -mno-thumb-interwork
MCUFLAGS += -ffunction-sections -fdata-sections -fno-common -fmessage-length=0

CDEFS = -DUSE_STDPERIPH_DRIVER

STMTYPE        = STM32F10X_MD
RCC_OPT        =
LINKER_SCRIPT  = $(MISCLIB)/stm32_flash_f100.ld
ASM_SRC        = $(MISCLIB)/startup_stm32f10x_md.s

COMMONFLAGS = -O$(OPTLVL) -g -Wall
CFLAGS      = $(COMMONFLAGS) $(MCUFLAGS) $(INCLUDE) $(CDEFS) $(RCC_OPT) -D$(STMTYPE)
LDLIBS      = -lm -lc -lg
LDFLAGS     = $(COMMONFLAGS) $(MCUFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -nostartfiles -Wl,-T,$(LINKER_SCRIPT)
LDFLAGS    += -Wl,--gc-sections -Wl,--Map=$(BUILDDIR)/$(NAME).map
OBJ         = $(STM_SRC:%.c=$(BUILDDIR)/%.o) $(SRC:%.c=$(BUILDDIR)/%.o) $(ASM_SRC:%.s=$(BUILDDIR)/%.o)

all: $(BUILDDIR)/$(NAME).elf


$(BUILDDIR)/$(NAME).elf: $(OBJ)
	$(CC) -o $(BUILDDIR)/$(NAME).elf $(LDFLAGS) $(OBJ) $(LDLIBS)

$(BUILDDIR)/$(NAME).hex: $(BUILDDIR)/$(NAME).elf
	$(OBJCOPY) --remove-section .ccm -O ihex   $(BUILDDIR)/$(NAME).elf $(BUILDDIR)/$(NAME).hex

$(BUILDDIR)/$(NAME).bin: $(BUILDDIR)/$(NAME).elf
	$(OBJCOPY) --remove-section .ccm -O binary $(BUILDDIR)/$(NAME).elf $(BUILDDIR)/$(NAME).bin

clean:
	rm -f $(OBJ)
	rm -f $(BUILDDIR)/$(NAME).elf
	rm -f $(BUILDDIR)/$(NAME).hex
	rm -f $(BUILDDIR)/$(NAME).bin
	rm -f $(BUILDDIR)/$(NAME).map

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

flash: $(BUILDDIR)/$(NAME).bin
	#stm32flash -w $(BUILDDIR)/$(NAME).bin -v -b 115200 -g 0x0 /dev/ttyUSB0
	stm32flash -w $(BUILDDIR)/$(NAME).bin -v -b 500000 -g 0x0 /dev/ttyUSB0

st-flash: $(BUILDDIR)/$(NAME).bin
	st-flash write $(BUILDDIR)/$(NAME).bin 0x8000000

monitor:
	sleep .5
	gtkterm --port /dev/ttyUSB0 --speed 115200

