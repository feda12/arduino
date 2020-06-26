/*
	debug.h
	C++ header file for Qopo Morning Shower device
	Based on Jean-Michel Mercier, 'Implementer un protocole de communication pour Arduino'
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved.
*/


#ifndef _DEBUG_H

#define __DEBUG

#ifdef __DEBUG
	#define DEBUG(a)    Serial.print( a )
	#define DEBUGLN(a)  Serial.println( a )
#else
	#define DEBUG(a)    (0)
	#define DEBUGLN(a)  (0)
#endif

#define DEBUGSTR(a) DEBUG(a.c_str())
#define DEBUGSTRLN(a) DEBUGLN(a.c_str())

#endif
