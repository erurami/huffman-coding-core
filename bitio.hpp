
#include <stdio.h>
#include <string.h>

#pragma once

#define FILE_INFO_READ 1
#define FILE_INFO_WRITE 2

namespace Bitio
{
    class File
    {
        private:

            int BitPosition_;
            unsigned long long Position_;

            bool DidFileLoaded_;

            int  OpeningMode_;
            // 1 : read
            // 2 : write

            // for reading mode
            bool pReadingCharBits_[8];

            void ReadNextAndCalcBits(void);


            // for writing mode
            bool pBitsBufToPut_[8];

            void PutBitsInBuf(void);

        public:

            FILE* pFile_;

            File();
            ~File();

            void SeekBits (int position);
            void SeekBytes(int position);

            bool GetBit();
            bool GetBits(bool* pDes, int getBits);

            void PutBit (bool bit);
            void PutBits(bool* pBits, int putBits);

            int  GetChar(void);
            void PutChar(int character);

            bool Open(char* pPath, const char* mode);
            void UseFileObj(FILE* pFile, const char* mode);
    };
};

Bitio::File::File()
{
    DidFileLoaded_ = false;
    pFile_ = NULL;
}

Bitio::File::~File()
{
    if (DidFileLoaded_ == true && OpeningMode_ == 1)
    {
        fclose(pFile_);
    }
    else if (DidFileLoaded_ == true && OpeningMode_ == FILE_INFO_WRITE)
    {
        PutBitsInBuf();
        fclose(pFile_);
    }
}





void Bitio::File::SeekBits(int position)
{
    BitPosition_ = position;
}


void Bitio::File::SeekBytes(int position)
{
    Position_ = position;
    _fseeki64(pFile_, Position_, SEEK_SET);

    ReadNextAndCalcBits();
}





bool Bitio::File::GetBit()
{

    if (DidFileLoaded_ == false ||
        OpeningMode_   != FILE_INFO_READ)
    {
        return false;
    }

    if (BitPosition_ >= 8)
    {
        Position_++;

        ReadNextAndCalcBits();

        BitPosition_ %= 8;
    }

    bool retval = pReadingCharBits_[BitPosition_];

    BitPosition_++;

    return retval;
}


bool Bitio::File::GetBits(bool* pBits, int getBits)
{
    if (DidFileLoaded_ == false ||
        OpeningMode_   != FILE_INFO_READ)
    {
        return false;
    }

    for (int i = 0; i < getBits; i++)
    {
        pBits[i] = GetBit();
    }

    return true;
}





void Bitio::File::PutBit(bool bit)
{
    if (DidFileLoaded_ == false ||
        OpeningMode_   != FILE_INFO_WRITE)
    {
        return;
    }

    pBitsBufToPut_[BitPosition_] = bit;

    BitPosition_++;

    if (BitPosition_ == 8)
    {
        Position_++;

        PutBitsInBuf();

        BitPosition_ = 0;
    }
}



void Bitio::FilePutBits(bool* pBits, int putBits)
{
    if (DidFileLoaded_ == false ||
        OpeningMode_   != FILE_INFO_WRITE)
    {
        return;
    }

    for (int i = 0; i < putBits; i++)
    {
        PutBit(pBits[i]);
    }
    return;
}





int  Bitio::File::GetChar(void)
{
    if (OpeningMode_ != FILE_INFO_READ)
    {
        return 0;
    }
    return fgetc(pFile_);
}



void Bitio::File::PutChar(int character)
{
    if (OpeningMode_ != FILE_INFO_WRITE)
    {
        return;
    }
    fputc(character, pFile_);
}





bool Bitio::File::Open(char* pPath, const char* mode)
    // r read
    // w write
{

    FILE* p_file;

    if (mode[0] == 'r')
    {
        p_file = fopen(pPath, "rb");
    }
    else if (mode[0] == 'w')
    {
        p_file = fopen(pPath, "wb");
    }

    if (p_file == NULL)
    {
        return false;
    }

    UseFileObj(p_file, mode);

    return true;
}

void Bitio::File::UseFileObj(FILE* pFile, const char* mode)
{
    pFile_ = pFile;
    DidFileLoaded_ = true;

    Position_ = 0;
    BitPosition_ = 0;

    fseek(pFile_, 0L, SEEK_SET);

    if (mode[0] ==  'r')
    {
        OpeningMode_ = FILE_INFO_READ;
        ReadNextAndCalcBits();
    }

    else if (mode[0] == 'w')
    {
        OpeningMode_ = FILE_INFO_WRITE;
    }
}





void Bitio::File::ReadNextAndCalcBits()
{
    int got_char = GetChar();

    int reading_char = got_char;
    for (int i = 7; i >= 0; i--)
    {
        pReadingCharBits_[i] = reading_char % 2;
        reading_char >>= 1;
    }
}



void Bitio::File::PutBitsInBuf(void)
{
    int char_to_put = 0;

    for (int i = 0; i < 8; i++)
    {
        char_to_put <<= 1;
        if (pBitsBufToPut_[i])
        {
            char_to_put++;
            printf("1\n");
        }
        else
        {
            printf("0\n");
        }
        pBitsBufToPut_[i] = false;
    }
    printf("%d\n", char_to_put);

    PutChar(char_to_put);
}
