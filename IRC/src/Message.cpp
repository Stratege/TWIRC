#include "Message.h"



Message::Message(char *msg, bool Outgoing)
{
	this->isNormalMessage = false;
	this->Outgoing = Outgoing;
	this->HasNickUserHost = false;
	this->Nick = NULL;
	this->User = NULL;
	this->Host = NULL;
//	this->Command = NULL;
	this->formatedMessage = NULL;
	if(this->Outgoing)
	{
		ParseOutgoingMessage(msg);
	}else{
		ParseIncomingMessage(msg);
	}
}

Message::~Message()
{
	SemiSmartDelete(&this->Nick);
	SemiSmartDelete(&this->User);
	SemiSmartDelete(&this->Host);
//	SemiSmartDelete(this->Command);
/*	for(register int i = 0; i < this->ParameterArray.size(); i++)
		{
			SemiSmartDelete((this->ParameterArray)[i]);
			this->ParameterArray.pop_back();
		}
	}*/
	while(!this->ParameterArray.empty())
	{
		SemiSmartDelete(&this->ParameterArray[this->ParameterArray.size()-1]);
		this->ParameterArray.pop_back();
	}
}

void Message::ParseParams(char *msg)
{
	int Offset = 0;
	int length = 0;
	do
	{
		char *newPara;
		if(msg[Offset] == ':')
		{
			Offset++;
		}
			length = CopyTillSymbol(&newPara,&msg[Offset],' ');
			if(length >= 0)
			{
				Offset += length + 1; //+1 because symbol
			}else{
				newPara = charAllocAndSectionCopy(&msg[Offset]);
			}
			this->ParameterArray.insert(this->ParameterArray.end(),newPara);
/*		}else{
			Offset++; //because we don't want the :
			CopyTillSymbol(&newPara,&msg[Offset],'\0');
			length = -1;
			this->ParameterArray.insert(this->ParameterArray.end(),newPara);
		}*/
	}while(length >= 0);
}

void Message::ParseOutgoingMessage(char *msg)
{
	//messages are either:
	// /command stuff
	//or:
	// msg
	//in the later case it's a privmsg, in the former it is not
	if(!strcspn(msg,"/"))
	{
			this->isNormalMessage = false;
			ParseParams(&msg[1]);
//			this->Command = charAllocAndSectionCopy(this->ParameterArray[0]);
	}else{
			this->isNormalMessage = true;
			CopyTillSymbol(&this->formatedMessage,msg,'\0');
//			this->Command = charAllocAndSectionCopy("PRIVMSG");
	}
}



void Message::ParseIncomingMessage(char *msg)
{
	//messages go in as :NICK!USER@HOST TYPE PARAMETERS
	//whereby PARAMETERS might be lead by a : to indicate that it's multiple words
	//and PARAMETERS might be anything like target channel or target name or a command or text or shit

	//or they can go in as COMMAND PARAMETER
	//whereby PARAMETER can be fully optional

	int Offset = 0;
	int length = 0;
	//let's get rid of the stupid \r\n
	stripNewLine(msg);

	if(msg[0] == ':')
	{
		Offset++;
		this->HasNickUserHost = true;
		length = CopyTillSymbolIfNotOtherSymbolFirst(&this->Nick,&msg[1],'!',' ');
		if(length == -1) goto ThingsWentBad;
		Offset += length + 1; //+1 because symbol
		length = CopyTillSymbolIfNotOtherSymbolFirst(&this->User,&msg[Offset],'@',' ');
		if(length == -1) goto ThingsWentBad;
		Offset += length + 1; //+1 because symbol
		length = CopyTillSymbol(&this->Host,&msg[Offset],' ');
		if(length == -1) goto ThingsWentBad;
		Offset += length + 1; //+1 because symbol

		ParseParams(&msg[Offset]);

	}else{
		//I know this isn't strictly correct, but it does the job
		goto ThingsWentBad;
	}

	if(StringBeginEqual(this->ParameterArray[0],"PRIVMSG"))	this->isNormalMessage = true;
	else this->isNormalMessage = false;

	return;



	ThingsWentBad:
	//wtf?
	this->HasNickUserHost = false;
	//shit is weird so we just toss out unparsed
	formatedMessage = new char[strlen(msg)+1];
	memcpy(formatedMessage,msg,strlen(msg)+1);
	Offset = strcspn(msg," ")+1;
	ParseParams(&msg[Offset]);
	return;
}

bool Message::GetNUHStatus()
{
	return this->HasNickUserHost;
}

char *Message::GetFormatedMessage()
{
	return this->formatedMessage;
}

void Message::MergeParameterArray(int begin, int end)
{
	if(end == 0) end = this->ParameterArray.size();
	else
	{
		//throw or not throw?
		if(end > this->ParameterArray.size()) end = this->ParameterArray.size();
	}

	SemiSmartDelete(&this->formatedMessage);
	int length = 0;
	for(int i = begin; i < end; i++)
	{
		length += strlen(this->ParameterArray[i])+1;
	}

	this->formatedMessage = new char[length+1];
	this->formatedMessage[0] = '\0';

	if(end > begin)
	{
		strcpy(this->formatedMessage,this->ParameterArray[begin]);

		for (int i = begin+1; i < end; i++)
		{
			strcat(this->formatedMessage," ");
			strcat(this->formatedMessage,this->ParameterArray[i]);
		}
	}
}