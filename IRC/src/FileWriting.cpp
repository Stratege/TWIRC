#include "FileWriting.h"
#include <fstream>


void FileWriting::WriteTimeAndDate(string &msgToWriteInto)
{
	char timeMsg[650];
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	//writes in the style of: "hh:mm:ss dd.mm.yy - "
	strftime(timeMsg,650,"%H:%M:%S %d.%m.%y - ",timeinfo);

	msgToWriteInto = timeMsg;
}

void FileWriting::WriteNickname(string &msgToWriteInto,const string Nickname)
{
	//write the nick
	//append ": " so it looks neat
	msgToWriteInto += Nickname + ": ";
}


void FileWriting::WriteMessage(string &msgToWriteInto,const string originalMsgToFilter)
{
	// figure out why this does what it does
	int Offset = originalMsgToFilter.find(" ");
	Offset++;
	Offset = originalMsgToFilter.find(" ",Offset) +2;
	msgToWriteInto += originalMsgToFilter.substr(Offset);
/*	int Offset = strcspn(originalMsgToFilter," ");
	Offset++;
	Offset += strcspn(&originalMsgToFilter[Offset]," ") +2;
	snprintf(msgToWriteInto,650,"%s%s",msgToWriteInto,&originalMsgToFilter[Offset]);
	*/
}

void FileWriting::PrintOutputToFile(const string PrintChannelName,const string msg)
{
	//strcmp returns 0 if strings are equal. != 0 means it isn't equal.
	if(this->saveDirectory.compare("VOID"))
	{
		savePath = saveDirectory+CurrentNickname+"/"+PrintChannelName+".log";
		std::ofstream logFile;
	
//		FILE *logFile = NULL;
		try{
			logFile.open(savePath,std::ofstream::out | std::ofstream::app);
		}
		catch(int e){
			CreateDirectory(saveDirectory.c_str(),NULL);
			string NickDirectory = saveDirectory+CurrentNickname;
			if(!CreateDirectory(NickDirectory.c_str(),NULL))
			{
				logFile.open(savePath,std::ofstream::out | std::ofstream::app);
			}
		}
		if(logFile.is_open())
		{
			logFile.write(msg.c_str(),msg.size());
			logFile.close();
		}
	}
}

FileWriting::FileWriting(string givenDirectory)
{
	//todo: rewrite this class to be more sensible.

	if(givenDirectory.compare("VOID"))
	{
		//savepath hasn't been set, this means we'll disable saving, currently this is done via setting it to void
		this->saveDirectory = "VOID";
		//todo: send msg informing the client that loading has been disabled
	}else{
		this->saveDirectory = givenDirectory;
	}
}

FileWriting::~FileWriting()
{
}