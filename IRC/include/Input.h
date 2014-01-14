#ifndef __INPUT_GUARD__
#define __INPUT_GUARD__

#include "Globals.h"
#include "FindFunctions.h"
#include "Message.h"

class Input
{
private:
	char *msg2;
	int length;
	void SendPrivMsg(Message *msg);
	void SendMeMsg(Message *msg);
	void SelectChannel(char *msg3);
	void RemoveCommandString(char *msg3);
	void CloseChannel(char *msg3);
	void InputCommands(Message *msg);

public:
	Input();
	~Input();
	void SendMessage(char *msg);
};

#endif