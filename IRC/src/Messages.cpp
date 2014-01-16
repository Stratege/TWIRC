#include "messages.h"
#include "FindFunctions.h"
#include "OtherFunctions.h"

//MessageBoxCF *g_pMessagebox = new MessageBoxCF;



char *prepareMessage(string Message)
{
	stripNewLine(Message);
	//We create a char that is as long as it needs to be and obtains the Messages
	int Msglength = Message.length();
	//so if I don't call the following in IncomingMessagesHandling.cpp:
	//it works with Msglength+2 (even though deletion is called at times and neither of those functions should be modifying the msg
	char *MessageInsert = new char[Msglength+1];
	strncpy(MessageInsert,Message.c_str(),Msglength+1);

/*	//don't do the below, it's just in here for historic reasons. And because it's a fun reminder.
	//here's a neat little way to trick the debug code:
	//uncomment this bit and it will think this variable was allocated via aligned
	(*(MessageInsert-1)) = 237;
	(*(MessageInsert-2)) = 237;
	(*(MessageInsert-3)) = 237;
	(*(MessageInsert-4)) = 237;
*/

	//removes newline stuff at the end from our Messages (relevant because of how we draw)
	//if(MessageInsert[strlen(MessageInsert)-2] == 13) MessageInsert[strlen(MessageInsert)-2] = 0;

	return MessageInsert;
}



//draws the messages on the screen
void MessageBoxCF::DrawMessages()
{
	if(g_pDrawingHandler == 0)
	{
		new DrawingHandler();
	}
	if(g_pDrawingHandler != 0)
	{
		HDC channelDC = g_pDrawingHandler->getChannelHDC();
		g_pDrawingHandler->SetChannelRect(rect);

		//let's box this shit first
		//a black box with a nice fine white surrounding
		BitBlt(channelDC,0,0,rect->right - rect->left,rect->bottom - rect->top,NULL,0,0,WHITENESS);
		BitBlt(channelDC,5,5,rect->right - rect->left-10,rect->bottom - rect->top-10,NULL,0,0,BLACKNESS);
		ExtFloodFill(channelDC,(0+rect->right)/2,(0+rect->bottom)/2,RGB(0,0,0),FLOODFILLSURFACE);
		RECT *temprect = new RECT;
		temprect->left = 0;
		temprect->right = rect->right - rect->left-10;
		temprect->top = 5;
		temprect->bottom = rect->bottom - rect->top-10;
		int bottomOfTemprect = 0;
		//draw Channel Name
		DrawText(channelDC,this->GetName().c_str(),-1,temprect,DT_SINGLELINE + DT_TOP + DT_CENTER);

		temprect->top = rect->bottom-TextSize - rect->top;
		temprect->left = 5;

		//	DrawText(g_pBackbufferDC,*BottomMostDisplayedMessage,-1,rect,DT_SINGLELINE + DT_TOP + DT_CENTER);
		if(!this->IsQueueEmpty())
		{
			std::list<char *>::iterator tempIter = BottomMostDisplayedMessage;
			for (int i = 0; i <= DisplayCount; i++)
			{
				if(tempIter == MessageQueue->end()) break;
				if(temprect->top <= 10) break;
				//TIL: DT_CALCRECT causes DrawText to not draw. Obvious, isn't it?
				//DT_CALCRECT keeps top the same, but changes bottom
				//however, we want bottom to stay the same and change top instead - so we do the following stuff
				//now we have multiline support! :D
				bottomOfTemprect = temprect->bottom;
				DrawText(channelDC,*tempIter,-1,temprect,DT_WORDBREAK + DT_CALCRECT);
				temprect->top -= (temprect->bottom - bottomOfTemprect);
				if(temprect->top <= 10) break;
				temprect->bottom = bottomOfTemprect;
				DrawText(channelDC,*tempIter,-1,temprect,DT_WORDBREAK);
				tempIter++;
				temprect->bottom = temprect->top;
				temprect->top = temprect->bottom-TextSize;
	//			temprect->left = rect->left;
				temprect->right = rect->right - rect->left - 10;
				//todo: when we are at the top of the rect, stop and update something that tells us how many messages we drew here
			}
		}
		delete(temprect);
	}
}

void MessageBoxCF::SetRect(int top, int left, int bottom, int right)
{
	rect->top = top;
	rect->bottom = bottom;
	rect->left = left;
	rect->right = right;
}

bool MessageBoxCF::SetTextSize(int Size)
{
	if(Size > 0)
	{
		this->TextSize = Size;
		return true;
	} else return false;
}



void MessageBoxCF::AddMessage(string Message)
{
	char *MessageInsert = prepareMessage(Message);

	//this piece of code means it keeps displaying the same message if one's not at the newest message
//	if(this->BottomMostDisplayedMessage != MessageQueue->begin()) BottomMostDisplayedMessage++;
	MessageQueue->insert(MessageQueue->begin(),MessageInsert);
	UnreadMessages++;

	std::list<char *>::iterator tempIter = MessageQueue->begin();
	if(!IsQueueEmpty())
	{
		tempIter++;
		if(this->BottomMostDisplayedMessage == tempIter)
		{
			BottomMostDisplayedMessage--;
		}
	}
}

bool MessageBoxCF::DisplayingNewestMessage()
{
	if(this->BottomMostDisplayedMessage == MessageQueue->begin()) return true;
	else return false;
}

//Original Method to check if the Queue is empty
bool MessageBoxCF::IsQueueEmpty()
{
	if(MessageQueue->begin() != MessageQueue->end()) return false;
	else return true;
}

//This function exists as a work around because everything exploded when the Queue was empty and I tried to get BottomMostDisplayedMessage
bool MessageBoxCF::IsQueueEmptyOrAlmostEmpty()
{
	if(!IsQueueEmpty())
	{
	std::list<char *>::iterator tempIter = MessageQueue->begin();
	tempIter++;
	if(tempIter != MessageQueue->end()) return false;
	else return true;
	}else return true;
}


void MessageBoxCF::RemoveOldestMessage()
{
	if(!IsQueueEmpty())
	{
		std::list<char *>::iterator tempIter = MessageQueue->end();
		tempIter--;
		if(BottomMostDisplayedMessage == tempIter && BottomMostDisplayedMessage != MessageQueue->begin())
		{
			BottomMostDisplayedMessage--;
		}
		delete[](*tempIter);
		MessageQueue->erase(tempIter);
//		MessageQueue->pop_back();
		if(IsQueueEmpty())
		{
			BottomMostDisplayedMessage = MessageQueue->begin();
		}
	}
}

void MessageBoxCF::ScrollUp(int lines)
{
	for(int i = 0; i < lines; i++)
	{
		if(this->BottomMostDisplayedMessage != MessageQueue->end()) BottomMostDisplayedMessage ++;
		else break;
	}
}

void MessageBoxCF::ScrollDown(int lines)
{
	for(int i = 0; i < lines; i++)
	{
		if(this->BottomMostDisplayedMessage != MessageQueue->begin()) BottomMostDisplayedMessage --;
		else break;
	}
}

void MessageBoxCF::ScrollToStart()
{
	this->BottomMostDisplayedMessage = MessageQueue->begin();
}

void MessageBoxCF::ScrollToEnd()
{
	this->BottomMostDisplayedMessage = MessageQueue->end();
	BottomMostDisplayedMessage--;
//	this->ScrollOneWindow(DOWN);
}

void MessageBoxCF::ScrollOneWindow(bool Up)
{
	if(Up){
		this->ScrollUp(DisplayCount);
	}else{
		this->ScrollDown(DisplayCount);
	}
}

void MessageBoxCF::ClearMessagebox()
{
	while(!IsQueueEmpty())
	{
		RemoveOldestMessage();
	}
}

string MessageBoxCF::GetName()
{
	return this->MessageBoxName;
}

void MessageBoxCF::ClearUnread()
{
	UnreadMessages = 0;
}


int MessageBoxCF::GetUnread()
{
	return UnreadMessages;
}

void MessageBoxCF::CopyCurrentLine()
{
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy; 

	if(OpenClipboard(g_pWindow))
		{
		if(EmptyClipboard())
			{
		//code from http://msdn.microsoft.com/en-us/library/ms649016%28VS.85%29.aspx
        hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
            (strlen(*BottomMostDisplayedMessage) + 1) * sizeof(TCHAR)); 
        if (hglbCopy == NULL) 
        { 
            CloseClipboard(); 
            return; 
        } 
 
        // Lock the handle and copy the text to the buffer. 
        lptstrCopy = (LPTSTR) GlobalLock(hglbCopy); 
        memcpy(lptstrCopy, *BottomMostDisplayedMessage, 
            strlen(*BottomMostDisplayedMessage)); 
        lptstrCopy[strlen(*BottomMostDisplayedMessage)] = (TCHAR) 0;    // null character 
        GlobalUnlock(hglbCopy); 
 
        // Place the handle on the clipboard. 
 
        SetClipboardData(CF_TEXT, hglbCopy); 

			}
		CloseClipboard();
		}	
}

string MessageBoxCF::GetCurrentLine()
{
	if(MessageQueue->size() != 0)
	{
		return *BottomMostDisplayedMessage;
	}else{
		return "Empty Channel";
	}
}

MessageBoxCF::MessageBoxCF(string Name)
{
	rect = new RECT;
	rect->top = 0;
	rect->left = 0;
	rect->bottom = 0;
	rect->right = 0;
	MessageQueue = new std::list<char *>;
	UserList = new std::list<char *>;
	TextSize = 18;
	DisplayCount = 50;
	ScrollToStart();
	UnreadMessages = 0;

	this->AddMessage("Channel Opened");
	BottomMostDisplayedMessage = MessageQueue->begin();
	MessageBoxName = Name;
}

MessageBoxCF::~MessageBoxCF()
{
	delete rect;
	ClearMessagebox();
	delete MessageQueue;
	while(UserList->begin() != UserList->end())
	{
		delete[] *(UserList->begin());
	}
	delete UserList;
}

void MessageBoxCF::AddName(string name)
{
	char *nameInsert = prepareMessage(name);
	UserList->insert(UserList->begin(),nameInsert);
}

void MessageBoxCF::RemoveNameIfExists(string name)
{
	std::list<char *>::iterator tempIter = UserList->begin();
	while(tempIter != UserList->end())
	{
		if(!name.compare(*tempIter))
		{
			delete(*tempIter);
			UserList->erase(tempIter);
			break;
		}

		tempIter++;
	}
}

void MessageBoxCF::DisplayUserList()
{
	string userListMsg = "Following users in Channel: ";

	
	for(std::list<char *>::iterator tempIter = UserList->begin(); tempIter != UserList->end(); tempIter++)
	{
		userListMsg += (string)" " + *tempIter;
	}

	this->AddMessage(userListMsg);
}