#ifndef __SERVERSTUFF_GUARD__
#define __SERVERSTUFF_GUARD__

#include "Globals.h" //has WinSock.h included

class ServerCF
{
private:
	SOCKET serverSocket;/// \var Is the Socket
	SOCKET ToClientSocket;/// \var This Socket handles the actual communication with the other client
//	WSADATA *SocketData; /// \var Enables Sockets

public:
	ServerCF(); /// \fn creator
	~ServerCF(); /// \fn destructor

	void CloseConnection(); /// \fn Closes the Current Connection
	SOCKET ServerCF::GetToClientSocket();
	void ServerCF::SendToClient(char *msg);
	void RecieveFromClient();
	void ServerCF::Accept();
	void ServerCF::CloseToClientSocket();
};


#endif