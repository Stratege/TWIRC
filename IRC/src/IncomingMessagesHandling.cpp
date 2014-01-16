#include "IncomingMessagesHandling.h"
#include "MainWindow.h"

const char *reasonString = ", reason: ";

bool IncomingMessagesHandling::PingStuff(Message *msg)
{
	if(StringBeginEqual(msg->ParameterArray[0],"PING"))
	{
		//Ping works, no need to confirm it anymore. Also fairly sure that can only be send by the server
		//OutputMessage(msg,0);
		msg->ParameterArray[0].replace(1,1,"O");
		msg->MergeParameterArray();
		appendNewLine(msg->formatedMessage);
		send(pMainWindow->GetSocket(),msg->formatedMessage.c_str(),msg->formatedMessage.length(),0);
		return true;
	}else return false;
}

void IncomingMessagesHandling::decodeServerMessages(Message *msg)
{
		if(!PingStuff(msg))
		{
			//it's not a ping
			//currently that means, just print it
			if(!msg->ParameterArray[1].compare(RPL_NAMREPLY))
			{
				string Channel = msg->ParameterArray[4];
				for(unsigned int i = 5; i < msg->ParameterArray.size(); i++)
				{
					pMainWindow->AddNick(msg->ParameterArray[i],Channel);
				}
			}
			pMainWindow->OutputMessage(msg);
		}
}



void IncomingMessagesHandling::decodeNonServerNonPrivmsg(Message *msg)
{
	msg->MergeParameterArray();
	pMainWindow->OutputInternalMessage(msg->formatedMessage);
	if(!msg->Nick.compare(CurrentNickname))
	{
		if(!msg->ParameterArray[0].compare("NICK"))
		{
			CurrentNickname = msg->ParameterArray[1];
			SemiSmartDelete(&msg->ParameterArray[0]);
			VectorInsertStuff<string>(msg->ParameterArray,"Nickname has been changed to: ",2);
			VectorInsertStuff<string>(msg->ParameterArray,msg->ParameterArray[1],3);
			msg->MergeParameterArray();
			pMainWindow->OutputMessage(msg);
		}else if(!msg->ParameterArray[0].compare("JOIN"))
		{
			VectorInsertStuff<string>(msg->ParameterArray,"You have joined the Channel: ",2);
			VectorInsertStuff<string>(msg->ParameterArray,msg->ParameterArray[1],3);
//			msg->ParameterArray[3] = charAllocAndSectionCopy(msg->ParameterArray[1]);
			pMainWindow->OutputMessage(msg);
		}
	}else{
		if(!msg->ParameterArray[0].compare("NICK"))
		{
			//snprintf(msg,499,"-!Nickchanges * :%s has changed his Nickname to: %s\r\n",oldNick, newNick);

			VectorInsertStuff<string>(msg->ParameterArray,"has changed their Nickname to:",3);
			VectorInsertStuff<string>(msg->ParameterArray,msg->ParameterArray[1],4);
			pMainWindow->OutputMessage(msg);
		}else if(!msg->ParameterArray[0].compare("JOIN")){
			//snprintf(msg,499,"%s * :%s has joined the Channel\r\n",channelName,Nick);
			VectorInsertStuff<string>(msg->ParameterArray,msg->Nick,2);
			VectorInsertStuff<string>(msg->ParameterArray,"has joined the Channel.",3);
			pMainWindow->OutputMessage(msg);
			pMainWindow->AddNick(msg->Nick,msg->ParameterArray[1]);
		}else if(!msg->ParameterArray[0].compare("QUIT")){
			//snprintf(msg,499,"-!Quitmessages * :%s has terminated his connection, reason: %s\r\n",Nick, Quitreason);	
			msg->MergeParameterArray(2);
			VectorInsertStuff<string>(msg->ParameterArray,msg->Nick,2);
			VectorInsertStuff<string>(msg->ParameterArray,"has terminated their connection, reason:",3);
			VectorInsertStuff<string>(msg->ParameterArray,msg->formatedMessage,4);
			pMainWindow->OutputMessage(msg);
			pMainWindow->RemoveNick(msg->Nick);
		}else if(!msg->ParameterArray[0].compare("PART")){
			msg->MergeParameterArray(2);
			//snprintf(msg,499,"%s * :%s has left the Channel %s \r\n",channelName,Nick, reason);
			VectorInsertStuff<string>(msg->ParameterArray,msg->Nick,2);
			VectorInsertStuff<string>(msg->ParameterArray,"has left the Channel, reason:",3);
			VectorInsertStuff<string>(msg->ParameterArray,msg->formatedMessage,4);
			pMainWindow->OutputMessage(msg);
			pMainWindow->RemoveNick(msg->Nick,msg->ParameterArray[1]);
		}
	}

}

void IncomingMessagesHandling::DecodeOutput(char *msg)
{
	Message *newMsg = new Message(msg,false);

	if(newMsg->GetNUHStatus() == false)
	{
		//it's a bloody server message
		decodeServerMessages(newMsg);

	}else{

		//strcmp returns 0 if it is equal. Nonzero -> something else
//		if(strcmp(newMsg->ParameterArray[0],"PRIVMSG") || strcmp(newMsg->ParameterArray[0],"NOTICE")){
		if(!newMsg->isNormalMessage){
			decodeNonServerNonPrivmsg(newMsg);			
		}else{
/*			//CHANNELNAME NAME MESSAGE
			//lets make sure we don't access illegal
			if(newMsg->ArrayCounter >= 3)
			{
			//newMsg->ParameterArray[1] == Channel Name
			//newMsg->ParameterArray[2] == Message
			int length = strlen(newMsg->ParameterArray[1])+strlen(newMsg->Nick)+strlen(newMsg->ParameterArray[2])+3; //+3 because ' ', ' ' and '\0'
			newMsg->formatedMessage = new char[length];
			snprintf(newMsg->formatedMessage,length,"%s %s %s",newMsg->ParameterArray[1],newMsg->Nick,newMsg->ParameterArray[2]);
			}*/
			pMainWindow->OutputMessage(newMsg);
		}
	}
	//for now going to be doing this
	delete newMsg;
}


void IncomingMessagesHandling::OutputHandling()
{
	int numbytes = 0;
	
	
	count++;
	if(count > 100000) count = 1000;

	*msg = '\0';

	WSAAsyncSelect(pMainWindow->GetSocket(),g_pWindow,0,0);

	recv(pMainWindow->GetSocket(),msg,MAXIRCMSGLENGTH,MSG_PEEK);
	char *Offset = strstr(msg,"\r\n");
	if(Offset != NULL)
	{
		numbytes = Offset - msg + 2;
	}else{
		if(WSAGetLastError() == WSAEMSGSIZE) {
			numbytes = MAXIRCMSGLENGTH;
		}
	}

	//this probably ought to get some catching or stuff
	WSAAsyncSelect(pMainWindow->GetSocket(),g_pWindow,WM_SOCKET,(FD_CLOSE|FD_READ));

	recv(pMainWindow->GetSocket(),msg,numbytes,0);
//		numbytes++;
	msg[numbytes]='\0';
	//std::cout << msg << std::endl;

	if(numbytes > 0) {
		DecodeOutput(msg);
		//we got a connection, so lets start ticking down its lifetime
		if(!SetTimer(g_pWindow,CONNECTIONTIMERID,300000,NULL))
		{
			//timer couldn't be set, do some error handling
			//just toss out a "timer couldn't be set msg"
			pMainWindow->OutputInternalMessage("INTERNAL_MESSAGE Timer could not be set");
		}
	}
	if(count == 10){
			pMainWindow->pReadInit->loadVars();
			pMainWindow->pReadInit->loadVars();
			pMainWindow->pReadInit->MessagesToExecute->ScrollToEnd();
//			std::cout << send(testSocket,msg,strlen(msg),0) << std::endl;
	}

	if(!pMainWindow->pReadInit->MessagesToExecute->IsQueueEmpty())
	{
		pMainWindow->SendMessage(pMainWindow->pReadInit->MessagesToExecute->GetCurrentLine());
		pMainWindow->pReadInit->MessagesToExecute->RemoveOldestMessage();
	}

}

void IncomingMessagesHandling::ResetCount()
{
	count = 0;
}

IncomingMessagesHandling::IncomingMessagesHandling(MainWindow *Parent, string sNick, string sUser, string sRealname)
{
	pMainWindow = Parent;
	this->nick = sNick;
	this->user = sUser;
	this->realname = sRealname;
	count = 0;
	CurrentNickname = nick;
}

IncomingMessagesHandling::~IncomingMessagesHandling()
{

}