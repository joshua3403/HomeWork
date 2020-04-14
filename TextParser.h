#pragma once
#include "stdafx.h"

#define BUFSIZE 1024

class CTextParser
{
private:
	FILE* pFile;
	char* cBuf;
	long lSize;
public:
	BOOL LoadFile(const char* name)
	{
		fopen_s(&pFile,name, "rb");
		if (pFile == nullptr)
		{
			printf("fopen() 오류\n");
			return FALSE;
		}

		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);
		cBuf = (char*)malloc(sizeof(char) * lSize);
		if (cBuf == nullptr)
		{
			printf("malloc() 오류\n");
			return FALSE;
		}

		int result = fread(cBuf, 1, lSize, pFile);
		if (result != lSize)
		{
			printf("fread() 오류\n");
			return FALSE;
		}

		return TRUE;
	}

	BOOL GetWord(const char* word, int* ipValue)
	{
		char* position = strstr(cBuf, word);
		int iWordLength = sizeof(word) - 1;
		char temp[20];

		int i = 0;
		while (i < iWordLength + 2)
		{
			position++;
			i++;
		}
		
		for (int i = 0; i < iWordLength; ++i)
		{
			temp[i] = (*position);
			position++;
		}

		*ipValue = atoi(temp);

		return TRUE;
	}

};