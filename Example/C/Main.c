#include "WNDSock.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	WNDSOCK x = createSocket();
	WNDSOCK* Clients = hostSocket(x, "192.168.1.1", 7209, 20);
	if (!Clients) return 1;

	free(Clients);
	return 0;
}
