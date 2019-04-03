#include <stdio.h>
#include <stdlib.h>
#include "lib/my-functions.h"

int main()
{

    unsigned char *mainMemory = malloc(sizeof(char) * 65536);
    unsigned char *diskMemory = malloc(sizeof(char) * 20480);

    assignMemoryValue(mainMemory);
    assignDiskMemoryValue(diskMemory);
    addTwoEntry(mainMemory);
    getUserInput(mainMemory,diskMemory);

    free(mainMemory);
    free(diskMemory);
    return 0;
}
