
#pragma once

bool k_Print_Infomations = false;

#define NODETYPE_LEAF 1
#define NODETYPE_PARENT 2



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


        HeapNode* pLeafNodes_;
        HeapNode* pParentNodes_;

        HeapNode* pRootNode_;

        int LeafNodesLength_;
        int ParentNodesLength_;

        int TreeBuildingProgress;

        #define TREE_PROG_1 1
        #define TREE_PROG_2 2
        #define TREE_PROG_1 1
        #define TREE_PROG_2 2

        HuffmanTree();

        ~HuffmanTree();

        void CountFreq(FILE* pFileToCount);
        void ReadFreqDatas(FILE* pFileToRead);

        void BuildHuffmanTree(CharAndFreq* pFreqDatas);

        void MakeConvertionTable();

        void Encode(FILE* pFileToEncode, FILE* pFileToWrite,
                    long* pProgressPermile = NULL);

        void Decode(FILE* pFileToDecode, FILE* pFileToWrite,
                    long* pProgressPermile = NULL);
};



void Compress(FILE* pFileSource, FILE* pFileTo, long* pProgressPermile = NULL);
void Extract (FILE* pFileSource, FILE* pFileTo, long* pProgressPermile = NULL);


#include "ehufbace.ipp"

