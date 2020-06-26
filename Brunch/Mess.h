/*
 Mess.h
 Code d'exemple associé au tutorial "Implémenter un protocole de communication"
 
 Copyright (c) 2012 Jean-Michel Mercier (aka Barbudor).  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#ifndef _MESS_H_
#define _MESS_H_

#include "Arduino.h"

// Nombre max d'arguments numériques par message
#define MESS_ARGS_MAX   (5)

// Déclaration des fonctions exportées du module Prot

// Fonctions d'initialisation et de boucle
extern void mess_setup();
extern void mess_dispatch();

// Fonctions pour emettre un message
void mess_sendShort( char mess_ID );
void mess_send1arg( char mess_ID, int arg1 );
void mess_send2arg( char mess_ID, int arg1, int arg2 );
void mess_send3arg( char mess_ID, int arg1, int arg2, int arg3 );
void mess_send4arg( char mess_ID, int arg1, int arg2, int arg3, int arg4 );


#endif // _MESS_H_

