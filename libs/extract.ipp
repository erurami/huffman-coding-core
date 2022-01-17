
#pragma once
#include "huffman.hpp"


bool _G_Extract_Reset_Static_Vars_ReadBranch = false;


bool _G_Extract_Use_Progress;
// TODO : name
ProgressManagerExtraction _G_Extract_Progress_Manager;






void Extract(FILE* pFileSource, FILE* pFileTo,
             void (*pCallbackFunc)(long, int, void*),
             void* pArgForFunc)
{
    _G_Extract_Reset_Static_Vars_ReadBranch = true;



    if (pCallbackFunc == NULL)
    {
        _G_Extract_Use_Progress = false;
    }
    else
    {
        _G_Extract_Use_Progress = true;

        _G_Extract_Progress_Manager.mpCallbackFunc = pCallbackFunc;
        _G_Extract_Progress_Manager.mpArgForFunc = pArgForFunc;
    }




    FileHeaderData file_infos;
    ReadHeaderData(pFileSource, &file_infos);

    HuffmanTree huffman_tree;
    ReadHuffmanTree(pFileSource, &huffman_tree, &file_infos);

    // PrintHuffmanTree(&huffman_tree);
    // printf("================================\n");

    Decode(pFileSource, pFileTo, &huffman_tree, &file_infos);
}










long long ReadNumber(FILE* pFile, int numberLocation, int maxNumberToGet);
void ReadHeaderData(FILE* pFile, FileHeaderData* pFileData)
{
    if (_G_Extract_Use_Progress)
    {
        _G_Extract_Progress_Manager.UpdateProg(EXTRACTION_STEP_READINFO, 0, 1);
    }

    pFileData->OffsetTreeStructure = ReadNumber(pFile, 0x07, 2);
    pFileData->OffsetTreeData      = ReadNumber(pFile, 0x09, 2);
    pFileData->OffsetMainData      = ReadNumber(pFile, 0x0b, 2);

    pFileData->BitsTreeStructure   = ReadNumber(pFile, 0x0d, 2);
    pFileData->BytesTreeData       = ReadNumber(pFile, 0x0f, 2);
    pFileData->BytesMainData       = ReadNumber(pFile, 0x11, 4);

    pFileData->BitsUsedInLastByte  = ReadNumber(pFile, 0x15, 1);

    pFileData->FileSize            = ReadNumber(pFile, 0x03, 4);

    if (_G_Extract_Use_Progress)
    {
        _G_Extract_Progress_Manager.UpdateProg(EXTRACTION_STEP_READINFO, 1, 1);
    }
}

long long ReadNumber(FILE* pFile, int numberLocation, int maxNumberToGet)
{
    fseek(pFile, numberLocation, SEEK_SET);

    long long Value = 0;
    for (int i = 0; i < maxNumberToGet; i++)
    {
        Value += fgetc(pFile) << (8 * i);
    }
    return Value;
}





HuffmanTreeNode* ReadBranch(Bitio::File* pFileToRead, HuffmanTree* pHuffmanTree);
void ReadHuffmanTree(FILE* pFileToRead, HuffmanTree* pHuffmanTree, FileHeaderData* pFileData)
{
    if (_G_Extract_Use_Progress)
    {
        _G_Extract_Progress_Manager.UpdateProg(EXTRACTION_STEP_READTREE, 0, 1);
    }

    fseek(pFileToRead, 0x0d, SEEK_SET);

    int exist_chars = pFileData->BitsTreeStructure;

    exist_chars += 1;
    exist_chars /= 2;

    // printf("%d\n", exist_chars);

    pHuffmanTree->pLeafNodes     = new HuffmanTreeNode [exist_chars];
    pHuffmanTree->pParentNodes   = new HuffmanTreeNode [exist_chars - 1];
    pHuffmanTree->LeafNodeLength = exist_chars;

    Bitio::File file_to_read_bitio;
    file_to_read_bitio.UseFileObj(pFileToRead, "r");

    file_to_read_bitio.SeekBytes(pFileData->OffsetTreeStructure);

    pHuffmanTree->pRootNode = ReadBranch(&file_to_read_bitio, pHuffmanTree);


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


    file_to_read_bitio.SeekBytes(pFileData->OffsetTreeData);
    for (int i = 0; i < pHuffmanTree->LeafNodeLength; i++)
    {
        pHuffmanTree->pLeafNodes[i].Data = file_to_read_bitio.GetChar();
    }

    if (_G_Extract_Use_Progress)
    {
        _G_Extract_Progress_Manager.UpdateProg(EXTRACTION_STEP_READTREE, 1, 1);
    }
}

HuffmanTreeNode* ReadBranch(Bitio::File* pFileToRead, HuffmanTree* pHuffmanTree)
{

    static int huffman_tree_parent_index = 0;
    static int huffman_tree_leaf_index   = 0;

    if (_G_Extract_Reset_Static_Vars_ReadBranch)
    {
        huffman_tree_parent_index = 0;
        huffman_tree_leaf_index   = 0;
        _G_Extract_Reset_Static_Vars_ReadBranch = false;
    }

    bool bit_just_read = pFileToRead->GetBit();

    HuffmanTreeNode  new_node;
    HuffmanTreeNode* p_new_node;

    if (bit_just_read == true)
    {

        new_node.NodeType = 1;

        new_node.pLeft  = ReadBranch(pFileToRead, pHuffmanTree);
        new_node.pRight = ReadBranch(pFileToRead, pHuffmanTree);

        pHuffmanTree->pParentNodes[huffman_tree_parent_index] = new_node;

        p_new_node = &pHuffmanTree->pParentNodes[huffman_tree_parent_index];

        huffman_tree_parent_index++;
    }
    else
    {
        new_node.NodeType = 2;

        pHuffmanTree->pLeafNodes[huffman_tree_leaf_index]     = new_node;

        p_new_node = &pHuffmanTree->pLeafNodes[huffman_tree_leaf_index];

        huffman_tree_leaf_index++;
    }

    return p_new_node;
}





void Decode(FILE* pFileSource, FILE* pFileToWrite, HuffmanTree* pHuffmanTree, FileHeaderData* pFileData)
{
    if (_G_Extract_Use_Progress)
    {
        _G_Extract_Progress_Manager.UpdateProg(EXTRACTION_STEP_DECODE, 0, 1);
    }

    long long bytes_left_in_main_data = pFileData->BytesMainData;
    int       bits_left_in_main_data  = pFileData->BitsUsedInLastByte;

    Bitio::File file_source;
    file_source.UseFileObj(pFileSource, "r");

    file_source.SeekBytes(pFileData->OffsetMainData);

    bool bit_just_read;


    HuffmanTreeNode current_node = *pHuffmanTree->pRootNode;


    unsigned long long update_prog_flag = pFileData->BytesMainData / _PROGRESS_UPDATE_FREQ;

    for (int i = 0 ; bytes_left_in_main_data > 0 || bits_left_in_main_data > 0; i++)
    {

        if (_G_Extract_Use_Progress && update_prog_flag == 0)
        {
            _G_Extract_Progress_Manager.UpdateProg(EXTRACTION_STEP_DECODE, pFileData->BytesMainData - bytes_left_in_main_data, pFileData->BytesMainData);
            update_prog_flag = pFileData->BytesMainData / _PROGRESS_UPDATE_FREQ;
        }
        update_prog_flag--;


        bit_just_read = file_source.GetBit();

        if (bit_just_read == true)
        {
            current_node = *current_node.pRight;
        }
        else
        {
            current_node = *current_node.pLeft;
        }


        if (current_node.NodeType == 2)
        {
            fputc(current_node.Data, pFileToWrite);
            current_node = *pHuffmanTree->pRootNode;
        }


        if (bits_left_in_main_data == 0 && bytes_left_in_main_data > 0)
        {
            bytes_left_in_main_data--;
            bits_left_in_main_data = 8;
        }
        bits_left_in_main_data--;
    }

    if (_G_Extract_Use_Progress)
    {
        _G_Extract_Progress_Manager.UpdateProg(EXTRACTION_STEP_DECODE, 1, 1);
    }
}
