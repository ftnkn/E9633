#include "VanDevice.h"
#include "Utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ql_oe.h>

int main(int argc, char *argv[])
{
    int result = Utils::ProcessExists();
    if (result) {
        if (result < 0) {
            printf("Process check error, exit !!!!!!\n");
            return -1;
        }

        printf("Process already exists, exit!\n");
        return 0;
    }
    
    VanDevice::Instance()->Open();

    printf("demo Startup Finish.\n");

    while (1) {
        usleep(20000);

        if ('q' == getchar()) {
            break;
        }
    }

    return 0;
}
