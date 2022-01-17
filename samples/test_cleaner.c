
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int isTestFile(char* filename);

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
            strcmp(pDir->d_name, "test_cleaner.c") &&
            strcmp(pDir->d_name, "testcl.exe"    )
            )
        {
            if (isTestFile(pDir->d_name))
            {
            }
            else
            {
                printf("removing  %s", pDir->d_name);

                remove(pDir->d_name);

                printf("\n");
            }
        }
    }

    closedir(pDirectry);

    return 0;
}

int isTestFile(char* filename)
{
    return 1;
}

