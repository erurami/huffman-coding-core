
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main()
{
    DIR* pDirectry;
    struct dirent* pDir;

    pDirectry = opendir(".");

    if (!pDirectry)
    {
        return -1;
    }

    while ((pDir = readdir(pDirectry)) != NULL)
    {
        if (strcmp(pDir->d_name, ".."            ) &&
            strcmp(pDir->d_name, "."             ) &&
            strcmp(pDir->d_name, "test1.txt"     ) &&
            strcmp(pDir->d_name, "test2.txt"     ) &&
            strcmp(pDir->d_name, "test_cleaner.c") &&
            strcmp(pDir->d_name, "testcl.exe"    )
            )
        {
            printf("removing  %s", pDir->d_name);

            remove(pDir->d_name);

            printf("\n");
        }
    }

    closedir(pDirectry);

    return 0;
}
