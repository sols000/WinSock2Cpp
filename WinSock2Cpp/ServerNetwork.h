#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "NetworkService.h"
#include <ws2tcpip.h>
#include <map>
#include <thread>

#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "15789"

class ServerNetwork
{

public:

	ServerNetwork(void);
	~ServerNetwork();

	void StartTVServer();

private:
	bool InitServer();

	// accept new connections
	bool acceptNewClient(unsigned int & id);

	void update();

	void ErraseSession(UINT id);

private:	
		
	// Socket to listen for new connections
	SOCKET ListenSocket = INVALID_SOCKET;
	// Socket to give to the clients
	SOCKET ClientSocket = INVALID_SOCKET;
	// table to keep track of each client's socket
	std::map<unsigned int, SOCKET> sessions;
	//�����߳�
	std::thread m_workThread;
	// for error checking return values
	int iResult;
	//�ܹ������ĶԻ���Ŀ
	static unsigned int client_id;
	//���ջ�����
	//char RecvBuffer[DEFAULT_BUFLEN];
	char *RecvBuffer;

};