#ifndef __FILEWRITING_GUARD__
#define __FILEWRITING_GUARD__

#include "Globals.h"
#include "OtherFunctions.h"
#include "FindFunctions.h"

class FileWriting
{
private: 

	string saveDirectory;
	string savePath;

public: 
	void WriteTimeAndDate(string &msgToWriteInto);
	void WriteNickname(string &msgToWriteInto, const string Nickname);
	void WriteMessage(string &msgToWriteInto, const string originalMsgToFilter);
	
	void PrintOutputToFile(const string PrintChannelName,const string msg); /// \fn Prints msg into File specified by PrintName.
	FileWriting(string givenDirectory);
	~FileWriting();
};

#endif