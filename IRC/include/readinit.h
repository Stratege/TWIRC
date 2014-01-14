#include <limits>
#include <string>
#include <vector>
#include <map>

#include <assert.h>
#include "messages.h"

class ReadInit{
private:
	char *User;
	char *Nick;
	char *Realname;
	char *Savepath;
	char *Serveraddress;
	int WindowHeight;
	int WindowWidth;
	int Port;
	int BackgroundColor;
	bool HasBeenRead;
	bool UserSet;
	bool NickSet;
	bool RealnameSet;
	bool SavepathSet;
	bool ServeraddressSet;
	void ReadInit::stringToChar(const std::string &s, char *msg);
	bool ReadInit::stringToInt(const std::string &s, int &i);
	bool ReadInit::stringHexToIntColor(const std::string &s, int &i);


public:
	ReadInit::ReadInit();
	ReadInit::~ReadInit();
	void ReadInit::loadVars();
	char *ReadInit::GetUser();
	char *ReadInit::GetNick();
	char *ReadInit::GetRealname();
	char *ReadInit::GetSavepath();
	char *ReadInit::GetServeraddress();
	short ReadInit::GetPort();
	int ReadInit::GetWindowHeight();
	int ReadInit::GetWindowWidth();
	int ReadInit::GetBackgroundColor();

	MessageBoxCF *MessagesToExecute;

};
