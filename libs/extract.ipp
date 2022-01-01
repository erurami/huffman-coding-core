
#pragma once


// bool G_Reset_ _Static_Vars = false;






void Extract(FILE* pFileSource, FILE* pFileTo, long* pProgress)
{
    // G_Reset_  _Static_Vars = true;


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

    pFileData->OffsetTreeStructure = ReadNumber(pFile, 0x07, 2);
    pFileData->OffsetTreeData      = ReadNumber(pFile, 0x09, 2);
    pFileData->OffsetMainData      = ReadNumber(pFile, 0x0b, 2);

    pFileData->BitsTreeStructure   = ReadNumber(pFile, 0x0d, 2);
    pFileData->BytesTreeData       = ReadNumber(pFile, 0x0f, 2);
    pFileData->BytesMainData       = ReadNumber(pFile, 0x11, 4);

    pFileData->BitsUsedInLastByte  = ReadNumber(pFile, 0x15, 1);

    pFileData->FileSize            = ReadNumber(pFile, 0x03, 4);
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

}

HuffmanTreeNode* ReadBranch(Bitio::File* pFileToRead, HuffmanTree* pHuffmanTree)
{

    static int huffman_tree_parent_index = 0;
    static int huffman_tree_leaf_index   = 0;

    // if (G_Reset_ReadBranch_Static_Vars)
    // {
    //     huffman_tree_parent_index = 0;
    //     huffman_tree_leaf_index   = 0;
    //     G_Reset_ReadBranch_Static_Vars = false;
    // }

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
    long long bytes_left_in_main_data = pFileData->BytesMainData;
    int       bits_left_in_main_data  = pFileData->BitsUsedInLastByte;

    Bitio::File file_source;
    file_source.UseFileObj(pFileSource, "r");

    file_source.SeekBytes(pFileData->OffsetMainData);

    bool bit_just_read;


    HuffmanTreeNode current_node = *pHuffmanTree->pRootNode;

    for ( ; bytes_left_in_main_data > 0 || bits_left_in_main_data > 0; )
    {

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


        bits_left_in_main_data--;
        if (bits_left_in_main_data == 0 && bytes_left_in_main_data > 0)
        {
            bytes_left_in_main_data--;
            bits_left_in_main_data = 8;
        }
    }
}
