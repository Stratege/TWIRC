#include "IncomingMessagesHandling.h"
#include "MainWindow.h"

const char *reasonString = ", reason: ";

bool IncomingMessagesHandling::PingStuff(Message *msg)
{
	if(StringBeginEqual(msg->GetFormatedMessage(),"PING"))
	{
		//Ping works, no need to confirm it anymore. Also fairly sure that can only be send by the server
		//OutputMessage(msg,0);
		msg->formatedMessage[1] = 'O';
		appendNewLine(&msg->formatedMessage);
		send(pMainWindow->GetSocket(),msg->formatedMessage,strlen(msg->formatedMessage),0);
		return true;
	}else return false;
}

void IncomingMessagesHandling::decodeServerMessages(Message *msg)
{
		if(!PingStuff(msg))
		{
			//it's not a ping
			//currently that means, just print it
			if(!strcmp(msg->ParameterArray[0],RPL_NAMREPLY))
			{
				char *Channel = msg->ParameterArray[3];
				for(int i = 4; i < msg->ParameterArray.size(); i++)
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
	if(!strcmp(msg->Nick,CurrentNickname))
	{
		if(!strcmp(msg->ParameterArray[0],"NICK"))
		{
			CurrentNickname = charAllocAndSectionCopy(msg->ParameterArray[1]);
			SemiSmartDelete(&msg->ParameterArray[0]);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy("Nickname has been changed to: "),2);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->ParameterArray[1]),3);
			msg->MergeParameterArray();
			pMainWindow->OutputMessage(msg);
		}else if(!strcmp(msg->ParameterArray[0],"JOIN"))
		{
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy("You have joined the Channel: "),2);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->ParameterArray[1]),3);
//			msg->ParameterArray[3] = charAllocAndSectionCopy(msg->ParameterArray[1]);
			pMainWindow->OutputMessage(msg);
		}
	}else{
		if(!strcmp(msg->ParameterArray[0],"NICK"))
		{
			//snprintf(msg,499,"-!Nickchanges * :%s has changed his Nickname to: %s\r\n",oldNick, newNick);

			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->Nick),2);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy("has changed their Nickname to:"),3);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->ParameterArray[1]),4);
			pMainWindow->OutputMessage(msg);
		}else if(!strcmp(msg->ParameterArray[0],"JOIN")){
			//snprintf(msg,499,"%s * :%s has joined the Channel\r\n",channelName,Nick);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->Nick),2);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy("has joined the Channel."),3);
			pMainWindow->OutputMessage(msg);
			pMainWindow->AddNick(msg->Nick,msg->ParameterArray[1]);
		}else if(!strcmp(msg->ParameterArray[0],"QUIT")){
			//snprintf(msg,499,"-!Quitmessages * :%s has terminated his connection, reason: %s\r\n",Nick, Quitreason);	
			msg->MergeParameterArray(2);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->Nick),2);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy("has terminated their connection, reason:"),3);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->formatedMessage),4);
			pMainWindow->OutputMessage(msg);
			pMainWindow->RemoveNick(msg->Nick);
		}else if(!strcmp(msg->ParameterArray[0],"PART")){
			msg->MergeParameterArray(2);
			//snprintf(msg,499,"%s * :%s has left the Channel %s \r\n",channelName,Nick, reason);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->Nick),2);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy("has left the Channel, reason:"),3);
			VectorInsertStuff<char>(msg->ParameterArray,charAllocAndSectionCopy(msg->formatedMessage),4);
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


/*	//TODO: rewrite*
	//holy shit this code is in dire need of rewriting
	int nameLength = FindSign(msg,'!');
	nameLength--;
	if(nameLength <= 0)
	{
		//server message
		decodeServerMessages(msg);
	}else
	{
		//what we recieve:
		//:NAME!...stuff...PRIVMSG CHANNELNAME :MESSAGE
		//what we want to display:
		//CHANNELNAME NAME :MESSAGE
		
		//set the name
//		char *Name = new char[NameLength+1];
//		memcpy(Name,&msg[1],NameLength);
//		Name[NameLength+1] = '\0';

		int ChannelOffset = FindSign(msg,' ');
		ChannelOffset++;
		if(!StringCompare(&msg[ChannelOffset],"PRIVMSG")){
			//not a PRIVMSG? hand it over to the next function
			decodeNonServerNonPrivmsg(msg,ChannelOffset);
		}else{
			ChannelOffset += FindSign(&msg[ChannelOffset],' ');
			ChannelOffset++;

			int ChannelNameLength = FindSign(&msg[ChannelOffset],' ');
	//		char *ChannelName = new char[ChannelNameLength+1];
	//		memcpy(ChannelName,&msg[ChannelOffset],ChannelNameLength);
	//		ChannelName[ChannelNameLength+1] = '\0';

			int MessageOffset = ChannelOffset + FindSign(&msg[ChannelOffset],':') + 1;
			int MessageLength = strlen(msg)-MessageOffset;

			if((ChannelOffset <= -1 ) || (MessageOffset <= -1) || (MessageLength <= -1) || (ChannelNameLength <= -1) || (nameLength <= -1))
			{
				//something went wrong, just print the message
				pMainWindow->OutputMessage(msg,0);
			}else{
				char *msg2 = new char[ChannelNameLength+MessageLength+nameLength+10];

				//CHANNELNAME NAME MESSAGE
				memcpy(msg2,&msg[ChannelOffset],ChannelNameLength);
				msg2[ChannelNameLength] = ' ';
				memcpy(&msg2[ChannelNameLength+1],&msg[1],nameLength);
				msg2[ChannelNameLength+nameLength+1] = ':';
				msg2[ChannelNameLength+nameLength+2] = ' ';
				msg2[ChannelNameLength+nameLength+3] = ':';
				memcpy(&msg2[ChannelNameLength+nameLength+4],&msg[MessageOffset],MessageLength);
				msg2[ChannelNameLength+nameLength+MessageLength+4] = '\0';

				pMainWindow->OutputMessage(msg2,0);
//				std::cout << " - " << SelectedChannel;
				
				delete msg2;
		//		delete ChannelName;
		//		delete Name;
			}
		}
	}
}*/

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

IncomingMessagesHandling::IncomingMessagesHandling(MainWindow *Parent, char *sNick, char *sUser, char *sRealname)
{
	pMainWindow = Parent;
	this->nick = sNick;
	this->user = sUser;
	this->realname = sRealname;
	msg = new char[500];
	count = 0;
	strncpy(CurrentNickname,nick,30);
}

IncomingMessagesHandling::~IncomingMessagesHandling()
{

}