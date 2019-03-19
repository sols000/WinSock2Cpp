//File: Client.h
#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "NetworkService.h"
#include <ws2tcpip.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientNetwork
{

public:

	// for error checking function calls in Winsock library
	int iResult;

	// socket for client to connect to server
	SOCKET ConnectSocket;

	// ctor/dtor
	ClientNetwork(void);
	~ClientNetwork(void);
};