
TARGET           := DPS3005
NAME             := SimpleDPS


SRC += src/main.c

include Libraries/ili9163/make.mk
include targets/$(TARGET)/make.mk


