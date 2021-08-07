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
		unsigned char ExSocketCount = 0;
		SOCKET* ExSocket = NULL;

	public:

		WNDSock(const char* Address, unsigned short Port);
		
		unsigned char Host(unsigned int Backlog);
		
		unsigned char Listen(unsigned int Backlog, unsigned char Count);

		unsigned char Join(const char* Address, unsigned short Port);

		unsigned char Send(char* Buffer, unsigned int BufferSize);

		unsigned char Send(char* Buffer, unsigned int BufferSize, unsigned char Index);

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
