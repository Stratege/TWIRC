#include "ChannelHandler.h"


ChannelHandler::ChannelHandler(int height, int width,int leftbarsize)
{
	ChannelList = new std::list<MessageBoxCF *>;
	this->height = height-20;
	this->width = width;
	this->leftbarsize = leftbarsize;
	this->SelectedChannel = NULL;
	SelectChannel("unknownRecievedMsgs");
}

ChannelHandler::~ChannelHandler()
{
	while(ChannelList->begin() != ChannelList->end())
	{
		delete(*ChannelList->begin());
		ChannelList->pop_front();
	}
	delete ChannelList;
}

void ChannelHandler::DrawChannellist()
{
	if(g_pDrawingHandler == 0)
	{
		new DrawingHandler();
	}
	if(g_pDrawingHandler != 0)
	{
		//let's box this shit first
		//a black box with a nice fine white surrounding
		RECT *rect = new RECT;
		rect->left = 0;
		rect->right = this->leftbarsize;
		rect->top = 0;
		rect->bottom = this->height;

		HDC channelListDC = g_pDrawingHandler->getChannelListHDC();
		g_pDrawingHandler->SetChannelListRect(rect);


		BitBlt(channelListDC,rect->left-5,rect->top-5,rect->right - rect->left + 10,rect->bottom - rect->top + 10,NULL,0,0,WHITENESS);
		BitBlt(channelListDC,rect->left,rect->top,rect->right - rect->left,rect->bottom - rect->top,NULL,0,0,BLACKNESS);
		ExtFloodFill(channelListDC,(rect->left+rect->right)/2,(rect->top+rect->bottom)/2,RGB(0,0,0),FLOODFILLSURFACE);

		int TextSize = 18;
		RECT *temprect = new RECT;
		temprect->left = rect->left;
		temprect->right = rect->right;
		temprect->top = rect->top;
		temprect->bottom = rect->top+TextSize;


		char *tempChar = new char[200];

		if(this->ChannelList->begin() != this->ChannelList->end())
		{
			for (std::list<MessageBoxCF *>::iterator tempIter = ChannelList->begin(); tempIter != ChannelList->end(); tempIter++)
			{
				strncpy(tempChar,(*tempIter)->GetName(),200);
				snprintf(tempChar,200,"%s %i",tempChar,(*tempIter)->GetUnread());
			
				// new draw
				DrawText(channelListDC,tempChar,-1,temprect,DT_WORDBREAK + DT_CALCRECT);
				DrawText(channelListDC,tempChar,-1,temprect,DT_WORDBREAK);
				temprect->top = temprect->bottom;
				temprect->right = rect->right;
	/*
				// old draw
				DrawText(g_pBackbufferDC,tempChar,-1,temprect,DT_SINGLELINE + DT_VCENTER + DT_NOCLIP);
				temprect->top += TextSize;
				temprect->bottom += TextSize;
	*/			
			}
		}
		delete(tempChar);
		delete(temprect);
		delete(rect);
	}
}


void ChannelHandler::SelectChannel(MessageBoxCF *NewChannel)
{
	SelectedChannel = NewChannel;
	SelectedChannel->ClearUnread();
	SelectedChannel->DrawMessages();
	this->DrawChannellist();
	SendMessage(g_pWindow,WM_PAINT,0,0);
}

void ChannelHandler::SelectChannel(char *Name)
{
	this->SelectChannel(FindChannel(Name));
}

void ChannelHandler::WriteToChannel(char *Name, char *msg)
{
	MessageBoxCF *tempChannel = FindChannel(Name);
	tempChannel->AddMessage(msg);

	//puts element at the top when msg is inserted
	ChannelList->erase(ChannelHandler::FindChannelIteratorByName(Name));
	ChannelList->push_front(tempChannel);

	if(!strcmp(SelectedChannel->GetName(),Name))
	{
		SelectedChannel->ClearUnread();
//		SelectedChannel->DrawMessages();
		if(SelectedChannel->DisplayingNewestMessage())
		{
			if(g_pDrawingHandler == 0)
			{
				new DrawingHandler();
			}
			if(g_pDrawingHandler != 0)
			{
				g_pDrawingHandler->AddMessage(SelectedChannel->GetCurrentLine(),true);
			}
		}
		//invokes redrawing the window
//		InvalidateRect (g_pWindow, NULL, TRUE);
//		UpdateWindow (g_pWindow);
//		RedrawWindow(g_pWindow,0,0,RDW_INTERNALPAINT);
	}

//	this->DrawChannellist();

	SendMessage(g_pWindow,WM_PAINT,0,0);
}

std::list<MessageBoxCF *>::iterator ChannelHandler::FindChannelIteratorByName(char *Name)
{
	if(ChannelList->begin() != ChannelList->end())
	{
		for (std::list<MessageBoxCF *>::iterator tempIter = ChannelList->begin(); tempIter != ChannelList->end(); tempIter++)
		{
			//Name exactly the same?
			if(!strcmp((*tempIter)->GetName(),Name))
			{
				//found it, exit the loop!
				return tempIter;
			}
		}
	}
	//couldn't find Channel? Create it.
	OpenChannel(Name);
	return ChannelList->begin();
}

MessageBoxCF *ChannelHandler::FindChannel(char *Name)
{
	//the vast majority of uses will modify the currently selected Channel, so it makes sense in terms of performance to do this.
	if((int)this->SelectedChannel != NULL) if (this->SelectedChannel->GetName() == Name) return this->SelectedChannel;
	return *ChannelHandler::FindChannelIteratorByName(Name);
}

void ChannelHandler::OpenChannel(char *Name)
{
	MessageBoxCF *NewChannel = new MessageBoxCF(Name);
	NewChannel->SetRect(0,this->leftbarsize,this->height,this->width);
	ChannelList->insert(ChannelList->begin(),NewChannel);
}

void ChannelHandler::CloseChannel(char *Name)
{

	MessageBoxCF *temp = FindChannel(Name);
	bool WasMainChannel = false;
	if(!strcmp(SelectedChannel->GetName(), Name)){
		SelectChannel("unknownRecievedMsgs");
		if(Name = "unknownRecievedMsgs") WasMainChannel = true;
	}
	ChannelList->remove(temp);
	delete(temp);
	if(WasMainChannel)
	{
		this->SelectedChannel = NULL;
		SelectChannel(Name);
	}
	SendMessage(g_pWindow,WM_PAINT,0,0);
}

void ChannelHandler::CloseAll()
{
	while(ChannelList->begin() != ChannelList->end())
	{
		delete(*ChannelList->begin());
		ChannelList->erase(ChannelList->begin());
	}
	SelectChannel("unknownRecievedMsgs");
}

char *ChannelHandler::GetSelectedChannelName()
{
	return SelectedChannel->GetName();
}

void ChannelHandler::DrawSelected()
{
	this->SelectedChannel->DrawMessages();
}

void ChannelHandler::ScrollChannelUp(int Lines)
{
	std::list<MessageBoxCF *>::iterator CurrentChannelIter = FindChannelIteratorByName(this->SelectedChannel->GetName());
	if(ChannelList->begin() != CurrentChannelIter)
	{
		CurrentChannelIter--;
	}
	SelectChannel(*CurrentChannelIter);
}

void ChannelHandler::ScrollChannelDown(int Lines)
{
	std::list<MessageBoxCF *>::iterator CurrentChannelIter = FindChannelIteratorByName(this->SelectedChannel->GetName());
	CurrentChannelIter++;
	if(CurrentChannelIter == ChannelList->end())
	{
		CurrentChannelIter--;
	}
	SelectChannel(*CurrentChannelIter);
}

void ChannelHandler::ScrollUp(char *ChannelName, int Lines)
{
	(this->FindChannel(ChannelName))->ScrollUp(Lines);
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollDown(char *ChannelName, int Lines)
{
	(this->FindChannel(ChannelName))->ScrollDown(Lines);
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollToStart(char *ChannelName)
{
	(this->FindChannel(ChannelName))->ScrollToStart();
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollToEnd(char *ChannelName)
{
	(this->FindChannel(ChannelName))->ScrollToEnd();
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollOneWindow(char *ChannelName, bool Up)
{
	(this->FindChannel(ChannelName))->ScrollOneWindow(Up);
	RedrawAllIfChannelSelected(ChannelName);
}

void ChannelHandler::RedrawAllIfChannelSelected(char *ChannelName)
{
	if(this->SelectedChannel->GetName() == ChannelName)
	{
		this->SelectedChannel->DrawMessages();
//		SendMessage(g_pWindow,WM_PAINT,0,0);
		g_pDrawingHandler->Draw();
	}
}

void ChannelHandler::CopyCurrentLine()
{
	SelectedChannel->CopyCurrentLine();
}


void ChannelHandler::AddNick(char *Nick, char *Channel)
{
	FindChannel(Channel)->AddName(Nick);
}
void ChannelHandler::RemoveNick(char *Nick, char *Channel)
{
	if(Channel != NULL)
	{
		FindChannel(Channel)->RemoveNameIfExists(Nick);
	}else{
			for (std::list<MessageBoxCF *>::iterator tempIter = ChannelList->begin(); tempIter != ChannelList->end(); tempIter++)
			{
				(*tempIter)->RemoveNameIfExists(Nick);
			}
	}
}

void ChannelHandler::ShowUserList()
{
	this->SelectedChannel->DisplayUserList();
}