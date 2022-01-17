
#pragma once

#include <stdio.h>



void Compress(FILE* pFileSource,
              FILE* pFileTo,
              void (*pCallbackFunc)(long, int, void*) = NULL,
              void* pArgForFunc = NULL);

void Compress(const char* pFileFrom,
              const char* pFileTo,
              void (*pCallbackFunc)(long, int, void*) = NULL,
              void* pArgForFunc = NULL);


void Extract (FILE* pFileSource,
              FILE* pFileTo,
              void (*pCallbackFunc)(long, int, void*) = NULL,
              void* pArgForFunc = NULL);

void Extract (const char* pFileFrom,
              const char* pFileTo,
              void (*pCallbackFunc)(long, int, void*) = NULL,
              void* pArgForFunc = NULL);





struct HuffmanTreeNode
{

    int NodeType;
    // 1 : Parent
    // 2 : Leaf

    HuffmanTreeNode* pParent;

    unsigned long long int Freq;

    int Data;

    HuffmanTreeNode* pRight;
    HuffmanTreeNode* pLeft;
};


struct HuffmanTree
{

    HuffmanTreeNode* pLeafNodes;
    HuffmanTreeNode* pParentNodes;

    HuffmanTreeNode* pRootNode;

    int LeafNodeLength;
};


struct FileHeaderData
{
    int OffsetTreeStructure;
    int OffsetTreeData;
    int OffsetMainData;

    long BitsTreeStructure;
    long BytesTreeData;
    unsigned long long BytesMainData;

    int  BitsUsedInLastByte;

    unsigned long long FileSize;
};




#include "bitio.hpp"


// --------------- //
//   compressing   //
// --------------- //
void CountFreq(FILE* pFileToCount, unsigned long long *pFreqDes);

void BuildHuffmanTree(unsigned long long *pFreqs, HuffmanTree* pHuffmanTree);

void WriteHuffmanTree(Bitio::File* pFileToWrite, HuffmanTree* pHuffmanTree                   , FileHeaderData* pFileData);
void WriteMainData   (Bitio::File* pFileToWrite, HuffmanTree* pHuffmanTree, FILE* pSourceFile, FileHeaderData* pFileData);
void WriteHeaderData (Bitio::File* pFileToWrite,                                               FileHeaderData* pFileData);





// --------------- //
//    extracting   //
// --------------- //
void ReadHeaderData(FILE* pFile, FileHeaderData* pFileData);

void ReadHuffmanTree(FILE* pFileToRead, HuffmanTree* pHuffmanTree, FileHeaderData* pFileData);

void Decode(FILE* pFileSource, FILE* pFileToWrite, HuffmanTree* pHuffmanTree, FileHeaderData* pFileData);






#define COMPRESSION_STEP_READFREQ    1
#define COMPRESSION_STEP_BUILDTREE   2
#define COMPRESSION_STEP_WRITEINFO1  3
#define COMPRESSION_STEP_ENCODE      4
#define COMPRESSION_STEP_WRITEINFO2  5
#define COMPRESSION_STEP_FINISHED    6

#define EXTRACTION_STEP_READINFO 1
#define EXTRACTION_STEP_READTREE 2
#define EXTRACTION_STEP_DECODE   3
#define EXTRACTION_STEP_FINISHED 4


#define COMPRESSION_READING_TOTAL_PROG      40
#define COMPRESSION_BUILDINGTREE_TOTAL_PROG 10
#define COMPRESSION_WRITING1_TOTAL_PROG     5
#define COMPRESSION_ENCODING_TOTAL_PROG     40
#define COMPRESSION_WRITING2_TOTAL_PROG     5

#define EXTRACTION_READINGINFO_TOTAL_PROG 10
#define EXTRACTION_READINGTREE_TOTAL_PROG 10
#define EXTRACTION_DECODING_TOTAL_PROG    80

// TODO : Update Progress when finished.
class ProgressManagerCompression
{

    public:

        void (*mpCallbackFunc)(long, int, void*);
        void* mpArgForFunc;

        // todo : higher resolution progress.
        long ProgressPercent;
        int  StepNow;

        void UpdateProg(int CompressionStep, unsigned long long Progress, unsigned long long Total);
};



class ProgressManagerExtraction
{

    public:

        void (*mpCallbackFunc)(long, int, void*);
        void* mpArgForFunc;

        long ProgressPercent;
        int  StepNow;

        void UpdateProg(int ExtractionStep , unsigned long long Progress, unsigned long long Total);
};



// unavailable : this variable doesn't effect any.
bool G_Print_Infomations = false;

void SetPrintInfos(bool printOrNot);





unsigned long long GetFileSize(FILE* pFile);

// for debugging
void PrintHuffmanTree(HuffmanTree* p_huffman_tree);





#include "huffman.ipp"

