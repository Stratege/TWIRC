#include "Globals.h"

HINSTANCE g_pInstance;
HWND g_pWindow;
int g_iHeight;
int g_iWidth;
HDC g_pBackbufferDC;
HDC g_pFrontbufferDC;
HBITMAP g_pBackbufferBitmap;

MainWindow *g_pMainWindow;

//those don't belong here
char *CurrentTime = new char[10];
char *CurrentDate = new char[10];
char *SelectedChannel = new char[50];
char *CurrentNickname = new char[30];

