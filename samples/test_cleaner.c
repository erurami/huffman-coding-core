
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>


//

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
            strcmp(pDir->d_name, "testFiles.txt" ) &&
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
    FILE* pTestFiles;

    pTestFiles = fopen("testFiles.txt", "r");

    if (pTestFiles == NULL)
    {
        printf("no testFiles.txt\n");
        exit(EXIT_SUCCESS);
    }


    char testFileName[FILENAME_MAX];

    while (1)
    {
        fscanf(pTestFiles, "%s", testFileName);

        if (!strcmp(testFileName, "end"))
        {
            break;
        }

        if (!strcmp(testFileName, filename))
        {
            return 1;
        }
    }

    return 0;
}

