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
	void OutputInternalMessage(string msg);
	void OutputMessage(Message *msg);
	void DrawAll();
	void ReadMessage();
	SOCKET GetSocket();
	string GetSelectedChannelName();
	void SocketClosedMessage();
	void SendMessage(string msg);
	void SelectChannel(string Name);
	void CloseChannel(string Name);
	void CloseAll();
	void ScrollChannelUp(int Lines);
	void ScrollChannelDown(int Lines);
	void ScrollUp(string ChannelName, int Lines);
	void ScrollDown(string ChannelName, int Lines);
	void ScrollToStart(string ChannelName);
	void ScrollToEnd(string ChannelName);
	void ScrollOneWindow(string ChannelName, bool Up);
	void CopyCurrentLine();

	void AddNick(string Nick, string Channel);
	void RemoveNick(string Nick, string Channel = "");
	void ShowUserList();

	void Resize(int newWidth, int newHeight);

	void SwapBuffers();

	ReadInit *pReadInit;
};


#endif