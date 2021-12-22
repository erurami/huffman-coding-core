
#pragma once

bool Print_Infos = false;


class CharAndFreq
{
    private:

        void SortExistChars(int first, int last);
        void SortExistChars(void);

    public:

        unsigned long long pFreqs[256];
        int pFreqSortedExistChars[256];
        int ExistCharsCount;

        CharAndFreq();

        void CountFreqFromFile   (FILE* pFileToRead, long* pProgressPermile = NULL, bool* pUpdateProgress = NULL);
        void ReadFreqDataFromFile(FILE* pFileToRead, long* pProgressPermile = NULL, bool* pUpdateProgress = NULL);
};

class HuffmanTree
{

    public:

        struct HeapNode
        {
            char NodeType;
            // 1 : Leaf
            // 2 : Parent

            unsigned long long Freq;
            HeapNode* pParent;

            // for leaf node
            int Data;

            // for parent node
            HeapNode* pRightNode;
            HeapNode* pLeftNode;
        };

        struct ConvertionTable
        {
            bool Codes[256][256];
            bool IsCodeExists[256];
            int  CharCodeLength[256];
        };


        HeapNode* mpLeafNodes;
        HeapNode* mpParentNodes;

        HeapNode* mpRootNode;

        int mLeafNodesLength;
        int mParentNodesLength;

        bool mTreeBuilt;

        HuffmanTree();

        ~HuffmanTree();

        void BuildHuffmanTree(CharAndFreq* pFreqDatas);

        void MakeConvertionTable();
};

void Encode(FILE* pFileToEncode, HuffmanTree* pHuffmanTree, FILE* pFileToWrite, long* pProgressPermile = NULL);
void Decode(FILE* pFileToDecode, HuffmanTree* pHuffmanTree, FILE* pFileToWrite, long* pProgressPermile = NULL);

void Compress(FILE* pFileSource, FILE* pFileTo, long* pProgressPermile = NULL);
void Extract (FILE* pFileSource, FILE* pFileTo, long* pProgressPermile = NULL);

unsigned long long GetFileSize(FILE* pFile);


#include "ehufbace.ipp"

