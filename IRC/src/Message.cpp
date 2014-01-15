#include "Message.h"



Message::Message(const string msg, bool Outgoing)
{
	this->isNormalMessage = false;
	this->Outgoing = Outgoing;
	this->HasNickUserHost = false;
/*	this->Nick = NULL;
	this->User = NULL;
	this->Host = NULL;
//	this->Command = NULL;
	this->formatedMessage = NULL;*/
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

void Message::ParseParams(const string msg)
{
	int Offset = 0;
	int length = 0;
	string tempmsg = msg;
	while(Offset != string::npos && tempmsg.size() > 0)
	{
		string newPara;
		if(tempmsg[0] == ':')
		{
//			Offset++;
		}
		
		Offset = tempmsg.find(' ');
		newPara = tempmsg.substr(0,Offset);
		if(Offset != string::npos)
		{
			tempmsg = tempmsg.substr(Offset+1);
		}
		this->ParameterArray.insert(this->ParameterArray.end(),newPara);
/*		}else{
			Offset++; //because we don't want the :
			CopyTillSymbol(&newPara,&msg[Offset],'\0');
			length = -1;
			this->ParameterArray.insert(this->ParameterArray.end(),newPara);
		}*/
	}
}

void Message::ParseOutgoingMessage(const string msg)
{
	//messages are either:
	// /command stuff
	//or:
	// msg
	//in the later case it's a privmsg, in the former it is not
	if(msg.find("/") == 0)
	{
			this->isNormalMessage = false;
			ParseParams(&msg[1]);
//			this->Command = charAllocAndSectionCopy(this->ParameterArray[0]);
	}else{
			this->isNormalMessage = true;
			this->formatedMessage = msg;
			//CopyTillSymbol(&this->formatedMessage,msg,'\0');
//			this->Command = charAllocAndSectionCopy("PRIVMSG");
	}
}



void Message::ParseIncomingMessage(string msg)
{
	//messages go in as :NICK!USER@HOST TYPE PARAMETERS
	//whereby PARAMETERS might be lead by a : to indicate that it's multiple words
	//and PARAMETERS might be anything like target channel or target name or a command or text or shit

	//or they can go in as COMMAND PARAMETER
	//whereby PARAMETER can be fully optional

	int Offset = 0;
	//let's get rid of the stupid \r\n
	stripNewLine(msg);

	if(msg[0] == ':')
	{
		string tempmsg = msg;
		Offset++;
		this->HasNickUserHost = true;

		Offset = tempmsg.find('!');
		if(Offset == string::npos) goto ThingsWentBad;
		this->Nick = tempmsg.substr(0,Offset);
		tempmsg = tempmsg.substr(Offset+1);

		Offset = tempmsg.find('@');
		if(Offset == string::npos) goto ThingsWentBad;
		this->User = tempmsg.substr(Offset);
		tempmsg = tempmsg.substr(Offset+1);


		Offset = tempmsg.find(' ');
		if(Offset == string::npos) goto ThingsWentBad;
		this->Host = tempmsg.substr(Offset);
		tempmsg = tempmsg.substr(Offset+1);


		ParseParams(tempmsg);

	}else{
		//I know this isn't strictly correct, but it does the job
		goto ThingsWentBad;
	}

	if(StringBeginEqual(this->ParameterArray[0],"PRIVMSG") || StringBeginEqual(this->ParameterArray[0],"NOTICE"))	this->isNormalMessage = true;
	else this->isNormalMessage = false;

	return;



	ThingsWentBad:
	//wtf?
	this->HasNickUserHost = false;
	//shit is weird so we just toss out unparsed

	Offset = msg.find(" ")+1;
	ParseParams(msg.substr(Offset));
	return;
}

bool Message::GetNUHStatus()
{
	return this->HasNickUserHost;
}

string Message::GetFormatedMessage()
{
	return this->formatedMessage;
}

void Message::MergeParameterArray(int begin, int end)
{
	this->formatedMessage.clear();

	if(end == 0) end = this->ParameterArray.size();
	else
	{
		//throw or not throw?
		if(end > this->ParameterArray.size()) end = this->ParameterArray.size();
	}
	
	if(end > begin)
	{
		this->formatedMessage += this->ParameterArray[begin];

		for (int i = begin+1; i < end; i++)
		{
			this->formatedMessage += " ";
			this->formatedMessage += this->ParameterArray[i];
		}
	}
}