@echo off
rem clock source is PLL (CKSEL = 0001)
rem CKDIV8 is unprogrammed (no division)
rem startup time = Slowly rising power (SUT = 10)
rem EEPROM is preserved during chip erase
rem BOD level is 4.3 V
rem Watch dog is enabled
C:\Programming\AvrDude\avrdude.exe -c usbasp -p t85 -B 100kHz -U hfuse:w:0xC4:m -U lfuse:w:0xD1:m
