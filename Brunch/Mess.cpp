/*
 Mess.cpp
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

#include "Mess.h"
#include "Prot.h"

#include "Arduino.h"

#include "Debug.h"

//--------------------------------------------------------
//---- Fonctions de l'application appellée par la distribution des messages
int brunch_alarm(bool onOff = -1, int hours = -1, int minutes = -1);
int brunch_time(int hours = -1, int minutes = -1);
int brunch_energyLight(bool onOff, int intensity = -1, int duration = -1);
int brunch_setting(char *name, char * value = NULL);
int brunch_reset();

//--------------------------------------------------------
//---- Variables globales de la tâche

// Tableau pour recevoir les arguments des messages entrant
int mess_Args[MESS_ARGS_MAX];

// Nombre d'arguments trouvés
int mess_NbArgs;

// Buffer de préparation de message
//  on ajoute 1 pour le caractère '\0' terminal d'une chaîne
char mess_BufferSend[PROT_MAXLEN+1];

//--------------------------------------------------------
// ---- Déclaration des fonctions situées plus loin dans le fichier

int mess_recupArgs( char *pMess );

//--------------------------------------------------------
// ---- Fonctions

void mess_setup()
{
}


void mess_dispatch()
{
  // Obtenir le pointeur sur le message
	char *pMess = prot_getMessage();
	DEBUG( "! received: " );
	DEBUGLN( pMess ); 

	// Découper le message pour récupérer chacun des arguments  
	mess_recupArgs( pMess );

	// En fonction de l'ID de message (1er caractère)
	switch( pMess[0] )
	{
		//--------------------------------------------
		case 'a': // $M<mot>,<dir>,<pas>,<vit>
			if(mess_NbArgs == 0)
				brunch_alarm();
			else if(mess_NbArgs == 1)
				brunch_alarm(mess_Args[0]);
			else if(mess_NbArgs == 3)
				brunch_alarm(mess_Args[0], mess_Args[1], mess_Args[2]);            
			else
			mess_send1arg( 'a', /* errCode =  */ -1 );
			// Passer la commande à l'application
			break;
	}
	// On libère le buffer de reception et on se prépare à recevoir un nouveau message
	prot_clearMessage();
}


// Analyse le message entrant pour en extraire les arguments
int mess_recupArgs( char *pMess )
{
  char *p;
  int nb = 0;
  // On ignore le 1er caractère qui est l'ID du message
  ++pMess;
  
  // On démarre le découpage du message
  p = strtok( pMess, PROT_SEP_S );
  while ( p )
  {
	mess_Args[nb++] = atoi( p );
	p = strtok( NULL, PROT_SEP_S );
  }
  
  return mess_NbArgs = nb;
}


void mess_sendShort( char mess_ID )
{
  // Composition du message
  mess_BufferSend[0] = mess_ID;
  mess_BufferSend[1] = '\0';
  
  // envoi du message
  prot_send( mess_BufferSend );
}

void mess_send1arg( char mess_ID, int arg1 )
{
  // Composition du message
  mess_BufferSend[0] = mess_ID;
  // Ajout du 1er argument
  itoa( arg1, &mess_BufferSend[1], 10 );

  // envoi du message
  prot_send( mess_BufferSend );
}

void mess_send2arg( char mess_ID, int arg1, int arg2 )
{
  // Composition du message
  mess_BufferSend[0] = mess_ID;
  // Ajout du 1er argument
  itoa( arg1, &mess_BufferSend[1], 10 );
  // Ajout du 2eme argument
  char *p = mess_BufferSend + strlen( mess_BufferSend );
  p[0] =  PROT_SEP;
  itoa( arg2, &p[1], 10 );
  
  // envoi du message
  prot_send( mess_BufferSend );
}

void mess_send3arg( char mess_ID, int arg1, int arg2, int arg3 )
{
  // Composition du message
  mess_BufferSend[0] = mess_ID;
  // Ajout du 1er argument
  itoa( arg1, &mess_BufferSend[1], 10 );
  // Ajout du 2eme argument
  char *p = mess_BufferSend + strlen( mess_BufferSend );
  p[0] =  PROT_SEP;
  itoa( arg2, &p[1], 10 );
  // Ajout du 3eme argument
  p = mess_BufferSend + strlen( mess_BufferSend );
  p[0] =  PROT_SEP;
  itoa( arg3, &p[1], 10 );
	
  // envoi du message
  prot_send( mess_BufferSend );
}

void mess_send4arg( char mess_ID, int arg1, int arg2, int arg3, int arg4 )
{
  // Composition du message
  mess_BufferSend[0] = mess_ID;
  // Ajout du 1er argument
  itoa( arg1, &mess_BufferSend[1], 10 );
  // Ajout du 2eme argument
  char *p = mess_BufferSend + strlen( mess_BufferSend );
  p[0] =  PROT_SEP;
  itoa( arg2, &p[1], 10 );
  // Ajout du 3eme argument
  p = mess_BufferSend + strlen( mess_BufferSend );
  p[0] =  PROT_SEP;
  itoa( arg3, &p[1], 10 );
  // Ajout du 4eme argument
  p = mess_BufferSend + strlen( mess_BufferSend );
  p[0] =  PROT_SEP;
  itoa( arg4, &p[1], 10 );
	
  // envoi du message
  prot_send( mess_BufferSend );
}

