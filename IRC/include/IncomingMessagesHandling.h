#ifndef __INCOMINGMESSAGESHANDLING_GUARD__
#define __INCOMINGMESSAGESHANDLING_GUARD__

#include "Globals.h"
#include "FindFunctions.h"
#include "Message.h"

class MainWindow;

class IncomingMessagesHandling
{
private:
	char msg[600];
	int count;
	string nick;
	string user;
	string realname;
	bool PingStuff(Message *msg);
	void decodeServerMessages(Message *msg);
	void decodeNonServerNonPrivmsg(Message *msg);
	void DecodeOutput(char *msg);
	MainWindow *pMainWindow;

public:
	void OutputHandling();
	IncomingMessagesHandling(MainWindow *Parent, string sNick, string sUser, string sRealname = "Ask if you want to know");
	~IncomingMessagesHandling();
	void ResetCount();
};

#endif