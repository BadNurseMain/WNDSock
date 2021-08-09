#ifndef WNDSockDefines
#define WNDSockDefines

#include "WNDSock.h"
#include <stdlib.h>

#define SOCK_HOST (unsigned char)1
#define SOCK_CLIENT (unsigned char)2

#define SD_RECIEVE (unsigned char)0
#define SD_SEND (unsigned char)1
#define SD_BOTH (unsigned char)2

#endif


#ifdef _WIN32

unsigned int OpenSockets = 0;
WSAData Data = { 0 };

WNDSock::WNDSock()
{
	//Check Winsock has been initialized.
	if (!OpenSockets) WSAStartup(MAKEWORD(2, 2), &Data);

	//Creation of a TCP socket.
	Conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Conn == INVALID_SOCKET) return;

	++OpenSockets;
	return;
}

unsigned char WNDSock::Host(const char* Address, unsigned short Port)
{
	struct sockaddr_in Addr = { 0 };
	Addr.sin_addr.S_un.S_addr = inet_addr(Address);
	Addr.sin_port = Port;
	Addr.sin_family = AF_INET;

	//Closing Socket if it was previously
	//used for anything else.
	if(CurrentType) closesocket(Conn);

	//Setting up Socket to receive connections.
	if (bind(Conn, (const sockaddr*)&Addr, sizeof(Addr))) return 1;

	CurrentType = SOCK_HOST;
	ExSocketCount = 0;
	return 0;
}

unsigned char WNDSock::Listen(unsigned int Backlog, unsigned char Count)
{
	if (listen(Conn, Backlog) == SOCKET_ERROR) return 1;

	for (unsigned char x = 0; x < Count; x++)
	{
		if (!ExSocketCount) ExSocket = (SOCKET*)malloc(sizeof(SOCKET));
		else
		{
			void* tempPtr = realloc(ExSocket, (ExSocketCount + 1) * sizeof(SOCKET));
			ExSocket = (SOCKET*)tempPtr;
		}

		if (!ExSocket) return 2;
		
		ExSocket[ExSocketCount] = accept(Conn, NULL, NULL);
		if (ExSocket[ExSocketCount] == INVALID_SOCKET) return 3;
		
		++ExSocketCount;
	}

	return 0;
}


unsigned char WNDSock::Join(const char* Address, unsigned short Port)
{
	struct sockaddr_in HostAddr = { 0 };
	HostAddr.sin_family = AF_INET;
	HostAddr.sin_addr.S_un.S_addr = inet_addr(Address);
	HostAddr.sin_port = Port;

	if(CurrentType)
	{
		free(ExSocket);
		ExSocketCount = 0;
	}

	CurrentType = SOCK_CLIENT;
	
	ExSocket = (SOCKET*)malloc(sizeof(SOCKET));
	if (!ExSocket) return 1;

	*ExSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while (connect(Conn, (const sockaddr*)&HostAddr, sizeof(HostAddr)));

	ExSocketCount = 1;
	return 0;
}

unsigned char WNDSock::Send(char* Buffer, unsigned int BufferSize)
{
	if (send(*ExSocket, (char*)&BufferSize, sizeof(int), 0) == SOCKET_ERROR) return 1;
	if (send(*ExSocket, Buffer, BufferSize, 0) != BufferSize) return 2;
	return 0;
}

unsigned char WNDSock::Send(char* Buffer, unsigned int BufferSize, unsigned char Index)
{
	if (ExSocketCount - 1 < Index) return 1;
	
	if (send(ExSocket[Index], (char*)&BufferSize, sizeof(int), 0) == SOCKET_ERROR) return 2;
	if (send(ExSocket[Index], Buffer, BufferSize, 0) != BufferSize) return 3;
	return 0;
}

char* WNDSock::Recieve(unsigned int* Size)
{
	if(recv(Conn, (char*)Size, sizeof(Size), 0) == SOCKET_ERROR) return NULL;
	
	char* DataPtr = (char*)malloc(*Size);
	if (!DataPtr) return NULL;

	if (recv(Conn, DataPtr, *Size, 0) != *Size) return NULL;
	return DataPtr;
}

WNDSock::~WNDSock()
{
	--OpenSockets;
	if (!OpenSockets) WSACleanup();
	return;
}


#elif __unix__
#define INVALID_SOCKET -1
#include <unistd.h>

unsigned int OpenSockets = 0;


WNDSock::WNDSock()
{
	Conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(Conn == -1) Conn = 0;
	return;
}

unsigned char WNDSock::Host(const char* Address, unsigned short Port)
{
	struct sockaddr_in Addr = {0};
	Addr.sin_addr.s_addr = inet_addr(Address);
	Addr.sin_port = Port;
	Addr.sin_family = AF_INET;

	CurrentType = SOCK_HOST;

	return bind(Conn, (const sockaddr*)&Addr, sizeof(Addr));
}

unsigned char WNDSock::Listen(unsigned int Backlog, unsigned char Count)
{
	listen(Conn, Backlog);

	ExSocket = (unsigned int*)malloc(sizeof(int) * Count);
	if(!ExSocket) return 1;

	ExSocketCount = Count;
	
	for(unsigned char x = 0; x < Count; x++)
	{
		ExSocket[x] = accept4(Conn, NULL, NULL, 0);
		if(ExSocket[x] == INVALID_SOCKET) return 2;
	}

	return 0;
}

unsigned char WNDSock::Join(const char* Address, unsigned short Port)
{
	struct sockaddr_in Addr = {0};
	Addr.sin_addr.s_addr = inet_addr(Address);
	Addr.sin_port = Port;
	Addr.sin_family = AF_INET;

	CurrentType = SOCK_CLIENT;

	return connect(Conn, (const sockaddr*)&Addr, sizeof(Addr));
}

WNDSock::~WNDSock()
{
	close(Conn);
	return;
}

#endif
