#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <sdkddkver.h>
#include <conio.h>
#include <stdio.h>
#include <string>

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

//#pragma comment(lib, "Ws2_32.lib")  // Or alternatively add this library in the additional dependencies in the property sheet

using namespace std;

int main() {
	char eduroam_pc_ip[] = "10.89.131.94";
	char myRouter_pc_ip[] = "192.168.1.101";

	WSADATA wsaData;
	WORD version;
	int error;

	version = MAKEWORD(2, 0);
	error = WSAStartup(version, &wsaData);
	if (error == 0) {
		printf("Wsa init success!\n");
	}
	else {
		printf("Wsa init failed\n");
		return -1;
	}
	/* check for correct version */
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		/* incorrect WinSock version */
		WSACleanup();
		printf("Incorrect winsock version!");
		return -1;
	}

	SOCKET server;
	server = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sin;

	memset(&sin, 0, sizeof sin);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("10.89.131.94");
	sin.sin_port = htons(8080);

	if (bind(server, (SOCKADDR*)&sin, sizeof sin) == SOCKET_ERROR)
	{
		printf("Cannot start server!");
		WSACleanup();
		return -1;
	}

	//Listen for incoming requests
	listen(server, SOMAXCONN);
	SOCKET client;
	int length = sizeof sin;

	//string s = to_string(sizeof (int));
	//cout << s << endl;

	while (true) {
		SOCKET client;
		client = socket(AF_INET, SOCK_STREAM, NULL);
		
		printf("Waiting for incoming connection...\n");
		client = accept(server, (SOCKADDR*)&sin, &length);
		
		printf("A Connection was established!\n");

		//send a message
		int t1 = -200;
		int t2 = -300;
		int32_t test = t1;
		uint32_t test1 = t2;
		test = htonl(test);
		test1 = htonl(test1);

		//Receive integers
		int32_t recv1;
		int32_t recv2;

		error = recv(client, (char *)&recv1, 32, 0);
		error = recv(client, (char *)&recv2, 32, 0);

		recv1 = ntohl(recv1);
		recv2 = ntohl(recv2);
		int r1 = recv1;
		int r2 = recv2;
		printf("Received integers: \n");
		cout << "Get 2 integers! " << endl;
		cout << r1 << " " << r2 << endl;
		
		//Send integers
		error = send(client, (char *)&test, sizeof(int), 0);
		error = send(client, (char *)&test1, sizeof(int), 0);
		
	}

	WSACleanup();
	return 0;
}