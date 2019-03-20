#include"ClientNetwork.h"
#include"ServerNetwork.h"
#include<process.h>
#include<thread>
#include <iostream>

ServerNetwork* server;
int main(int argc, char *argv[]) {

	//std::cerr << "Server Start\n";
	server = new ServerNetwork();
	server->StartTVServer();

	getchar();
}
