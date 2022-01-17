
#include <stdio.h>

#include "libs\\huffman.hpp"

void ProgressTimerCompression(long progress, int step, void* param)
{
    printf("\033[0Gcompress progress : %ld", progress);
}

void ProgressTimerExtraction (long progress, int step, void* param)
{
    printf("\033[0Gextract  progress : %ld", progress);
}


int main()
{
    long progress;


    printf("compressing   samples\\test1.txt -> samples\\test1.txt.huff\n");

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

    Compress(p_file1, p_file2, ProgressTimerCompression);

    fclose(p_file1);
    fclose(p_file2);

    printf("\n\n");




    printf("compressing   samples\\test2.txt -> samples\\test2.txt.huff\n");

    FILE* p_file3;
    FILE* p_file4;

    if (fopen_s(&p_file3, "samples\\test2.txt"     , "rb") != 0)
    {
        printf("error loading test2.txt\n");
        return -1;
    }
    if (fopen_s(&p_file4, "samples\\test2.txt.huff", "wb") != 0)
    {
        printf("error loading test2.txt.ehuf\n");
        return -1;
    }

    Compress(p_file3, p_file4, ProgressTimerCompression);

    fclose(p_file3);
    fclose(p_file4);

    printf("\n\n");





    printf("compressing   samples\\test3.txt -> samples\\test3.txt.huff\n");

    FILE* p_file5;
    FILE* p_file6;

    if (fopen_s(&p_file5, "samples\\test3.txt"     , "rb") != 0)
    {
        printf("error loading test3.txt\n");
        return -1;
    }
    if (fopen_s(&p_file6, "samples\\test3.txt.huff", "wb") != 0)
    {
        printf("error loading test3.txt.ehuf\n");
        return -1;
    }

    Compress(p_file5, p_file6, ProgressTimerCompression);

    fclose(p_file5);
    fclose(p_file6);

    printf("\n\n");




    printf("extracting    samples\\test2.txt.huff -> samples\\test2.txt\n");

    FILE* p_file7;
    FILE* p_file8;

    if (fopen_s(&p_file7, "samples\\test2.txt.huff", "rb") != 0)
    {
        printf("error loading test1.txt\n");
        return -1;
    }
    if (fopen_s(&p_file8, "samples\\test2.txt.huff.txt", "wb") != 0)
    {
        printf("error loading test1.txt.ehuf\n");
        return -1;
    }

    Extract(p_file7, p_file8, ProgressTimerExtraction);

    fclose(p_file7);
    fclose(p_file8);

    return 0;
}
