
#include <stdio.h>

#include "libs\\huffman.hpp"

int main()
{
    long progress;

    FILE* p_file1;
    FILE* p_file2;

    if (fopen_s(&p_file1, "samples\\test1.txt"     , "rb") != 0)
    {
        printf("error loading test.txt\n");
        return -1;
    }

    if (fopen_s(&p_file2, "samples\\test1.txt.huff", "wb") != 0)
    {
        printf("error loading test.txt.ehuf\n");
        return -1;
    }

    Compress(p_file1, p_file2, &progress);

    fclose(p_file1);
    fclose(p_file2);



    FILE* p_file3;
    FILE* p_file4;

    if (fopen_s(&p_file3, "samples\\test2.txt"     , "rb") != 0)
    {
        printf("error loading test1.txt\n");
        return -1;
    }
    if (fopen_s(&p_file4, "samples\\test2.txt.huff", "wb") != 0)
    {
        printf("error loading test1.txt.ehuf\n");
        return -1;
    }

    Compress(p_file3, p_file4);

    fclose(p_file3);
    fclose(p_file4);



    FILE* p_file5;
    FILE* p_file6;

    if (fopen_s(&p_file5, "samples\\test1.txt.huff", "rb") != 0)
    {
        printf("error loading test1.txt\n");
        return -1;
    }
    if (fopen_s(&p_file6, "samples\\test1.txt.huff.txt", "wb") != 0)
    {
        printf("error loading test1.txt.ehuf\n");
        return -1;
    }

    Extract(p_file5, p_file6);

    fclose(p_file5);
    fclose(p_file6);

    return 0;
}
