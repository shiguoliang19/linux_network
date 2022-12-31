#include <stdio.h>
#include <string.h>

#include "Config.h"

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        if(strcmp(argv[1], "-v") == 0)
        {
            printf("VERSION_BUILD_TIME: %s_%d.%d.%d_%s\n", PROJECT_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, COMPACT_BUILD_TIMESTAMP);
            printf("\n");
        }

        if(strcmp(argv[1], "-V") == 0)
        {
            printf("PROJECT_NAME: %s\n", PROJECT_NAME);
            printf("VERSION: %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
            printf("INDENTED_BUILD_TIMESTAMP: %s\n", INDENTED_BUILD_TIMESTAMP);
            printf("GIT_BRANCH: %s\n", GIT_BRANCH);
            printf("GIT_HASH: %s\n", GIT_HASH);
            printf("\n");
        }

        return 0;
    }

    return 0;
}