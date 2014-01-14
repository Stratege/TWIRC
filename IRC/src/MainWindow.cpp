#include "MainWindow.h"

#define MouseSpeedMultiplikator 0.1

MainWindow::MainWindow(HINSTANCE g_pInstance, HWND g_pWindow)
{

	this->pInstance = g_pInstance;
	this->pWindow = g_pWindow;

	//this needs to be up here
	this->pReadInit = new ReadInit();

	this->iWindowWidth = this->pReadInit->GetWindowWidth();
	this->iWindowHeight = this->pReadInit->GetWindowHeight();

	this->pReadInit->MessagesToExecute->ClearMessagebox();


	this->pHDC = GetDC(this->pWindow);
	this->pBufferHDC = CreateCompatibleDC(this->pHDC);
//	HBITMAP g_pBackbufferBitmap = CreateCompatibleBitmap(this->pHDC,this->iWindowWidth,this->iWindowHeight);
//	SelectObject(this->pBufferHDC,g_pBackbufferBitmap);
//	SetBkColor(this->pBufferHDC,RGB(0,255,0));
//	SetTextColor(this->pBufferHDC,RGB(255,255,255));

//	this->pBufferHDC = CreateCompatibleDC(this->pHDC);
//	HBITMAP g_pBackbufferBitmap = CreateCompatibleBitmap(this->pHDC,this->iWindowWidth,this->iWindowHeight);
//	SelectObject(this->pBufferHDC,g_pBackbufferBitmap);
//	SetBkColor(g_pBackbufferDC,RGB(0,0,0));
//	SetTextColor(g_pBackbufferDC,RGB(255,255,255));


	this->pInput = new Input();
	this->pChannelHandler = new ChannelHandler(this->iWindowHeight,this->iWindowWidth, LEFTBARSIZE);
	this->pSocket = new SocketHandling();
	this->pFileWriting = new FileWriting(this->pReadInit->GetSavepath());
	this->pIncomingMessagesHandling = new IncomingMessagesHandling(this,this->pReadInit->GetNick(),this->pReadInit->GetUser(),this->pReadInit->GetRealname());

	SetWindowPos(this->pWindow, 0, 0, 0, this->iWindowWidth, this->iWindowHeight+25, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	this->pReadInit->MessagesToExecute->ClearMessagebox();

}

MainWindow::~MainWindow()
{
	delete(pChannelHandler);
	delete(pSocket);
	delete(pFileWriting);
	delete(pIncomingMessagesHandling);
	delete(pInput);
	delete(pReadInit);
}

bool MainWindow::OpenConnection()
{
	if(pSocket->OpenConnection("IRC",this->pReadInit->GetServeraddress(),this->pReadInit->GetPort()))
	{
		char startingMessage[600];
		snprintf(startingMessage,600,"/USER %s 0 *: %s\r\n",this->pReadInit->GetUser(),this->pReadInit->GetRealname());
		this->SendMessage(startingMessage);
		snprintf(startingMessage,600,"/NICK %s\r\n",this->pReadInit->GetNick());
		this->SendMessage(startingMessage);
		return true;
	}
	return false;
}

void MainWindow::InitProgram()
{

	if(OpenConnection()){
		MSG msg;
		while(GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void MainWindow::DrawAll()
{
	this->pChannelHandler->DrawSelected();
	this->pChannelHandler->DrawChannellist();

	this->SwapBuffers();
}

void MainWindow::SwapBuffers()
{
	BitBlt(g_pFrontbufferDC,0,0,this->iWindowWidth,this->iWindowHeight,g_pBackbufferDC,0,0,SRCCOPY);
	BitBlt(g_pBackbufferDC,0,0,this->iWindowWidth,this->iWindowHeight,NULL,0,0,BLACKNESS);
}

void MainWindow::OutputInternalMessage(char *msg)
{
	char *channelname = "-!InternalMessage";
#ifndef DisableTextLogging
	pFileWriting->PrintOutputToFile(channelname,msg);
#endif
	pChannelHandler->WriteToChannel(channelname,msg);
}

void MainWindow::OutputMessage(Message *msg)
{

	//everything wants us to copy things into a new string
	//and have the date at the beginning. So that can be done independently
	char *newMsg = new char[800];
	char *channelname = NULL;
	pFileWriting->WriteTimeAndDate(newMsg);

	if(msg->Outgoing)
	{
		if(msg->isNormalMessage) // || StringCompare(msg,"NOTICE"))
		{
			//Outgoing:
			//this is an outgoing transmission of the style:
			//PRIVMSG CHANNELNAME :MSG
			//so we need to print it into channelname.log
			//final printing should look something like:
			//time - nick: msg

			//so:
			//copy date into new string - done
			//copy nick into new string - done
			//copy actual message into new string - done
			//write new string into channelname.log - done
			//write new string into Chanel with Name channelname - done
			pFileWriting->WriteNickname(newMsg,CurrentNickname);
			pFileWriting->WriteMessage(newMsg,msg->formatedMessage);
			
			//shody code for channel selection
//			int Offset = FindSign(msg,' ') + 1;
//			int Offset2 = FindSign(&msg[Offset],' ');
//			memcpy(channelname,&msg[Offset],Offset2);
//			channelname[Offset2] = '\0';
			channelname = this->GetSelectedChannelName();

		}else{

			//Outgoing but not a PRIVMSG?
			//something else. Something strange. Something undefined.
			//but still outgoing. Supposedly?	
			//log it to unknownSentMessages.log

			//so:
			//copy date into new string - done
			//copy msg into new string - done
			//write new string into unknownSentMessages.log - done
			//write new string into Chanel with Name unknownSentMessages - done
			snprintf(newMsg,800,"%s%s",newMsg,msg->formatedMessage);
			channelname = "unknownSentMessages";
		}
	}else{
		//Incoming
		//Type 1: Query
		//Type 2: Channelmsg
		//Type -1: Unknown
		if(msg->GetNUHStatus())
		{
			int type = 0;
			if(!strcmp(msg->ParameterArray[1],CurrentNickname)) type = 1;
			else if(!strncmp(msg->ParameterArray[1],"#",1)) type = 2;
			//for now we declare "-!" to be a channel as well
			else if(!strncmp(msg->ParameterArray[1],"-!",2)) type = 2;
			else type = -1;

			if(type == 1 || type == 2)
			{
				//Query:
				//date - OtherNick: msg
				//into OtherNick.log

				strcat(newMsg,msg->Nick);
				strcat(newMsg,": ");
				msg->MergeParameterArray(2);
				strcat(newMsg,msg->formatedMessage);

				if(type == 1){
					channelname = msg->Nick;
				}else
				if(type == 2){
					channelname = msg->ParameterArray[1];
				}
			}else if (type == -1){
				//If Type -1: Unknown
				//unknown message means we want to print:
				//date - msg
				//into unknownRecievedMsgs.log
		
				//so:
				//copy date into new string - done
				//copy msg into new string
				//write new string into unknownRecievedMsgs.log
				//write new string into Chanel with Name unknownRecievedMsgs
				snprintf(newMsg,800,"%s%s",newMsg,msg->formatedMessage);
				channelname = "unknownRecievedMsgs";
			}
		}else{
			snprintf(newMsg,800,"%s%s",newMsg,msg->formatedMessage);
			channelname = "unknownRecievedMsgs";
		}
	}

#ifndef DisableTextLogging
	pFileWriting->PrintOutputToFile(channelname,newMsg);
#endif
	pChannelHandler->WriteToChannel(channelname,newMsg);
	delete newMsg;
	//delete channelname;
}

SOCKET MainWindow::GetSocket()
{
	return pSocket->GetSocket();
}

char *MainWindow::GetSelectedChannelName()
{
	return this->pChannelHandler->GetSelectedChannelName();
}

void MainWindow::ReadMessage()
{
	this->pIncomingMessagesHandling->OutputHandling();
}

void MainWindow::SocketClosedMessage()
{
	//socket got closed. Socket gets opened.
	//as soon as it is implemented, this should probably also reopen all channels and do an identify - though that might already be handled then via the ResetCount() call.
	//the above concern has been handled
	this->pIncomingMessagesHandling->ResetCount();
	this->OpenConnection();
}

void MainWindow::SendMessage(char *msg)
{
	this->pInput->SendMessage(msg);
}

void MainWindow::SelectChannel(char *Name)
{
	this->pChannelHandler->SelectChannel(Name);
}

void MainWindow::CloseChannel(char *Name)
{
	this->pChannelHandler->CloseChannel(Name);
}

void MainWindow::CloseAll()
{
	this->pChannelHandler->CloseAll();
}

void MainWindow::ScrollChannelUp(int Lines)
{
	this->pChannelHandler->ScrollChannelUp(Lines);
}

void MainWindow::ScrollChannelDown(int Lines)
{
	this->pChannelHandler->ScrollChannelDown(Lines);	
}

void MainWindow::ScrollUp(char *ChannelName, int Lines)
{
	this->pChannelHandler->ScrollUp(ChannelName,Lines);
}
void MainWindow::ScrollDown(char *ChannelName, int Lines)
{
	this->pChannelHandler->ScrollDown(ChannelName,Lines);
}
void MainWindow::ScrollToStart(char *ChannelName)
{
	this->pChannelHandler->ScrollToStart(ChannelName);
}
void MainWindow::ScrollToEnd(char *ChannelName)
{
	this->pChannelHandler->ScrollToEnd(ChannelName);
}
void MainWindow::ScrollOneWindow(char *ChannelName, bool Up)
{
	this->pChannelHandler->ScrollOneWindow(ChannelName,Up);
}
void MainWindow::CopyCurrentLine()
{
	this->pChannelHandler->CopyCurrentLine();
}

void MainWindow::AddNick(char *Nick, char *Channel)
{
	this->pChannelHandler->AddNick(Nick,Channel);
}
void MainWindow::RemoveNick(char *Nick, char *Channel)
{
	this->pChannelHandler->RemoveNick(Nick,Channel);
}

void MainWindow::ShowUserList()
{
	this->pChannelHandler->ShowUserList();
}