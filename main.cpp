#include "defines.h"

// Timer ISR variables
int g_timer0Counter = 0;
uint32_t g_sinusPosition = 0;
volatile uint16_t g_sinusPositionAdvance = 0;

// 100 Hz counter. Is incremented by timer ISR
volatile uint8_t g_100HzCounter = 0;

// Output sinus amplitude [0..128]
uint8_t g_sinusAmplitude = 0;

// Output frequency x10
uint16_t g_outputFreqX10 = 0;

// Current encoder state
uint8_t g_encoderState = 0;

// Last key that was down
volatile uint8_t g_lastKeyDown = KEY_NONE;


void SetOutputFrequency()
{
	// advance = freq*8388608/15625 = freqX100*8388608/1562500
	// advance = freqX100*2097152/390625 ~ freqX100*419430/78125
	// advance = freqX10*4194304/78125
	uint16_t newAdvance = (uint32_t)g_outputFreqX10*4194304/78125;

	uint8_t sreg = SREG;
	cli();
	g_sinusPositionAdvance = newAdvance;
	SREG = sreg;
}

void Setup()
{
	// Setup watch dog to 1 second
	WDTCR = BV(WDE) | BV(WDP2) | BV(WDP1);

	if (!ReadFromEeprom())
	{
		g_outputFreqX10 = 500;
		g_sinusAmplitude = 128;
	}

	SetOutputFrequency();
	
	// By default all encoder pins are high
	g_encoderState = BV(PIN_ENCDT) | BV(PIN_ENCCLK) | BV(PIN_ENCSW);
	
	// Adjust MCU frequency
	OSCCAL = 85;
	
	// Output pins. Only PIN_OC1A and PIN_MAXCS pins are set up
	// always as output. The other MAX pins are used both
	// for output and input, so they are switched between these modes
	// as needed.
	PORTB = 0;
	DDRB = BV(PIN_OC1A) | BV(PIN_MAXCS);
	
	// Setup timer0 to output fast PWM to the OC1A pin (PB0).
	// Timer clock frequency is 16 MHz, timer interrupt frequency is 62.5 KHz
	OCR0A = 127;
	TCCR0A = BV(COM0A1) | BV (WGM01) | BV (WGM00);
	TCCR0B = BV(CS00);
	TIMSK = BV(TOIE0);
	sei();
}

uint8_t SleepAndWaitForAKey(uint8_t sleepTicks)
{
	uint8_t tickToReturn = g_100HzCounter + sleepTicks;
	for (;;)
	{
		uint8_t key = g_lastKeyDown;
		if (key != KEY_NONE)
		{
			g_lastKeyDown = KEY_NONE;
			return key;
		}
		
		if (g_100HzCounter == tickToReturn)
			return KEY_NONE;

		__asm __volatile__ ("sleep");
	}	
}

void SaveSettings()
{
	if (!SaveToEeprom())
		return;
		
	ClearScreen();
	g_screen[0] = 0b01011011;
	g_screen[1] = 0b01110111;
	g_screen[2] = 0b00111110;
	g_screen[3] = 0b01001111;
	g_screen[4] = 0b00111101;
	MaxCopyScreen();

	SleepAndWaitForAKey(100);
}

int main()
{
	Setup();
	
	for (;;)
	{
		DisplayMain();
	}
}


