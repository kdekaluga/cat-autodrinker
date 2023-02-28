#include "defines.h"
#include "led_display.h"

typedef void (*PFun)();

uint8_t g_displayModeNumber = 0;
uint8_t g_idleCounter = 0;
bool g_mode3Flag = false;
uint8_t g_mode3Ampl = 0;


struct SDispMode
{
	uint8_t m_nextMode;
	PFun m_displayFunction;
	PFun m_upFunction;
	PFun m_downFunction;
	PFun m_idleFunction;
	
	void OnDisplay() const
	{
		CallFunc(&m_displayFunction);
	}

	void OnUp() const
	{
		CallFunc(&m_upFunction);
	}
	
	void OnDown() const
	{
		CallFunc(&m_downFunction);
	}
	
	void OnIdle() const
	{
		CallFunc(&m_idleFunction);
	}
	
	uint8_t GetNextMode() const
	{
		return pgm_read_byte(&m_nextMode);
	}

	static inline void CallFunc(const PFun *member)
	{
		PFun func = (PFun)pgm_read_ptr(member);
		if (func)
			func();
	}
};

// *** Mode 0 ***
// Display frequency and amplitude

void Mode0Display()
{
	PrintFreq();
	PrintAmpl();
}

void Mode0Idle()
{
	SaveSettings();
	g_displayModeNumber = 4;
}

// *** Mode 1 ***
// Edit frequency

void Mode1Display()
{
	if (!(g_idleCounter & 1))
		PrintFreq();

	PrintAmpl();
}

void Mode1Up()
{
	if (g_outputFreqX10 < 990)
		++g_outputFreqX10;

	SetOutputFrequency();
}

void Mode1Down()
{
	if (g_outputFreqX10 > 100)
		--g_outputFreqX10;

	SetOutputFrequency();
}

void Mode1Idle()
{
	g_displayModeNumber = 0;
}

// *** Mode 2 ***
// Edit amplitude

void Mode2Display()
{
	PrintFreq();

	if (!(g_idleCounter & 1))
		PrintAmpl();	
}

void Mode2Up()
{
	if (g_sinusAmplitude < 128)
		++g_sinusAmplitude;	
}

void Mode2Down()
{
	if (g_sinusAmplitude > 0)
		--g_sinusAmplitude;
}

// *** Mode 3 ***
// On/off

void Mode3Display()
{
	PrintAmpl();

	if (g_idleCounter & 1)
		return;

	g_screen[0] = LED_O;
	if (g_mode3Flag)
	{
		g_screen[1] = LED_F;
		g_screen[2] = LED_F;

	} else {
		g_mode3Ampl = g_sinusAmplitude;
		g_screen[1] = LED_N;
	}
}

void Mode3UpDown()
{
	g_mode3Flag = !g_mode3Flag;
	g_sinusAmplitude = g_mode3Flag ? 0 : g_mode3Ampl;

	/*
	if (g_mode3Flag)
	{
		g_sinusAmplitude = 0;
		return;
	}

	uint16_t outputFrq = g_outputFreqX10;
	g_sinusAmplitude = 128;
	g_outputFreqX10 = 500;

	while (g_outputFreqX10 > outputFrq)
	{
		--g_outputFreqX10;
		SetOutputFrequency();
		SleepAndWaitForAKey(2);
	}

	g_sinusAmplitude = g_mode3Ampl;
	*/
}

void Mode3Idle()
{
	g_displayModeNumber = 0;
	g_mode3Flag = false;
	g_sinusAmplitude = g_mode3Ampl;
}

// *** Mode 4 ***
// Blank screen
void Mode4UpDown()
{
	g_displayModeNumber = 0;
}

// Display modes
PROGMEM const SDispMode g_modes[] = 
{
	{1, &Mode0Display, nullptr, nullptr, &Mode0Idle},
	{2, &Mode1Display, &Mode1Up, &Mode1Down, &Mode1Idle},
	{3, &Mode2Display, &Mode2Up, &Mode2Down, &Mode1Idle},
	{0, &Mode3Display, &Mode3UpDown, &Mode3UpDown, &Mode3Idle},
	{0, nullptr, &Mode4UpDown, &Mode4UpDown, nullptr},

};

void PrintFreq()
{
	PrintDec(g_outputFreqX10, 2);
	g_screen[1] |= 0x80;
	g_screen[3] = 0b00110111;
}

void PrintAmpl()
{
	PrintDec(g_sinusAmplitude, 7);
}

void DisplayMain()
{
	// Current display mode description
	const SDispMode *mode = &g_modes[g_displayModeNumber];

	// Display
	ClearScreen();
	mode->OnDisplay();
	MaxCopyScreen();
	
	uint8_t key = SleepAndWaitForAKey(32);
	if (key == KEY_NONE)
	{
		if (++g_idleCounter == 30)
		{
			g_idleCounter = 0;
			mode->OnIdle();
		}

		return;
	}
	
	g_idleCounter = 0;
	
	if (key == KEY_SWITCH)
		g_displayModeNumber = mode->GetNextMode();	
	
	else if (key == KEY_UP)
		mode->OnUp();

	else if (key == KEY_DOWN)
		mode->OnDown();
}

