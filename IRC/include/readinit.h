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
	void stringToChar(const std::string &s, char *msg);
	bool stringToInt(const std::string &s, int &i);
	bool stringHexToIntColor(const std::string &s, int &i);


public:
	ReadInit();
	~ReadInit();
	void loadVars();
	char *GetUser();
	char *GetNick();
	char *GetRealname();
	char *GetSavepath();
	char *GetServeraddress();
	short GetPort();
	int ReadInit::GetWindowHeight();
	int ReadInit::GetWindowWidth();
	int ReadInit::GetBackgroundColor();

	MessageBoxCF *MessagesToExecute;

};
