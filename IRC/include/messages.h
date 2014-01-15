#ifndef __MESSAGES_GUARD__
#define __MESSAGES_GUARD__

#include "Globals.h"
#include "DrawingHandler.h"

/** \brief Contains the messagebox class
 *  \file
 *  Has been adapted for IRC usage, works for any multiline textoutput though. (More Versatile than the SRHSTUB Version)
*/

class MessageBoxCF
{
private:
	std::list<char *> *MessageQueue; /// \var Contains the messages
	std::list<char *> *UserList; // con

	RECT *rect; /// \var contains placement of messagebox
	int DisplayCount; ///
	std::list<char *>::iterator BottomMostDisplayedMessage;
	int TextSize;
	string MessageBoxName;
	int UnreadMessages;

public:
	MessageBoxCF(string Name); /// \fn creator
	~MessageBoxCF(); /// \fn destructor
	void DrawMessages(); /// \fn Draws the current message, swaps messages if NewTime > turnNumber
	void SetRect(int top, int left, int bottom, int right); /// \fn Sets the position of the Message Box
	bool SetTextSize(int Size); /// \fn Sets the Size of the to be displayed Text. Returns true if successful.
	void AddMessage(string Message); /// \fn Adds a message to the Queue
	bool IsQueueEmpty(); /// \fn Checks if the Queue is empty
	void ClearMessagebox(); /// \fn Clears the Queue
	void RemoveOldestMessage(); /// \fn Removes the oldest message from the Queue
	void ScrollUp(int lines=1); /// \fn Scrolls up by the amount of lines specified, stops when oldest message is hit
	void ScrollDown(int lines=1); /// \fn Scrolls down by the amount of lines specified, stops when newest message is hit
	void ScrollToStart(); /// \fn Scrolls to newest message
	void ScrollToEnd(); /// \fn Scrolls to oldest message - DisplayCount
	void ScrollOneWindow(bool Up); /// \fn Scrolls by the amount specified in DisplayCount. Either up or down depending on direction
	string GetName(); /// \fn Returns the name of the Messagebox
	void ClearUnread();
	int GetUnread();
	void CopyCurrentLine();
	string GetCurrentLine();
	bool IsQueueEmptyOrAlmostEmpty();
	bool DisplayingNewestMessage();
	void AddName(string name);
	void RemoveNameIfExists(string name);
	void DisplayUserList();
};

//extern MessageBoxCF *g_pMessagebox; /// \var global handle to the messagebox


#endif