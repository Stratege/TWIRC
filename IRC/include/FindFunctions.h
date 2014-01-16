#ifndef __FINDFUNCTIONS_GUARD__
#define __FINDFUNCTIONS_GUARD__

#include "Globals.h"


#include <string.h>


bool StringFromStringListEqual(string str1,const std::vector<string> strlist2);

bool StringBeginEqual(const string msg, const char *str2);


/*
bool StringCompare(char *str1, char *str2); /// \fn Function compares Str1 and Str2 to be the same till Str2 ends. WARNING HAS SOME ISSUES, SEE CODE DETAILS
int FindString(char *str1, char *str2); /// \fn Function attempts to find Str2 in Str1 and returns offset if found, or -1 if not
int FindSign(char *msg, char sign); /// \fn Function attempts to find sign in msg and returns offset if found, or -1 if not
*/

#endif