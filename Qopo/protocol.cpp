/*
	protocol.cpp
	C++ source file for Qopo Morning Shower device
	Based on Jean-Michel Mercier, 'Implementer un protocole de communication pour Arduino'
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved.
*/

#include "protocol.h"


namespace Qopo{

Protocol::Protocol(class Stream *stream, std::string name)
{
	// Memorize pointer to stream
	this->_stream = stream;
	// init timestamp
	this->_timeLastReceived = millis();
	// init protocol
	this->reset();

	this->_name = name;
}

// Loop for receiving messages
void Protocol::loop()
{
	if(!this->_stream){
		return;
	}
	// 'c' represents the next character to be read
	char c;

	// Variable to loop as long as we get characters
	// Default is true, we loop until we reach last character
	// Might need to be false in certain cases: for example when RECEIVED flag is up:
	// It would be trapped as it would not get any characters
	bool loop_on_available = true;
	if(this->_stream->available())
	{
		while (loop_on_available && this->_stream->available())
		{
			switch(this->_state)
			{
				/*
					Waiting for message
				*/
				case WAIT_SYNC:
					c = this->_stream->read();
					// If character received corresponds to message first characters
					// Then we read the message
					if (c == PROT_SYNC)
					{
						// We reset buffer
						this->reset();
						// and we start reading message
						this->_state = RECEIVING;
					}
					break;
				/*
					Receiving message
				*/
				case RECEIVING:
					c = this->_stream->read();
					if(c == PROT_SYNC)
					{
						this->reset();
					} else if(c == PROT_CHKSEP)
					{
						this->_state = CHECKSUM;
					} else if((c == PROT_EOM1) || (c == PROT_EOM2))
					{
						this->_state = RECEIVED;
					} else if(this->_buffer.size() < PROT_MAXLEN)
					{
						this->_buffer += c;
						this->_checksumCalc ^= c;
					}
					else
					{
						this->_state = WAIT_SYNC;
					}
					break;
				/*
					Receiving checksum
				*/
				case CHECKSUM:
					c = this->_stream->read();
					if((c == PROT_EOM1) || (c == PROT_EOM2))
					{
						if(this->_checksum == this->_checksumCalc)
						{
							// Valid message
							this->_state = RECEIVED;
							DEBUGLN("Protocol got that.");
						} else {
							// Invalid message, throw away
							this->_state = WAIT_SYNC;
						}
						delay(3000);
					}
					else
					{
						// We get checksum
						byte x = hexchar2dec(c);
						this->_checksum = (this->_checksum << 4) + x;
						this->_checksumReceived = true; 
					}
					break;
				/*
					Message is received
				*/
				case RECEIVED:
					// We stop reading characters and wait for call to clearMessage to free buffer
					loop_on_available = false;
					break;
				/*
					Communication lost with master
				*/
				case LOST_COMM:
					this->reset();
					this->_state = WAIT_SYNC;
					break;

			};
		}
		// Update timestamp
		this->_timeLastReceived = millis();
	}
	else if((this->_state >= RECEIVING) 
			&& (this->_state <= CHECKSUM) 
			&& (millis() - this->_timeLastReceived) > PROT_RECEIVE_TMOUT )
	{
		// If we did not get all message in timeout window, we reset
		this->_state = WAIT_SYNC;
		DEBUGPROTLN("--Timeout while receiving"); 
	}
	else if((this->_state != LOST_COMM) 
			&& (millis() - this->_timeLastReceived) > PROT_LOSTCOM_TIMEOUT)
	{
		// If we have received anything since last timeout then we lost connection
		this->lostCommTimeout();
		// Change state to lost connection to avoid multiple alarms
		this->_state = LOST_COMM;
	}
}

// Reset buffer and checum calculations
void Protocol::reset()
{
	_buffer = std::string();
	// Reset checksum calculation
	this->_checksumCalc = 0;
	this->_checksumReceived = false;
	this->_checksum = 0;
	this->clearMessage();
}

// Function called when connection is lost
void Protocol::lostCommTimeout()
{
	//TODO: insert what to do in case communication is lost with master
	DEBUGPROTLN("--Lost comm"); 
}

// Determine if a message is available
bool Protocol::isMessageAvailable()
{
	return (this->_state == RECEIVED);
}

// Return last received message
std::string Protocol::getMessage()
{
	return this->_buffer;
}

// Indicates that it can now receive message again
void Protocol::clearMessage()
{
	_buffer.clear();
	this->_state = WAIT_SYNC;
}

// Function to send a message
void Protocol::send(const std::string &message)
{
	DEBUGPROT("--sending: ");
	DEBUGSTRLN(message); 

	this->_stream->write(PROT_SYNC);
	this->_stream->print(message.c_str());

	byte checksum = this->calcChecksum(message.c_str());
	this->_stream->write(PROT_CHKSEP);
	this->_stream->println(checksum, HEX);
}

//Function to calculate checksum
byte Protocol::calcChecksum(const char * p)
{
	byte ck = 0;
	for ( ; *p ; p++ )
		ck ^= (byte)(*p);
	return ck;
}

} // Namespace Qopo
