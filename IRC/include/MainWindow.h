#ifndef __MAINWINDOW_GUARD__
#define __MAINWINDOW_GUARD__


#include "Globals.h"
#include "SocketHandling.h"
#include "ChannelHandler.h"
#include "FileWriting.h"
#include "IncomingMessagesHandling.h"
#include "Input.h"
#include "readinit.h"


class MainWindow
{
private:
	SocketHandling *pSocket;
	ChannelHandler *pChannelHandler;
	FileWriting *pFileWriting;
	IncomingMessagesHandling *pIncomingMessagesHandling;
	Input *pInput;


protected:
	HINSTANCE pInstance;
	HWND pWindow;
	HDC pHDC;
	HDC pBufferHDC;
	int iWindowWidth;
	int iWindowHeight;



public:
	MainWindow(HINSTANCE g_pInstance, HWND g_pWindow);
	~MainWindow();
	bool OpenConnection();
	void InitProgram();
	void OutputInternalMessage(char *msg);
	void OutputMessage(Message *msg);
	void DrawAll();
	void ReadMessage();
	SOCKET GetSocket();
	char *GetSelectedChannelName();
	void SocketClosedMessage();
	void SendMessage(char *msg);
	void SelectChannel(char *Name);
	void CloseChannel(char *Name);
	void CloseAll();
	void ScrollChannelUp(int Lines);
	void ScrollChannelDown(int Lines);
	void ScrollUp(char *ChannelName, int Lines);
	void ScrollDown(char *ChannelName, int Lines);
	void ScrollToStart(char *ChannelName);
	void ScrollToEnd(char *ChannelName);
	void ScrollOneWindow(char *ChannelName, bool Up);
	void CopyCurrentLine();

	void AddNick(char *Nick, char *Channel);
	void RemoveNick(char *Nick, char *Channel = NULL);
	void ShowUserList();

	void SwapBuffers();

	ReadInit *pReadInit;
};


#endif