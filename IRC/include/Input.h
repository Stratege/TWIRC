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
	void Input::SendPrivMsg(Message *msg);
	void Input::SendMeMsg(Message *msg);
	void Input::SelectChannel(char *msg3);
	void Input::RemoveCommandString(char *msg3);
	void Input::CloseChannel(char *msg3);
	void Input::InputCommands(Message *msg);

public:
	Input();
	~Input();
	void SendMessage(char *msg);
	void KeyDown(char Key);
};

#endif