#include "Input.h"
#include "MainWindow.h"

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
	appendNewLine(&msg->formatedMessage);
	SendAMsg(g_pMainWindow->GetSocket(),msg->formatedMessage,strlen(msg->formatedMessage)-1,0);
	g_pMainWindow->OutputMessage(msg);
}

void Input::SendPrivMsg(Message *msg)
{

	strcpy(msg2,"PRIVMSG ");
	strcat(msg2,g_pMainWindow->GetSelectedChannelName());
	strcat(msg2," :");
	strcat(msg2,msg->formatedMessage);

	delete msg->formatedMessage;
	msg->formatedMessage = charAllocAndSectionCopy(msg2);
	
	SendTheMessage(msg);
}


void Input::SendMeMsg(Message *msg)
{
	//first modify the message to cut /me
	//then add \001ACTION msg \001
	char *tempMeMsg = new char[800];
	strncpy(tempMeMsg,"0ACTION ",799);
	tempMeMsg[0] = 1;
	msg->MergeParameterArray(1);

	strncat(tempMeMsg,msg->formatedMessage,799-8);//8 = length of "0ACTION "; 4 = length of "/me "
	int tempLength = strlen(tempMeMsg);
	tempMeMsg[tempLength] = 1;
	tempMeMsg[tempLength+1] = 0;

	SemiSmartDelete(&msg->formatedMessage);
	msg->formatedMessage = tempMeMsg;

	//do the normal privmsg stuff
	SendPrivMsg(msg);
}


//conceptually I have a function that does the following:
//It accepts a list of strings to check against
//It accepts what it should do once it checked against that


template<class Functor>
bool InputCommand(Message *msg,char* list[], Functor func, int minMsgArrayCounter) {

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
	char *temp = charAllocAndSectionCopy(msg->ParameterArray[0]);
	delete msg->ParameterArray[0];
	msg->ParameterArray[0] = temp;
	msg->MergeParameterArray();
	
	SendTheMessage(msg);
}

void Input::InputCommands(Message *msg)
{
	msg->MergeParameterArray();
	g_pMainWindow->OutputInternalMessage(msg->formatedMessage);

	if(msg->ParameterArray.size() > 0)
	{
		char* Select[] = {"Select","select","s","S",0};
		if(InputCommand(msg,Select,[&](){g_pMainWindow->SelectChannel(msg->ParameterArray[1]);},2))
			return;

		char *Close[] = {"Close","close","C","c",0};
		if(InputCommand(msg,Close,[&](){g_pMainWindow->CloseChannel(msg->ParameterArray[1]);},2))
			return;

		char *CloseAll[] = {"Closeall","closeall","CloseAll","closeAll",0};
		if(InputCommand(msg,CloseAll,[&](){g_pMainWindow->CloseAll();},1))
			return;

		char *Copy[] = {"Copy","copy",0};
		if(InputCommand(msg,Copy,[&](){g_pMainWindow->CopyCurrentLine();},1))
			return;

		char *Me[] = {"Me","me","ME","mE",0};
		if(InputCommand(msg,Me,[&](){SendMeMsg(msg);},2))
			return;

		char *Quit[] = {"Quit","quit",0};
		if(InputCommand(msg,Me,[&](){
				SendUnknownCommand(msg);
				::SendMessage(g_pWindow,WM_DESTROY,NULL,NULL);
			},1)){
				return;
		}

		char *ShowUserList[] = {"UserList","Userlist","userlist","ulist","Ulist","UList",0};
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

void Input::SendMessage(char *msg)
{
	Message *InputMsg = new Message(msg,true);
	int InputMsgLength = strlen(msg);

	if(!InputMsg->isNormalMessage)
	{
		InputCommands(InputMsg);
	}else
	{
		//it's a normal privmsg. Go send it.
		SendPrivMsg(InputMsg);
	}
//		std::cout << " - " << SelectedChannel;
	length = 0;

	delete InputMsg;
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
