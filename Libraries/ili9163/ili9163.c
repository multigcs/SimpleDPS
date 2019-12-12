
#include <main.h>
#include <string.h>
#include <ili9163.h>


static const uint8_t ili9163Font8x16[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 0
  0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00, // ! 1
  0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // " 2
  0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00, // # 3
  0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00, // $ 4
  0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00, // % 5
  0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10, // & 6
  0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ' 7
  0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00, // ( 8
  0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00, // ) 9
  0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00, // * 10
  0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00, // + 11
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00, // , 12
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01, // - 13
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00, // . 14
  0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00, // / 15
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00, // 0 16
  0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00, // 1 17
  0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00, // 2 18
  0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00, // 3 19
  0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00, // 4 20
  0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00, // 5 21
  0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00, // 6 22
  0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00, // 7 23
  0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00, // 8 24
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00, // 9 25
  0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00, // : 26
  0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00, // ; 27
  0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00, // < 28
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00, // = 29
  0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00, // > 30
  0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00, // ? 31
  0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00, // @ 32
  0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20, // A 33
  0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00, // B 34
  0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00, // C 35
  0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00, // D 36
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00, // E 37
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00, // F 38
  0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00, // G 39
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20, // H 40
  0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00, // I 41
  0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00, // J 42
  0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00, // K 43
  0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00, // L 44
  0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00, // M 45
  0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00, // N 46
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00, // O 47
  0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00, // P 48
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00, // Q 49
  0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20, // R 50
  0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00, // S 51
  0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00, // T 52
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00, // U 53
  0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00, // V 54
  0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00, // W 55
  0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20, // X 56
  0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00, // Y 57
  0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00, // Z 58
  0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00, // [ 59
  0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00, // \ 60
  0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00, // ] 61
  0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ^ 62
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80, // _ 63
  0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ` 64
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20, // a 65
  0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00, // b 66
  0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00, // c 67
  0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20, // d 68
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00, // e 69
  0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00, // f 70
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00, // g 71
  0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20, // h 72
  0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00, // i 73
  0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00, // j 74
  0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00, // k 75
  0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00, // l 76
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F, // m 77
  0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20, // n 78
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00, // o 79
  0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00, // p 80
  0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80, // q 81
  0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00, // r 82
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00, // s 83
  0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00, // t 84
  0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20, // u 85
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00, // v 86
  0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00, // w 87
  0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00, // x 88
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00, // y 89
  0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00, // z 90
  0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40, // { 91
  0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00, // | 92
  0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00, // } 93
  0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ~ 94
};

static const uint8_t ili9163Font5x8[] = {
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, // sp
	0x03, 0x00, 0x00, 0x2f, 0x00, 0x00, // !
	0x04, 0x00, 0x07, 0x00, 0x07, 0x00, // "
	0x05, 0x14, 0x7f, 0x14, 0x7f, 0x14, // #
	0x05, 0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
	0x05, 0x62, 0x64, 0x08, 0x13, 0x23, // %
	0x05, 0x36, 0x49, 0x55, 0x22, 0x50, // &
	0x03, 0x00, 0x05, 0x03, 0x00, 0x00, // '
	0x05, 0x00, 0x1c, 0x22, 0x41, 0x00, // (
	0x05, 0x00, 0x41, 0x22, 0x1c, 0x00, // )
	0x05, 0x14, 0x08, 0x3E, 0x08, 0x14, // *
	0x05, 0x08, 0x08, 0x3E, 0x08, 0x08, // +
	0x05, 0x00, 0x00, 0xA0, 0x60, 0x00, // ,
	0x05, 0x08, 0x08, 0x08, 0x08, 0x08, // -
	0x02, 0x60, 0x60, 0x00, 0x00, 0x00, // .
	0x05, 0x20, 0x10, 0x08, 0x04, 0x02, // /
	0x05, 0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
	0x03, 0x42, 0x7F, 0x40, 0x00, 0x00, // 1
	0x05, 0x42, 0x61, 0x51, 0x49, 0x46, // 2
	0x05, 0x21, 0x41, 0x45, 0x4B, 0x31, // 3
	0x05, 0x18, 0x14, 0x12, 0x7F, 0x10, // 4
	0x05, 0x27, 0x45, 0x45, 0x45, 0x39, // 5
	0x05, 0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
	0x05, 0x01, 0x71, 0x09, 0x05, 0x03, // 7
	0x05, 0x36, 0x49, 0x49, 0x49, 0x36, // 8
	0x05, 0x06, 0x49, 0x49, 0x29, 0x1E, // 9
	0x02, 0x36, 0x36, 0x00, 0x00, 0x00, // :
	0x02, 0x56, 0x36, 0x00, 0x00, 0x00, // ;
	0x05, 0x08, 0x14, 0x22, 0x41, 0x00, // <
	0x05, 0x14, 0x14, 0x14, 0x14, 0x14, // =
	0x05, 0x00, 0x41, 0x22, 0x14, 0x08, // >
	0x05, 0x02, 0x01, 0x51, 0x09, 0x06, // ?
	0x05, 0x32, 0x49, 0x59, 0x51, 0x3E, // @
	0x05, 0x7C, 0x12, 0x11, 0x12, 0x7C, // A
	0x05, 0x7F, 0x49, 0x49, 0x49, 0x36, // B
	0x05, 0x3E, 0x41, 0x41, 0x41, 0x22, // C
	0x05, 0x7F, 0x41, 0x41, 0x22, 0x1C, // D
	0x05, 0x7F, 0x49, 0x49, 0x49, 0x41, // E
	0x05, 0x7F, 0x09, 0x09, 0x09, 0x01, // F
	0x05, 0x3E, 0x41, 0x49, 0x49, 0x7A, // G
	0x05, 0x7F, 0x08, 0x08, 0x08, 0x7F, // H
	0x05, 0x00, 0x41, 0x7F, 0x41, 0x00, // I
	0x05, 0x20, 0x40, 0x41, 0x3F, 0x01, // J
	0x05, 0x7F, 0x08, 0x14, 0x22, 0x41, // K
	0x05, 0x7F, 0x40, 0x40, 0x40, 0x40, // L
	0x05, 0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
	0x05, 0x7F, 0x04, 0x08, 0x10, 0x7F, // N
	0x05, 0x3E, 0x41, 0x41, 0x41, 0x3E, // O
	0x05, 0x7F, 0x09, 0x09, 0x09, 0x06, // P
	0x05, 0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
	0x05, 0x7F, 0x09, 0x19, 0x29, 0x46, // R
	0x05, 0x46, 0x49, 0x49, 0x49, 0x31, // S
	0x05, 0x01, 0x01, 0x7F, 0x01, 0x01, // T
	0x05, 0x3F, 0x40, 0x40, 0x40, 0x3F, // U
	0x05, 0x1F, 0x20, 0x40, 0x20, 0x1F, // V
	0x05, 0x3F, 0x40, 0x38, 0x40, 0x3F, // W
	0x05, 0x63, 0x14, 0x08, 0x14, 0x63, // X
	0x05, 0x07, 0x08, 0x70, 0x08, 0x07, // Y
	0x05, 0x61, 0x51, 0x49, 0x45, 0x43, // Z
	0x05, 0x00, 0x7F, 0x41, 0x41, 0x00, // [
	0x05, 0x55, 0x2A, 0x55, 0x2A, 0x55, // backslash
	0x05, 0x00, 0x41, 0x41, 0x7F, 0x00, // ]
	0x05, 0x04, 0x02, 0x01, 0x02, 0x04, // ^
	0x05, 0x40, 0x40, 0x40, 0x40, 0x40, // _
	0x05, 0x00, 0x01, 0x02, 0x04, 0x00, // '
	0x05, 0x20, 0x54, 0x54, 0x54, 0x78, // a
	0x05, 0x7F, 0x48, 0x44, 0x44, 0x38, // b
	0x05, 0x38, 0x44, 0x44, 0x44, 0x20, // c
	0x05, 0x38, 0x44, 0x44, 0x48, 0x7F, // d
	0x05, 0x38, 0x54, 0x54, 0x54, 0x18, // e
	0x05, 0x08, 0x7E, 0x09, 0x01, 0x02, // f
	0x05, 0x18, 0xA4, 0xA4, 0xA4, 0x7C, // g
	0x05, 0x7F, 0x08, 0x04, 0x04, 0x78, // h
	0x05, 0x00, 0x44, 0x7D, 0x40, 0x00, // i
	0x05, 0x40, 0x80, 0x84, 0x7D, 0x00, // j
	0x05, 0x7F, 0x10, 0x28, 0x44, 0x00, // k
	0x05, 0x00, 0x41, 0x7F, 0x40, 0x00, // l
	0x05, 0x7C, 0x04, 0x18, 0x04, 0x78, // m
	0x05, 0x7C, 0x08, 0x04, 0x04, 0x78, // n
	0x05, 0x38, 0x44, 0x44, 0x44, 0x38, // o
	0x05, 0xFC, 0x24, 0x24, 0x24, 0x18, // p
	0x05, 0x18, 0x24, 0x24, 0x18, 0xFC, // q
	0x05, 0x7C, 0x08, 0x04, 0x04, 0x08, // r
	0x05, 0x48, 0x54, 0x54, 0x54, 0x20, // s
	0x05, 0x04, 0x3F, 0x44, 0x40, 0x20, // t
	0x05, 0x3C, 0x40, 0x40, 0x20, 0x7C, // u
	0x05, 0x1C, 0x20, 0x40, 0x20, 0x1C, // v
	0x05, 0x3C, 0x40, 0x30, 0x40, 0x3C, // w
	0x05, 0x44, 0x28, 0x10, 0x28, 0x44, // x
	0x05, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C, // y
	0x05, 0x44, 0x64, 0x54, 0x4C, 0x44, // z
	0x05, 0x00, 0x08, 0x77, 0x41, 0x00, // {
	0x05, 0x00, 0x00, 0x63, 0x00, 0x00, // Â¦
	0x05, 0x00, 0x41, 0x77, 0x08, 0x00, // }
	0x05, 0x08, 0x04, 0x08, 0x08, 0x04, // ~
};



void ili9163Command(uint8_t cmd) {
//	GPIO_WriteBit(GPIOB, ili9163_CS, Bit_RESET);
  	GPIO_WriteBit(GPIOB, ili9163_A0, Bit_RESET);
	spiWrite(&cmd, 1);
//	GPIO_WriteBit(GPIOB, ili9163_CS, Bit_SET);
}

void ili9163DataN(uint8_t *data, uint16_t len) {
//	GPIO_WriteBit(GPIOB, ili9163_CS, Bit_RESET);
  	GPIO_WriteBit(GPIOB, ili9163_A0, Bit_SET);
	spiWrite(data, len);
//	GPIO_WriteBit(GPIOB, ili9163_CS, Bit_SET);
}

void ili9163Data(uint8_t cmd) {
//	GPIO_WriteBit(GPIOB, ili9163_CS, Bit_RESET);
  	GPIO_WriteBit(GPIOB, ili9163_A0, Bit_SET);
	spiWrite(&cmd, 1);
//	GPIO_WriteBit(GPIOB, ili9163_CS, Bit_SET);
}

void ili9163Window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    ili9163Command(SET_COLUMN_ADDRESS);
    ili9163Data(0x00);
    ili9163Data(x0 + 3);
    ili9163Data(0x00);
    ili9163Data(x1 + 3);
    ili9163Command(SET_PAGE_ADDRESS);
    ili9163Data(0x00);
    ili9163Data(y0 + 2);
    ili9163Data(0x00);
    ili9163Data(y1 + 2);
    ili9163Command(WRITE_MEMORY_START);
}


void ili9163c_draw_pixel(int16_t x, int16_t y, uint16_t color) {
    ili9163Window(x, y, x, y);
	ili9163Data((uint8_t)(color >> 8));
	ili9163Data((uint8_t)(color & 0x00ff));
}

void ili9163RamAddress(void)  {
    ili9163Command(SET_COLUMN_ADDRESS);
    ili9163Data(0x00);
    ili9163Data(0x00 + 3);
    ili9163Data(0x00);
    ili9163Data(0x7f + 3);
    ili9163Command(SET_PAGE_ADDRESS);
    ili9163Data(0x00);
    ili9163Data(0x00 + 2);
    ili9163Data(0x00);
    ili9163Data(0x7f + 2);
}

void ili9163FillColor(uint16_t color)  {
	uint16_t i = 0;
	uint16_t j = 0;
	ili9163RamAddress();
	ili9163Command(WRITE_MEMORY_START);
	for (i = 0; i < 128; i++)  {
		for (j = 0; j < 128; j++)  {
			ili9163Data((uint8_t)(color >> 8));
			ili9163Data((uint8_t)(color & 0x00ff));
		}
	}
}

static const uint8_t ili9163Initseq[] = {
	SOFT_RESET, 0,
	EXIT_SLEEP_MODE, 0,
	SET_PIXEL_FORMAT, 1, 0x05,
	SET_GAMMA_CURVE, 1, 0x04,
	GAM_R_SEL, 1, 0x01,
	ENTER_NORMAL_MODE, 0,
	DISPLAY_FUNCTR, 2, 0b11111111, 0b00000110,
	POSITIVE_GAMMA_CORRECT, 15, 0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00,
	NEGATIVE_GAMMA_CORRECT, 15, 0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29,
    FRAME_RATE_CONTROL1, 2, 0x08, 0x02, 
	DISPLAY_INVERSION, 1, 0x07, 
	POWER_CONTROL1, 2, 0x0a, 0x02, 
	POWER_CONTROL2, 1, 0x02, 
	VCOM_CONTROL1, 2, 0x50, 0x63, 
	VCOM_OFFSET_CONTROL, 0, 
	SET_COLUMN_ADDRESS, 4, 0x00, 0x00, 0x00, 0x7f, 
	SET_PAGE_ADDRESS, 4, 0x00, 0x00, 0x00, 0x7f, 
	SET_SCROLL_AREA, 6, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x00,
	SET_DISPLAY_ON, 0, 
	SET_ADDRESS_MODE, 1, 0b10101000,
	WRITE_MEMORY_START, 0
};

void ili9163Init(void) {
	spiInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ili9163_A0 | ili9163_RST | ili9163_BL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	GPIO_WriteBit(GPIOB, ili9163_A0, Bit_SET);
	GPIO_WriteBit(GPIOB, ili9163_BL, Bit_SET);

	// HW-Reset
	GPIO_WriteBit(GPIOB, ili9163_RST, Bit_SET);
	delay(1000);
	GPIO_WriteBit(GPIOB, ili9163_RST, Bit_RESET);
	delay(5000);
	GPIO_WriteBit(GPIOB, ili9163_RST, Bit_SET);
	delay(5000);

	// init sequence
	uint16_t n = 0;
	uint16_t i = 0;
	for (n = 0; n < sizeof(ili9163Initseq); n++) {
		ili9163Command(ili9163Initseq[n]);
		n++;
		for (i = 0; i < ili9163Initseq[n]; i++) {
			ili9163Data(ili9163Initseq[n + 1 + i]);
		}
		n += i;
	}
	ili9163FillColor(BLACK);
}



void ili9163PutcBig(uint16_t x, uint16_t y, uint16_t color, char c) {
	uint8_t ch = c - 32;
    ili9163Window(x, y, x + 7, y + 15);
	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t i = 0; i < 8; i++) {
			if (ili9163Font8x16[ch * 16 + i] & (1<<j)) {
				ili9163Data((uint8_t)(color >> 8));
				ili9163Data((uint8_t)(color & 0x00ff));
			} else {
				ili9163Data(0x07);
				ili9163Data(0xC0);
			};
		}
	}
	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t i = 0; i < 8; i++) {
			if (ili9163Font8x16[ch * 16 + i + 8] & (1<<j)) {
				ili9163Data((uint8_t)(color >> 8));
				ili9163Data((uint8_t)(color & 0x00ff));
			} else {
				ili9163Data(0x0);
				ili9163Data(0x0);
			};
		}
	}
}


void ili9163PutsBig(uint16_t x, uint16_t y, uint16_t color, char *text) {
	uint16_t px = x;
	uint16_t n = 0;
	for (n = 0; n < strlen(text); n++) {
		ili9163PutcBig(px, y, color, text[n]);
		px += 9;
	}
}


uint16_t ili9163Putc(uint16_t x, uint16_t y, uint8_t scale, uint16_t color, char c) {
	uint16_t len = ili9163Lenc(scale, c);
	uint8_t ch = c - 32;
    ili9163Window(x, y, x + (ili9163Font5x8[ch * 6] * scale) + scale - 1, y + (8 * scale) - 1);
	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t sy = 0; sy < scale; sy++) {
			for (uint8_t i = 1; i < ili9163Font5x8[ch * 6] + 1; i++) {
				if (ili9163Font5x8[ch * 6 + i] & (1<<j)) {
					for (uint8_t sx = 0; sx < scale; sx++) {
						ili9163Data((uint8_t)(color >> 8));
						ili9163Data((uint8_t)(color & 0x00ff));
					}
				} else {
					for (uint8_t sx = 0; sx < scale; sx++) {
						ili9163Data(0x0);
						ili9163Data(0x0);
					}
				}
			}
			// Fill spaces
			for (uint8_t sx = 0; sx < scale; sx++) {
				ili9163Data(0x0);
				ili9163Data(0x0);
			}

		}
	}
	return len;
}


uint16_t ili9163Puts(uint16_t x, uint16_t y, uint8_t scale, uint16_t color, char *text) {
	uint16_t px = x;
	uint16_t n = 0;
	for (n = 0; n < strlen(text); n++) {
		uint16_t len = ili9163Putc(px, y, scale, color, text[n]);
		px += len + scale;
	}
	return px;
}

uint16_t ili9163Lenc(uint8_t scale, char c) {
	uint8_t ch = c - 32;
	uint16_t len = ili9163Font5x8[ch * 6];
	return len * scale;
}


uint16_t ili9163Lens(uint8_t scale, char *text) {
	uint16_t len = 0;
	uint16_t n = 0;
	for (n = 0; n < strlen(text); n++) {
		len += ili9163Lenc(scale, text[n]) + scale;
	}
	return len;
}


void ili9163Line(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color) {
	int16_t CurrentX, CurrentY, Xinc, Yinc, Dx, Dy, TwoDx, TwoDy, TwoDxAccumulatedError, TwoDyAccumulatedError;
	Dx = (X2-X1);
	Dy = (Y2-Y1);
	TwoDx = Dx + Dx;
	TwoDy = Dy + Dy;
	CurrentX = X1;
	CurrentY = Y1;
	Xinc = 1;
	Yinc = 1;
	if (Dx < 0) {
		Xinc = -1;
		Dx = -Dx;
		TwoDx = -TwoDx;
	}
	if (Dy < 0) {
		Yinc = -1;
		Dy = -Dy;
		TwoDy = -TwoDy;
	}
	ili9163c_draw_pixel(X1, Y1, color);
	if ((Dx != 0) || (Dy != 0)) {
		if (Dy <= Dx) { 
			TwoDxAccumulatedError = 0;
			do {
				CurrentX += Xinc;
				TwoDxAccumulatedError += TwoDy;
				if(TwoDxAccumulatedError > Dx) {
					CurrentY += Yinc;
					TwoDxAccumulatedError -= TwoDx;
				}
				ili9163c_draw_pixel(CurrentX,CurrentY, color);
			} while (CurrentX != X2);
		} else {
			TwoDyAccumulatedError = 0; 
			do {
				CurrentY += Yinc; 
				TwoDyAccumulatedError += TwoDx;
				if(TwoDyAccumulatedError>Dy) {
					CurrentX += Xinc;
					TwoDyAccumulatedError -= TwoDy;
				}
				ili9163c_draw_pixel(CurrentX,CurrentY, color); 
			} while (CurrentY != Y2);
		}
	}
}

void ili9163Rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t filled, uint8_t color) {
	uint8_t x = 0;
	uint8_t y = 0;
	if (filled == 1) {
		uint8_t x = 0;
		uint8_t y = 0;
		for (y = y1; y <= y2; y++) {
			for (x = x1; x <= x2; x++) {
				ili9163c_draw_pixel(x, y, color);
			}
		}
	} else {
		for (y = y1; y <= y2; y++) {
			ili9163c_draw_pixel(x1, y, color);
			ili9163c_draw_pixel(x2, y, color);
		}
		for (x = x1; x <= x2; x++) {
			ili9163c_draw_pixel(x, y1, color);
			ili9163c_draw_pixel(x, y2, color);
		}
	}
}

