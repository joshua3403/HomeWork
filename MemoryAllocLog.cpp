/*
	MemoryAllocLog.cpp
	프로그램 설명 : 메모리를 할당하고 해제하는 과정에서 로그를 출력하여 추적하자
*/
#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include "My_New.h"

using namespace std;

struct st_ALLOC_INFO
{
	void* pPtr;			// 할당된 포인터 주소를 저장(자료형을 특정할 수 없으니 void)
	int iSize;			// 할당된 메모리의 크기
	char szFile[256];	// 메모리를 할당한 파일이름과 경로를 저장
	int iLine;			// 메모리 할당을 시도한 파일의 줄을 저장
};

st_ALLOC_INFO g_stAllocInfo[100];		// 메모리 정보 구조체를 저장할 배열
int g_iTotalAllocCount = 0;				// 메모리 할당의 횟수를 저장할 변수
int g_iTotalAllocSize = 0;				// 할당된 메모리 총 크기

#define _MemoryAlloc(T, Size)	MemoryAlloc<T>(Size,__FILE__,__LINE__)

// 메모리 정보 배열에서 빈칸을 찾아서 정보를 넣는다
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

// 메모리 정보 배열에서 입력된 포인터의 정보를 찾는다.
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

// 메모리 할당 함수
template<typename T>
T* MemoryAlloc(int iCount, char* szFile, int iLine)
{
	T* pNew = new T[iCount];

	// 매크로에서 szFile로 __FILE__이 들어오고 iLine으로 __LINE__이 들어온다.
	SaveAllocInfo(pNew, iCount, szFile, iLine);

	g_iTotalAllocCount++;
	g_iTotalAllocSize += sizeof(T) * iCount;

	return pNew;
}

// 메모리 해제 함수
template<typename T>
bool MemoryRelease(T* pPtr)
{
	// 할당 정보에서 해당 포인터를 찾는다.
	int iAllocIndex = FindAllocInfo(pPtr);
	if (iAllocIndex == -1)
		return false;

	// 해당 정보 삭제
	memset(&g_stAllocInfo[iAllocIndex], 0, sizeof(st_ALLOC_INFO));

	// 메모리 삭제
	delete[] pPtr;

	return true;
}

// 메모리 정보 뽑기
void PrintAlloc(void)
{
	printf("\n Total Alloc Size : %d \nTotal Alloc Count : %d \n", g_iTotalAllocSize, g_iTotalAllocCount);
}

int main()
{
	int temp = new int;
	return 0;
}