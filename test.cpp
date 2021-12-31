
#include <stdio.h>

#include "huffman.hpp"

int main()
{
    FILE* p_file1;
    FILE* p_file2;

    if (fopen_s(&p_file1, "test.txt"     , "rb") != 0)
    {
        printf("error loading test.txt\n");
        return -1;
    }

    if (fopen_s(&p_file2, "test.txt.ehuf", "wb") != 0)
    {
        printf("error loading test.txt.ehuf\n");
        return -1;
    }

    Compress(p_file1, p_file2);

    fclose(p_file1);
    fclose(p_file2);



    if (fopen_s(&p_file1, "test1.txt"     , "rb") != 0)
    {
        printf("error loading test1.txt\n");
        return -1;
    }
    if (fopen_s(&p_file2, "test1.txt.ehuf", "wb") != 0)
    {
        printf("error loading test1.txt.ehuf\n");
        return -1;
    }

    Compress(p_file1, p_file2);

    fclose(p_file1);
    fclose(p_file2);

    printf("a\n");

    printf("ê≥èÌèIóπ\n");

    return 0;
}
