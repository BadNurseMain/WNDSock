#ifdef _WIN32

#ifndef STD
#define STD

#include <stdlib.h>
#include <stdio.h>
#endif

#include <Windows.h>
#include <winsock.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib")

class WNDSock
{
	private:
		//Socket Setup Information.
		SOCKET Conn = NULL;
		struct sockaddr_in Addr = { 0 };
		
		//Socket State Information.
		unsigned char CurrentType = 0;

		//Hosting only.
		unsigned __int64 ClientSocketCount;
		SOCKET ClientSockets;

		//Client Only.
		SOCKET HostSocket;

	public:

		WNDSock(const char* Address, unsigned short Port);
		
		unsigned char Host(unsigned int Backlog);
		
		unsigned char Join(const char* Address, unsigned short Port);

		unsigned char Send(char* Buffer, unsigned int BufferSize);

		char* Recieve();
		
		~WNDSock();

};

#elif __unix__
#ifndef STD
#define STD

#include <stdio.h>
#include <stdlib.h>
#endif

#include <sys/socket.h>
class WNDSock
{
	private:
		unsigned int xd;
};

#endif