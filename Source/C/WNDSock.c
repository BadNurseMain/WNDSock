#include "WNDSock.h"

#ifdef _WIN32

//Definitions.
#pragma comment(lib, "Ws2_32.lib")

#define WND_IPV4 AF_INET
#define WND_STREAM SOCK_STREAM
#define WND_TCP IPPROTO_TCP
#define WNDSOCK_INVAL INVALID_SOCKET

#define WND_STRTOIP(IP) inet_addr(IP);

//Variables Required for WSA.
unsigned char TOTALSOCKCOUNT = 0;
WSADATA SOCKDATA = { 0 };

//Windows Functions for Sockets.
WNDSOCK createSocket()
{
	if(!TOTALSOCKCOUNT) WSAStartup(MAKEWORD(2, 2), &SOCKDATA);

	WNDSOCK Conn = socket(WND_IPV4, WND_STREAM, WND_TCP);
	if (Conn == WNDSOCK_INVAL) return 1;

	++TOTALSOCKCOUNT;
	return Conn;
}

WNDSOCK* hostSocket(WNDSOCK Socket, const char* IP, unsigned short Port, unsigned char ClientCount)
{
	WNDSOCKADDR Addr = { 0 };
	Addr.sin_addr.S_un.S_addr = WND_STRTOIP(IP);
	Addr.sin_port = Port;
	Addr.sin_family = WND_IPV4;
	
	if(bind(Socket, (const struct sockaddr*)&Addr, sizeof(Addr))) return NULL;
	listen(Socket, (int)~0);

	if (!ClientCount) return NULL;

	WNDSOCK* Ptr = malloc(sizeof(WNDSOCK) * ClientCount);
	if (!Ptr) return NULL;

	for (unsigned char x = 0; x < ClientCount; x++)
	{
		Ptr[x] = accept(Socket, NULL, NULL);
		
		if(Ptr[x] == WND_ERROR)
		{
			free(Ptr);
			return NULL;
		}
	}

	return Ptr;
}

unsigned char joinSocket(WNDSOCK Socket, const char* IP, unsigned short Port)
{
	WNDSOCKADDR addr = { 0 };
	addr.sin_addr.S_un.S_addr = WND_STRTOIP(IP);
	addr.sin_family = WND_IPV4;
	addr.sin_port = Port;

	if(connect(Socket, (const struct sockaddr*)&addr, sizeof(addr))) WND_ERROR;

	return 0;
}

unsigned char sendData(WNDSOCK Socket, const char* Data, unsigned int Size)
{
	if(send(Socket, (const char*)&Size, sizeof(Size), 0) != sizeof(Size)) return WND_ERROR;
	if(send(Socket, Data, Size, 0) != Size) return WND_ERROR;
	return 0;
}

char* recieveData(WNDSOCK Socket, unsigned int* Size)
{
	if(recv(Socket, (char*)&Size, sizeof(*Size), 0) != sizeof(*Size)) return NULL;

	char* Ptr = malloc(*Size);
	if (!Ptr) return NULL;

	if (recv(Socket, Ptr, *Size, 0) != *Size) return NULL;
	return Ptr;
}

void closeSocket(WNDSOCK Socket)
{
	closesocket(Socket);
	if (!(--TOTALSOCKCOUNT)) WSACleanup();
	return;
}

#elif __unix__


#endif
