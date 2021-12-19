
#include <stdio.h>
#include <string.h>
#pragma once

#define PI_COM_COUNTING_START 0
#define PI_COM_COUNTING_END   50

#define PI_COM_HUFFMANTREE_START 50
#define PI_COM_HUFFMANTREE_END   51

#define PI_COM_ENCODE_START 51
#define PI_COM_ENCODE_END   100


#define PI_EXT_READING_START 0
#define PI_EXT_READING_END   50

#define PI_EXT_HUFFMANTREE_START 50
#define PI_EXT_HUFFMANTREE_END   51

#define PI_EXT_DECODE_START 51
#define PI_EXT_DECODE_END   100


class CharAndFreq
{
    private:

        unsigned long long pFreqs[256];
        int pFreqSortedExistChars[256];
        int ExistCharsCount;

        CharAndFreq();

        void CountFreqFromFile   (FILE* pFileToRead, long* pProgressPermile = NULL);
        void ReadFreqDataFromFIle(FILE* pFileToRead, long* pProgressPermile = NULL);
};
// TODO : class matomeru sort tokano kinou ituka

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

        HeapNode* mpLeafNodes;
        HeapNode* mpParentNodes;

        HeapNode* mpRootNode;

        int mLeafNodesLength;
        int mParentNodesLength;

        bool mInitialized;

        HuffmanTree();

        ~HuffmanTree();

        void BuildHuffmanTree(CharAndFreq* pFreqDatas);
};

void Encode(FILE* pFileToEncode, HuffmanTree* pHuffmanTree, FILE* pFileToWrite, long* pProgressPermile = NULL);
void Decode(FILE* pFileToDecode, HuffmanTree* pHuffmanTree, FILE* pFileToWrite, long* pProgressPermile = NULL);

void Compress(FILE* pFileSource, FILE* pFileTo, long* pProgressPermile = NULL);
void Extract (FILE* pFileSource, FILE* pFileTo, long* pProgressPermile = NULL);




void Compress(FILE* pFileSource, FILE* pFileTo, long* pProgressPermile)
{
    CharAndFreq char_freq_data;
    char_freq_data.CountFreqFromFile(pFileSource);

    HuffmanTree huffman_tree;
    huffman_tree.BuildHuffmanTree(&char_freq_data);

    Encode(pFileSource, &huffman_tree, pFileTo);

    return;
}

void Extract (FILE* pFileSource, FILE* pFileTo, long* pProgressPermile)
{
    CharAndFreq char_freq_data;
    char_freq_data.ReadFreqDatasFromFile(pFileSource);

    HuffmanTree huffman_tree;
    huffman_tree.BuildHuffmanTree(&char_freq_data);

    Decode(pFileSource, &huffman_tree, pFileTo);

    return;
}





HuffmanTree::HuffmanTree(int LeafNodesLength)
{
    printf("initializing huffman tree... ");

    mLeafNodesLength = LeafNodesLength;
    mParentNodesLength = mLeafNodesLength - 1;

    mpLeafNodes   = new HeapNode [mLeafNodesLength];
    mpParentNodes = new HeapNode [mParentNodesLength];

    mpRootNode = NULL;

    printf("done.\n");
}

HuffmanTree::~HuffmanTree()
{
    delete [] mpLeafNodes;
    delete [] mpParentNodes;
}

void HuffmanTree::BuildHuffmanTree(CharAndFreq* pFreqDatas)
{
    printf("building huffman tree... ");

    for (int i = 0; i < pFreqDatas->ExistCharsCount; i++)
    {
        mpLeafNodes[i].NodeType = 1;
        mpLeafNodes[i].Freq = pFreqDatas->pFreqs[pFreqDatas->pFreqSortedExistChars[i]];

        mpLeafNodes[i].pParent = NULL;
        mpLeafNodes[i].Data = pFreqDatas->pFreqSortedExistChars[i];

        mpLeafNodes[i].pRightNode = NULL;
        mpLeafNodes[i].pLeftNode = NULL;
    }

    HeapNode** min_heap;
    min_heap = new HeapNode* [mLeafNodesLength];
    int min_heap_length = mLeafNodesLength;

    for (int i = 0; i < mLeafNodesLength; i++)
    {
        min_heap[i] = &mpLeafNodes[i];
    }

    HeapNode new_parent_node;
    new_parent_node.NodeType = 2;

    int i;
    for (i = 0; min_heap_length >= 2; i++)
    {

        // make new parent node to combine two least frequent datas.

        new_parent_node.Freq = min_heap[0]->Freq + min_heap[1]->Freq;

        new_parent_node.pRightNode = min_heap[0];
        new_parent_node.pLeftNode  = min_heap[1];

        mpParentNodes[i] = new_parent_node;

        min_heap[0]->pParent = &mpParentNodes[i];
        min_heap[1]->pParent = &mpParentNodes[i];

        // update min heap
        //    slide for 2 nodes until the new parent node's frequency is greater than next node.
        //    after that, slide for 1 node.
        //    if the sliding reaches the end of the min heap, and the new node is not inserted, insert and finish it.

        bool inserted = false;
        for (int j = 0; 1; j++)
        {
            if (j == (min_heap_length - 2))
            {
                if (inserted == false)
                {
                    min_heap[j] = &mpParentNodes[i];
                    inserted = true;
                }
                else
                {
                    min_heap[j] = min_heap[j + 1];
                }

                break;
            }

            if (inserted)
            {
                min_heap[j] = min_heap[j + 1];
                continue;
            }

            if (min_heap[j + 2]->Freq >= new_parent_node.Freq)
            {
                min_heap[j] = &mpParentNodes[i];
                inserted = true;
                continue;
            }

            min_heap[j] = min_heap[j + 2];
        }

        min_heap_length--;

        // for (int j = 0; j < min_heap_length; j++)
        // {
        //     if (min_heap[j]->NodeType == 2)
        //     {
        //         printf("internal : %llu\n", min_heap[j]->Freq);
        //     }
        //     else
        //     {
        //         printf("      %2x : %llu\n", min_heap[j]->Data, min_heap[j]->Freq);
        //     }
        // }
        // printf("length : %d\n", min_heap_length);
        // getchar();
    }

    mpRootNode = &mpParentNodes[mParentNodesLength - 1];

    printf("done.\n");
    printf("total freq : %llu\n", mpRootNode->Freq);
}



void Encode(FILE* pFileToEncode, HuffmanTree* pHuffmanTree, FILE* pFileToWrite)
{
    printf("building convertion table... ");

    bool char_and_codes[256][256];
    int  char_code_lengthes[256] = {0};

    for (int i = 0; i < pHuffmanTree->mLeafNodesLength; i++)
    {
        int char_now = pHuffmanTree->mpLeafNodes[i].Data;
        HuffmanTree::HeapNode* p_current_node = &pHuffmanTree->mpLeafNodes[i];

        char_code_lengthes[char_now] = 0;

        for (; p_current_node != pHuffmanTree->mpRootNode; char_code_lengthes[char_now]++)
        {

            for (int j = char_code_lengthes[char_now]; j > 0; j--)
            {
                char_and_codes[char_now][j] = char_and_codes[char_now][j - 1];
            }

            char_and_codes[char_now][0] = p_current_node->pParent->pRightNode == p_current_node;

            p_current_node = p_current_node->pParent;
        }
    }

    printf("done.\n");

    // for (int i = 0; i < pHuffmanTree->mLeafNodesLength; i++)
    // {
    //     printf("\n");
    //     int char_now = pHuffmanTree->mpLeafNodes[i].Data;
    //     printf("%2X : %10llu :", char_now, pHuffmanTree->mpLeafNodes[i].Freq);
    //     for (int j = 0; j < char_code_lengthes[char_now]; j++)
    //     {
    //         if (char_and_codes[char_now][j])
    //         {
    //             printf("1");
    //         }
    //         else
    //         {
    //             printf("0");
    //         }
    //     }
    // }
    // printf("\n");

    printf("encoding frequency datas... ");



    unsigned long long file_size;

    fseek(pFileToEncode, 0L, SEEK_END);
    file_size = _ftelli64(pFileToEncode);
    fseek(pFileToEncode, 0L, SEEK_SET);



    unsigned long long maximum_freq = 0;
    for (int i = 0; i < pHuffmanTree->mLeafNodesLength; i++)
    {
        if (maximum_freq < pHuffmanTree->mpLeafNodes[i].Freq)
        {
            maximum_freq = pHuffmanTree->mpLeafNodes[i].Freq;
        }
    }

    int a = 2;
    int i = 1;
    while (1)
    {
        if (maximum_freq < a)
        {
            break;
        }
        a = a << 1;
        i++;
    }

    int freq_bytes = (i % 8) == 0 ? i / 8 : i / 8 + 1;

    fseek(pFileToWrite, 0xe, SEEK_SET);

    // Exist chars
    fputc(pHuffmanTree->mLeafNodesLength, pFileToWrite);

    // Bytes of single freq datas
    fputc(freq_bytes, pFileToWrite);



    // freq data

    long freq_data_length = 0;

    fseek(pFileToWrite, 0x21, SEEK_SET);

    for (i = 0; i < pHuffmanTree->mLeafNodesLength; i++)
    {
        fputc(pHuffmanTree->mpLeafNodes[i].Data, pFileToWrite);
        freq_data_length++;

        unsigned long long char_freq = pHuffmanTree->mpLeafNodes[i].Freq;
        for (int j = 0; j < freq_bytes; j++)
        {
            fputc(char_freq % 0x100, pFileToWrite);
            freq_data_length++;
            char_freq >>= 8;
        }
    }

    fseek(pFileToWrite, 0x10, SEEK_SET);

    long freq_data_length_cpy = freq_data_length;
    // freq length
    for (i = 0; i < 4; i++)
    {
        fputc(freq_data_length_cpy % 0x100, pFileToWrite);
        freq_data_length_cpy >>= 8;
    }

    // main data offset
    freq_data_length += 0x21;
    freq_data_length_cpy = freq_data_length;
    for (i = 0; i < 4; i++)
    {
        fputc(freq_data_length_cpy % 0x100, pFileToWrite);
        freq_data_length_cpy >>= 8;
    }

    printf("done.\n");






    printf("encoding main data... ");

    fseek(pFileToWrite, freq_data_length, SEEK_SET);
    // encode main_data
    int writing_char = 0;
    int encoding_char;
    int char_bits_count = 0;
    unsigned long long main_data_bytes_count = 0;

    for (i = 0; i < file_size; i++)
    {
        encoding_char = fgetc(pFileToEncode);
        for (int j = 0; j < char_code_lengthes[encoding_char]; j++)
        {
            writing_char <<= 1;
            if (char_and_codes[encoding_char][j])
            {
                writing_char++;
            }
            char_bits_count++;

            if (char_bits_count == 8)
            {
                fputc(writing_char, pFileToWrite);
                writing_char = 0;
                main_data_bytes_count++;
                char_bits_count = 0;
            }
        }
    }
    if (char_bits_count != 0)
    {
        fputc(writing_char, pFileToWrite);
    }
    printf("done.\n");

    fseek(pFileToWrite, 0x18, SEEK_SET);

    // main data size
    unsigned long long main_data_bytes_count_cpy = main_data_bytes_count;
    for (i = 0; i < 8; i++)
    {
        fputc(main_data_bytes_count_cpy % 0x100, pFileToWrite);
        main_data_bytes_count_cpy >>= 8;
    }

    // last byte info
    fputc(char_bits_count, pFileToWrite);



    // header
    fseek(pFileToWrite, 0L, SEEK_SET);

    // header
    fputc('E', pFileToWrite);
    fputc('H', pFileToWrite);

    // offset
    fputc(0x21, pFileToWrite);
    fputc(0, pFileToWrite);
    fputc(0, pFileToWrite);
    fputc(0, pFileToWrite);

    // file size
    unsigned long long compressed_file_size = freq_data_length + main_data_bytes_count;
    for (i = 0; i < 8; i++)
    {
        fputc(compressed_file_size % 0x100, pFileToWrite);
        compressed_file_size >>= 8;
    }
}





void ConvertToBoolArr(int Number, bool* pBoolArr, int max_reading = 8)
{
    for (int i = max_reading - 1; i >= 0; i--)
    {
        pBoolArr[i] = Number % 2;
        Number >>= 1;
    }
}
void Decode(FILE* pFileToDecode, HuffmanTree* pHuffmanTree, FILE* pFileToWrite)
{
    unsigned long long main_data_bytes = 0;
    fseek(pFileToDecode, 0x18, SEEK_SET);
    for (int i = 0; i < 8; i++)
    {
        main_data_bytes += fgetc(pFileToDecode) << (8 * i);
    }

    int main_data_offset = 0;
    fseek(pFileToDecode, 0x14, SEEK_SET);
    for (int i = 0; i < 4; i++)
    {
        main_data_offset += fgetc(pFileToDecode) << (8 * i);
    }

    fseek(pFileToDecode, 0x20, SEEK_SET);
    int last_byte_length = fgetc(pFileToDecode);



    printf("decoding... ");
    HuffmanTree::HeapNode current_node = *(pHuffmanTree->mpRootNode);

    bool reading_char_binary[8];

    fseek(pFileToDecode, main_data_offset, SEEK_SET);
    for (int i = 0; i < main_data_bytes; i++)
    {
        ConvertToBoolArr(fgetc(pFileToDecode), reading_char_binary);
        for (int j = 0; j < 8; j++)
        {
            if (reading_char_binary[j])
            {
                current_node = *(current_node.pRightNode);
            }
            else
            {
                current_node = *(current_node.pLeftNode);
            }

            if (current_node.NodeType == 1)
            {
                fputc(current_node.Data, pFileToWrite);
                current_node = *(pHuffmanTree->mpRootNode);
            }
        }
    }

    int last_char = fgetc(pFileToDecode);
    ConvertToBoolArr(last_char, reading_char_binary, last_byte_length);
    for (int i = 0; i < last_byte_length; i++)
    {
        if (reading_char_binary[i])
        {
            current_node = *(current_node.pRightNode);
        }
        else
        {
            current_node = *(current_node.pLeftNode);
        }

        if (current_node.NodeType == 1)
        {
            fputc(current_node.Data, pFileToWrite);
            current_node = *(pHuffmanTree->mpRootNode);
        }
    }

    printf("done.\n");
}







CharAndFreq::CharAndFreq()
{
    for (int i = 0; i < 256; i++)
    {
        pFreqs[i] = 0;
        pFreqSortedExistChars[i] = 0;
    }
    ExistCharsCount = 0;
}

void CharAndFreq::CountFreqFromFile(FILE* pFileToRead)
{
}

void CharAndFreq::ReadFreqDataFromFile(FILE* pFileToRead)
{
}

void SortExistChars(CharAndFreq* pFreqDatas);
void CountFreqFromFile    (FILE* pFileToRead, CharAndFreq* pFreqDatas)
{
    unsigned long long file_size;
    fseek(pFileToRead, 0L, SEEK_END);
    file_size = _ftelli64(pFileToRead);
    fseek(pFileToRead, 0L, SEEK_SET);

    printf("file size : %llubytes\n", file_size);



    for (int i = 0; i < 256; i++)
    {
        pFreqDatas->pFreqs[i] = 0;
        pFreqDatas->pFreqSortedExistChars[i] = 0;
    }
    pFreqDatas->ExistCharsCount = 0;



    int c;
    for (unsigned long long i = 0; i < file_size; i++)
    {
        c = fgetc(pFileToRead);
        pFreqDatas->pFreqs[c]++;
    }

    for (int i = 0; i < 256; i++)
    {
        if (pFreqDatas->pFreqs[i] >= 1)
        {
            pFreqDatas->pFreqSortedExistChars[pFreqDatas->ExistCharsCount] = i;
            pFreqDatas->ExistCharsCount++;
        }
    }

    printf("count finished!\n");
    printf("%d characters found\n", pFreqDatas->ExistCharsCount);
    printf("sorting..");

    SortExistChars(pFreqDatas);

    printf("done\n");

    // for (int i = 0; i < pFreqDatas->ExistCharsCount; i++)
    // {
    //     c = pFreqDatas->pFreqSortedExistChars[i];
    //     printf("%2X : %llu\n", c, pFreqDatas->pFreqs[c]);
    // }
}

void ReadFreqDatasFromFile(FILE* pFileToRead, CharAndFreq* pFreqDatas)
{

    for (int i = 0; i < 256; i++)
    {
        pFreqDatas->pFreqs[i] = 0;
        pFreqDatas->pFreqSortedExistChars[i] = 0;
    }
    pFreqDatas->ExistCharsCount = 0;

    fseek(pFileToRead, 0xe, SEEK_SET);
    int exist_chars = fgetc(pFileToRead);
    pFreqDatas->ExistCharsCount = exist_chars == 0 ? 256 : exist_chars;

    int single_freq_data_bytes = fgetc(pFileToRead);

    fseek(pFileToRead, 0x21, SEEK_SET);
    for (int i = 0; i < pFreqDatas->ExistCharsCount; i++)
    {
        pFreqDatas->pFreqSortedExistChars[i] = fgetc(pFileToRead);
        for (int j = 0; j < single_freq_data_bytes; j++)
        {
            pFreqDatas->pFreqs[pFreqDatas->pFreqSortedExistChars[i]] += fgetc(pFileToRead) << (8 * j);
        }
    }

    // int c;
    // for (int i = 0; i < pFreqDatas->ExistCharsCount; i++)
    // {
    //     c = pFreqDatas->pFreqSortedExistChars[i];
    //     printf("%2X : %llu\n", c, pFreqDatas->pFreqs[c]);
    // }
}


void quickSort(CharAndFreq* pFreqDatas, int first, int last);
void SortExistChars(CharAndFreq* pFreqDatas)
{
    quickSort(pFreqDatas, 0, pFreqDatas->ExistCharsCount - 1);
}

void quickSort(CharAndFreq* pFreqDatas, int first, int last)
{
    int i, j;
    unsigned long long pivot;
    int tmp;

    pivot = pFreqDatas->pFreqs[pFreqDatas->pFreqSortedExistChars[(first + last) / 2]];
    i = first; j = last;

    while (1)
    {
        while (pFreqDatas->pFreqs[pFreqDatas->pFreqSortedExistChars[i]] < pivot)
        {
            i++;
        }
        while (pivot < pFreqDatas->pFreqs[pFreqDatas->pFreqSortedExistChars[j]])
        {
            j--;
        }

        if (i >= j)
        {
            break;
        }

        tmp = pFreqDatas->pFreqSortedExistChars[i];
        pFreqDatas->pFreqSortedExistChars[i] = pFreqDatas->pFreqSortedExistChars[j];
        pFreqDatas->pFreqSortedExistChars[j] = tmp;

        i++; j--;
    }

    if (first < (i - 1))
    {
        quickSort(pFreqDatas, first, (i - 1));
    }

    if ((j + 1) < last)
    {
        quickSort(pFreqDatas, (j + 1), last);
    }
}






