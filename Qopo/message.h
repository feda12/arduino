/*
	messages.h
	C++ header file for Qopo Morning Shower device
	Based on Jean-Michel Mercier, 'Implementer un protocole de communication pour Arduino'
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved.
*/

#ifndef _MESSAGES_H
#define _MESSAGES_H

#include "protocol.h"
#include "helpers.h"

// C++ libraries
#include <StandardCplusplus.h>
#include <string>
#include <vector>
#include <pair>
#include <sstream>


namespace Qopo{
/*
	Message require functions for commands to have the following syntax
	And to return 0 if success
	int <cmd name>(char *<args>[]){ return 0; }
 */
// Maximum number of arguments
#define MESS_ARGS_MAX   (5)


template <class T>
class Message
{

	friend class Protocol;
	public:
		enum CmdError {
			INVALID_ARGUMENTS,
			INVALID_NB_ARGUMENTS,
			INVALID_MESSAGE_TYPE,
		};
		typedef int(T::*CommandFunc)(const std::vector<std::string>&);
		typedef std::vector<CommandFunc> CommandList;
		typedef std::vector<std::string> CommandNames;

		Message(Protocol * protocol);
		Message(Protocol * protocol, T *obj, CommandList *cmdList, CommandNames *cmdNames);
		void dispatch();
		void setCmdSet(CommandList *cmdList, CommandNames *cmdNames);
		void setObj(T *obj);
	protected:
		void cmdError(int err_code);
		void send(std::vector<std::string> &args);
		std::vector<std::string> tokenize(const std::string &message);

		// Array to receive arguments
		int _args[MESS_ARGS_MAX];

		// Number of arguments found
		int _nbArgs;

		// Pointer on protocol used by app
		Protocol * _protocol;

		// Buffer to prepare messages
		// +1 for '\0' message
		std::string _bufferSend;

		T *_obj;
		CommandList *_cmdList;
		CommandNames *_cmdNames;
};

template <typename T>
Message<T>::Message(Protocol * protocol)
{
	_protocol = protocol;
	_obj = NULL;
	_cmdList = NULL;
	_cmdNames = NULL;
}

template <typename T>
Message<T>::Message(Protocol * protocol, T *obj, CommandList *cmdList, CommandNames *cmdNames)
{
	_protocol = protocol;
	_obj = obj;
	_cmdList = cmdList;
	_cmdNames = cmdNames;
}

template <typename T>
void Message<T>::dispatch()
{
	DEBUGLN("Dispatching message");
	// std::vector<std::string> tokens = this->tokenize(this->_protocol->getMessage());
	std::vector<std::string> PROGMEM tokens;
	tokens.push_back("mess");
	tokens.push_back("1");
	tokens.push_back("2");
	DEBUGLN("MEssage tokenized");
	bool validCmd = false;
	if(tokens.size() > 0)
	{
		for(int i = 0; i < _cmdNames->size(); ++ i)
		{
			if(tokens.at(0) == _cmdNames->at(i))
			{
				this->cmdError((_obj->*(_cmdList->at(i)))(tokens));
				validCmd = true;
			}
		}
	}
	_protocol->clearMessage();
	if(!validCmd)
		this->cmdError(INVALID_MESSAGE_TYPE);
}

// Change the cmd set pointer
template <typename T>
void Message<T>::setCmdSet(CommandList *cmdList, CommandNames *cmdNames)
{
	_cmdList = cmdList;
	_cmdNames = cmdNames;	
}

// Change the cmd set pointer
template <typename T>
void Message<T>::setObj(T *obj)
{
	_obj = obj;	
}

// Analyse le message entrant pour en extraire les arguments
template <typename T>
std::vector<std::string> Message<T>::tokenize(const std::string &message)
{
	std::vector<std::string> tokens;
	std::istringstream iss(message);
	std::string token;
	while(getline(iss, token, PROT_SEP)){
		DEBUGSTRLN(token);
		tokens.push_back(token);
	}
	return tokens;
}

template <typename T>
void Message<T>::send(std::vector<std::string> &args)
{
	// Composition du message
	std::string bufferSend;

	// First argument
      // snprintf(&this->_bufferSend[1], sizeof(&this->_bufferSend[1]), "%d", arg1);


	// Sending message
	_protocol->send(bufferSend);
}

template <typename T>
void Message<T>::cmdError(int err_code)
{
	switch(err_code)
	{
		case INVALID_MESSAGE_TYPE:
			DEBUGLN("Invalid message");
			break;
	};
}

} // Namespace Qopo

#endif // _MESSAGES_H_
