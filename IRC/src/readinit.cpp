/**
 * 
 * \author based on code provided by Martin Felis <martin@silef.de>
 */


#include <assert.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "readinit.h"
#include "DrawingHandler.h"

ReadInit::ReadInit(){
	MessagesToExecute = new MessageBoxCF("ReadInit");
	User = new char[256];
	Nick = new char[256];
	Realname = new char[256];
	Savepath = new char[500];
	Serveraddress = new char[500];
	
	this->HasBeenRead = false;
	WindowHeight = 0;
	WindowWidth = 0;
	Port = 0;
	BackgroundColor = 0;

	this->UserSet = false;
	NickSet = false;
	RealnameSet = false;
	SavepathSet = false;
	ServeraddressSet = false;
}

ReadInit::~ReadInit(){
	delete MessagesToExecute;
	delete User;
	delete Nick;
	delete Realname;
	delete Savepath;
}

void ReadInit::stringToChar(const std::string &s, char *msg){
	//std::copy(s.begin(), s.end(), msg);
//	memcpy(msg,s.begin(),s.size());
	strncpy(msg,s.c_str(),399);
	msg[s.size()] = '\0';
}

bool ReadInit::stringToInt(const std::string &s, int &i){
	std::istringstream myStream(s);
	return (myStream>>i);
}

bool ReadInit::stringHexToIntColor(const std::string &s, int &i){
	std::stringstream myStream;
	myStream << std::hex << s;
	if(myStream>>i)
	{
		//now we need to swap the int around
		//because fucking hell this is stupid:
		//colorref is safed as 0x00bbggrr
		//while the typical way to write a color is 0xrrggbb
		byte red = (i & 0xFF0000) >> 16;
		byte green = (i & 0xFF00) >> 8;
		byte blue = i & 0xFF;
		i = RGB(red,green,blue);
		return true;
	}else return false;
}




char *ReadInit::GetUser(){
	if(!HasBeenRead) this->loadVars();
	if(!UserSet) return "DefaultTwIRCUser";
	return User;
}

char *ReadInit::GetNick(){
	if(!HasBeenRead) this->loadVars();
	if(!NickSet) return "DefaultTwIRCNick";
	return Nick;
}

char *ReadInit::GetRealname(){
	if(!HasBeenRead) this->loadVars();
	if(!RealnameSet) return "Not a Real Name";
	return Realname;
}

char *ReadInit::GetSavepath(){
	if(!HasBeenRead) this->loadVars();
	if(!SavepathSet) return "VOID";
	return Savepath;
}

char *ReadInit::GetServeraddress(){
	if(!HasBeenRead) this->loadVars();
	if(!ServeraddressSet) return "localhost";
	return Serveraddress;
}

int ReadInit::GetBackgroundColor(){
	if(!HasBeenRead) this->loadVars();
	return BackgroundColor;
}

short ReadInit::GetPort(){
	if(!HasBeenRead) this->loadVars();
	if(Port == 0) return 6667;
	return Port;
}

int ReadInit::GetWindowHeight(){
	if(!HasBeenRead) this->loadVars();
	if(WindowHeight == 0) return 1280;
	return WindowHeight;
}

int ReadInit::GetWindowWidth(){
	if(!HasBeenRead) this->loadVars();
	if(WindowHeight == 0) return 1024;
//	this->HasBeenRead = true;
	return WindowWidth;
}


void ReadInit::loadVars(){
	//dateiname__
	char name[]="config.ini";
	std::ifstream file;
    char     zeile[1024];
	this->HasBeenRead = true;
    // Datei öffnen
    file.open(name, std::ios::in);

	this->MessagesToExecute->ClearMessagebox();

    if (file.good()){

        // Wenn die Datei geoeffnet werden konnte...
        // An den Anfang der Datei springen
        file.seekg(0L, std::ios::beg);
		char msg[400];
        while (! file.eof()){
            int sizehelper = 0;
            float heighthelper = 800;
            float widthhelper = 1000;
            std::stringstream str;

            file.getline(zeile, 1024);
            if(zeile[0]=='#') continue;
            str << zeile;
            int trenner = str.str().find(":",0);
		if(trenner != -1)
		{
		 stringToChar(str.str().erase(0,trenner+2),msg);
              	if(str.str().erase(trenner).compare("SEND") == 0)
					this->MessagesToExecute->AddMessage(msg);
				else if(str.str().erase(trenner).compare("USER") == 0)
				{
					if(strlen(msg) < 256)
					{
						strncpy(User,msg,255);
						User[strlen(msg)] = '\0';
						UserSet = true;
					}else{
						//todo: error
					}
				}else if(str.str().erase(trenner).compare("NICK") == 0)
				{
					if(strlen(msg) < 256)
					{
						strncpy(Nick,msg,255);
						Nick[strlen(msg)] = '\0';
						NickSet = true;
					}else{
						//todo: error
					}
				}else if(str.str().erase(trenner).compare("REALNAME") == 0)
				{
					if(strlen(msg) < 256)
					{
						strncpy(Realname,msg,255);
						Realname[strlen(msg)] = '\0';
						RealnameSet = true;
					}else{
						//todo: error
					}
				}else if(str.str().erase(trenner).compare("SAVEPATH") == 0)
				{
					if(strlen(msg) < 500)
					{
						strncpy(Savepath,msg,499);
						Savepath[strlen(msg)] = '\0';
						SavepathSet = true;
					}else{
						//todo: error
					}
				}else if(str.str().erase(trenner).compare("SERVER") == 0)
				{
					if(strlen(msg) < 500)
					{
						strncpy(Serveraddress,msg,499);
						Serveraddress[strlen(msg)] = '\0';
						ServeraddressSet = true;
					}else{
						//todo: error
					}
				}else if(str.str().erase(trenner).compare("WINDOWHEIGHT") == 0)
				{
					stringToInt(str.str().erase(0,trenner+2),WindowHeight);
				}else if(str.str().erase(trenner).compare("WINDOWWIDTH") == 0)
				{
					stringToInt(str.str().erase(0,trenner+2),WindowWidth);
				}else if(str.str().erase(trenner).compare("PORT") == 0)
				{
					stringToInt(str.str().erase(0,trenner+2),Port);
				}else if(str.str().erase(trenner).compare("TEXTCOLOR") == 0)
				{
					int TextColor;

					if(g_pDrawingHandler == 0)
					{
						new DrawingHandler();
					}
					if(g_pDrawingHandler != 0)
					{
						stringHexToIntColor(str.str().erase(0,trenner+2),TextColor);
						g_pDrawingHandler->SetNewTextColor(TextColor);
					}
				}else if(str.str().erase(trenner).compare("BACKGROUNDCOLOR") == 0)
				{
					if(g_pDrawingHandler == 0)
					{
						new DrawingHandler();
					}
					if(g_pDrawingHandler != 0)
					{
						stringHexToIntColor(str.str().erase(0,trenner+2),BackgroundColor);
						g_pDrawingHandler->SetNewBackgroundColor(BackgroundColor);
					}
				}



				/*
				F-it, I'm tired. For now everything else is deprecated.


              	else if(str.str().erase(trenner).compare("Collision_Radius") == 0)

				Boid::mCRadius = number;
            	else if(str.str().erase(trenner).compare("Acceleration_Magnitude") == 0)
            		Boid::mAccelerationMagnitude = Boid::mBrakeMagnitude = number;
/*            	else if(str.str().erase(trenner).compare("Höhe") == 0) {
					if (ViewInstance.GetHeight() != number){
						sizehelper = 1;
					};
					heighthelper = ViewInstance.GetHeight();
            	}
            	else if(str.str().erase(trenner).compare("Breite") == 0){
					if (ViewInstance.GetWidth() != number){
						sizehelper = 1;
					};
					widthhelper = ViewInstance.GetWidth();
            	}*/
				

            }
/*            if (sizehelper = 1) {
            	ViewInstance.OnResize(widthhelper, heighthelper);
            	sizehelper = 0;
            }
*/        }
    }else{
        // Wenn die Datei nicht geoeffnet werden konnte
//        cout << "Datei \"vars.dat\" nicht gefunden." << endl;
    }
}