/*
 Prot.h
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
 
#ifndef _PROT_H_
#define _PROT_H_

#include "Arduino.h"

// Longueur max d'un message, compris entête, checksum, ...
#define PROT_MAXLEN    (40)

// Marqueurs de début, separateurs, ..
#define PROT_SYNC     '$'
#define PROT_SEP      ','
#define PROT_SEP_S    ","
#define PROT_CHKSEP   '*'
#define PROT_EOM1     '\n'
#define PROT_EOM2     '\r'

// Timeout de réception d'un message (1 seconde)
//  L'intégralité d'un message doit être reçu dans cet intervalle de temps
#define PROT_RECEIVE_TMOUT (1000)

// Timeout de perte de communication (defaut: 30 secondes
//  On doit recevoir quelque chose au moins 1 fois par intervalle de temps
//  avant de déclarer une perte de connexion
#define PROT_LOSTCOM_TIMEOUT  (30000)

// Déclaration des fonctions exportées du module Prot

// Fonctions d'initialisation et de boucle
extern void prot_setup( class Stream *pStream );
extern void prot_loop();

// Fonctions pour traiter le message entrant
bool prot_isMessageAvailable();
char *prot_getMessage();
void prot_clearMessage();

// Fonctions pour emettre un message
void prot_send( char *pMess );

#endif // _PROT_H_

