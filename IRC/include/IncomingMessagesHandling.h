#ifndef __INCOMINGMESSAGESHANDLING_GUARD__
#define __INCOMINGMESSAGESHANDLING_GUARD__

#include "Globals.h"
#include "FindFunctions.h"
#include "Message.h"

class MainWindow;

class IncomingMessagesHandling
{
private:
	char *msg;
	int count;
	char *nick;
	char *user;
	char *realname;
	bool PingStuff(Message *msg);
	void decodeServerMessages(Message *msg);
	void decodeNonServerNonPrivmsg(Message *msg);
	void DecodeOutput(char *msg);
	MainWindow *pMainWindow;

public:
	void OutputHandling();
	IncomingMessagesHandling(MainWindow *Parent, char *sNick, char *sUser, char *sRealname = "Ask if you want to know");
	~IncomingMessagesHandling();
	void ResetCount();
};

#endif