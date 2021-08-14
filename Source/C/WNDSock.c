#include "WNDSock.h"

#ifdef _WIN32
unsigned char TOTALSOCKCOUNT = 0;
WSADATA SOCKDATA = { 0 };

WNDSOCK createSocket()
{
	if(!TOTALSOCKCOUNT)
	{
		WSAStartup(MAKEWORD(2, 2), &SOCKDATA);
	}

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
	
	if(bind(Socket, (const struct sockaddr*)&Addr, sizeof(Addr))) return WND_ERROR;
	listen(Socket, (int)~0);

	if (!ClientCount) return WND_ERROR;

	WNDSOCK* Ptr = malloc(sizeof(WNDSOCK) * ClientCount);
	if (!Ptr) return WND_ERROR;

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
	if(recv(Socket, &Size, sizeof(*Size), 0) != sizeof(*Size)) return WND_ERROR;

	char* Ptr = malloc(*Size);
	if (!Ptr) return WND_ERROR;

	if (recv(Socket, Ptr, *Size, 0) != *Size) return WND_ERROR;
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
