#include "defines.h"

#include <avr/eeprom.h>

#define EEPROM_SIGNATURE 0xA5;

struct SEeprom
{
	uint16_t m_outputFreqX10;
	uint8_t m_sinusAmplitude;
	uint8_t m_controlByte;
	
	void FromData()
	{
		m_outputFreqX10 = g_outputFreqX10;
		m_sinusAmplitude = g_sinusAmplitude;
		m_controlByte = EEPROM_SIGNATURE;
	}
	
	void ToData()
	{
		g_outputFreqX10 = m_outputFreqX10;
		g_sinusAmplitude = m_sinusAmplitude;
	}
	
	bool NeedUpdate()
	{
		return g_outputFreqX10 != m_outputFreqX10 ||
			g_sinusAmplitude != m_sinusAmplitude;
	}
	
	bool IsValid()
	{
		return m_controlByte == EEPROM_SIGNATURE;
	}
	
	void FromEeprom()
	{
		eeprom_read_block(this, (void*)0, sizeof(SEeprom));
	}
	
	void ToEeprom()
	{
		eeprom_write_block(this, (void*)0, sizeof(SEeprom));
	}
};

bool ReadFromEeprom()
{
	SEeprom eeprom;
	eeprom.FromEeprom();
	if (!eeprom.IsValid())
		return false;
		
	eeprom.ToData();
	return true;
}

bool SaveToEeprom()
{
	SEeprom eeprom;
	eeprom.FromEeprom();
	if (eeprom.IsValid() && !eeprom.NeedUpdate())
		return false;
		
	eeprom.FromData();
	eeprom.ToEeprom();
	return true;
}

