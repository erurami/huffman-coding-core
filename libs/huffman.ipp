
#pragma once

#include "bitio.hpp"
#include <stdio.h>
#include <string.h>


#include "compress.ipp"
#include "extract.ipp"









unsigned long long GetFileSize(FILE* pFile)
{
    fseek(pFile, 0L, SEEK_END);
    unsigned long long file_size = ftell(pFile);
    fseek(pFile, 0L, SEEK_SET);

    return file_size;
}





void PrintNode(HuffmanTreeNode* pNode);
void PrintHuffmanTree(HuffmanTree* pHuffmanTree)
{
    PrintNode(pHuffmanTree->pRootNode);
}

void PrintNode(HuffmanTreeNode* pNode)
{
    static int depth = 0;

    if (pNode->NodeType == 1)
    {
        depth++; PrintNode(pNode->pLeft); depth--;
    }

    if (pNode->NodeType == 2)
    {
        printf("%*c%2x\n", 5 * depth, ' ', pNode->Data);
    }
    else
    {
        printf("%*cin\n", 5 * depth, ' ');
    }

    if (pNode->NodeType == 1)
    {
        depth++; PrintNode(pNode->pRight); depth--;
    }
}
