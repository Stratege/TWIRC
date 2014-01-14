#include "FileWriting.h"



void FileWriting::WriteTimeAndDate(char *msgToWriteInto)
{
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	//writes in the style of: "hh:mm:ss dd.mm.yy - "
	strftime(msgToWriteInto,650,"%H:%M:%S %d.%m.%y - ",timeinfo);
}

void FileWriting::WriteNickname(char *msgToWriteInto, char *Nickname)
{
	//write the nick
	//append ": " so it looks neat
	snprintf(msgToWriteInto,650,"%s%s: ",msgToWriteInto,Nickname);
}


void FileWriting::WriteMessage(char *msgToWriteInto, char *originalMsgToFilter)
{
	int Offset = strcspn(originalMsgToFilter," ");
	Offset++;
	Offset += strcspn(&originalMsgToFilter[Offset]," ") +2;
	snprintf(msgToWriteInto,650,"%s%s",msgToWriteInto,&originalMsgToFilter[Offset]);
}

void FileWriting::PrintOutputToFile(char *PrintChannelName,char *msg)
{
	//strcmp returns 0 if strings are equal. != 0 means it isn't equal.
	if(strcmp(this->saveDirectory,"VOID"))
	{
		snprintf(savePath,999,"%s%s/%s.log",saveDirectory,CurrentNickname,PrintChannelName);
		FILE *logFile = NULL;
		try{
			logFile = fopen(savePath,"ab");
		}
		catch(int e){
			CreateDirectory(saveDirectory,NULL);
			snprintf(savePath,999,"%s%s",saveDirectory,CurrentNickname);
			if(!CreateDirectory(savePath,NULL))
			{
				snprintf(savePath,999,"%s%s/%s.log",saveDirectory,CurrentNickname,PrintChannelName);
				logFile = fopen(savePath,"ab");
			}
		}
		if(logFile != NULL)
		{
			fwrite(msg,strlen(msg),1,logFile);
			fclose(logFile);
		}
	}
}

FileWriting::FileWriting(char *givenDirectory)
{
	if(strcmp(givenDirectory,"VOID"))
	{
		//savepath hasn't been set, this means we'll disable saving, currently this is done via setting it to void
		this->saveDirectory = new char[5];
		strncpy(this->saveDirectory,"VOID\0",5);
		//todo: send msg informing the client that loading has been disabled
	}else{
		this->saveDirectory = new char[500];
		strncpy(this->saveDirectory,givenDirectory,499);
		this->saveDirectory[strlen(givenDirectory)] = '\0';
	}
	this->savePath = new char[1000];
}

FileWriting::~FileWriting()
{
	delete(saveDirectory);
	delete(savePath);
}