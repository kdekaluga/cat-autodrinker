#pragma once

#include "constants.h"

#include <avr/io.h>
#include <avr/common.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//#include <stdlib.h>

#define _countof(a) (sizeof(a)/sizeof(a[0]))

extern PROGMEM const uint8_t g_sinus[];
extern uint8_t g_screen[8];
extern uint8_t g_sinusAmplitude;
extern uint16_t g_outputFreqX10;

// Main
uint8_t SleepAndWaitForAKey(uint8_t sleepTicks);
void SetOutputFrequency();
void SaveSettings();

// MAX7219
void ClearScreen();
void MaxCopyScreen();
void PrintDec(uint16_t number, uint8_t screenEndPos);

// EEPROM
bool ReadFromEeprom();
bool SaveToEeprom();

// Display
void PrintFreq();
void PrintAmpl();
void DisplayMain();


/*
void* operator new(size_t objsize)
{
	return malloc(objsize);
}

void* operator new[](size_t objsize)
{
	return malloc(objsize);
}
*/

