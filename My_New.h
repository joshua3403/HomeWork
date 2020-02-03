#pragma once

#include "stdafx.h"

#define MAX_FILENAME_LENGTH 255
#define MAX_TRACE_ALLOC_COUNT 1024*10

struct st_Alloc_Info
{
	void* pPtr;								// 할당된 메모리의 시작 주소값
	int iSize;								// 할당된 메모리의 크기
	bool bIsArray;							// 배열 형태로 할당 되었는지
	char* szFile[MAX_FILENAME_LENGTH];		// 파일 이름 저장
	int iLine;								// main 함수에서 호출한 위치
} AllocInfo;

namespace MemoryControl {
	class MemoryAllocator
	{
	private:
		int iTotal_Alloc_Size;	
		size_t alloc_count;
		size_t alloc_failed_count;
	};
}