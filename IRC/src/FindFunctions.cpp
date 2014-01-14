#include "FindFunctions.h"

bool StringFromStringListEqual(char *str1, char *strlist2[])
{
	for(int i = 0; strlist2[i] != 0; i++)
	{
		if(!strcmp(str1,strlist2[i])) return true;
	}
	return false;
}

bool StringBeginEqual(char *str1, char *str2)
{
	return (str1 == strstr(str1,str2));
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