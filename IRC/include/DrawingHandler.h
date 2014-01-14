#ifndef __DRAWINGHANDLER_GUARD__
#define __DRAWINGHANDLER_GUARD__

#include "Globals.h"

class DrawingHandler
{
private:
	HDC ChannelHDC;
	HBITMAP ChannelBitmap;
	RECT ChannelRect;
	HDC ChannelListHDC;
	HBITMAP ChannelListBitmap;
	RECT ChannelListRect;
	HDC tempBufferHDC;
	HBITMAP tempBufferBitmap;
	int TextColor;
	int BackgroundColor;

public:
	DrawingHandler(void);
	~DrawingHandler(void);
	void Draw();
	HDC getChannelHDC();
	HDC getChannelListHDC();
	void SetChannelRect(RECT *channelRect);
	void SetChannelListRect(RECT *channelListRect);

	void SetNewTextColor(int Color);
	void SetNewBackgroundColor(int Color);
	void AddMessage(char *Message, bool Channel);

	void Resize(int newWidth, int newHeight);
};


#endif

extern DrawingHandler *g_pDrawingHandler;