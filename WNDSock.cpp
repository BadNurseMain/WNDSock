#ifdef _WIN32
#include "WNDSock.h"

#ifndef WNDSockDefines
#define WNDSockDefines

#define SOCK_HOST (unsigned char)1
#define SOCK_CLIENT (unsigned char)2

#define SD_RECIEVE (unsigned char)0
#define SD_SEND (unsigned char)1
#define SD_BOTH (unsigned char)2

#endif

unsigned int OpenSockets = 0;
WSAData Data = { 0 };

WNDSock::WNDSock(const char* Address, unsigned short Port)
{
	//Check Winsock has been initialized.
	if (!OpenSockets) WSAStartup(MAKEWORD(2, 2), &Data);

	//Creation of a TCP socket.
	Conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Conn == INVALID_SOCKET) return;

	//Preparing the information for the Socket.
	Addr.sin_family = AF_INET;
	Addr.sin_addr.S_un.S_addr = inet_addr(Address);
	Addr.sin_port = Port;
	++OpenSockets;
	return;
}

unsigned char WNDSock::Host(unsigned int Backlog)
{
	//Closing Socket if it was previously
	//used for anything else.
	if(CurrentType) closesocket(Conn);

	//Setting up Socket to receive connections.
	if (bind(Conn, (const sockaddr*)&Addr, sizeof(Addr)))
	{
		printf("Error occured creating socket. \nError code: %d", WSAGetLastError());
		return 1;
	}

	CurrentType = SOCK_HOST;
	
	if(listen(Conn, Backlog) == SOCKET_ERROR) return 2;
	
	ClientSockets = accept(Conn, NULL, NULL);
	if (ClientSockets == INVALID_SOCKET) return 3;

	++ClientSocketCount;

	return 0;
}

unsigned char WNDSock::Join(const char* Address, unsigned short Port)
{
	struct sockaddr_in HostAddr = { 0 };
	HostAddr.sin_family = AF_INET;
	HostAddr.sin_addr.S_un.S_addr = inet_addr(Address);
	HostAddr.sin_port = Port;

	if(connect(HostSocket, (const sockaddr*)&HostAddr, sizeof(HostAddr))) return 1;
	return 0;
}


unsigned char WNDSock::Send(char* Buffer, unsigned int BufferSize)
{
	if (send(ClientSockets, (char*)&BufferSize, sizeof(int), 0) == SOCKET_ERROR)
	{
		printf("Error has occurred sending buffer size. \nError code: %d", WSAGetLastError());
		return 1;
	}
	
	if (send(ClientSockets, Buffer, BufferSize, 0) != BufferSize)
	{
		printf("Error has occurred sending buffer. \nError code: %d", WSAGetLastError());
		return 2;
	}

	return 0;
}

char* WNDSock::Recieve()
{
	unsigned int Size = 0;
	if(recv(Conn, (char*)&Size, sizeof(Size), 0) == SOCKET_ERROR)
	{
		printf("%d", WSAGetLastError());
		return NULL;
	}
	
	char* DataPtr = (char*)malloc(Size);
	if (!DataPtr) return NULL;

	if (recv(Conn, DataPtr, Size, 0) != Size) return NULL;
	
	return DataPtr;
}

WNDSock::~WNDSock()
{
	--OpenSockets;
	if (!OpenSockets) WSACleanup();
	return;
}


#elif __unix__


#endif