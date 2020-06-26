/*
	protocol.h
	C++ header file for Qopo Morning Shower device
	Based on Jean-Michel Mercier, 'Implementer un protocole de communication pour Arduino'
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved.
 */

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#define DEBUGPROT(a) DEBUG((this->_name+a).c_str())
#define DEBUGPROTLN(a) DEBUGLN((this->_name+a).c_str())

// C++ libraries
#include <StandardCplusplus.h>
#include <string>
#include <vector>

#include "helpers.h"

namespace Qopo{

// Max length of a message including checksum, etc..
#define PROT_MAXLEN    (50)

// Separators for messages
#define PROT_SYNC     '$' //Initial separator
#define PROT_SEP      ',' //Separator for arguments
#define PROT_SEP_S    "," //Separator for arguments(secondary)
#define PROT_CHKSEP   '*' //Checksum separator
#define PROT_EOM1     '\n' //End of message separator
#define PROT_EOM2     '\r' //End of message separator(secondary)

// Message receiving timeout in ms
#define PROT_RECEIVE_TMOUT (1000) // 1s = 1000ms

// Connection time out in ms
// (We have to receive something in this timing otherwise connection is lost)
#define PROT_LOSTCOM_TIMEOUT  (30000)

class Protocol
{
	public:
		Protocol(class Stream *stream = NULL, std::string name = std::string());
		void loop();
		// Processing messages functions
		bool isMessageAvailable();
		std::string getMessage();
		void clearMessage();

		// Fonctions pour emettre un message
		void send(const std::string &message);
	protected:
		//Protocol functions
		void reset();
		void lostCommTimeout();
		byte calcChecksum(const char * p);
	private:
		// Stream class later used to store Serial pointer
		class Stream *_stream;

		// Message buffer
		// +1 for the '\0' character
		std::string _buffer;

		// Checksum calculation that is currently received
		byte _checksumCalc;

		// Checksum received in message
		bool _checksumReceived;
		
		// Actual checksum
		byte _checksum;

		// State of message reception
		enum STATE_E {
		  WAIT_SYNC, // waiting for character sync
		  RECEIVING, // currently receiving message
		  CHECKSUM, // currently receiving checksum
		  RECEIVED, // message received and not processed. While not received, we cannot receive any other.
		  LOST_COMM // connection lost
		} _state;

		// Keep track of time last character was received to determine timeout
		unsigned long _timeLastReceived;

		// Protocol name to differentiate multiple
		std::string _name;
};

} // Namespace Qopo
#endif // _PROTOCOL_H