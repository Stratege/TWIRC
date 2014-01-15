
#include "Globals.h"
#include "MainWindow.h"
#include <io.h>
#include <fcntl.h>
#include "DrawingHandler.h"

HWND hwndEdit;

#define ID_EDITCHILD 100

WNDPROC wpOrigEditProc; 

LRESULT APIENTRY EditSubclassProc(HWND hWnd, WORD wMessage,WORD wParam,LONG lParam)
{
	switch (wMessage)
	{
		case WM_GETDLGCODE:
			return (DLGC_WANTALLKEYS | CallWindowProc(wpOrigEditProc, hWnd, wMessage,wParam, lParam));
		case WM_CHAR:
			 //Process this message to avoid message beeps.
			if ((wParam == VK_RETURN) || (wParam == VK_TAB))
				return 0;
			else
				return (CallWindowProc(wpOrigEditProc, hWnd, wMessage, wParam, lParam));

		case WM_KEYDOWN:
			if (wParam == VK_RETURN) 
			{
				char temp[500];
				*((WORD*)&temp[0]) = 499; //set the first word of the buffer to the size of the buffer
	int i = SendMessage(hwndEdit, EM_GETLINE, 0, (LPARAM)temp); //get line 0 from ec and put it in buffer retbuf
//				int i = SendMessage(hwndEdit,EM_GETLINE,0,(LPARAM)temp);
				SendMessage(hwndEdit,WM_SETTEXT,0,(LPARAM)"");
				temp[i] = '\0';
				//don't send if the line's empty
				if(i > 0)
				{
					g_pMainWindow->SendMessage(temp);
				}
				return FALSE;
			}else if(wParam == VK_UP){
				//check if ctrl is down
				if(HIBYTE(GetKeyState(VK_CONTROL)))
				{
				//if so, switch channel up
					g_pMainWindow->ScrollChannelUp(1);
				}else{
					//else scroll line up by 1
					g_pMainWindow->ScrollUp(g_pMainWindow->GetSelectedChannelName(), 1);
				}
			}else if(wParam == VK_DOWN){
				//check if ctrl is down
				if(HIBYTE(GetKeyState(VK_CONTROL)))
				{
				//if so, switch channel up
					g_pMainWindow->ScrollChannelDown(1);
				}else{
					//scroll down by 1
					g_pMainWindow->ScrollDown(g_pMainWindow->GetSelectedChannelName(), 1);
				}
			}else if(wParam == VK_END){
				//scroll to end
				//needs to be in reverse for reasons
				g_pMainWindow->ScrollToStart(g_pMainWindow->GetSelectedChannelName());
			}else if(wParam == VK_HOME){
				//scroll to start
				//needs to be in reverse for reasons
				g_pMainWindow->ScrollToEnd(g_pMainWindow->GetSelectedChannelName());
			}else if(wParam == VK_PRIOR){
				//scroll up 1 window
				g_pMainWindow->ScrollOneWindow(g_pMainWindow->GetSelectedChannelName(),UP);
			}else if(wParam == VK_NEXT){
				//scroll down 1 window
				g_pMainWindow->ScrollOneWindow(g_pMainWindow->GetSelectedChannelName(),DOWN);
			}
			return (CallWindowProc(wpOrigEditProc, hWnd, wMessage,wParam, lParam));
			break ;

		default:
			break;

        }
	return (CallWindowProc(wpOrigEditProc, hWnd, wMessage,wParam, lParam));
}


LRESULT CALLBACK WndProc(HWND pHWND,UINT iMessage,WPARAM wParam,LPARAM lParam)
{

	//This function checks each message that the windows receives.
	if(iMessage == WM_DESTROY){
		PostQuitMessage(0);
		return 0;
	}else if(iMessage == WM_CREATE){
					HWND editWindow;
		           hwndEdit = CreateWindowEx(
                                0, "EDIT",   // predefined class 
								    NULL,         // no window title 
                                WS_CHILD | WS_VISIBLE | ES_LEFT , 
								0,780, 1000, 40,   // set size in WM_SIZE message 
                                pHWND,         // parent window 
                                (HMENU) ID_EDITCHILD,   // edit control ID 
                                (HINSTANCE) GetWindowLong(pHWND, GWL_HINSTANCE), 
                                NULL);        // pointer not needed 
				   SetWindowPos(hwndEdit,HWND_TOPMOST,0,0, 0, 0,SWP_NOMOVE|SWP_NOSIZE);
 
           // Add text to the window. 
			wpOrigEditProc = (WNDPROC) SetWindowLongPtr(hwndEdit,GWLP_WNDPROC,(LONG) EditSubclassProc);
//            SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) "testtext test und test test"); 
 
//	} else      if(iMessage== WM_COMMAND)	{
		//set focus =
		//wParam  16777316
		//lParam 111676670

		//lose focus =
		//wParam  33554532
		//lParam 111676670

		//backspace
		//wParam 67108964
		//lParam 12715578

//		if(wParam != 16777316 && wParam != 33554532)
//		std::cout<<iMessage<<" "<<wParam<<" "<<lParam<<" - edit control Ausgabe"<<std::endl;
/*            switch (wParam) 
            { 
                case IDM_EDUNDO: 
                    // Send WM_UNDO only if there is something to be undone. 
 
                    if (SendMessage(hw	ndEdit, EM_CANUNDO, 0, 0)) 
                        SendMessage(hwndEdit, WM_UNDO, 0, 0); 
                    else 
                    {
                        MessageBox(hwndEdit, 
                                   "Nothing to undo.", 
                                   "Undo notification", 
                                   MB_OK); 
                    }
                    break; 

                case IDM_EDCUT: 
                    SendMessage(hwndEdit, WM_CUT, 0, 0); 
                    break; 
 
                case IDM_EDCOPY: 
                    SendMessage(hwndEdit, WM_COPY, 0, 0); 
                    break; 
 
                case IDM_EDPASTE: 
                    SendMessage(hwndEdit, WM_PASTE, 0, 0); 
                    break; 
 
                case IDM_EDDEL: 
                    SendMessage(hwndEdit, WM_CLEAR, 0, 0); 
                    break; 

                default: 
			       break; 
            } 
			*/
	
}else if(iMessage == WM_SIZE){
		MoveWindow(hwndEdit, 0, HIWORD(lParam)-20, LOWORD(lParam), 40, TRUE);
		if(g_pMainWindow != NULL) g_pMainWindow->Resize(LOWORD(lParam),HIWORD(lParam));
//		SetWindowPos(hwndEdit,HWND_TOPMOST,0,0, 0, 0,SWP_NOMOVE|SWP_NOSIZE);
//		return 0;
	}else if(iMessage == WM_KILLFOCUS){
	}else if(iMessage == WM_SETFOCUS){
		if(hwndEdit != NULL)
			SetFocus(hwndEdit);
	}else if(iMessage == WM_KEYDOWN){
/*		if(wParam == VK_RETURN)
		{
			char *temp = new char[500];
			int i = SendMessage(hwndEdit,EM_GETLINE,0,(LPARAM)temp);
			SendMessage(hwndEdit,WM_SETTEXT,0,(LPARAM)"");
			i++;
			temp[i] = '\0';
			g_pMainWindow->SendMessage(temp);
			delete temp;
		}*/
	}else if(iMessage == WM_CHAR){
//		if(g_pMainWindow != NULL)
//		g_pMainWindow->KeyDown((char)wParam);
	}else if(iMessage == WM_KEYUP){
	}else if(iMessage == WM_MOUSEMOVE){
	}else if(iMessage == WM_LBUTTONDOWN){
	}else if(iMessage == WM_RBUTTONDOWN){
	}else if(iMessage == WM_PAINT){
		/*if(g_pMainWindow != NULL)
		g_pMainWindow->DrawAll();*/
		if(g_pDrawingHandler != NULL)
			g_pDrawingHandler->Draw();
		RedrawWindow(hwndEdit,NULL,NULL,RDW_INVALIDATE);
		SendMessage(hwndEdit,WM_PAINT,0,0);
	}else if(iMessage == WM_SETFOCUS){
		SetFocus(hwndEdit);
	}else if(iMessage == WM_ERASEBKGND){
	}else if(iMessage == WM_SOCKET){
		if(WSAGETSELECTERROR(lParam))
		{	
			MessageBox(pHWND,
				"Connection to server failed",
				"Error",
				MB_OK|MB_ICONERROR);
			//SendMessage(pHWND,WM_DESTROY,NULL,NULL);
			
			//not having this commented out might cause loops. Will have to check.
			//g_pMainWindow->SocketClosedMessage();
		}
		switch(WSAGETSELECTEVENT(lParam))
		{
			case FD_READ:
			{
				g_pMainWindow->ReadMessage();
			}
			break;

			case FD_CLOSE:
			{
/*				MessageBox(pHWND,
					"Server closed connection, reopening.",
					"Connection closed!",
					MB_ICONINFORMATION|MB_OK);*/
				//for some reason the server sends this message twice when one uses /QUIT.
				//not a big issue since one is not supposed to be reopening after /QUIT anyway and that therefor needs to be caught somewhere else
				g_pMainWindow->SocketClosedMessage();
//					SendMessage(hWnd,WM_DESTROY,NULL,NULL);
			}
			break;
		}
	}else if(iMessage == WM_TIMER)
	{
		if(wParam == CONNECTIONTIMERID)
		{
			SendMessage(g_pWindow,WM_SOCKET,NULL,LOWORD(FD_CLOSE));
		}
	}

	return DefWindowProc(pHWND,iMessage,wParam,lParam);
}

void RegisterWindowClass()
{
	//Create the window class.
	WNDCLASS wcClass;
	ZeroMemory(&wcClass,sizeof(WNDCLASS));
	wcClass.lpszClassName = g_pWindowName;
	wcClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcClass.lpfnWndProc = WndProc;
	wcClass.hInstance = g_pInstance;

	//Register the class so we can create a window based on this class.
	RegisterClass(&wcClass);
}

void LoadWindow(HINSTANCE hInstance,int iHeight,int iWidth,const char *pTitle)
{	
	g_pInstance = hInstance;

	RECT rWindowRect;
	rWindowRect.bottom = iHeight;
	rWindowRect.right = iWidth;
	rWindowRect.top = rWindowRect.left = 0;

	AdjustWindowRect(&rWindowRect,WS_OVERLAPPEDWINDOW,FALSE);
	
	
	RegisterWindowClass();	

	//Try to create the window. (If that failed, g_pWindow will be NULL)
	g_pWindow = CreateWindow(g_pWindowName,pTitle,WS_OVERLAPPEDWINDOW,0,0,
		rWindowRect.right - rWindowRect.left,rWindowRect.bottom - rWindowRect.top,
		NULL,NULL,g_pInstance,NULL);

	//Check if the window has been created successfully.
	if(!g_pWindow){
		//If the window has not been created successfully, throw an exception.
		throw 0;
	}else{
		//If the window has been created successfully, show the window
		//and save the height and width of the window.
		ShowWindow(g_pWindow,5);
		UpdateWindow(g_pWindow);
		SetForegroundWindow(g_pWindow);
		SetFocus(g_pWindow);


		g_iHeight = iHeight;
		g_iWidth = iWidth;

		//Create front- and backbuffer of the window for double buffering.
		g_pFrontbufferDC = GetDC(g_pWindow);
		g_pBackbufferDC = CreateCompatibleDC(NULL);
		g_pBackbufferBitmap = CreateCompatibleBitmap(g_pFrontbufferDC,iWidth,iHeight);
		SelectObject(g_pBackbufferDC,g_pBackbufferBitmap);

		//Background of the backbuffer is transparent, so that we can draw text without problems.
		SetBkColor(g_pBackbufferDC,TRANSPARENT);



//		ZeroMemory(&g_pHandler,sizeof(SRHHandler));
	}
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR pCommandLine,int iCmdShow)
{

	int iHeight = 800;
	int iWidth = 1000;

	//sets time stuff
	//_tzset();


	try
	{
		//Register Window class and create window
		LoadWindow(hInstance,iHeight,iWidth,g_pWindowName);		


		//todo: read config file
		//read User and Nick from it and toss it to the MainWindow Creation
		//then read all the other config things and figure out where the fuck to toss them?



		g_pMainWindow = new MainWindow(g_pInstance,g_pWindow);


		g_pMainWindow->InitProgram();


	}catch(int e)
	{
//		printf(e.what());
		char pBuffer[50] = {0};
		snprintf(pBuffer,50,"Error %i. Terminating program.",e);
		MessageBox(NULL,pBuffer,"Error!",5);
	}
	return 0;
}