#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <winsock.h>

//Definitions.
typedef SOCKET WNDSOCK;
typedef struct sockaddr_in WNDSOCKADDR;

#define WND_ERROR (unsigned char)1

#elif __unix__
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef unsigned int WNDSOCK;
typedef struct sockaddr_in WNDSOCKADDR;

#define WND_ERROR (unsigned char)0

#endif

WNDSOCK createSocket();
WNDSOCK* hostSocket(WNDSOCK Socket, const char* IP, unsigned short Port, unsigned char ClientCount);
unsigned char joinSocket(WNDSOCK Socket, const char* IP, unsigned short Port);
unsigned char sendData(WNDSOCK Socket, const char* Data, unsigned int Size);
char* recieveData(WNDSOCK Socket, unsigned int* Size);
void closeSocket(WNDSOCK Socket);
