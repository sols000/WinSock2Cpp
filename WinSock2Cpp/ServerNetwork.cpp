#include "ServerNetwork.h"

unsigned int ServerNetwork::client_id = 0;
ServerNetwork::ServerNetwork(void)
{
	InitServer();
}

ServerNetwork::~ServerNetwork()
{
}

bool ServerNetwork::InitServer()
{
	bool res = false;	
	// create WSADATA object
	WSADATA wsaData;
	// address info for the server to listen to
	struct addrinfo *result = NULL;
	// Initialize Winsock	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	do
	{	
		struct addrinfo hints;
		// set address information
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;    // TCP connection!
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			break;
		}

		// Create a SOCKET for connecting to server
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			break;
		}

		// Set the mode of the socket to be nonblocking
		u_long iMode = 1;
		iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);
		if (iResult == SOCKET_ERROR) {
			printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			break;
		}

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			break;
		}
		// no longer need address information
		freeaddrinfo(result);

		// start listening for new clients attempting to connect
		iResult = listen(ListenSocket, SOMAXCONN);

		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			break;
		}
		res = true;
	} while (0);

	if (!res)
	{//��������
		WSACleanup();
	}

	return res;
}


void ServerNetwork::StartTVServer()
{
	m_workThread = std::thread(&ServerNetwork::update, this);
	m_workThread.detach();
}

// accept new connections
bool ServerNetwork::acceptNewClient(unsigned int & id)
{
	using namespace std;
	// if client waiting, accept the connection and save the socket
	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket != INVALID_SOCKET)
	{
		//disable nagle on the client's socket
		//char value = 1;
		//setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

		// insert new client into session id table
		sessions.insert(pair<unsigned int, SOCKET>(id, ClientSocket));

		return true;
	}

	return false;
}


void ServerNetwork::update()
{
	int ErrorCode = 0;
	while (true)
	{
		if (this->acceptNewClient(client_id))//Not block
		{
			printf("client %d has been connected to the server\n", client_id);
			client_id++;
		}
		auto it = sessions.begin();
		if (it != sessions.end())
		{
			int res = recv(it->second, RecvBuffer, DEFAULT_BUFLEN - 1, 0);
			if (res > 0)
			{
				//const char *Data = "hello";
				//memcpy(RecvBuffer, Data , strlen(Data)+1);
				RecvBuffer[res] = '\0';
				std::string JsonCommandData(RecvBuffer);
				printf("rec:%d\n", res);
			}
			else if (res == -1)
			{
				ErrorCode = WSAGetLastError();
				if (ErrorCode == WSAEWOULDBLOCK)
				{
					//�ȴ�����
				}
				else if (ErrorCode == WSAECONNRESET)
				{
					//���߶���
					ErraseSession(it->first);
				}
				else
				{
					//δ֪����
					ErraseSession(it->first);
					printf("Error:%d��LastCode %d\n", res, WSAGetLastError());
				}
			}
			else if (res == 0 )
			{
				//�Է��ر�����
				ErraseSession(it->first);
			}
			else
			{
				printf("rec Error:%d��code %d\n", res, WSAGetLastError());
			}
		}
	}
}

void ServerNetwork::ErraseSession(UINT id)
{
	auto it = sessions.find(id);
	if (it != sessions.end())
	{
		closesocket(it->second);
		sessions.erase(it);
		printf("Erase A Session,Error:%d\n", WSAGetLastError());
	}
}
