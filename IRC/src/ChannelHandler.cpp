#include "ChannelHandler.h"


ChannelHandler::ChannelHandler(int height, int width,int leftbarsize)
{
	ChannelList = new std::list<MessageBoxCF *>;
	Resize(height, width);
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


		char tempChar[200];

		if(this->ChannelList->begin() != this->ChannelList->end())
		{
			for (std::list<MessageBoxCF *>::iterator tempIter = ChannelList->begin(); tempIter != ChannelList->end(); tempIter++)
			{
				strncpy(tempChar,(*tempIter)->GetName().c_str(),200);
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

void ChannelHandler::SelectChannel(string Name)
{
	this->SelectChannel(FindChannel(Name));
}

void ChannelHandler::WriteToChannel(string Name, string msg)
{
	MessageBoxCF *tempChannel = FindChannel(Name);
	tempChannel->AddMessage(msg);

	//puts element at the top when msg is inserted
	ChannelList->erase(ChannelHandler::FindChannelIteratorByName(Name));
	ChannelList->push_front(tempChannel);

	if(!SelectedChannel->GetName().compare(Name))
	{
		SelectedChannel->ClearUnread();
//		SelectedChannel->DrawMessages();
		if(SelectedChannel->DisplayingNewestMessage())
		{
			this->DrawSelected();
			if(g_pDrawingHandler == 0)
			{
				new DrawingHandler();
			}
			if(g_pDrawingHandler != 0)
			{
				//temporarily disabled. Needs to be reenabled
				//g_pDrawingHandler->AddMessage(SelectedChannel->GetCurrentLine(),true);

			}
		}
		//invokes redrawing the window
//		InvalidateRect (g_pWindow, NULL, TRUE);
//		UpdateWindow (g_pWindow);
//		RedrawWindow(g_pWindow,0,0,RDW_INTERNALPAINT);
	}

	this->DrawChannellist();

	SendMessage(g_pWindow,WM_PAINT,0,0);
}

std::list<MessageBoxCF *>::iterator ChannelHandler::FindChannelIteratorByName(string Name)
{
	if(ChannelList->begin() != ChannelList->end())
	{
		for (std::list<MessageBoxCF *>::iterator tempIter = ChannelList->begin(); tempIter != ChannelList->end(); tempIter++)
		{
			//Name exactly the same?
			if(!(*tempIter)->GetName().compare(Name))
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

MessageBoxCF *ChannelHandler::FindChannel(string Name)
{
	//the vast majority of uses will modify the currently selected Channel, so it makes sense in terms of performance to do this.
	if((int)this->SelectedChannel != NULL) if (this->SelectedChannel->GetName() == Name) return this->SelectedChannel;
	return *ChannelHandler::FindChannelIteratorByName(Name);
}

void ChannelHandler::OpenChannel(string Name)
{
	MessageBoxCF *NewChannel = new MessageBoxCF(Name);
	NewChannel->SetRect(0,this->leftbarsize,this->height,this->width);
	ChannelList->insert(ChannelList->begin(),NewChannel);
}

void ChannelHandler::CloseChannel(string Name)
{

	MessageBoxCF *temp = FindChannel(Name);
	bool WasMainChannel = false;
	if(!SelectedChannel->GetName().compare(Name)){
		SelectChannel("unknownRecievedMsgs");
		if(!Name.compare( "unknownRecievedMsgs")) WasMainChannel = true;
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

string ChannelHandler::GetSelectedChannelName()
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

void ChannelHandler::ScrollUp(string ChannelName, int Lines)
{
	(this->FindChannel(ChannelName))->ScrollUp(Lines);
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollDown(string ChannelName, int Lines)
{
	(this->FindChannel(ChannelName))->ScrollDown(Lines);
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollToStart(string ChannelName)
{
	(this->FindChannel(ChannelName))->ScrollToStart();
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollToEnd(string ChannelName)
{
	(this->FindChannel(ChannelName))->ScrollToEnd();
	RedrawAllIfChannelSelected(ChannelName);
}
void ChannelHandler::ScrollOneWindow(string ChannelName, bool Up)
{
	(this->FindChannel(ChannelName))->ScrollOneWindow(Up);
	RedrawAllIfChannelSelected(ChannelName);
}

void ChannelHandler::RedrawAllIfChannelSelected(string ChannelName)
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


void ChannelHandler::AddNick(string Nick, string Channel)
{
	FindChannel(Channel)->AddName(Nick);
}
void ChannelHandler::RemoveNick(string Nick, string Channel)
{
	if(Channel.length())
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

void ChannelHandler::Resize(int newWidth, int newHeight)
{
	this->height = newHeight-20;
	this->width = newWidth;
	for (std::list<MessageBoxCF *>::iterator tempIter = ChannelList->begin(); tempIter != ChannelList->end(); tempIter++)
	{
		(*tempIter)->SetRect(0,this->leftbarsize,this->height,this->width);
	}	
}