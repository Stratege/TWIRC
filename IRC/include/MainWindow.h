#ifndef __MAINWINDOW_GUARD__
#define __MAINWINDOW_GUARD__


#include "Globals.h"
#include "SocketHandling.h"
#include "ChannelHandler.h"
#include "FileWriting.h"
#include "IncomingMessagesHandling.h"
#include "Input.h"
#include "readinit.h"

#ifdef SERVER_ON
#include "Serverstuff.h"
#endif

class MainWindow
{
private:
	SocketHandling *pSocket;
	ChannelHandler *pChannelHandler;
	FileWriting *pFileWriting;
	IncomingMessagesHandling *pIncomingMessagesHandling;
	Input *pInput;
#ifdef SERVER_ON
	ServerCF *pServer;
#endif

protected:
	HINSTANCE pInstance;
	HWND pWindow;
	HDC pHDC;
	HDC pBufferHDC;
	int iWindowWidth;
	int iWindowHeight;



public:
	MainWindow::MainWindow(HINSTANCE g_pInstance, HWND g_pWindow);
	~MainWindow();
	void InitProgram();
	void MainWindow::OutputInternalMessage(char *msg);
	void MainWindow::OutputMessage(Message *msg);
	void DrawAll();
	void ReadMessage();
	SOCKET GetSocket();
	char *MainWindow::GetSelectedChannelName();
	void SocketClosedMessage();
	void SendMessage(char *msg);
	void SelectChannel(char *Name);
	void CloseChannel(char *Name);
	void CloseAll();
	void MainWindow::ScrollChannelUp(int Lines);
	void MainWindow::ScrollChannelDown(int Lines);
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