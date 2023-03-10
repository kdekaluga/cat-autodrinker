#include "defines.h"

// 256-byte 1/2 sinus table
PROGMEM const uint8_t g_sinus[] =
{
	0x00, 0x02, 0x03, 0x05, 0x06, 0x08, 0x09, 0x0B, 0x0C, 0x0E, 0x10, 0x11, 0x13, 0x14, 0x16, 0x17, 
	0x19, 0x1A, 0x1C, 0x1D, 0x1F, 0x20, 0x22, 0x23, 0x25, 0x26, 0x28, 0x29, 0x2B, 0x2C, 0x2E, 0x2F, 
	0x31, 0x32, 0x33, 0x35, 0x36, 0x38, 0x39, 0x3A, 0x3C, 0x3D, 0x3F, 0x40, 0x41, 0x43, 0x44, 0x45, 
	0x47, 0x48, 0x49, 0x4A, 0x4C, 0x4D, 0x4E, 0x4F, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x58, 0x59, 
	0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
	0x6A, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x6F, 0x70, 0x71, 0x71, 0x72, 0x73, 0x73, 0x74, 0x75, 
	0x75, 0x76, 0x76, 0x77, 0x78, 0x78, 0x79, 0x79, 0x7A, 0x7A, 0x7A, 0x7B, 0x7B, 0x7C, 0x7C, 0x7C, 
	0x7D, 0x7D, 0x7D, 0x7D, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
	0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7D, 0x7D, 0x7D, 
	0x7D, 0x7C, 0x7C, 0x7C, 0x7B, 0x7B, 0x7A, 0x7A, 0x7A, 0x79, 0x79, 0x78, 0x78, 0x77, 0x76, 0x76, 
	0x75, 0x75, 0x74, 0x73, 0x73, 0x72, 0x71, 0x71, 0x70, 0x6F, 0x6F, 0x6E, 0x6D, 0x6C, 0x6B, 0x6A, 
	0x6A, 0x69, 0x68, 0x67, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61, 0x60, 0x5F, 0x5E, 0x5D, 0x5C, 0x5B, 
	0x5A, 0x59, 0x58, 0x56, 0x55, 0x54, 0x53, 0x52, 0x51, 0x4F, 0x4E, 0x4D, 0x4C, 0x4A, 0x49, 0x48, 
	0x47, 0x45, 0x44, 0x43, 0x41, 0x40, 0x3F, 0x3D, 0x3C, 0x3A, 0x39, 0x38, 0x36, 0x35, 0x33, 0x32, 
	0x31, 0x2F, 0x2E, 0x2C, 0x2B, 0x29, 0x28, 0x26, 0x25, 0x23, 0x22, 0x20, 0x1F, 0x1D, 0x1C, 0x1A, 
	0x19, 0x17, 0x16, 0x14, 0x13, 0x11, 0x10, 0x0E, 0x0C, 0x0B, 0x09, 0x08, 0x06, 0x05, 0x03, 0x02, 
};