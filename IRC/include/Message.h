#ifndef __MESSAGE_GUARD__
#define __MESSAGE_GUARD__

#include "Globals.h"
#include "FindFunctions.h"
#include "OtherFunctions.h"
#include <vector>

using std::vector;

class Message
{
private:

	void ParseParams(const string msg);
	void ParseOutgoingMessage(const string msg);
	void ParseIncomingMessage(string msg);
public:
	Message(std::string msg, bool Outgoing);
	~Message();
	bool GetNUHStatus();
	string GetFormatedMessage();
	void MergeParameterArray(int begin = 0, int end = 0);

	//fuck it
	bool HasNickUserHost;
	string Nick;
	string User;
	string Host;
//	char *Command;
	vector<string> ParameterArray;
	string formatedMessage;	
	bool Outgoing;
	bool isNormalMessage;
};

#endif