#include "WNDSock.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Working for both Linux and Windows right now.
int main()
{
    int x = 0;
    scanf("%d", &x);

    char* Buff = (char*)malloc(10);
    strcpy(Buff, "Howdyfela");

    if(x == 1)
    {
        WNDSock x = WNDSock();
        if(x.Host("127.0.0.1", 7209)) printf("Error hosting! \n");
        x.Listen(1,1);
        x.Send(Buff, 10);
    }
    else
    {
        WNDSock y = WNDSock();
        if(y.Join("127.0.0.1", 7209)) printf("Error joining! \n");
        unsigned int Size = 0;
        char* Buff = y.Recieve(&Size);

        printf("%s", Buff);
        free(Buff);
    }

    return 0;
}
