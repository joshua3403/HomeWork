/*
	MemoryAllocLog.cpp
	���α׷� ���� : �޸𸮸� �Ҵ��ϰ� �����ϴ� �������� �α׸� ����Ͽ� ��������
*/
#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include "My_New.h"

using namespace std;

struct st_ALLOC_INFO
{
	void* pPtr;			// �Ҵ�� ������ �ּҸ� ����(�ڷ����� Ư���� �� ������ void)
	int iSize;			// �Ҵ�� �޸��� ũ��
	char szFile[256];	// �޸𸮸� �Ҵ��� �����̸��� ��θ� ����
	int iLine;			// �޸� �Ҵ��� �õ��� ������ ���� ����
};

st_ALLOC_INFO g_stAllocInfo[100];		// �޸� ���� ����ü�� ������ �迭
int g_iTotalAllocCount = 0;				// �޸� �Ҵ��� Ƚ���� ������ ����
int g_iTotalAllocSize = 0;				// �Ҵ�� �޸� �� ũ��

#define _MemoryAlloc(T, Size)	MemoryAlloc<T>(Size,__FILE__,__LINE__)

// �޸� ���� �迭���� ��ĭ�� ã�Ƽ� ������ �ִ´�
bool SaveAllocInfo(void* pPtr, int iSize, char* szFile, int iLine)
{
	int iCnt = 0;
	for (iCnt = 0; iCnt < 100; ++iCnt)
	{
		if (g_stAllocInfo[iCnt].pPtr == nullptr)
		{
			g_stAllocInfo[iCnt].pPtr = pPtr;
			g_stAllocInfo[iCnt].iSize = iSize;
			strcpy_s(g_stAllocInfo[iCnt].szFile,sizeof(g_stAllocInfo[iCnt].szFile), szFile);
			g_stAllocInfo[iCnt].iLine = iLine;
			return true;
		}
	}

	return false;
}

// �޸� ���� �迭���� �Էµ� �������� ������ ã�´�.
int FindAllocInfo(void* pPtr)
{
	int iCnt = 0;

	if (pPtr == nullptr)
		return -1;

	for (iCnt = 0; iCnt < 100; ++iCnt)
	{
		if (g_stAllocInfo[iCnt].pPtr == pPtr)
			return iCnt;
	}
	return -1;
}

// �޸� �Ҵ� �Լ�
template<typename T>
T* MemoryAlloc(int iCount, char* szFile, int iLine)
{
	T* pNew = new T[iCount];

	// ��ũ�ο��� szFile�� __FILE__�� ������ iLine���� __LINE__�� ���´�.
	SaveAllocInfo(pNew, iCount, szFile, iLine);

	g_iTotalAllocCount++;
	g_iTotalAllocSize += sizeof(T) * iCount;

	return pNew;
}

// �޸� ���� �Լ�
template<typename T>
bool MemoryRelease(T* pPtr)
{
	// �Ҵ� �������� �ش� �����͸� ã�´�.
	int iAllocIndex = FindAllocInfo(pPtr);
	if (iAllocIndex == -1)
		return false;

	// �ش� ���� ����
	memset(&g_stAllocInfo[iAllocIndex], 0, sizeof(st_ALLOC_INFO));

	// �޸� ����
	delete[] pPtr;

	return true;
}

// �޸� ���� �̱�
void PrintAlloc(void)
{
	printf("\n Total Alloc Size : %d \nTotal Alloc Count : %d \n", g_iTotalAllocSize, g_iTotalAllocCount);
}

int main()
{
	int temp = new int;
	return 0;
}