#ifdef SERVER_ON

#include "Serverstuff.h"
#include "FindFunctions.h"
#include "MainWindow.h"

ServerCF::ServerCF()
{
	
	ToClientSocket = 0;

	struct sockaddr_in server;

	memset( &server, 0, sizeof (server));
	// IPv4-Adresse
	server.sin_family = AF_INET;
	// Jede IP-Adresse ist gültig
	server.sin_addr.s_addr = htonl( INADDR_ANY );

	// Portnummer 1023 'cause that one's open here
	server.sin_port = htons(1023);

	if(bind(serverSocket, (struct sockaddr*)&server, sizeof( server)) < 0) {
	   //Fehler bei bind()
	 }else{
		WSAAsyncSelect(serverSocket,g_pWindow,WM_SERVERSOCKET,FD_ACCEPT);
		if(listen(serverSocket,SOMAXCONN))
		{
			//error handling stuff
		}
	}
}

ServerCF::~ServerCF()
{
	//clears the sockets in question
	closesocket(serverSocket);
	closesocket(ToClientSocket);
	//clears the sockets
	WSACleanup();
}

void ServerCF::Accept()
{
	struct sockaddr_in client;

	int len;


	len = sizeof( client );

        ToClientSocket = accept(serverSocket, (struct sockaddr*)&client, &len);
	if(ToClientSocket < 0){
		//error handling
	}else{
		WSAAsyncSelect(serverSocket,g_pWindow,WM_TOCLIENTSOCKET,(FD_READ|FD_CLOSE));
		//we don't want more than one connection, cause it crashes
		closesocket(serverSocket);
	}
}


SOCKET ServerCF::GetToClientSocket()
{
	return ToClientSocket;
}

void ServerCF::SendToClient(char *msg)
{
	if(ToClientSocket != 0)
		send(ToClientSocket,msg,strlen(msg),0);
}

void ServerCF::CloseToClientSocket()
{
	ToClientSocket = 0;
	closesocket(ToClientSocket);
}

void ServerCF::RecieveFromClient()
{
	int numbytes = 0;
	char *msg = new char[500];
	
	//Recv & print Data
	WSAAsyncSelect(ToClientSocket,g_pWindow,0,0);

	recv(ToClientSocket,msg,498,MSG_PEEK);
	numbytes = FindSign(msg,10);

	WSAAsyncSelect(ToClientSocket,g_pWindow,WM_SOCKET,(FD_CLOSE|FD_READ));

	if(numbytes == -1) {
		numbytes = 498;
	}
	numbytes++;
	recv(ToClientSocket,msg,numbytes,0);
//	numbytes++;
	msg[numbytes]='\0';
	//std::cout << msg << std::endl;

	if(numbytes > 0) {
		g_pMainWindow->SendMessage(msg);
	}
	delete msg;
}

#endif