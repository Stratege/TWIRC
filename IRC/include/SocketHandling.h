#ifndef __SOCKETHANDLING_GUARD__
#define __SOCKETHANDLING_GUARD__

#include "Globals.h"

/** \brief Contains the Sockethandling class
 *  \file
 *  
*/

class SocketHandling
{
private:
	SOCKET testSocket; /// \var Is the Socket
	WSADATA *SocketData; /// \var Enables Sockets

public:
	SocketHandling(); /// \fn creator
	~SocketHandling(); /// \fn destructor
	bool OpenConnection(char *protocol, char *hostname, short port); /// \fn Opens a new Connection. Closes Existing beforehand
	void CloseConnection(); /// \fn Closes the Current Connection
	SOCKET GetSocket();
};

#endif