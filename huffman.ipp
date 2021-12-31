
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

    PrintHuffmanTree(&huffman_tree);

    WriteHuffmanTree(pFileTo, &huffman_tree);
    WriteMainData   (pFileTo, &huffman_tree, pFileSource);
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
    int leaf_node_length = exist_chars;

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
//                  ª
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






void WriteHuffmanTree(FILE* pFileToWrite, HuffmanTree* pHuffmanTree)
{
}

void WriteMainData   (FILE* pFileToWrite, HuffmanTree* pHuffmanTree, FILE* pSourceFile)
{
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
        depth++; PrintNode(pNode->pRight); depth--;
    }

    printf("%*c%llu\n", 5 * depth, ' ', pNode->Freq);

    if (pNode->NodeType == 1)
    {
        depth++; PrintNode(pNode->pLeft); depth--;
    }
}
