
#include <stdio.h>

#include "huffman.hpp"

int main()
{
    FILE* p_file1;
    FILE* p_file2;

    p_file1 = fopen("test.txt"     , "rb");
    p_file2 = fopen("test.txt.ehuf", "wb");

    Compress(p_file1, p_file2);

    fclose(p_file1);
    fclose(p_file2);

    return 0;
}
