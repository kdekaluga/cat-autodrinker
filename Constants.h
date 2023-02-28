#pragma once

#define BV(n) (1 << n)

// Pins description:
// PB0: OC1A, main PWM output
// PB1: MAX7219 Din
//		Encoder DT
// PB2: MAX7219 CLK
//		Encoder CLK
// PB3: Encoder switch
// PB4: MAX7219 Cs

#define PIN_OC1A 0

#define PIN_MAXDIN 1
#define PIN_MAXCLK 2
#define PIN_MAXCS 4

#define PIN_ENCDT 1
#define PIN_ENCCLK 2
#define PIN_ENCSW 3

// Keys
#define KEY_NONE 0
#define KEY_SWITCH 1
#define KEY_UP 2
#define KEY_DOWN 3

