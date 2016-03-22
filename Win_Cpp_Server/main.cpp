#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <sdkddkver.h>
#include <conio.h>
#include <stdio.h>
#include <string>
#include <iterator>

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

//#pragma comment(lib, "Ws2_32.lib")  // Or alternatively add this library in the additional dependencies in the property sheet

using namespace std;
using namespace cv;

int main() {
	char eduroam_pc_ip[] = "10.89.131.94";
	char myRouter_pc_ip[] = "192.168.1.103";

	char window_name[] = "Video";
	Mat frame;

	WSADATA wsaData;
	WORD version;
	int error;

	version = MAKEWORD(2, 2);
	error = WSAStartup(version, &wsaData);
	if (error == 0) {
		printf("Wsa init success!\n");
	}
	else {
		printf("Wsa init failed\n");
		return -1;
	}
	/* check for correct version */
	/*if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		// incorrect WinSock version 
		WSACleanup();
		printf("Incorrect winsock version!");
		return -1;
	} */

	SOCKET server;
	server = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sin;

	memset(&sin, 0, sizeof sin);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(eduroam_pc_ip);
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

	while (true) {
		SOCKET client;
		client = socket(AF_INET, SOCK_STREAM, NULL);		
		client = accept(server, (SOCKADDR*)&sin, &length);
		
		//send integers
		int32_t command1_t = -200;
		uint32_t command2_t = 300;
		command1_t = htonl(command1_t);
		command2_t = htonl(command2_t);

		//Receive values
		int32_t size_t;
		int32_t info1_t;
		int32_t info2_t;

		error = recv(client, (char *)&size_t, sizeof int32_t, 0);
		error = recv(client, (char *)&info1_t, sizeof int32_t, 0);
		error = recv(client, (char *)&info2_t, sizeof int32_t, 0);

		size_t = ntohl(size_t);
		info1_t = ntohl(info1_t);
		info2_t = ntohl(info2_t);
		int size = size_t;
		int info1 = info1_t;
		int info2 = info2_t;
		printf("Received integers: \n");
		cout << info1 << " " << info2 << endl;
		cout << "Received long size: ";
		cout << size << endl;

		//Receive image frame
		//vector<char> buffer(size);
		vector<char> buffer;
		char buf[1024];
		int size_read = 0;
		while(size > 0) {
			size_read = recv(client, buf, min(size,1024), 0);
			//buffer.data()[i] = buf;
			buffer.insert(buffer.end(), buf, buf + size_read);
			size = size - size_read;
		}
		
		imdecode(buffer,CV_LOAD_IMAGE_COLOR, &frame); 
		
		//Send integers
		error = send(client, (char *)&command1_t, sizeof(int), 0);
		error = send(client, (char *)&command2_t, sizeof(int), 0);

		//Show the image frame
		imshow(window_name, frame);


		int c = waitKey(5);
		if ((char)c == 27) { break; } // escape 
	}

	WSACleanup();
	return 0;
}