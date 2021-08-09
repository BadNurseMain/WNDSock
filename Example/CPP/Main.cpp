#include "WNDSock.h"
#include <stdlib.h>
#include <stdio.h>

//Works for Linux and Windows, will only demonstrate what works 
//seamlessly with both Linux and Windows.
int main()
{
    int x = 0;
    scanf("%d", &x);
    if(x == 1)
    {
        WNDSock x = WNDSock();
        if(x.Host("127.0.0.1", 7209)) printf("Error hosting! \n");
        x.Listen(1,1);
    }
    else
    {
        WNDSock y = WNDSock();
        if(y.Join("127.0.0.1", 7209)) printf("Error joining! \n");
    }

    return 0;
}
