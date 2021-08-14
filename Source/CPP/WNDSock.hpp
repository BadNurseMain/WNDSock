#ifdef _WIN32

#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")

#elif __unix__

#include <sys/socket.h>
#include <arpa/inet.h>
#define NULL (void*)0

#endif

class WNDSock
{
private:
#ifdef _WIN32
	//Socket Setup Information.
	SOCKET Conn = NULL;

	//Socket State Information.
	unsigned char CurrentType = 0;
	unsigned char ExSocketCount = 0;
	SOCKET* ExSocket = NULL;

#elif __unix__
	//Socket Setup Information.
	unsigned int Conn = 0;

	//Socket State Information.
	unsigned char CurrentType = 0;
	unsigned char ExSocketCount = 0;
	unsigned int* ExSocket = (unsigned int*)NULL;
#endif

public:

	WNDSock();

	unsigned char Host(const char* Address, unsigned short Port);

	unsigned char Listen(unsigned int Backlog, unsigned char Count);

	unsigned char Join(const char* Address, unsigned short Port);

	unsigned char Send(char* Buffer, unsigned int BufferSize);

	unsigned char Send(char* Buffer, unsigned int BufferSize, unsigned char Index);

	char* Recieve(unsigned int* Size);

	~WNDSock();
};
