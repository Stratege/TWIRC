#include "Input.h"
#include "MainWindow.h"

#include <array>


#define PRIVMSG_LENGTH 8

/*
int send(
  _In_  SOCKET s,
  _In_  const char *buf,
  _In_  int len,
  _In_  int flags
);
*/
int SendAMsg(SOCKET s, const char *buf, int len, int flags)
{

	if(!SetTimer(g_pWindow,CONNECTIONTIMERID,300000,NULL))
	{
		//timer couldn't be set, do some error handling
		//just toss out a "timer couldn't be set msg"
		g_pMainWindow->OutputInternalMessage("INTERNAL_MESSAGE Timer could not be set");
	}
	return send(s,buf,len,flags);
}

void SendTheMessage(Message *msg)
{
	appendNewLine(msg->formatedMessage);
	SendAMsg(g_pMainWindow->GetSocket(),msg->formatedMessage.c_str(),msg->formatedMessage.length(),0);
	g_pMainWindow->OutputMessage(msg);
}

void Input::SendPrivMsg(Message *msg)
{
	msg->formatedMessage = (string)"PRIVMSG "+g_pMainWindow->GetSelectedChannelName()+" :"+msg->formatedMessage;
	
	SendTheMessage(msg);
}


void Input::SendMeMsg(Message *msg)
{
	//first modify the message to cut /me
	//then add \001ACTION msg \001
	msg->MergeParameterArray(1);
	msg->formatedMessage = (char)1+"ACTION "+msg->formatedMessage+(char)1;

	//do the normal privmsg stuff
	SendPrivMsg(msg);
}


//conceptually I have a function that does the following:
//It accepts a list of strings to check against
//It accepts what it should do once it checked against that


template<class Functor>
bool InputCommand(Message *msg,std::vector<string> list, Functor func, unsigned int minMsgArrayCounter) {

	if(StringFromStringListEqual(msg->ParameterArray[0],list))
	{
		//1 == variadic, depending on how many are required
		if(msg->ParameterArray.size() >= minMsgArrayCounter)
		{
			func();
		}else{
			//possibly variable part, doesn't have to be, that returns an error msg if the array isn't large enough for our purpose. Probably should toss out a helpful msg for the user.
			//error
			g_pMainWindow->OutputInternalMessage("Not enough Parameters in message.");
		}
		return true;
	}
	return false;
}



void SendUnknownCommand(Message *msg)
{
	msg->MergeParameterArray();
	
	SendTheMessage(msg);
}

void Input::InputCommands(Message *msg)
{
	msg->MergeParameterArray();
	g_pMainWindow->OutputInternalMessage(msg->formatedMessage);

	if(msg->ParameterArray.size() > 0)
	{
		std::vector<string> Select;
		Select.push_back("Select");
		Select.push_back("select");
		Select.push_back("s");
		Select.push_back("S");
		if(InputCommand(msg,Select,[&](){g_pMainWindow->SelectChannel(msg->ParameterArray[1]);},2))
			return;

		std::vector<string> Close;
		Select.push_back("Close");
		Select.push_back("close");
		Select.push_back("C");
		Select.push_back("c");
		if(InputCommand(msg,Close,[&](){g_pMainWindow->CloseChannel(msg->ParameterArray[1]);},2))
			return;

		std::vector<string> CloseAll;
		Select.push_back("Closeall");
		Select.push_back("closeall");
		Select.push_back("CloseAll");
		Select.push_back("closeAll");
		if(InputCommand(msg,CloseAll,[&](){g_pMainWindow->CloseAll();},1))
			return;

		std::vector<string> Copy;
		Select.push_back("Copy");
		Select.push_back("copy");
		if(InputCommand(msg,Copy,[&](){g_pMainWindow->CopyCurrentLine();},1))
			return;

		std::vector<string> Me;
		Select.push_back("Me");
		Select.push_back("me");
		Select.push_back("ME");
		Select.push_back("mE");	
		if(InputCommand(msg,Me,[&](){SendMeMsg(msg);},2))
			return;

		std::vector<string> Quit;
		Select.push_back("Quit");
		Select.push_back("quit");
		if(InputCommand(msg,Me,[&](){
				SendUnknownCommand(msg);
				::SendMessage(g_pWindow,WM_DESTROY,NULL,NULL);
			},1)){
				return;
		}

		std::vector<string> ShowUserList;
		Select.push_back("UserList");
		Select.push_back("Userlist");
		Select.push_back("userlist");
		Select.push_back("ulist");
		Select.push_back("Ulist");
		Select.push_back("UList");
		if(InputCommand(msg,ShowUserList,[&](){g_pMainWindow->ShowUserList();},1))
			return;

			//if we don't know the message, see if the server can deal with it
			//or we simply don't care about the message: example /join
			SendUnknownCommand(msg);
	}else{
		//this should never happen
		g_pMainWindow->OutputInternalMessage("No Parameter found? What.");
	}
}

void Input::SendMessage(std::string msg)
{
	Message InputMsg(msg,true);

	if(!InputMsg.isNormalMessage)
	{
		InputCommands(&InputMsg);
	}else
	{
		//it's a normal privmsg. Go send it.
		SendPrivMsg(&InputMsg);
	}
//		std::cout << " - " << SelectedChannel;
	length = 0;
}

Input::Input()
{
	msg2 = new char [600];
	length = 0;
}

Input::~Input()
{
	delete msg2;
}
