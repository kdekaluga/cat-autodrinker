#include <avr/io.h>

#include "constants.h"

/*
In C++ this code would look like:

uint8_t GetSinus(int arg)
{
	if (arg < 256)
		return 127 + pgm_read_byte(g_sinus + arg)*g_sinusAmplitude/128;
	else
		return 127 - pgm_read_byte(g_sinus + arg - 256)*g_sinusAmplitude/128;
}

ISR(TIM0_OVF_vect)
{
	PORTB = BV(3);
	// Generate waveform. Target frequency = 62500/512*(advance/65536) =
	// = 62500*advance/33554432 = 15625*advance/8388608 = advance/536.870912
	g_sinusPosition += g_sinusPositionAdvance;
	OCR0A = GetSinus((g_sinusPosition >> 16) & 0x1FF);
	
	if (++g_timer0Counter == 625)
	{
		g_timer0Counter = 0;

		// Enable interrupts and call the 100 Hz routine
		sei();
		Timer100Hz();
	}
	PORTB = 0;
}
*/

.global TIM0_OVF_vect
TIM0_OVF_vect:

	push	R18
	in		R18, _SFR_IO_ADDR(SREG)
	push	R18

	//ldi		R18, (1 << 3)
	//out		_SFR_IO_ADDR(PORTB), R18

	push	R19
	push	R20
	push	ZL
	push	ZH

	; Normally the compiler uses R1 as a zero register, but we cannot
	; be sure of it in the ISR (what if it interrupted some code
	; that temporary uses R1 for some other purpose?). Thus we
	; clear R20 here
	clr		R20

	; Reset watch dog here
	wdr
	

	// *** PWM ***
	lds		R18, (g_sinusPosition)
	lds		R19, (g_sinusPosition + 1)
	lds		ZL, (g_sinusPositionAdvance)
	lds		ZH, (g_sinusPositionAdvance + 1)
	add		R18, ZL
	adc		R19, ZH
	sts		(g_sinusPosition), R18
	sts		(g_sinusPosition + 1), R19
	lds		ZL, (g_sinusPosition + 2)
	lds		ZH, (g_sinusPosition + 3)
	adc		ZL, R20
	adc		ZH, R20
	sts		(g_sinusPosition + 2), ZL
	sts		(g_sinusPosition + 3), ZH
	// Z = (g_sinusPosition >> 16)

	ldi		R18, lo8(g_sinus)
	add		ZL, R18
	ldi		ZH, hi8(g_sinus)
	adc		ZH, R20
	lpm		R18, Z
	// R18 = g_sinus[(g_sinusPosition >> 16) & 0xFF]

	clr		R19
	lds		R20, (g_sinusAmplitude)
	clr		ZL
	clr		ZH

.macro mul_step
	lsr		R20
	brcc	.+4

	add		ZL, R18
	adc		ZH, R19

	lsl		R18
	rol		R19
.endm

	mul_step
	mul_step
	mul_step
	mul_step
	mul_step
	mul_step
	mul_step
	mul_step
	// Z = g_sinus[]*g_sinusAmplitude
	
	lsl		ZL
	rol		ZH
	// ZH = g_sinus[]*g_sinusAmplitude/128

	lds		ZL, (g_sinusPosition + 3)
	sbrc	ZL, 0
	neg		ZH

	ldi		ZL, 127
	add		ZL, ZH
	out		_SFR_IO_ADDR(OCR0A), ZL


	// *** Encoder ***
	// Can we check the encoder right now?
	// If PORTB encoder clock pin is in the output mode, we
	// should skip encoder check
	in		R18, _SFR_IO_ADDR(DDRB)
	sbrc	R18, PIN_ENCCLK
	rjmp	.skip_encoder_check

	in		R18, _SFR_IO_ADDR(PINB)
	andi	R18, BV(PIN_ENCDT) | BV(PIN_ENCCLK) | BV(PIN_ENCSW)
	
	// Encoder state changes?
	lds		R20, (g_encoderState)
	eor		R20, R18
	breq	.skip_encoder_check

	sts		(g_encoderState), R18
	sbrs	R20, PIN_ENCSW
	rjmp	.check_encoder_clock
	sbrc	R18, PIN_ENCSW
	rjmp	.check_encoder_clock

	ldi		R20, KEY_SWITCH
	rjmp	.encoder_new_key

.check_encoder_clock:
	and		R20, R18
	sbrs	R20, PIN_ENCCLK
	rjmp	.skip_encoder_check

	ldi		R20, KEY_UP
	sbrc	R18, PIN_ENCDT
	ldi		R20, KEY_DOWN

.encoder_new_key:
	sts		(g_lastKeyDown), R20

.skip_encoder_check:


	// *** 100 Hz timer ***
	lds		ZL, (g_timer0Counter)
	lds		ZH, (g_timer0Counter + 1)
	adiw	Z, 1
	cpi		ZL, lo8(625)
	ldi		R18, hi8(625)
	cpc		ZH, R18
	brne	.no_100Hz_inc

	lds		ZL, (g_100HzCounter)
	inc		ZL
	sts		(g_100HzCounter), ZL

	clr		ZL
	clr		ZH

.no_100Hz_inc:
	sts		(g_timer0Counter), ZL
	sts		(g_timer0Counter + 1), ZH


	pop		ZH
	pop		ZL
	pop		R20
	pop		R19

	//out		_SFR_IO_ADDR(PORTB), R1

	pop		R18
	out		_SFR_IO_ADDR(SREG), R18
	pop		R18
	
	reti

