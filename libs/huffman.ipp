
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





void ProgressManagerCompression::UpdateProg(int CompressionStep, unsigned long long Progress, unsigned long long Total)
{
    switch (CompressionStep)
    {

        case COMPRESSION_STEP_READFREQ:

            ProgressPercent = 
                COMPRESSION_READING_TOTAL_PROG * Progress / Total;

            break;

        case COMPRESSION_STEP_BUILDTREE:

            ProgressPercent = 
                COMPRESSION_READING_TOTAL_PROG + 
                COMPRESSION_BUILDINGTREE_TOTAL_PROG * Progress / Total;

            break;

        case COMPRESSION_STEP_WRITEINFO1:

            ProgressPercent = 
                COMPRESSION_READING_TOTAL_PROG + 
                COMPRESSION_BUILDINGTREE_TOTAL_PROG + 
                COMPRESSION_WRITING1_TOTAL_PROG * Progress / Total;

            break;

        case COMPRESSION_STEP_ENCODE:

            ProgressPercent = 
                COMPRESSION_READING_TOTAL_PROG + 
                COMPRESSION_BUILDINGTREE_TOTAL_PROG + 
                COMPRESSION_WRITING1_TOTAL_PROG + 
                COMPRESSION_ENCODING_TOTAL_PROG * Progress / Total;

            break;

        case COMPRESSION_STEP_WRITEINFO2:

            ProgressPercent = 
                COMPRESSION_READING_TOTAL_PROG + 
                COMPRESSION_BUILDINGTREE_TOTAL_PROG + 
                COMPRESSION_WRITING1_TOTAL_PROG + 
                COMPRESSION_ENCODING_TOTAL_PROG + 
                COMPRESSION_WRITING2_TOTAL_PROG * Progress / Total;
            break;
    }

    StepNow = CompressionStep;

    (*mpCallbackFunc)(ProgressPercent, StepNow, mpArgForFunc);
}



void ProgressManagerExtraction::UpdateProg(int ExtractionStep, unsigned long long Progress, unsigned long long Total)
{
    switch (ExtractionStep)
    {

        case EXTRACTION_STEP_READINFO:

            ProgressPercent = 
                EXTRACTION_READINGINFO_TOTAL_PROG * Progress / Total;

            break;

        case EXTRACTION_STEP_READTREE:

            ProgressPercent = 
                EXTRACTION_READINGINFO_TOTAL_PROG + 
                EXTRACTION_READINGTREE_TOTAL_PROG * Progress / Total;

            break;

        case EXTRACTION_STEP_DECODE:

            ProgressPercent = 
                EXTRACTION_READINGINFO_TOTAL_PROG + 
                EXTRACTION_READINGTREE_TOTAL_PROG + 
                EXTRACTION_DECODING_TOTAL_PROG * Progress / Total;

            break;
    }

    StepNow = ExtractionStep;

    (*mpCallbackFunc)(ProgressPercent, StepNow, mpArgForFunc);
}
