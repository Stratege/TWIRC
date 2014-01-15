#include "OtherFunctions.h"
#include "FindFunctions.h"



/*
bool GetTimeAndDate()
{


	//sets the global variables of time and date via system time
	if(!_strtime_s(CurrentTime,10))
		if(!_strdate_s(CurrentDate,10))
		{
			//now we do some swapping
			//because we get mm/dd/yyyy
			//and we want dd/mm/yyyy
			char d1, d2, m1, m2;
			d1 = CurrentDate[3];
			d2 = CurrentDate[4];
			m1 = CurrentDate[0];
			m2 = CurrentDate[1];
			CurrentDate[0] = d1;
			CurrentDate[1] = d2;
			CurrentDate[3] = m1;
			CurrentDate[4] = m2;
			return true;
		}
	return false;
}*/

char *charAllocAndSectionCopy(char *msg, int length, int extralength)
{
	if(length == 0) length = strlen(msg);
	char *newString = new char[length+1+extralength];
	memcpy(newString,msg,length);
	newString[length] = '\0';
	return newString;
}

int CopyTillSymbol(char **target, char *source, char symbol)
{
	//copies until it finds a specific symbol. Returns the length it copied.
	//does not copy anything if the symbol is not found in the string (which will cause it to return -1;
	//does EXCLUDE the symbol it finds. so searching for ! in :test!stuff will return :test
	int length = 0;
	length = strcspn(source,&symbol);
	if(length != strlen(source) || symbol == '\0') *target = charAllocAndSectionCopy(source,length);
	else length = -1;
	return length;
}

int CopyTillSymbolIfNotOtherSymbolFirst(char **target, char *source, char symbol, char othersymbol)
{
	//does what CopyTillSymbol does, except it compares if another symbol exists before, returns -1 if so
	int lengthTillSymbol = strcspn(source,&symbol);
	int lengthTillOtherSymbol = strcspn(source,&othersymbol);
	if(lengthTillSymbol > lengthTillOtherSymbol) return -1;
	return CopyTillSymbol(target,source,symbol);
}

void SemiSmartDelete(void *ThatThatIsToDelete)
{
	if(ThatThatIsToDelete != NULL)
	{
		if(*(int*)ThatThatIsToDelete != NULL)
		{
			delete *(int**)ThatThatIsToDelete;
			*(int**)ThatThatIsToDelete = NULL;
		}
	}
}

bool stripNewLine(char *msg)
{
	int length = strlen(msg);
	if(msg[length-2] == '\r' && msg[length-1] == '\n')
	{
		msg[length-2] = '\0';
		return true;
	}
	return false;
}

char *stripNewLineAndAlloc(char *msg)
{
	stripNewLine(msg);
	return charAllocAndSectionCopy(msg);
}

bool appendNewLine(char **msg)
{
	int length = strlen(*msg);
	if((*msg)[length-2] == '\r' && (*msg)[length-1] == '\n') return false;
	else
	{
		char *newMsg = charAllocAndSectionCopy(*msg,length,2);
		newMsg[length] = '\r';
		newMsg[length+1] = '\n';
		newMsg[length+2] = '\0';
		delete *msg;
		*msg = newMsg;
		return true;
	}
}
