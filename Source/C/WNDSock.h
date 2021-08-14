#ifdef _WIN32
#include <winsock.h>

//Definitions.
typedef SOCKET WNDSOCK;
typedef struct sockaddr_in WNDSOCKADDR;

#define WND_ERROR (unsigned char)1

#elif __unix__

#include <sys/socket.h>
#include <arpa/inet.h>
#define NULL (void*)0

typedef unsigned int WNDSOCK;

#define WND_IPV4 AF_INET
#define WND_STREAM SOCK_STREAM
#define WND_TCP IPPROTO_TCP

#define WND_ERROR (unsigned char)0

#define WND_STRTOIP(IP) inet_addr(IP);

#endif

WNDSOCK createSocket();
WNDSOCK* hostSocket(WNDSOCK Socket, const char* IP, unsigned short Port, unsigned char ClientCount);
unsigned char joinSocket(WNDSOCK Socket, const char* IP, unsigned short Port);
unsigned char sendData(WNDSOCK Socket, const char* Data, unsigned int Size);
char* recieveData(WNDSOCK Socket, unsigned int* Size);
void closeSocket(WNDSOCK Socket);
