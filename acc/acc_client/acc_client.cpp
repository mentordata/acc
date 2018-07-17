// acc_client.cpp: Definiuje funkcje wyeksportowane dla aplikacji DLL.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
//#include "..\acc\acc.h"
#include "..\acc\tcp_server.h"


#define MT4_EXPFUNC __declspec(dllexport)

static SOCKET AccClientSocket;
//MT4_EXPFUNC int __stdcall acc_client_init(wchar_t* _wserver, int _port);

extern "C"
{

MT4_EXPFUNC int __stdcall acc_client_init(wchar_t* _wserver, int _port)
	{
		WSADATA wsaData;
//		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		int iResult;
		int recvbuflen = TCP_SERVER_BUFLEN;

		char _sport[10];
		char _server[50];



//		wcstombs(_server, _wserver, 50);

		sprintf(_sport, "%d", TCP_SERVER_PORT);

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return -1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo("localhost", _sport, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return -2;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			AccClientSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (AccClientSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return -3;
			}

			// Connect to server.
			iResult = connect(AccClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(AccClientSocket);
				AccClientSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (AccClientSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return -4;
		}

		return 0;	// success
	};

	MT4_EXPFUNC int __stdcall acc_client_send(wchar_t* _wstr, char *_buf)
	{
//		wchar_t _wbuf[TCP_SERVER_BUFLEN];
//		char _buf[TCP_SERVER_BUFLEN];
		char _str[TCP_SERVER_BUFLEN];
		int iResult;

		//wsprintf(_buf,L"Hello DLL. Twin Peaks is everywhere !!! String passed: %s\n", _str);
		// Send an initial buffer
		
		wcstombs(_str, _wstr, TCP_SERVER_BUFLEN);//  mbstring, wcstring, N)

		iResult = send(AccClientSocket, _str, (int)strlen(_str), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(AccClientSocket);
			WSACleanup();
			return NULL;
		}

		printf("Bytes Sent: %ld\n", iResult);
	
		// shutdown the connection since no more data will be sent
/*		iResult = shutdown(AccClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(AccClientSocket);
			WSACleanup();
			return NULL;

		}
*/
		// Receive until the peer closes the connection
//		do {

			iResult = recv(AccClientSocket, _buf, TCP_SERVER_BUFLEN, 0);
			if (iResult > 0)
				printf("Bytes received: %d\n", iResult);
			else if (iResult == 0)
				printf("Connection closed\n");
			else
				printf("recv failed with error: %d\n", WSAGetLastError());

//		} while (iResult > 0);


		
		return iResult;
	};
}

