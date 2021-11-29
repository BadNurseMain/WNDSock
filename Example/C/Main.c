#include <stdlib.h>
#include <stdio.h>
#include "WNDSock.h"

//Works for both Windows and Linux :)
int main()
{
	int Res = 0;
	scanf_s("%d", &Res);

	if (Res == 1)
	{
		WNDSOCK x = createSocket();
		WNDSOCK* Clients = hostSocket(x, "192.168.1.23", 7209, 1);

		if (!Clients)
		{
			printf("No Clients Found.\n");
			return 1;
		}

		const char* Buffer = "HowdyGamer\0";

		if (sendData(Clients[0], Buffer, strlen(Buffer) + 1) == WND_ERROR)
		{
			printf("Error Sending Data.\n");
			return 2;
		}

		free(Clients);
		closeSocket(x);
	}
	else
	{
		WNDSOCK x = createSocket();

		if (joinSocket(x, "192.168.1.23", 7209))
		{
			printf("Could not connect to Socket.\n");
			return 1;
		}

		unsigned int TotalSize = 0;
		char* Buffer = recieveData(x, &TotalSize);

		if (!Buffer)
		{
			printf("Could not receive data.\n");
			return 2;
		}

		printf("%s \n", Buffer);
		closeSocket(x);
	}

	return 0;
}
