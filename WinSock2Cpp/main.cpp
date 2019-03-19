#include"ClientNetwork.h"
#include"ServerNetwork.h"
#include<process.h>
#include<thread>
void serverLoop(void *);
ServerNetwork* server;
ClientNetwork* client;
int main(int argc, char *argv[]) {

	server = new ServerNetwork();
	server->StartTVServer();

	//client = new ClientNetwork();

	system("Pause");
}
