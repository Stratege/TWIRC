#ifndef __FILEWRITING_GUARD__
#define __FILEWRITING_GUARD__

#include "Globals.h"
#include "OtherFunctions.h"
#include "FindFunctions.h"

class FileWriting
{
private: 

	char *saveDirectory;
	char* savePath;

public: 
	void WriteTimeAndDate(char *msgToWriteInto);
	void WriteNickname(char *msgToWriteInto, char *Nickname);
	void WriteMessage(char *msgToWriteInto, char *originalMsgToFilter);

	void PrintOutputToFile(char *PrintChannelName,char *msg); /// \fn Prints msg into File specified by PrintName.
	FileWriting(char *givenDirectory);
	~FileWriting();
};

#endif