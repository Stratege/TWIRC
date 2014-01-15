#include "FindFunctions.h"

bool StringFromStringListEqual(string str1,std::vector<string> strlist2)
{
	for(int i = 0; i < strlist2.size(); i++)
	{
		if(!str1.compare(strlist2[i])) return true;
	}
	return false;
}

bool StringBeginEqual(const string str1, const char *str2)
{
	int length = strlen(str2);
	return !str1.compare(0,length,str2);
}


int FindSign(char *msg, char sign)
{
	unsigned int Counter = 0;
	while(Counter <= strlen(msg))
	{
		if(msg[Counter] == sign) return Counter;
		Counter++;
	}
	return -1;
}