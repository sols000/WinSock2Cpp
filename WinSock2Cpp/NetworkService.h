#pragma once
#include<WinSock2.h>
#include<Windows.h>

class NetworkServices
{
public:
	static int sendMessage(SOCKET curSocket, char* message, int messagesize);
	static int receiveMessage(SOCKET curSocket, char* buffer, int bufsize);


};

