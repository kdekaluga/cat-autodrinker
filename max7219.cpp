#include "defines.h"

#include <string.h>

#define MAX7219_DECODE 0x900
#define MAX7219_INTENSITY 0xA00
#define MAX7219_SCANLIMIT 0xB00
#define MAX7219_SHUTDOWN 0xC00
#define MAX7219_DISPTEST 0xF00

// Screen
uint8_t g_screen[8];

PROGMEM const uint8_t g_digitMap[] = 
{
	0b01111110, 0b00110000, 0b01101101, 0b01111001,
	0b00110011, 0b01011011, 0b01011111, 0b01110010,
	0b01111111, 0b01111011, 0b01110111, 0b00011111,
	0b01001110, 0b00111101, 0b01001111, 0b01000111,
};

void MaxWriteWord(uint16_t word)
{
	uint8_t portVal = PORTB & ~(BV(PIN_MAXDIN) | BV(PIN_MAXCLK) | BV(PIN_MAXCS));
	uint16_t bitVal = 0x8000;
	for (uint8_t i = 0; i < 16; ++i, bitVal >>= 1)
	{
		uint8_t newVal = portVal | ((word & bitVal) ? BV(PIN_MAXDIN) : 0);
		PORTB = newVal;
		PORTB = newVal | BV(PIN_MAXCLK);
	}
	
	// Load data to MAX
	PORTB = portVal | BV(PIN_MAXCS);
}

void MaxCopyScreen()
{
	// Switch pins to output
	DDRB |= BV(PIN_MAXDIN) | BV(PIN_MAXCLK);

	// Init MAX
	MaxWriteWord(MAX7219_DECODE | 0x00);
	MaxWriteWord(MAX7219_INTENSITY | 0x01);
	MaxWriteWord(MAX7219_SCANLIMIT | 0x07);
	MaxWriteWord(MAX7219_SHUTDOWN | 0x01);
	MaxWriteWord(MAX7219_DISPTEST | 0x00);
	
	// Copy screen
	for (uint8_t i = 0; i < 8; ++i)
	{
		MaxWriteWord((((uint16_t)i + 1) << 8) | g_screen[7 - i]);
	}

	// Switch pins back to input
	DDRB &= ~(BV(PIN_MAXDIN) | BV(PIN_MAXCLK));
}

void ClearScreen()
{
	memset(g_screen, 0, sizeof(g_screen));
}

void PrintDec(uint16_t number, uint8_t screenEndPos)
{
	do
	{
		g_screen[screenEndPos--] = pgm_read_byte(g_digitMap + number % 10);
		number /= 10;
	
	} while(number);
}
