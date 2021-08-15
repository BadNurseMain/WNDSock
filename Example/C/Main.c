#include <stdlib.h>
#include <stdio.h>
#include "WNDSock.h"

//Works for both Windows and Linux :)
int main()
{
	int Res = 0;
	scanf_s("%d", &Res);

	if(Res == 1)
	{
		WNDSOCK x = createSocket();
		WNDSOCK* Clients = hostSocket(x, "192.168.1.1", 7209, 1);
		
		if (!Clients) return 1;

		const char* Buffer = "HowdyGamer\0";
		
		if(sendData(Clients[0], Buffer, strlen(Buffer) + 1) == WND_ERROR) return 2;
		
		free(Clients);
		closeSocket(x);
	} else 
	{
		WNDSOCK x = createSocket();
		
		if (joinSocket(x, "192.168.1.1", 7209)) return 1;
		
		unsigned int Total Size = 0;
		char* Buffer = receieveData(x, &TotalSize);
		
		if(!Buffer) return 2;
		
		printf("%s \n", Buffer);
		closeSocket(x);
	}
	
	return 0;
}
