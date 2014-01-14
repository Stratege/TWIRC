#ifndef __GLOBALS_GUARD__
#define __GLOBALS_GUARD__

#define WM_SOCKET (WM_USER + 1)


#define LEFTBARSIZE 200

//for the timer
#define CONNECTIONTIMERID 301


#include <windows.h>
//#include <windowsx.h>

#include <string>
#include <list>
//#include <map>
//#include <exception>
#include <time.h>
#include <WinSock.h>
//turns out I don't need threading when I do proper message handling. Who knew?
//#include <boost/thread.hpp>
#include <stdio.h>
#if _MSC_VER
#define snprintf sprintf_s
#define strncpy(dest,source,size) strcpy_s(dest,size,source)

#define strtime(time) _strtime(time)
#define strdate(time) _strdate(time)
#endif


//enable this if for some crypticpurpose logging needs to be off
//#define DisableTextLogging

#define UP true
#define DOWN false

class MainWindow;

extern MainWindow *g_pMainWindow;

extern HINSTANCE g_pInstance;
extern HWND g_pWindow;
extern int g_iHeight;
extern int g_iWidth;
extern HDC g_pBackbufferDC;
extern HDC g_pFrontbufferDC;
extern HBITMAP g_pBackbufferBitmap;

extern char *CurrentTime;
extern char *CurrentDate;
//extern char *SelectedChannel;
extern char *CurrentNickname;

static char *g_pWindowName = "IRC";

struct MessageMetaInfo
{
	bool isPinged;
//	int 
};


#define MAXIRCMSGLENGTH 512

//new feature: defines




//defines given by the IRC Protocol:
#define RPL_NAMREPLY "353"
#define RÜL_ENDOFNAMES "366"



#endif