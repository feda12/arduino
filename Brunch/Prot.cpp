/*
 Prot.cpp
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
 
#include "Prot.h"

#include "Arduino.h"

#include "Debug.h"

//--------------------------------------------------------
//---- Variables globales de la tâche

// Classe de type Stream qui sera utilisée pour la comm
class Stream *prot_pStream = NULL;

// Buffer de réception des messages
//  on ajoute 1 pour le caractère '\0' terminal d'une chaîne
char prot_BufferRec[PROT_MAXLEN+1];

// Index dans le buffer
int prot_IndexRec;

// Calcul du checksum du message en cours de reception
byte prot_ChecksumCalc;

// Checksum reçu dans le message
byte prot_Checksum;
bool prot_ChecksumReceived;

// Etats de la tâche de réception des messages
enum prot_STATE_E {
  WAIT_SYNC,    // attente du caractère de synchronisation
  RECEIVING,    // reception de message en cours
  CHECKSUM,     // reception du checksum
  RECEIVED,     // message reçu disponible pour traitement, on ne peut pas recevoir de nouveau
                // message tant que celui-ci n'a pas été acquitté
  LOST_COMM     // on a perdu la connexion avec le maitre
} prot_State;

// Memorise la date du dernier caractère reçu pour le test des timeouts
unsigned long prot_TimeLastReceived;

//--------------------------------------------------------
// ---- Déclaration des fonctions définies plus loin dans le fichier

void prot_reset();
void prot_lostCommTimeout();
byte hexchar2dec( char c );
byte prot_calcChecksum( char *p );

//--------------------------------------------------------
// ---- Fonctions

// Fonction d'initialisation a apeller 1 fois depuis setup();
void prot_setup( class Stream *pStream )
{
  // Mémoriser le pointeur vers le Stream
  prot_pStream = pStream;
  // initialisation du timestamp de dernière réception
  prot_TimeLastReceived = millis();
  // initialisation
  prot_reset();
}

// Fonction de traitement de fond, réception des caractères
void prot_loop()
{
  // variable pour recevoir le prochain caractère reçu
  char c;
  // indicateur pour boucler dans la fonction tant qu'il y a des caractères dispo
  // Par defaut : true : on boucle ici jusqu'à épuisement des caractères
  // false dans certains état de l'automate : RECEIVED car comme on ne consomme
  // pas les caractères, on resterait bloqué
  bool loop_on_available = true;
  
  if ( prot_pStream->available() )
  {
    while ( loop_on_available && prot_pStream->available() )
    {
      // En fonction de l'état de la tâche de réception
      switch( prot_State )
      {
        //--- Attente du caractère de synchro ----------
        case WAIT_SYNC:
          // lecture de l'octet reçu
          c = prot_pStream->read();
          // Si on a reçu le caractère de synchro, on passe en mode reception de message
          if ( c == PROT_SYNC )
          {
            // on remet tout à zérodémarre à l'index 0 du buffer
            prot_reset();
            // et on commence la réception
            prot_State = RECEIVING;
            DEBUG("! Sync OK, receiving\n" ); 
          }
          break;
        //--- Reception en cours -----------------------
        case RECEIVING:
          // lecture de l'octet reçu
          c = prot_pStream->read();
          // On effectue d'abord le traitement des cas particuliers
          if ( c == PROT_SYNC )
          {
            // Reçu caractère de synchro en plein millieu d'un message
            // C'est une erreur, on annule tout et on repart au debut de message
            prot_reset();
            DEBUG("! Sync while receiving - reset\n" ); 
          }
          else if ( c == PROT_CHKSEP )
          {
            // Reçu caractère de séparation du checksum
            // on marque la fin du message par un caractère de fin de chaîne
            prot_BufferRec[prot_IndexRec] = '\0';
            // On passe dans l'etat de reception du checksum
            prot_State = CHECKSUM;
            DEBUG("! * OK, goto checksum\n" ); 
          }
          else if ( (c == PROT_EOM1) || (c == PROT_EOM2) )
          {
            // Reçu l'un des 2 marqueurs de fin (sans checksum donc)
            // on marque la fin du message par un caractère de fin de chaîne
            prot_BufferRec[prot_IndexRec] = '\0';
            // On passe dans l'état Received
            prot_State = RECEIVED;
            DEBUG("! CRLF while receiving - received w/o checksum\n" ); 
          }
          // Et maintenant le cas général, on accumule les caractères .. s'il reste de la place
          else if ( prot_IndexRec < PROT_MAXLEN )
          {
            // Si on a encore de la place, on garde le caractère dans le buffer
            prot_BufferRec[prot_IndexRec++] = c;
            // et on met a jour le calcul du checksum
            prot_ChecksumCalc ^= c;
          }
          else
          {
            // problème, on est arrivé au bout du buffer sans recevoir de message entier
            // On repart à 0 en attente d'un nouveau message
            prot_State = WAIT_SYNC;
            DEBUG("! Buffer full - Wait new Sync\n" ); 
          }
          break;
        //--- Reception du checksum ----------------------
        case CHECKSUM:
          // lecture de l'octet reçu
          c = prot_pStream->read();
          if ( (c == PROT_EOM1) || (c == PROT_EOM2) )
          {
            // Reçu l'un des 2 marqueurs de fin
            // On marque la fin du message
            prot_BufferRec[prot_IndexRec] = '\0';
            // Vérifions le checksum
            if ( prot_Checksum == prot_ChecksumCalc )
            {
              // Checksum ok, on passe dans l'état Received
              prot_State = RECEIVED;
              DEBUG("! Checksum OK - RECEIVED\n" ); 
            }
            else
            {
              // Message invalide, on poubellise
              prot_State = WAIT_SYNC;
              DEBUG("! Checksum KO - Wait new Sync\n" ); 
            }
          }
          else
          {
            // On reçoit le checksum sous forme de 2 caractères hexa
            byte x = hexchar2dec( c );
            prot_Checksum = (prot_Checksum << 4) + x;
            prot_ChecksumReceived = true; 
          }
          break;
        //--- Message disponible -------------------------
        case RECEIVED:
          // on ne continue plus à lire les caractères même s'il y a de dispo
          // tant que l'on est dans cet état. On attend que l'application
          // appelle prot_clearMessage() pour liberer le buffer de réception
          loop_on_available = false;
          break;        
        //--- Perte de comm avec le maitre ----------------
        case LOST_COMM:
          // dès qu'on a de nouveau de la réception, on reset l'automate et
          // on continue a relire la liaison série
          prot_reset();
          prot_State = WAIT_SYNC;
          break;        
        //--- Message disponible -------------------------
      }
    }
    // on mémorise la date de maintenant pour le calcul des timeouts
    prot_TimeLastReceived = millis();
  }
  else if ( (prot_State >= RECEIVING) && (prot_State <= CHECKSUM) && (millis() - prot_TimeLastReceived) > PROT_RECEIVE_TMOUT )
  {
    // Si on n'a pas reçu un message dans son intégralité en moins de PROT_RECEIVE_TMOUT
    // alors on oublie tout et on se replace en attente
    prot_State = WAIT_SYNC;
    DEBUG("! Timeout while receiving\n" ); 
  }
  else if ( (prot_State != LOST_COMM) && (millis() - prot_TimeLastReceived) > PROT_LOSTCOM_TIMEOUT )
  {
    // Si on n'a rien reçu du tout depuis PROT_LOSTCOM_TIMEOUT
    // alors c'est une alarme de perte de connexion
    prot_lostCommTimeout();
    // on passe dans l'etat LOST_COMM pour éviter de regénérer l'alarme systématiquement
    prot_State = LOST_COMM;
  }
}

// Remise à zéro du buffer de réception et des calculs de checksum
void prot_reset()
{
  // Remise a zéro de l'index du buffer de réception
  prot_IndexRec = 0;
  // RAZ du calcul de checksum
  prot_ChecksumCalc = 0;
  prot_ChecksumReceived = false;
  prot_Checksum = 0;
}

// Fonction appelée en cas de perte de communication
void prot_lostCommTimeout()
{
  // inserer ici ce qu'il faut faire en cas de perte de communication 
  // avec le maitre
  DEBUG("! Lost comm\n" ); 
}

// Fonction appelée par la tâche de distribution des messages pour savoir 
// s'il y a un message de disponible
bool prot_isMessageAvailable()
{
  return ( prot_State == RECEIVED );
}

// Fonction appelée par la tâche de distribution des messages pour
// récupérer le pointeur vers le dernier message dispo
char *prot_getMessage()
{
  return prot_BufferRec;
}

// Fonction appelée par la tâche de distribution des messages pour
// signaler que le message a été traité et qu'on peut en recevoir un autre
void prot_clearMessage()
{
  prot_State = WAIT_SYNC;
}

// Utilitaire de conversion hexa
byte hexchar2dec( char c)
{
  byte x = 0;
  if ( (c >= '0') && (c <= '9') )
    x = c & 0x0F;
  else if  ( (c >= 'A') && (c <= 'F') )
    x = (c & 0x07) + 9;
  return x;
}

// Fonctions pour emettre un message
void prot_send( char *pMess )
{
  DEBUG( "! sending: " );
  DEBUGLN( pMess ); 

  prot_pStream->write( PROT_SYNC );
  prot_pStream->print( pMess );
  
  byte ck = prot_calcChecksum( pMess );
  prot_pStream->write( PROT_CHKSEP );
  prot_pStream->println( ck, HEX );
}

byte prot_calcChecksum( char *p )
{
  byte ck = 0;
  for ( ; *p ; p++ )
    ck ^= (byte)(*p);
  return ck;
}

