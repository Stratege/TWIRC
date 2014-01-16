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
	void RedrawAllIfChannelSelected(string ChannelName);
	std::list<MessageBoxCF *>::iterator FindChannelIteratorByName(string Name);
	void SelectChannel(MessageBoxCF *NewChannel);
	int height;
	int width;
	int leftbarsize;

public:
	ChannelHandler(int height, int width,int leftbarsize); /// \fn creator
	~ChannelHandler(); /// \fn destructor
	void SelectChannel(string Name); /// \fn Changes the current Channel to be the selected Channel, creates it if it doesn't exist yet.
	void WriteToChannel(string Name, string msg); /// \fn Writes something to the Channel with the Name specified in Name, creates it if it doesn't exist.
	MessageBoxCF *FindChannel(string Name); /// \fn Finds an existing Channel with the Name specified in Name - or creates it if it does not exist yet.
	void OpenChannel(string Name); /// \fn Creates a new Channel
	void CloseChannel(string Name); /// \fn Closes a Channel
	void CloseAll();
	string GetSelectedChannelName();
	void DrawSelected();
	void DrawChannellist();
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
};

#endif