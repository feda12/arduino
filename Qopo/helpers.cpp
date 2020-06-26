/*
	helpers.cpp
	C++ source file for Qopo Morning Shower device
	Ben Le Cam - Qopo Inc. - qopo.com
	June 2014
	All rights reserved.
 */

#include "helpers.h"

// Helper to convert hexadecimal character to decimal byte
byte hexchar2dec(char c)
{
	byte x = 0;
	if ( (c >= '0') && (c <= '9') )
		x = c & 0x0F;
	else if  ( (c >= 'A') && (c <= 'F') )
		x = (c & 0x07) + 9;
	return x;
}
