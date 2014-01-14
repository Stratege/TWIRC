#include "SocketHandling.h"
#include "FindFunctions.h"
#include "MainWindow.h"


SocketHandling::SocketHandling()
{
	//Initialises Sockets
	SocketData = new WSADATA; 
}

SocketHandling::~SocketHandling()
{
	CloseConnection();
	delete SocketData;
}

bool SocketHandling::OpenConnection(char *protocol, char *hostname, short port)
{

	int a = 0;
	struct in_addr addr;
	int i = 0;

	CloseConnection();
	if(!WSAStartup(MAKEWORD(2,0),SocketData))
	{
		//register Socket
		testSocket = socket(AF_INET, SOCK_STREAM , 0);
		if(testSocket != -1)
		{
			SERVENT *testServ = getservbyname(protocol,NULL);
			hostent *testHost;
				testHost = gethostbyname(hostname);

				if (testHost->h_addrtype == AF_INET)
				{
				while (testHost->h_addr_list[i] != 0) {
					addr.s_addr = *(u_long *) testHost->h_addr_list[i++];
					printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
					}
				}
			//now we need the address
			sockaddr_in testSockAddr_in;
			testSockAddr_in.sin_family = AF_INET;
			testSockAddr_in.sin_port = htons(port);


			testSockAddr_in.sin_addr = addr;

			int test2 = connect(testSocket,(struct sockaddr *)&testSockAddr_in,sizeof(testSockAddr_in));
 			if(!test2)
			{
				//success! Time to create a listener
				WSAAsyncSelect(testSocket,g_pWindow,WM_SOCKET,(FD_CLOSE|FD_READ));
				return true;

			}else{
				//std::cout << WSAGetLastError();
				g_pMainWindow->OutputInternalMessage("-!Client  :ERROR Couldn't connect to server");
				return false;
			}
		}else { // toss error
			g_pMainWindow->OutputInternalMessage("-!Client  :ERROR Couldn't create socket");
		}
	} else { //toss error
		g_pMainWindow->OutputInternalMessage("-!Client  :ERROR Couldn't start Socket service");
	}
	return false;
}

void SocketHandling::CloseConnection()
{
	//clears the socket in question
	closesocket(testSocket);
	//clears the sockets
	WSACleanup();
}

SOCKET SocketHandling::GetSocket()
{
	return testSocket;
}