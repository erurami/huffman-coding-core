
#pragma once

#include "bitio.hpp"

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
    long BytesMainData;

    int  BitsUsedInLastByte;

    unsigned long long FileSize;
};



void Compress(FILE* pFileSource, FILE* pFileTo, long* pProgress = NULL);


void CountFreq(FILE* pFileToCount, unsigned long long *pFreqDes);

void BuildHuffmanTree(unsigned long long *pFreqs, HuffmanTree* pHuffmanTree);

void WriteHuffmanTree(Bitio::File* pFileToWrite, HuffmanTree* pHuffmanTree                   , FileHeaderData* pFileData);
void WriteMainData   (Bitio::File* pFileToWrite, HuffmanTree* pHuffmanTree, FILE* pSourceFile, FileHeaderData* pFileData);
void WriteHeaderData (Bitio::File* pFileToWrite,                                               FileHeaderData* pFileData);





void Extract (FILE* pFileSource, FILE* pFileTo, long* pProgress = NULL);


void ReadHeaderData(FILE* pFile, FileHeaderData* pFileData);

void ReadHuffmanTree(FILE* pFileToRead, HuffmanTree* pHuffmanTree, FileHeaderData* pFileData);

void Decode(FILE* pFileSource, FILE* pFileToWrite, HuffmanTree* pHuffmanTree, FileHeaderData* pFileData);




// class ProgressManager
// {
// };

void SetPrintInfos(bool printOrNot);


#include "huffman.ipp"

