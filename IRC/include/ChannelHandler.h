#ifndef __CHANNELHANDLER_GUARD__
#define __CHANNELHANDLER_GUARD__

#include <list>
#include "messages.h"
#include "FindFunctions.h"
#include "DrawingHandler.h"

/** \brief Contains the Channelhandler class
 *  \file
 *  Handles the currently opened Channels and Queries. Other parts of the code should access this Object to access the Channels
*/

class ChannelHandler 
{
private:
	std::list<MessageBoxCF *> *ChannelList; /// \var Contains the Channels
	MessageBoxCF *SelectedChannel; /// \var The currently selected Channel
	void RedrawAllIfChannelSelected(char *ChannelName);
	std::list<MessageBoxCF *>::iterator FindChannelIteratorByName(char *Name);
	void SelectChannel(MessageBoxCF *NewChannel);
	int height;
	int width;
	int leftbarsize;

public:
	ChannelHandler(int height, int width,int leftbarsize); /// \fn creator
	~ChannelHandler(); /// \fn destructor
	void SelectChannel(char *Name); /// \fn Changes the current Channel to be the selected Channel, creates it if it doesn't exist yet.
	void WriteToChannel(char *Name, char *msg); /// \fn Writes something to the Channel with the Name specified in Name, creates it if it doesn't exist.
	MessageBoxCF *FindChannel(char *Name); /// \fn Finds an existing Channel with the Name specified in Name - or creates it if it does not exist yet.
	void OpenChannel(char *Name); /// \fn Creates a new Channel
	void CloseChannel(char *Name); /// \fn Closes a Channel
	void CloseAll();
	char *GetSelectedChannelName();
	void DrawSelected();
	void DrawChannellist();
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
};

#endif