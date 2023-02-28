use strict;
use Math::Trig;

open F, (">sinus.cpp");

print F <<END
#include "defines.h"

// 256-byte 1/2 sinus table
PROGMEM const uint8_t g_sinus[] =
END
;
print F "{";

my $step = pi/(256*1);
foreach my $n (0..255)
{
	my $val = int(sin($step*$n)*127 + 0.5);
	
	if (!($n & 0x0F))
	{
		print F "\n\t";
	}

	print F sprintf("0x%02X, ", $val);
}

print F "\n};";

close F;
