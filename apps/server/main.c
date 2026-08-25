#include <stdio.h>

#include "papacc/version.h"

int main(void)
{
    puts("PapinhoAccelerator Server");
    puts("Foundation build");
    printf("Software version %s\n", papacc_version_string());

    return 0;
}
