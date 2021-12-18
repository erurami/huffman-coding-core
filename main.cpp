
#include "ehufbase.hpp"
#include <string.h>

bool isEhufFile(char* pFilename);
bool GetFileExtensionUnderEhuf(char* pFilename, char* pFileExtension);
void RemoveFileExtennsion(char* pFilename);


int main(int argc, char** argv)
{
    FILE* pTestFile1;
    FILE* pTestFile2;

    if (argc != 2)
    {
        printf("too much or few arguments");
        return 1;
    }

    // you should know.. but the string must be maximum about 250 char length

    if (isEhufFile(argv[1]))
    {

        if (fopen_s(&pTestFile1, argv[1], "rb") != 0)
        {
            printf("opening file error 1;\n");
            return 1;
        }

        char* p_file_name;
        p_file_name = new char[strlen(argv[1])];
        strcpy(p_file_name, argv[1]);

        RemoveFileExtennsion(p_file_name);
        printf("from : %s\n to : %s\n", argv[1], p_file_name);

        if (fopen_s(&pTestFile2, p_file_name, "wb") != 0)
        {
            printf("opening_file error 2;\n");
            return 1;
        }

        Extract(pTestFile1, pTestFile2);

        fclose(pTestFile1);
        fclose(pTestFile2);
    }
    else
    {
        if (fopen_s(&pTestFile1, argv[1], "rb") != 0)
        {
            printf("opening file error 1;\n");
            return 1;
        }

        char* p_file_name;
        p_file_name = new char[strlen(argv[1]) + 10];
        strcpy(p_file_name, argv[1]);
        strcat(p_file_name, ".ehuf");

        if (fopen_s(&pTestFile2, p_file_name, "wb") != 0)
        {
            printf("opening_file error 2;\n");
            return 1;
        }

        Compress(pTestFile1, pTestFile2);

        fclose(pTestFile1);
        fclose(pTestFile2);

        delete [] p_file_name;
    }

    return 0;
}


bool isEhufFile(char* pFilename)
{
    char file_extension[10];
    int a = 0;
    for (int i = 0; i < strlen(pFilename); i++)
    {
        if (pFilename[i] == '.')
        {
            a = 0;
        }
        if (a < 10)
        {
            file_extension[a] = pFilename[i];
        }
        a++;
    }
    if (file_extension[1] == 'e' &&
        file_extension[2] == 'h' &&
        file_extension[3] == 'u' &&
        file_extension[4] == 'f')
    {
        return true;
    }
    file_extension[a] = 0;
    printf("%s", file_extension);
    return false;
}

void RemoveFileExtennsion(char* pFilename)
{
    int second_last_period_pos = 0;
    int last_period_pos = 0;

    int i;
    for (i = 0; i < strlen(pFilename); i++)
    {
        if (pFilename[i] == '.')
        {
            second_last_period_pos = last_period_pos;
            last_period_pos = i;
        }
    }
    printf("%d", second_last_period_pos);

    for (i = last_period_pos; i > second_last_period_pos; i--)
    {
        pFilename[i] = pFilename[i - 1];
    }

    pFilename[second_last_period_pos] = '1';
    pFilename[last_period_pos + 1] = 0;

}
