
#pragma once

#include "bitio.hpp"
#include <stdio.h>
#include <string.h>


// unavailable this variable doesn't effect any.
bool G_Print_Infomations = false;


unsigned long long GetFileSize(FILE* pFile);



// for debugging
void PrintHuffmanTree(HuffmanTree* p_huffman_tree);


void Compress(FILE* pFileSource, FILE* pFileTo, long* pProgress)
{
    unsigned long long freqs[256] = {0};
    CountFreq(pFileSource, freqs);

    HuffmanTree huffman_tree;
    BuildHuffmanTree(freqs, &huffman_tree);

    // PrintHuffmanTree(&huffman_tree);

    Bitio::File file_to_write;
    file_to_write.UseFileObj(pFileTo, "w");

    FileHeaderData header_data;

    WriteHuffmanTree(&file_to_write, &huffman_tree             , &header_data);
    WriteMainData   (&file_to_write, &huffman_tree, pFileSource, &header_data);
    WriteHeaderData (&file_to_write                            , &header_data);

    printf("Compress関数終了1\n");
    printf("Compress関数終了2\n");
    printf("Compress関数終了3\n");
    printf("Compress関数終了4\n");
}




void CountFreq(FILE* pFileToCount, unsigned long long *pFreqDes)
{

    unsigned long long file_size = GetFileSize(pFileToCount);

    fseek(pFileToCount, 0L, SEEK_SET);

    for (unsigned long long i = 0; i < file_size; i++)
    {
        pFreqDes[fgetc(pFileToCount)]++;
    }
}



void SortMinHeap    (HuffmanTreeNode** minHeap, int minHeapLength);
void InsertToMinHeap(HuffmanTreeNode** minHeap, int minHeapLength, HuffmanTreeNode* pNodeToInsert);
void BuildHuffmanTree(unsigned long long *pFreqs, HuffmanTree* pHuffmanTree)
{

    int exist_chars = 0;

    for (int i = 0; i < 256; i++)
    {
        if (pFreqs[i] >= 1)
        {
            exist_chars++;
        }
    }


    pHuffmanTree->pLeafNodes   = new HuffmanTreeNode [exist_chars];
    pHuffmanTree->pParentNodes = new HuffmanTreeNode [exist_chars - 1];
    pHuffmanTree->LeafNodeLength = exist_chars;

    // Notice & TODO : min heap is decending order.
    HuffmanTreeNode** min_heap;
    min_heap = new HuffmanTreeNode* [exist_chars];
    int min_heap_length = exist_chars;


    int index = 0;
    for (int i = 0; i < 256; i++)
    {
        if (pFreqs[i] >= 1)
        {
            pHuffmanTree->pLeafNodes[index].NodeType = 2;
            pHuffmanTree->pLeafNodes[index].Freq     = pFreqs[i];
            pHuffmanTree->pLeafNodes[index].Data     = i;

            min_heap[index] = &pHuffmanTree->pLeafNodes[index];

            index++;
        }
    }


    SortMinHeap(min_heap, min_heap_length);



    index = 0;

    HuffmanTreeNode* p_node_to_combine_1;
    HuffmanTreeNode* p_node_to_combine_2;

    while (min_heap_length >= 2)
    {

        p_node_to_combine_1 = min_heap[min_heap_length - 1];
        p_node_to_combine_2 = min_heap[min_heap_length - 2];

        pHuffmanTree->pParentNodes[index].NodeType = 1;
        pHuffmanTree->pParentNodes[index].Freq     = p_node_to_combine_1->Freq + p_node_to_combine_2->Freq;
        pHuffmanTree->pParentNodes[index].pRight   = p_node_to_combine_1;
        pHuffmanTree->pParentNodes[index].pLeft    = p_node_to_combine_2;

        p_node_to_combine_1->pParent = &pHuffmanTree->pParentNodes[index];
        p_node_to_combine_2->pParent = &pHuffmanTree->pParentNodes[index];

        min_heap_length--;

        InsertToMinHeap(min_heap, min_heap_length, &pHuffmanTree->pParentNodes[index]);

        pHuffmanTree->pRootNode = &pHuffmanTree->pParentNodes[index];

        index++;
    }
}


void SortMinHeap(HuffmanTreeNode** minHeap, int first, int last);
void SortMinHeap(HuffmanTreeNode** minHeap, int minHeapLength)
{
    SortMinHeap(minHeap, 0, minHeapLength - 1);
}

// mergesort
void MergeMinHeap(HuffmanTreeNode** minHeap, int first, int middle, int last);
void SortMinHeap(HuffmanTreeNode** minHeap, int first, int last)
{
    int middle;

    if (first < last)
    {
        middle = (first + last) / 2;
        SortMinHeap(minHeap, first, middle);
        SortMinHeap(minHeap, middle + 1, last);

        MergeMinHeap(minHeap, first, middle, last);
    }
}

void MergeMinHeap(HuffmanTreeNode** minHeap, int first, int middle, int last)
{

    int left_length   = middle - first + 1;
    int right_length = last   - middle;

    HuffmanTreeNode** left_arr;
    HuffmanTreeNode** right_arr;

    left_arr  = new HuffmanTreeNode* [left_length ];
    right_arr = new HuffmanTreeNode* [right_length];

    for (int i = 0; i < left_length; i++)
    {
        left_arr [i] = minHeap[first + i];
    }

    for (int i = 0; i < right_length; i++)
    {
        right_arr[i] = minHeap[middle + 1 + i];
    }


    int i = 0;
    int j = 0;
    int k = first;

    while (i < left_length && j < right_length)
    {
        if (left_arr[i]->Freq >= right_arr[j]->Freq)
        {
            minHeap[k] = left_arr[i];
            i++;
        }
        else
        {
            minHeap[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < left_length)
    {
        minHeap[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < right_length)
    {
        minHeap[k] = right_arr[j];
        j++;
        k++;
    }
}

// before this function, the min heap is like this
//   { 9, 7, 4, 3, 2, 2(merged), 1(merged)}
// so we should insert merged node which has 2 + 1 (=3) freq.
//   { 9, 7, 4, 3, 3, 2}
// 
// first, slide from the back of the array, and if the freq was more than the node which merged
//  just before this, insert and return.
//
//  1. {9, 7, 4, 3, 2   , NULL}
//
//  2. {9, 7, 4, 3, NULL, 2   }
//
//  3. {9, 7, 4, 3, 2   , 2   }
//                  ↑
//                  (inserted)

void InsertToMinHeap(HuffmanTreeNode** minHeap, int minHeapLength, HuffmanTreeNode* pNodeToInsert)
{
    HuffmanTreeNode* p_node_to_slide;

    for (int i = minHeapLength - 1; i > 0; i--)
    {
        p_node_to_slide = minHeap[i - 1];

        if (p_node_to_slide->Freq >= pNodeToInsert->Freq)
        {
            minHeap[i] = pNodeToInsert;
            return;
        }
        minHeap[i] = p_node_to_slide;
    }
    minHeap[0] = pNodeToInsert;
    return;
}






long WriteNode(Bitio::File* pFileToWrite, HuffmanTreeNode* pNode, int* pCharsAppered);
void WriteHuffmanTree(Bitio::File* pFileToWrite, HuffmanTree* pHuffmanTree, FileHeaderData* pFileData)
{
    int offset_tree_data;
    int offset_main_data;

    long bits_of_tree_structure;


    pFileToWrite->SeekBytes(0x16);

    int* appered_chars;
    appered_chars = new int [pHuffmanTree->LeafNodeLength];
    bits_of_tree_structure = WriteNode(pFileToWrite, pHuffmanTree->pRootNode, appered_chars);

    //
    offset_tree_data = pFileToWrite->TellBytes() + 1;


    for (int i = 0; i < pHuffmanTree->LeafNodeLength; i++)
    {
        pFileToWrite->PutChar(appered_chars[i]);
    }

    //
    offset_main_data = pFileToWrite->TellBytes();


    pFileData->OffsetTreeStructure = 0x16;
    pFileData->OffsetTreeData      = offset_tree_data;
    pFileData->OffsetMainData      = offset_main_data;

    pFileData->BitsTreeStructure   = bits_of_tree_structure;
    pFileData->BytesTreeData       = pHuffmanTree->LeafNodeLength;

}

long WriteNode(Bitio::File* pFileToWrite, HuffmanTreeNode* pNode, int* pCharsAppered)
{

    static int appered_chars_count = 0;
    static long bits_written = 0;
    if (pNode->NodeType == 1)
    {
        pFileToWrite->PutBit(true);
        bits_written++;
        WriteNode(pFileToWrite, pNode->pLeft , pCharsAppered);
        WriteNode(pFileToWrite, pNode->pRight, pCharsAppered);
    }
    else
    {
        pFileToWrite->PutBit(false);
        bits_written++;
        pCharsAppered[appered_chars_count] = pNode->Data;
        appered_chars_count++;
    }

    return bits_written;
}





void MakeConvertionTable(HuffmanTree* pHuffmanTree, bool* pConvertionTable, int* pConvertionTableLengthes);
void WriteMainData   (Bitio::File* pFileToWrite, HuffmanTree* pHuffmanTree, FILE* pSourceFile, FileHeaderData* pFileData)
{
    bool convertion_table[256][256];
    int  convertion_table_lengthes[256];

    MakeConvertionTable(pHuffmanTree, &convertion_table[0][0], convertion_table_lengthes);

    // for (int i = 0; i < pHuffmanTree->LeafNodeLength; i++)
    // {
    //     int character;
    //     character = pHuffmanTree->pLeafNodes[i].Data;
    //     printf("%2x : ", character);

    //     for (int j = 0; j < convertion_table_lengthes[character]; j++)
    //     {
    //         printf("%d", convertion_table[character][j]);
    //     }

    //     printf("\n");
    // }


    pFileToWrite->SeekBytes(pFileData->OffsetMainData);

    fseek(pSourceFile, 0L, SEEK_SET);
    unsigned long long file_size = GetFileSize(pSourceFile);

    int character;
    for (unsigned long long i = 0; i < file_size; i++)
    {
        character = fgetc(pSourceFile);
        pFileToWrite->PutBits(&convertion_table[character][0], convertion_table_lengthes[character]);
    }

    // XXX : TellBytes is supporting only 32bit intager.
    pFileData->FileSize           = pFileToWrite->TellBytes();
    pFileData->BytesMainData      = pFileData->FileSize - pFileData->OffsetMainData;
    pFileData->BitsUsedInLastByte = pFileToWrite->TellBits();


    // printf("OffsetTreeStructure : %x\n"  , pFileData->OffsetTreeStructure);
    // printf("OffsetTreeData      : %x\n"  , pFileData->OffsetTreeData     );
    // printf("OffsetMainData      : %x\n"  , pFileData->OffsetMainData     );

    // printf("\n");

    // printf("BitsTreeStructure   : %lx\n" , pFileData->BitsTreeStructure  );
    // printf("BytesTreeData       : %lx\n" , pFileData->BytesTreeData      );
    // printf("BytesMainData       : %lx\n" , pFileData->BytesMainData      );

    // printf("\n");
    // printf("BitsUsedInLastByte  : %x\n"  , pFileData->BitsUsedInLastByte );

    // printf("\n");

    // printf("FileSize            : %llx\n", pFileData->FileSize           );

}


void MakeConvertionTable(HuffmanTree* pHuffmanTree, bool* pConvertionTable, int* pConvertionTableLengthes)
{
    int character;

    bool tmp[256];
    int  index;
    for (int i = 0; i < pHuffmanTree->LeafNodeLength; i++)
    {
        character = pHuffmanTree->pLeafNodes[i].Data;

        pConvertionTableLengthes[character] = 0;

        for (HuffmanTreeNode* current_node = &pHuffmanTree->pLeafNodes[i]; current_node != pHuffmanTree->pRootNode; current_node = current_node->pParent)
        {
            tmp[pConvertionTableLengthes[character]] = current_node == current_node->pParent->pRight ? true : false;
            pConvertionTableLengthes[character]++;
        }

        index = 0;
        for (int j = pConvertionTableLengthes[character] - 1; j >= 0; j--)
        {
            pConvertionTable[character * 256 + j] = tmp[index];
            index++;
        }
    }
}






void WriteNumber(Bitio::File* pFileToWrite, long long number, int bits);
void WriteHeaderData(Bitio::File* pFileToWrite, FileHeaderData* pFileData)
{
    pFileToWrite->SeekBytes(0);

    pFileToWrite->PutChar('H');
    pFileToWrite->PutChar('F');

    pFileToWrite->PutChar(1);

    WriteNumber(pFileToWrite, pFileData->FileSize           , 4);

    WriteNumber(pFileToWrite, pFileData->OffsetTreeStructure, 2);
    WriteNumber(pFileToWrite, pFileData->OffsetTreeData     , 2);
    WriteNumber(pFileToWrite, pFileData->OffsetMainData     , 2);

    WriteNumber(pFileToWrite, pFileData->BitsTreeStructure  , 2);
    WriteNumber(pFileToWrite, pFileData->BytesTreeData      , 2);
    WriteNumber(pFileToWrite, pFileData->BytesMainData      , 4);

    pFileToWrite->PutChar(pFileData->BitsUsedInLastByte);
}

void WriteNumber(Bitio::File* pFileToWrite, long long number, int bits)
{
    for (int i = 0; i < bits; i++)
    {
        pFileToWrite->PutChar(number % 0x100);
        number >>= 8;
    }
}




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
