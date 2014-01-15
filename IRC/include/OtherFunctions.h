#ifndef __OTHERFUNCTIONS_GUARD__
#define __OTHERFUNCTIONS_GUARD__

#include "Globals.h"
#include <vector>


//bool GetTimeAndDate();
char *charAllocAndSectionCopy(char *msg, int length = 0, int extralength = 0);
int CopyTillSymbol(char **target, char *source, char symbol);
int CopyTillSymbolIfNotOtherSymbolFirst(char **target, char *source, char symbol, char othersymbol);
void SemiSmartDelete(void *ThatThatIsToDelete);

bool appendNewLine(string &msg);

bool stripNewLine(string &msg);
//char *stripNewLineAndAlloc(char *msg);


//only declaring the name here lead to linker error. So it's fully here.
template <typename type>
void VectorInsertStuff(std::vector<type> list, type Value, int Position)
{
	while(Position >= list.size())
	{
		list.insert(list.end(), type());
	}
	list[Position] = Value;
}



#endif