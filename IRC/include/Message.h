#ifndef __MESSAGE_GUARD__
#define __MESSAGE_GUARD__

#include "Globals.h"
#include "FindFunctions.h"
#include "OtherFunctions.h"
#include <vector>

class Message
{
private:

	void ParseParams(char *msg);
	void ParseOutgoingMessage(char *msg);
	void ParseIncomingMessage(char *msg);
public:
	Message(char *msg, bool Outgoing);
	~Message();
	bool GetNUHStatus();
	char *GetFormatedMessage();
	void MergeParameterArray(int begin = 0, int end = 0);

	//fuck it
	bool HasNickUserHost;
	char *Nick;
	char *User;
	char *Host;
//	char *Command;
	std::vector<char *> ParameterArray;
	char *formatedMessage;	
	bool Outgoing;
	bool isNormalMessage;
};

#endif