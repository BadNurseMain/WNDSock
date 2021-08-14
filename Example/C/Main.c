#include <stdlib.h>
#include <stdio.h>
#include "WNDSock.h"

//Works for Windows as of right now.
int main()
{
	int Res = 0;
	scanf_s("%d", &Res);

	if(Res == 1)
	{
		WNDSOCK x = createSocket();
		WNDSOCK* Clients = hostSocket(x, "192.168.1.1", 7209, 1);
		if (!Clients) return 1;

		free(Clients);
		closeSocket(x);
	} else 
	{
		WNDSOCK x = createSocket();
		if (joinSocket(x, "192.168.1.1", 7209)) return 1;
		closeSocket(x);
	}
	
	return 0;
}
