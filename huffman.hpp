
#pragma once


struct HuffmanTreeNode
{

    int NodeType;
    // 1 : Parent
    // 2 : Leaf

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



void Compress(FILE* pFileSource, FILE* pFileTo, long* pProgress = NULL);


void CountFreq(FILE* pFileToCount, unsigned long long *pFreqDes);

void BuildHuffmanTree(unsigned long long *pFreqs, HuffmanTree* pHuffmanTree);

void WriteHuffmanTree(FILE* pFileToWrite, HuffmanTree* pHuffmanTree);
void WriteMainData   (FILE* pFileToWrite, HuffmanTree* pHuffmanTree, FILE* pSourceFile);





void Extract (FILE* pFileSource, FILE* pFileTo, long* pProgress = NULL);


void ReadTree(FILE* pFileToRead, HuffmanTree* pHuffmanTree);

void Decode(FILE* pFileSource, FILE* pFileToWrite, HuffmanTree* pHuffmanTree);




// class ProgressManager
// {
// };

void SetPrintInfos(bool printOrNot);


#include "huffman.ipp"

