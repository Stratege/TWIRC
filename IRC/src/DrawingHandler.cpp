#include "DrawingHandler.h"
#include "MainWindow.h"

DrawingHandler *g_pDrawingHandler = 0;

DrawingHandler::DrawingHandler(void)
{
	//there is always only one drawing handler
	if(g_pDrawingHandler != 0)
	{
		delete(g_pDrawingHandler);
		g_pDrawingHandler = 0;
	}

	this->ChannelHDC = CreateCompatibleDC(g_pBackbufferDC);
	this->ChannelBitmap = CreateCompatibleBitmap(g_pBackbufferDC,g_iWidth,g_iHeight);
	SelectObject(ChannelHDC,ChannelBitmap);
	this->ChannelListHDC = CreateCompatibleDC(g_pBackbufferDC);
	this->ChannelListBitmap = CreateCompatibleBitmap(g_pBackbufferDC,g_iWidth,g_iHeight);
	SelectObject(ChannelListHDC,ChannelListBitmap);

	this->tempBufferHDC = CreateCompatibleDC(g_pBackbufferDC);
	this->tempBufferBitmap = CreateCompatibleBitmap(g_pBackbufferDC,g_iWidth,g_iHeight);
	SelectObject(tempBufferHDC,tempBufferBitmap);


	ChannelRect.bottom = 0;
	ChannelRect.left = 0;
	ChannelRect.right = 0;
	ChannelRect.top = 0;

	ChannelListRect.bottom = 0;
	ChannelListRect.left = 0;
	ChannelListRect.right = 0;
	ChannelListRect.top = 0;


	SetBkColor(ChannelHDC,RGB(0,0,0));
	SetTextColor(ChannelHDC,RGB(255,255,255));
	SelectObject(ChannelHDC,CreateSolidBrush(RGB(0,0,0)));

	SetBkColor(ChannelListHDC,RGB(0,0,0));
	SetTextColor(ChannelListHDC,RGB(255,255,255));
	SelectObject(ChannelListHDC,CreateSolidBrush(RGB(0,0,0)));

	//this is part of the only one drawing handler hack
	g_pDrawingHandler = this;
}


DrawingHandler::~DrawingHandler(void)
{
}

void DrawingHandler::Draw()
{
	BitBlt(g_pBackbufferDC,ChannelRect.left,ChannelRect.top,ChannelRect.right,ChannelRect.bottom,this->ChannelHDC,0,0,SRCCOPY);
	BitBlt(g_pBackbufferDC,ChannelListRect.left,ChannelListRect.top,ChannelListRect.right,ChannelListRect.bottom,this->ChannelListHDC,0,0,SRCCOPY);
	if(g_pMainWindow != 0)
	{
		g_pMainWindow->SwapBuffers();
	}
}

HDC DrawingHandler::getChannelHDC()
{
	return ChannelHDC;
}
HDC DrawingHandler::getChannelListHDC()
{
	return ChannelListHDC;
}

void DrawingHandler::SetChannelListRect(RECT *channelListRect)
{
	ChannelListRect.bottom = channelListRect->bottom;
	ChannelListRect.left = channelListRect->left;
	ChannelListRect.right = channelListRect->right;
	ChannelListRect.top = channelListRect->top;
}

void DrawingHandler::SetChannelRect(RECT *channelRect)
{
	ChannelRect.bottom = channelRect->bottom;
	ChannelRect.left = channelRect->left;
	ChannelRect.right = channelRect->right;
	ChannelRect.top = channelRect->top;
}

void DrawingHandler::SetNewTextColor(int Color)
{
	SetTextColor(ChannelHDC,Color);
	SetTextColor(ChannelListHDC,Color);
}

void DrawingHandler::SetNewBackgroundColor(int Color)
{
	SetBkColor(ChannelHDC,Color);
	SelectObject(ChannelHDC,CreateSolidBrush(Color));

	SetBkColor(ChannelListHDC,Color);
	SelectObject(ChannelListHDC,CreateSolidBrush(Color));
}

void DrawingHandler::AddMessage(char *Message, bool Channel)
{
	int i;

	HDC tempHDC;
	RECT tempRect;
	if(Channel)
	{
		tempHDC = ChannelHDC;
		tempRect = ChannelRect;
	}else{
		tempHDC = ChannelListHDC;
		tempRect = ChannelListRect;
	}
	RECT temprect2;
	temprect2.top = 0;
	temprect2.left = 0;
	temprect2.bottom = tempRect.bottom - tempRect.top;
	temprect2.right = tempRect.right - tempRect.left;
	DrawText(tempHDC,Message,-1,&temprect2,DT_WORDBREAK + DT_CALCRECT);
	if(!BitBlt(tempBufferHDC,0,0,tempRect.right - tempRect.left,tempRect.bottom-temprect2.bottom - tempRect.top,tempHDC,0,temprect2.bottom,SRCCOPY))
	{
		i = GetLastError();
	}
	if(!BitBlt(tempHDC,0,0,tempRect.right - tempRect.left,tempRect.bottom - tempRect.top,tempBufferHDC,0,0,SRCCOPY))
			{
		i = GetLastError();
	}

	temprect2.top = tempRect.bottom-temprect2.bottom - tempRect.top;
	temprect2.bottom = tempRect.bottom - tempRect.top;
	DrawText(tempHDC,Message,-1,&temprect2,DT_WORDBREAK);
	this->Draw();
}